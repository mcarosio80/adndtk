#ifndef STORE_H
#define STORE_H

#include <string>
#include <map>
#include <optional>

#include <defs.h>
#include <money_bag.h>
#include <coin.h>

namespace Adndtk
{
    class Store
    {
    public:
        Store();
        Store(const std::string& name);
        ~Store();

        inline const std::string& store_name() { return _name; }

        void supply(const Defs::equipment& equipmentId, const short& count = 1);
        void supply_all(const short& count = 1);
        void remove(const Defs::equipment& equipmentId, const short& count = 1);
        bool check_supply(const Defs::equipment& goodsId, const int& count) const;

        void add_money(const Coin& amt) { _money.add(amt); }
        void subtract_money(const Coin& amt) { _money.subtract(amt); }

        const uint32_t& operator[](const Defs::coin& c) const
        {
            return _money[c];
        }
        inline bool check_availability(const Coin& price, bool strictCheck = false) const
        {
            return check_availability(price.currency(), price.value(), strictCheck);
        };
        bool check_availability(const Defs::coin& currency, const uint32_t& amount, bool strictCheck = false) const;

        Coin get_buy_price(const Defs::equipment& id, const short& count = 1);
        Coin get_sell_price(const Defs::equipment& id, const short& count = 1);

    private:
        std::string     _name;
        MoneyBag        _money;

        std::map<Defs::equipment, int>
                        _goods;
        std::map<Defs::equipment, Coin>
                        _variablePrices;

        void init();

        double get_amount(const double& minPrice, const std::optional<double>& maxPrice) const;
        Coin get_price(const Defs::equipment& id);
        std::tuple<Defs::coin, double, std::optional<double>> get_price_min_max(const Defs::equipment& id) const;
    };
}

#endif // STORE_H