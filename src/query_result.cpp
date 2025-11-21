#include <sqlite3.h>
#include <query_result.h>

#include <defs.h>
#include <config.h>
#include <adnd_errors.h>

#include <sstream>


Adndtk::QueryResult::QueryResult()
{
}

Adndtk::QueryResult::~QueryResult()
{
}

const std::optional<std::string>& Adndtk::QueryResult::operator[](const std::string& key) const
{
    if (_values.find(key) == _values.end())
    {
        throw DataException("Invalid key specified", key);
    }
    return _values.at(key);
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

const Adndtk::QueryResult& Adndtk::QueryResultSet::operator[](const size_t& index) const
{
    if (index < 0 || index > _records.size())
    {
        throw DataException("Index out of bound", index);
    }
    return _records.at(index);
}

Adndtk::QueryResultSet::operator QueryResult()
{
    if (_records.size() == 0)
    {
        throw DataException("QueryResultSet is empty");
    }
    return _records[0];
}

Adndtk::QueryResultSet& Adndtk::QueryResultSet::operator+=(Adndtk::QueryResultSet res)
{
    _records.insert(_records.end(), res._records.begin(), res._records.end());
    return *this;
}