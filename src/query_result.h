#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include <optional>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <type_traits>

#include <defs.h>
#include <config.h>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/allocators.h"

namespace Adndtk
{
    class QueryResult
    {
    public:
        QueryResult();
        ~QueryResult();

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

        template <typename _T>
        _T as(const std::string& key) const
        {
            std::string v = (*this)[key].value();
            std::stringstream ss{v};
            return convert_to<_T>(ss);
        }
        
        template <typename _T>
        std::optional<_T> try_as(const std::string& key) const
        {
            if (_values.find(key) == _values.end() || !_values.at(key).has_value())
            {
                return std::nullopt;
            }

            auto v = as<_T>(key);
            return std::make_optional(v);
        }
        
        template <typename _T>
        _T try_or(const std::string& key, const _T& defaultValue) const
        {
            auto v = try_as<_T>(key);
            return (v.has_value()) ? v.value() : defaultValue;
        }

    private:
        template <typename _T>
        _T convert_to(std::stringstream& ss) const
        {
            _T res{};
            if constexpr(std::is_same_v<_T, std::string>) {
                std::getline(ss, res);
            } else if constexpr(std::is_enum<_T>::value)
            {
                typename std::underlying_type<_T>::type underValue;
                ss >> underValue;
                res = static_cast<_T>(underValue);
            } else {
                ss >> res;
            }
            return res;
        }

        std::map<std::string, std::optional<std::string>>  _values;
    };

    class QueryResultSet
    {
    public:
        QueryResultSet();
        ~QueryResultSet();

        friend std::ostream& operator<< (std::ostream& out, const QueryResultSet& res)
        {
            for (auto& r : res._records)
            {
                out << r;
            }
            return out;
        }

        const QueryResult& operator[](const size_t& index) const
        {
            if (index < 0 || index > _records.size())
            {
                ErrorManager::get_instance().error("Index out of bound");
            }
            return _records.at(index);
        }

        operator bool()
        {
            return _records.size() > 0;
        }
        operator QueryResult()
        {
            if (_records.size() == 0)
            {
                ErrorManager::get_instance().error("QueryResultSet is empty");
            }
            return _records[0];
        }
        
        QueryResultSet& operator+=(QueryResult res)
        {
            _records.push_back(res);
            return *this;
        }
        
        QueryResultSet& operator+=(QueryResultSet res)
        {
            _records.insert(_records.end(), res._records.begin(), res._records.end());
            return *this;
        }

        size_t size() const { return _records.size(); }

        auto begin() { return _records.begin(); } 
        auto end() { return _records.end(); } 
        auto cbegin() const { return _records.cbegin(); } 
        auto cend() const { return _records.cend(); }

        template<typename _T>
        std::vector<_T> to_vector(const std::string& fieldName) const
        {
            std::vector<_T> data{};
            data.reserve(_records.size());
            for (auto& r : _records)
            {
                auto value = r.as<_T>(fieldName);
                data.push_back(value);
            }
            return data;
        }

        template<typename _KeyType, typename _ValueType>
        std::map<_KeyType, _ValueType> to_map(const std::string& keyField, const std::string& valueField) const
        {
            std::map<_KeyType, _ValueType> data{};
            for (auto& r : _records)
            {
                auto key = r.as<_KeyType>(keyField);
                auto value = r.as<_ValueType>(valueField);
                data[key] = value;
            }
            return data;
        }

        template<typename _KeyType>
        std::set<_KeyType> to_set(const std::string& fieldName) const
        {
            std::set<_KeyType> data{};
            for (auto& r : _records)
            {
                auto value = r.as<_KeyType>(fieldName);
                data.emplace(value);
            }
            return data;
        }

    private:
        std::vector<QueryResult>    _records;
    };
}

#endif // QUERY_RESULT_H