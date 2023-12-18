#include <market.h>
#include <stdexcept>
#include <functional>


Adndtk::Market& Adndtk::Market::get_instance()
{
    static Market _instance;
    return _instance;
}

Adndtk::Market::Market()
{
}

Adndtk::Market::~Market()
{
}

Adndtk::Store& Adndtk::Market::operator[] (const std::string& storeName)
{
    if (_stores.find(storeName) == _stores.end())
    {
        throw std::runtime_error("Unknown store");
    }
    return _stores.at(storeName);
}

Adndtk::Store& Adndtk::Market::add(const std::string& storeName)
{
    if (storeName.empty())
    {
        throw std::runtime_error("Store name cannot be empty");
    }
    if (_stores.find(storeName) != _stores.end())
    {
        throw std::runtime_error("Store already exists");
    }

    Store s(storeName);
    _stores.emplace(s.store_name(), std::move(s));
    return _stores[storeName];
}

int Adndtk::Market::count()
{
    return _stores.size();
}

bool Adndtk::Market::remove(const std::string& storeName)
{
    if (_stores.find(storeName) == _stores.end())
    {
        return false;
    }
    _stores.erase(storeName);
    return true;
}

void Adndtk::Market::clear()
{
    _stores.clear();
}

Adndtk::Store& Adndtk::Market::select_best_buyer(const Defs::equipment& equipmentId)
{
    auto& selected = _stores.begin()->second;
    for (auto& store : _stores)
    {
        if (store.second.get_buy_price(equipmentId) < selected.get_buy_price(equipmentId))
        {
            selected = std::ref(store.second);
        }
    }
    return selected;
}

Adndtk::Store& Adndtk::Market::select_best_seller(const Defs::equipment& equipmentId)
{
    auto& selected = _stores.begin()->second;
    for (auto& store : _stores)
    {
        if (store.second.get_sell_price(equipmentId) > selected.get_sell_price(equipmentId))
        {
            selected = store.second;
        }
    }
    return selected;
}