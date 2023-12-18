#ifndef MARKET_H
#define MARKET_H

#include <map>
#include <string>

#include <stdint.h>

#include <store.h>

namespace Adndtk
{
    class Market
    {
    public:
        static Market& get_instance();
        Market(Market const&) = delete;
        void operator=(Market const&) = delete;

        Store& operator[] (const std::string& storeName);
        Store& add(const std::string& storeName);
        int count();
        bool remove(const std::string& storeName);
        void clear();

        Store& select_best_buyer(const Defs::equipment& equipmentId);
        Store& select_best_seller(const Defs::equipment& equipmentId);

    private:
        Market();
        ~Market();

        std::map<std::string, Store>           _stores;
    };
}

#endif // MARKET_H