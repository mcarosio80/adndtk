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
#include <query_result.h>
#include <advancement_table.h>
#include <experience.h>
#include <hit_points.h>

namespace Adndtk
{
    class Cyclopedia
    {
    public:
        static Cyclopedia& get_instance();
        Cyclopedia(Cyclopedia const&) = delete;
        void operator=(Cyclopedia const&) = delete;

        static QueryResultSet from_json(const char *jsonText);

		template<class... _T>
        QueryResultSet exec_prepared_statement(const Query& queryId, _T... values)
		{
            QueryResultSet resultSet;
            if (_statements.find(queryId) == _statements.end())
            {
                ErrorManager::get_instance().error("Unable to find prepared statement for query ID");
                return resultSet;
            }
            auto stmt = _statements[queryId];
            
            std::tuple<_T...> params{values...};
            ParamsBinder<decltype(params), sizeof...(_T)>::bind_values(*this, queryId, params);

            if (Settings::query_type == QueryType::json)
            {
                resultSet = parse_json_result(stmt);
            }
            else if (Settings::query_type == QueryType::plain)
            {
                resultSet = parse_tabular_result(stmt);
            }
            
            sqlite3_reset(stmt);

            return resultSet;
        }

        static int onSqliteDataRetrieved(void *data, int argc, char **argv, char **columnName);
        QueryResultSet exec(const char* stmt);

        template <typename _T>
        std::vector<_T> split(const _T& cls)
        {
            std::vector<_T> result;
            int classId = static_cast<int>(cls);
            int mask = 1;
            while (mask != 0)
            {
                if (classId & mask)
                {
                    auto c = static_cast<_T>(mask);
                    result.push_back(c);
                }
                mask <<= 1;
            }
            return result;
        }

        template <Defs::character_class_type probeType>
        bool is_type_of(const Defs::character_class_type& clsType)
        {
            bool found = false;
            auto types = split<Defs::character_class_type>(clsType);
            for (auto& t : types)
            {
                if (t == probeType)
                    found = true;
            }
            return found;
        }

        template <Defs::character_class_type probeType>
        bool is_type_of(const Defs::character_class& cls)
        {
            Adndtk::Query query = Adndtk::Query::select_character_class;
            int clsId = static_cast<int>(cls);
            auto classInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(query, clsId);
            Defs::character_class_type clsType = static_cast<Defs::character_class_type>(classInfo[0].as<int>("class_type_id"));

            return is_type_of<probeType>(clsType);
        }

        template <Defs::character_class probeClass>
        bool is_class_of(const Defs::character_class& cls)
        {
            bool found = false;
            auto classes = split<Defs::character_class>(cls);
            for (auto& c : classes)
            {
                if (c == probeClass)
                    found = true;
            }
            return found;
        }

        const AdvancementTable& advancement_table() { return _advTable; }

    private:
        Cyclopedia();
        ~Cyclopedia();

        bool init();
        bool check_state(int return_code);
        void prepare_statement(const char *stmt, const Query& queryId, const QueryType& queryType = Settings::query_type);

        void bind(const Query& queryId, int position, int value)
        {
            sqlite3_stmt* stmt = _statements[queryId];
            sqlite3_bind_int(stmt, position, value);
        }

        void bind(const Query& queryId, int position, double value)
        {
            sqlite3_stmt* stmt = _statements[queryId];
            sqlite3_bind_double(stmt, position, value);
        }

        template<class ParamTuple, std::size_t PackSize>
		struct ParamsBinder
		{
			static void bind_values(Cyclopedia& cp, const Query& queryId, const ParamTuple& t)
			{
				ParamsBinder<ParamTuple, PackSize - 1>::bind_values(cp, queryId, t);
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

		template<class ParamTuple>
		struct ParamsBinder<ParamTuple, 0>
		{
			static void bind_values(Cyclopedia& cp, const Query& queryId, const ParamTuple& t)
            {
            }
		};

        QueryResultSet parse_json_result(sqlite3_stmt* stmt);
        QueryResultSet parse_tabular_result(sqlite3_stmt* stmt);

		static bool     _initialised;
        sqlite3         *_dbConn;
        std::map<Query, sqlite3_stmt*>      _statements;
        AdvancementTable        _advTable;

        void load_advancement_table();
    };
}

#endif // ADNDTK_H