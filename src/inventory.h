#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>

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

    private:
        std::map<Defs::body_slot, std::map<Defs::equipment, short>>     _items;
        double                                                          _totalWeight;

        bool has_capacity(const Adndtk::QueryResult& res, const Defs::equipment& id, const short& count);
    };
}

#endif // INVENTORY_H