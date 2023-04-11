#ifndef COIN_EXCHANGE_H
#define COIN_EXCHANGE_H

#include "../generated/defs.h"
#include <map>
#include <vector>

#include <cstdint>

namespace Adndtk
{
    class CoinExchange
    {
    public:
        static CoinExchange& get_instance();
        CoinExchange(CoinExchange const&) = delete;
        void operator=(CoinExchange const&) = delete;

        double get_conversion_ratio(const Defs::coin& from, const Defs::coin& to) const;

    private:
        CoinExchange();
        ~CoinExchange();

        void init();
		static bool                         _initialised;
        std::map<Defs::coin,
            std::map<Defs::coin, double>
        >                                   _exchangeRates;
    };
}

#endif // COIN_EXCHANGE_H