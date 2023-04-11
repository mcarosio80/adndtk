#ifndef MONEY_BAG_H
#define MONEY_BAG_H

#include <map>

#include "../generated/defs.h"
#include <coin.h>

namespace Adndtk
{
    class MoneyBag
    {
    public:
        MoneyBag();
        ~MoneyBag();

        bool check_availability(const Coin& amt) const;
        bool check_availability(const Defs::coin& currency, const uint32_t& amount) const;

        const uint32_t& operator[](const Defs::coin& c);

        MoneyBag& add(const Defs::coin& coinType, const uint32_t& value);
        MoneyBag& add(const Coin& c);
        MoneyBag& operator+= (const Coin& c);

        MoneyBag& subtract(const Defs::coin& coinType, const uint32_t& value);
        MoneyBag& subtract(const Coin& c);
        MoneyBag& operator-= (const Coin& c);

        void normalise();

    private:
        std::map<Defs::coin, uint32_t>       _money;
    };
}

#endif // MONEY_BAG_H