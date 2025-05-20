#include <battleground.h>

bool Adndtk::Battleground::_initialised = false;

Adndtk::Battleground& Adndtk::Battleground::get_instance()
{
    static Battleground _instance;

    if (!_initialised)
    {
        _initialised = true;
    }
    return _instance;
}

Adndtk::Battleground::Battleground()
{
}

Adndtk::Battleground::~Battleground()
{
}

bool Adndtk::Battleground::has_battlefield(const std::string& fieldName)
{
    return _fields.find(fieldName) != _fields.end();
}

void Adndtk::Battleground::add_battlefield(const std::string& fieldName)
{
    if (!has_battlefield(fieldName))
    {
        _fields.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(fieldName),
            std::forward_as_tuple(fieldName));
    }
}

Adndtk::Battlefield& Adndtk::Battleground::get_battlefield(const std::string& fieldName)
{
    add_battlefield(fieldName);
    return _fields[fieldName];
}

std::optional<Adndtk::Battlefield> Adndtk::Battleground::try_get_battlefield(const std::string& fieldName) const
{
    if (_fields.find(fieldName) == _fields.end())
    {
        return std::nullopt;
    }
    return _fields.at(fieldName);
}

void Adndtk::Battleground::clear()
{
    for (auto& bf : _fields)
    {
        bf.second.clear();
    }
    _fields.clear();
}