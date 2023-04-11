#include <money_bag.h>
#include "../generated/config.h"
#include <coin_exchange.h>

#include <cstdint>

Adndtk::MoneyBag::MoneyBag()
{
	for (auto c : { Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece })
	{
		_money[c] = 0;
	}
}

Adndtk::MoneyBag::~MoneyBag()
{
}

bool Adndtk::MoneyBag::check_availability(const Coin& amt) const
{
	return check_availability(amt.currency(), amt.value());
}

bool Adndtk::MoneyBag::check_availability(const Defs::coin& currency, const uint32_t& amount) const
{
	uint32_t total = 0;
	for (auto& m : _money)
    {
		total += Coin::convert<Defs::coin::copper_piece>(m.first, m.second);
    }

	return Coin::convert<Defs::coin::copper_piece>(currency, amount) <= total;
}

const uint32_t& Adndtk::MoneyBag::operator[](const Defs::coin& c)
{
	return _money.at(c);
}

Adndtk::MoneyBag& Adndtk::MoneyBag::add(const Defs::coin& coinType, const uint32_t& value)
{
    _money[coinType] += value;
    return *this;
}

Adndtk::MoneyBag& Adndtk::MoneyBag::add(const Coin& c)
{
    return add(c.currency(), c.value());
}

Adndtk::MoneyBag& Adndtk::MoneyBag::operator+= (const Adndtk::Coin& c)
{
	_money[c.currency()] += c.value();
	return (*this);
}

Adndtk::MoneyBag& Adndtk::MoneyBag::subtract(const Defs::coin& coinType, const uint32_t& value)
{
    uint32_t amount = value;
	if (!check_availability(coinType, amount))
    {
        ErrorManager::get_instance().error("Not enough money");
    }

	if (_money[coinType] >= amount)
    {
		_money[coinType] -= amount;
    }
	else
	{	//Look for other currencies...
		
		for (auto it = _money.begin(); it != _money.end() && amount > 0; ++it)
		{
            double rate = CoinExchange::get_instance().get_conversion_ratio(it->first, coinType);

			if (rate <= 1) // currency of lesser value
			{
				ldiv_t res = std::ldiv(it->second, static_cast<long>(1/rate));
				amount -= res.quot;
				_money[it->first] = res.rem;
			}
			else // currency of greater value
			{
				auto ccyAmt = static_cast<uint32_t>(_money[it->first] * rate);
				_money[it->first] = 0; // Money turned into another currency
				if (ccyAmt >= amount)
				{
					_money[coinType] = ccyAmt - amount;
					amount = 0;
				}
				else
				{
					amount -= ccyAmt;
				}
			}
		}
	}
    return *this;
}

Adndtk::MoneyBag& Adndtk::MoneyBag::subtract(const Coin& c)
{
	return subtract(c.currency(), c.value());
}

Adndtk::MoneyBag& Adndtk::MoneyBag::operator-= (const Adndtk::Coin& c)
{
	subtract(c.currency(), c.value());
	return (*this);
}

void Adndtk::MoneyBag::normalise()
{
	for (auto ccy : { Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece })
	{
		if (_money[ccy] == 0)
			continue;

		auto amtList = Coin::normalise(ccy, _money[ccy]);
		if (!amtList.empty())
			_money[ccy] = 0;
		for (auto& a : amtList)
		{
			_money[a.currency()] += a.value();
		}
	}
}