#ifndef __ADNDTK__SRC__COIN_H
#define __ADNDTK__SRC__COIN_H

#include <defs.h>
#include <coin_exchange.h>

#include <cstdint>
#include <compare>

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

        std::strong_ordering operator<=>(const uint32_t& amount) const;
        bool operator== (const uint32_t& amount) const;

        Coin& operator= (const Coin& amount);
        Coin& operator+= (const Coin& amount);
        Coin& operator-= (const Coin& amount);

        bool operator== (const Coin& amount) const;
        std::weak_ordering operator<=>(const Coin& amount) const;

        const Defs::coin& currency() const;
        uint32_t value() const;

        static Coin convert(const Coin& c, const Defs::coin& currency_to);
        static Coin convert(const Defs::coin& currency_from, const Defs::coin& currency_to, const uint32_t& amount);

        static std::map<Defs::coin, uint32_t> split(const Defs::coin& currency, const double& value);
        static Coin normalise_fractional(const Defs::coin& currency, const double& value);
        static std::vector<Coin> normalise(const Defs::coin& currency, const uint32_t& value);

    private:
        uint32_t	_amount;
        Defs::coin	_currency;
    };
}

#endif // __ADNDTK__SRC__COIN_H