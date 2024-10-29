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
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_equipment, id);
    const QueryResult& res = rs[0];

    std::optional<Defs::body_slot> bodySlot = std::nullopt;
    if (res.exists("body_slot"))
    {
        bodySlot = static_cast<Defs::body_slot>(res.as<int>("body_slot"));
    }
    else
    {
       bodySlot = Defs::body_slot::backpack;
    }

    if (id != Defs::equipment::quiver && bodySlot == Defs::body_slot::quiver && !has_item(Defs::equipment::quiver) && !has_item(Defs::equipment::backpack))
    {
        ErrorManager::get_instance().error("Quiver or backpack required");
        return false;
    }
    else if (id != Defs::equipment::quiver && bodySlot == Defs::body_slot::quiver && !has_item(Defs::equipment::quiver) && has_item(Defs::equipment::backpack))
    {
       bodySlot = Defs::body_slot::backpack;
    }

    if (id != Defs::equipment::backpack && bodySlot == Defs::body_slot::backpack && !has_item(Defs::equipment::backpack))
    {
        ErrorManager::get_instance().error("Backpack required");
        return false;
    }

    if (!has_capacity(res, id, count))
    {
        return false;
    }
    
    _items[bodySlot.value()][id] = count_items(id) + count;

    auto weight = res.try_or<double>("weight", 0.0);
    _totalWeight += weight * count;

    return true;
}

bool Adndtk::Inventory::remove(const Defs::equipment& id, const short& count/*=1*/)
{
    std::optional<Defs::body_slot> bodySlot = find(id);
    if (!bodySlot.has_value())
    {
        return false;
    }

    auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_equipment, id);
    const QueryResult& res = rs[0];

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

std::optional<Adndtk::Defs::body_slot> Adndtk::Inventory::find(const Defs::equipment& id)
{
    std::optional<Adndtk::Defs::body_slot> slot = std::nullopt;
    for (auto& i : _items)
    {
        if (i.second.find(id) != i.second.end())
        {
            slot = i.first;
            break;
        }
    }
    return slot;
}

bool Adndtk::Inventory::move(const Defs::equipment& id, const Defs::body_slot& slot, short quantity/*=1*/)
{
    auto currentSlot = find(id);
    if (!currentSlot.has_value())
    {
        return false;
    }

    if (slot == Defs::body_slot::quiver && !has_item(Defs::equipment::quiver))
    {
        return false;
    }

    if (slot == Defs::body_slot::backpack && !has_item(Defs::equipment::backpack))
    {
        return false;
    }

    _items[currentSlot.value()][id] -= quantity;
    _items[slot][id] += quantity;
    return true;
}

bool Adndtk::Inventory::has_item(const Defs::equipment& id, const short& count/*=1*/)
{
    return count_items(id) >= count;
}

double Adndtk::Inventory::total_weight() const
{
    return _totalWeight + Const::clothes_weight;
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