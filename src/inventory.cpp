#include <inventory.h>
#include <cyclopedia.h>

Adndtk::Inventory::Inventory()
    : _totalWeight{0.0}
{
}

Adndtk::Inventory::~Inventory()
{
}

bool Adndtk::Inventory::add(const Defs::equipment& id, const short& count/*=1*/)
{
    int equipmentId = static_cast<int>(id);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_equipment, equipmentId);
    const QueryResult& res = rs[0];

    std::optional<Defs::body_slot> bodySlot = std::nullopt;
    if (res.exists("body_slot"))
    {
        bodySlot = static_cast<Defs::body_slot>(res.as<int>("body_slot"));
    }

    if (bodySlot == Defs::body_slot::quiver && !has_item(Defs::equipment::quiver))
    {
        ErrorManager::get_instance().error("Quiver required");
        return false;
    }

    if (bodySlot == Defs::body_slot::backpack && !has_item(Defs::equipment::backpack))
    {
        ErrorManager::get_instance().error("Backpack required");
        return false;
    }

    if (!has_capacity(res, id, count))
    {
        return false;
    }
    
    _items.at(bodySlot.value()).at(id) = count_items(id) + count;

    auto weight = res.as<double>("weight");
    _totalWeight += weight * count;

    return true;
}

bool Adndtk::Inventory::remove(const Defs::equipment& id, const short& count/*=1*/)
{
    int equipmentId = static_cast<int>(id);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_equipment, equipmentId);
    const QueryResult& res = rs[0];

    std::optional<Defs::body_slot> bodySlot = std::nullopt;
    if (res.exists("body_slot"))
    {
        bodySlot = static_cast<Defs::body_slot>(res.as<int>("body_slot"));
    }

    short num = _items[bodySlot.value()][id];
    if (num < count)
    {
        ErrorManager::get_instance().error("Not enough items");
        return false;
    }
    _items[bodySlot.value()][id] -= count;

    auto weight = res.as<double>("weight");
    _totalWeight -= weight * count;

    return true;
}

short Adndtk::Inventory::count_items(const Defs::equipment& id)
{
    short totalCount{0};
    for (auto& i : _items)
    {
        totalCount += i.second[id];
    }
    return totalCount;
}

bool Adndtk::Inventory::has_item(const Defs::equipment& id)
{
    return count_items(id) > 0;
}

bool Adndtk::Inventory::has_capacity(const Adndtk::QueryResult& res, const Defs::equipment& id, const short& count)
{
    std::optional<short> capacityLimit = res.try_as<short>("capacity_limit");
    if (!capacityLimit.has_value())
    {
        return true;
    }

    if (count_items(id) + count > capacityLimit.value())
    {
        ErrorManager::get_instance().error("Capacity limit exceeded");
        return false;
    }
    return true;
}