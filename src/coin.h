#ifndef COIN_H
#define COIN_H

#include "../generated/defs.h"
#include <coin_exchange.h>

#include <cstdint>

namespace Adndtk
{
    class Coin
    {
    public:
        Coin();
        Coin(const Defs::coin& c, const uint32_t& amount);
        ~Coin();

        Coin& operator= (const uint32_t amount);
        Coin& operator+= (const uint32_t amount);
        Coin& operator-= (const uint32_t amount);

        bool operator== (const uint32_t& amount) const;
        bool operator< (const uint32_t& amount) const;
        bool operator<= (const uint32_t& amount) const;
        bool operator>= (const uint32_t& amount) const;
        bool operator> (const uint32_t& amount) const;
        bool operator!= (const uint32_t& amount) const;

        Coin& operator= (const Coin& amount);
        Coin& operator+= (const Coin& amount);
        Coin& operator-= (const Coin& amount);

        bool operator== (const Coin& amount) const;
        bool operator< (const Coin& amount) const;
        bool operator<= (const Coin& amount) const;
        bool operator>= (const Coin& amount) const;
        bool operator> (const Coin& amount) const;
        bool operator!= (const Coin& amount) const;

        const Defs::coin& currency() const;
        operator uint32_t() const
        {
            return _amount;
        }
        uint32_t value() const;

        template<Defs::coin t>
        static Coin convert(const Coin& c)
        {
            return convert<t>(c.currency(), c.value());
        }
        
        template<Defs::coin t>
        static Coin convert(const Defs::coin& currency, const uint32_t& amount)
        {
            double ratio = CoinExchange::get_instance().get_conversion_ratio(currency, t);
            return Coin(t, ratio * amount);
        }

        static std::map<Defs::coin, uint32_t> split(const Defs::coin& currency, const double& value);
        static Coin normalise_fractional(const Defs::coin& currency, const double& value);
        static std::vector<Coin> normalise(const Defs::coin& currency, const uint32_t& value);

    private:
        uint32_t	_amount;
        Defs::coin	_currency;
    };
}

#endif // COIN_H