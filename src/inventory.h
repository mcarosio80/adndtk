#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <optional>

#include <defs.h>
#include <query_result.h>

namespace Adndtk
{
    class Inventory
    {
    public:
        Inventory();
        ~Inventory();

        bool add(const Defs::equipment& id, const short& count = 1);
        bool remove(const Defs::equipment& id, const short& count = 1);

        short count_items(const Defs::equipment& id);
        bool has_item(const Defs::equipment& id);
        double total_weight();
        std::optional<Adndtk::Defs::body_slot> find(const Defs::equipment& id);
        bool move(const Defs::equipment& id, const Defs::body_slot& slot, short quantity = 1);

    private:
        std::map<Defs::body_slot, std::map<Defs::equipment, short>>     _items;
        double                                                          _totalWeight;

        bool has_capacity(const Adndtk::QueryResult& res, const Defs::equipment& id, const short& count);
    };
}

#endif // INVENTORY_H