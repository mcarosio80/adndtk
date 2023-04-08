#include <coin_exchange.h>
#include <cyclopedia.h>
#include <common_types.h>

bool Adndtk::CoinExchange::_initialised = false;


Adndtk::CoinExchange& Adndtk::CoinExchange::get_instance()
{
    static CoinExchange _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

Adndtk::CoinExchange::CoinExchange()
{
}

Adndtk::CoinExchange::~CoinExchange()
{
}

void Adndtk::CoinExchange::init()
{
    auto result = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_coin_exchange_rates);

    for (auto& t : result)
    {
        auto coinFrom = static_cast<Defs::coin>(t.as<int>("coin_from"));
        auto coinTo = static_cast<Defs::coin>(t.as<int>("coin_to"));
        auto fx = t.as<double>("exchange_value");

        _exchangeRates[coinFrom][coinTo] = fx;
    }
}