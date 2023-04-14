#include <sqlite3.h>
#include "query_result.h"

#include <defs.h>
#include <config.h>

#include <sstream>


Adndtk::QueryResult::QueryResult()
{
}

Adndtk::QueryResult::~QueryResult()
{
}

bool Adndtk::QueryResult::exists(const std::string& key) const
{
    return _values.find(key) != _values.end() && _values.at(key).has_value();
}

void Adndtk::QueryResult::add(const char *jsonKey, const rapidjson::Value& jsonValue)
{
    auto valType = jsonValue.GetType();
    std::stringstream ss;

    if (valType == rapidjson::kNullType)
    {
        add(jsonKey);
    }
    else if (valType == rapidjson::kNumberType)
    {
        if (jsonValue.IsUint())
        {
            ss << jsonValue.GetUint();
        }
        else if (jsonValue.IsInt())
        {
            ss << jsonValue.GetInt();
        }
        else if (jsonValue.IsUint64())
        {
            ss << jsonValue.GetUint64();
        }
        else if (jsonValue.IsInt64())
        {
            ss << jsonValue.GetInt64();
        }
        else if (jsonValue.IsDouble())
        {
            ss << jsonValue.GetDouble();
        }
        else if (jsonValue.IsFloat())
        {
            ss << jsonValue.GetFloat();
        }
        std::string value;
        ss >> value;
        add(jsonKey, value);
    }
    else if (valType == rapidjson::kStringType)
    {
        add(jsonKey, jsonValue.GetString());
    }
    else if (valType == rapidjson::kTrueType)
    {
        add(jsonKey, "true");
    }
    else if (valType == rapidjson::kFalseType)
    {
        add(jsonKey, "false");
    }
}

void Adndtk::QueryResult::add(const std::string& key, const std::string& value)
{
    _values[key] = value;
}

void Adndtk::QueryResult::add(const std::string& key)
{
    _values[key] = std::nullopt;
}


Adndtk::QueryResultSet::QueryResultSet()
{
}

Adndtk::QueryResultSet::~QueryResultSet()
{
}