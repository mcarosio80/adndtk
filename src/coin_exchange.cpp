#include <coin_exchange.h>
#include <coin.h>
#include <cyclopedia.h>
#include <common_types.h>

#include "../generated/config.h"

#include <cmath>

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

double Adndtk::CoinExchange::get_conversion_ratio(const Defs::coin& from, const Defs::coin& to) const
{
    return _exchangeRates.at(from).at(to);
}

std::map<Adndtk::Defs::coin, uint32_t> Adndtk::CoinExchange::split(const Defs::coin& currency, const double& value)
{
	double currencyValue = value;
	std::map<Defs::coin, uint32_t> result;

	if (currencyValue >= 1.0)
	{
		double intPart = 0.0;
		double fracPart = modf(currencyValue, &intPart);

		Coin amt{currency, static_cast<uint32_t>(intPart)};
		result.emplace(currency, amt);
		currencyValue = fracPart;
	}

	for (auto cny : {Defs::coin::platinum_piece, Defs::coin::gold_piece, Defs::coin::electrum_piece, Defs::coin::silver_piece, Defs::coin::copper_piece})
	{
		if (static_cast<short>(cny) < static_cast<short>(currency) && currencyValue > 0.0)
		{
			currencyValue *= CoinExchange::get_instance().get_conversion_ratio(currency, cny);

			double intPart = 0.0;
			double fracPart = std::modf(currencyValue, &intPart);
			if (intPart > 0)
			{
				Coin amt{cny, static_cast<uint32_t>(intPart)};
				result.emplace(cny, amt);
			}
			currencyValue = fracPart;
		}
	}

	if (currencyValue > 0.0)
	{
        ErrorManager::get_instance().error("Split failed");
	}

	return result;
}