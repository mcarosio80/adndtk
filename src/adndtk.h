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
#include <skills.h>
#include <skill_creator.h>

namespace Adndtk
{
    class Cyclopedia
    {
    public:

        enum class Query
        {
            select_coin,
            select_skill_boundaries_default,
            select_skill_boundaries_class_type,
            select_skill_boundaries_class,
            select_skill_boundaries_race
        };

        class QueryResult
        {
        public:
            QueryResult();

            const std::optional<std::string>& operator[](const std::string& key) const
            {
                if (_values.find(key) == _values.end())
                {
                    ErrorManager::get_instance().error("Invalid key specified");
                }
                return _values.at(key);
            }

            friend std::ostream& operator<< (std::ostream& out, const QueryResult& res)
            {
                out << "{\n";
                for (auto& t : res._values)
                {
                    out << "\t" << t.first << ": " << (t.second.has_value() ? t.second.value() : "(null)") << "\n";
                }
                out << "}";
                return out;
            }

            bool exists(const std::string& key) const;
            void add(const char *jsonKey, const rapidjson::Value& jsonValue);
            void add(const std::string& key, const std::string& value);
            void add(const std::string& key);

        private:
            std::map<std::string, std::optional<std::string>>  _values;
        };

        static Cyclopedia& get_instance();
        Cyclopedia(Cyclopedia const&) = delete;
        void operator=(Cyclopedia const&) = delete;

        static std::vector<QueryResult> from_json(const char *jsonText);

		template<class... _T>
        std::vector<QueryResult> exec_prepared_statement(const Query& queryId, _T... values)
		{
            std::vector<QueryResult> resultSet;
            if (_statements.find(queryId) == _statements.end())
            {
                ErrorManager::get_instance().error("Unable to find prepared statement for query ID");
                return resultSet;
            }
            auto stmt = _statements[queryId];
            
            auto params = std::make_tuple<_T...>(std::move(values...));
            ParamsBinder<decltype(params), sizeof...(_T)>::bind_values(*this, queryId, params);

            if (Settings::query_type == QueryType::json)
            {
                resultSet = parse_json_result(stmt);
            }
            else if (Settings::query_type == QueryType::plain)
            {
                resultSet = parse_tabular_result(stmt);
            }
            
            sqlite3_finalize(stmt);

            return resultSet;
        }

        static int onSqliteDataRetrieved(void *data, int argc, char **argv, char **columnName);
        std::vector<QueryResult> exec(const char* stmt);

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

        std::vector<QueryResult> parse_json_result(sqlite3_stmt* stmt);
        std::vector<QueryResult> parse_tabular_result(sqlite3_stmt* stmt);

		static bool     _initialised;
        sqlite3         *_dbConn;
        std::map<Query, sqlite3_stmt*>      _statements;
    };
}

#endif // ADNDTK_H