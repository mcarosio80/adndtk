#ifndef ADNDTK_H
#define ADNDTK_H

#include "../generated/defs.h"
#include "../generated/config.h"

#include <sqlite3.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/allocators.h"

#include <map>
#include <vector>
#include <functional>

#include <dice.h>

namespace Adndtk
{
    class Cyclopedia
    {
    public:

        enum class Query
        {
            select_coin
        };

        using OnQueryResult = std::function<void (int, int, const char *, const unsigned char *)>;

        static Cyclopedia& get_instance();
        Cyclopedia(Cyclopedia const&) = delete;
        void operator=(Cyclopedia const&) = delete;

		template<class... _T>
        void exec_prepared_statement(const Query& queryId, const OnQueryResult& cbkOnResult, _T... values)
		{
            if (_statements.find(queryId) == _statements.end())
            {
                ErrorManager::get_instance().error("Unable to find prepared statement for query ID");
                return;
            }
            
            auto params = std::make_tuple<_T...>(std::move(values...));
            ParamsBinder<decltype(params), sizeof...(_T)>::bind_values(*this, queryId, params);

            int rowCount = 0;
            auto stmt = _statements[queryId];
            int columnCount = sqlite3_column_count(stmt);
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                for (int col = 0; col<columnCount; ++col)
                {
                    auto colType = sqlite3_column_decltype(stmt, col);
                    auto colValue = sqlite3_column_text(stmt, col);
                    cbkOnResult(rowCount, col, colType, colValue);
                }
            }
        }

        static int cbk(void *data, int argc, char **argv, char **columnName)
        {
            rapidjson::Document jsonDoc;
            jsonDoc.SetObject();
            rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

            for (int i=0; i<argc; ++i)
            {
                rapidjson::Value key{rapidjson::kStringType};
                rapidjson::Value val{rapidjson::kStringType};
                key.SetString(columnName[i], static_cast<rapidjson::SizeType>(std::strlen(columnName[i])), allocator);
                val.SetString(argv[i], static_cast<rapidjson::SizeType>(std::strlen(argv[i])), allocator);

                jsonDoc.AddMember(key, val, allocator);
            }

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            jsonDoc.Accept(writer);

            OnQueryResult* queryCbk = (OnQueryResult*)data;
            if (queryCbk)
            {
                std::string s(buffer.GetString(), buffer.GetSize());
                auto jsonText = reinterpret_cast<const unsigned char*>(buffer.GetString()); 
                (*queryCbk)(0, 0, "JSON", jsonText);
            }
            return 0;
        }

        void exec(const char* stmt, const OnQueryResult& cbkOnResult)
		{
            char *error = nullptr;
            auto rc = sqlite3_exec(_dbConn, stmt, Cyclopedia::cbk, (void*)&cbkOnResult, &error);

            if (rc != SQLITE_OK)
            {
                std::string msg{error};
                sqlite3_free(error);
                throw std::runtime_error(msg);
            }
        }

    private:
        Cyclopedia();
        ~Cyclopedia();

        bool init();
        bool check_state(int return_code);
        void prepare_statement(const char *stmt, const Query& queryId, const QueryType& queryType = Settings::query_type);

        void bind(const Query& queryId, int position, int value)
        {
            auto& stmt = _statements[queryId];
            sqlite3_bind_int(stmt, position, value);
        }

        void bind(const Query& queryId, int position, double value)
        {
            auto& stmt = _statements[queryId];
            sqlite3_bind_double(stmt, position, value);
        }

        template<class ParamTuple, std::size_t PackSize>
		struct ParamsBinder
		{
			static void bind_values(Cyclopedia& cp, const Query& queryId, const ParamTuple& t)
			{
				ParamsBinder<ParamTuple, PackSize - 1>::bind_values(queryId, t);
                cp.bind(queryId, PackSize, std::get<PackSize - 1>(t));
			}
		};

		template<class ParamTuple>
		struct ParamsBinder<ParamTuple, 1>
		{
			static void bind_values(Cyclopedia& cp, const Query& queryId, const ParamTuple& t)
            {
                cp.bind(queryId, 1, std::get<0>(t));
            }
		};

		static bool     _initialised;
        sqlite3         *_dbConn;
        std::map<Query, sqlite3_stmt*>      _statements;
    };
}

#endif // ADNDTK_H