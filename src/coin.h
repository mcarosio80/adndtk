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
        static Coin convert(const Coin& c) const
        {
            double ratio = CoinExchange::get_instance().get_conversion_ratio(c.currency(), t);
            return Coin(t, ratio * c.value());
        }

    private:
        uint32_t	_amount;
        Defs::coin	_currency;
    };
}

#endif // COIN_H