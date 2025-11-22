#include <money_bag.h>
#include <config.h>
#include <coin_exchange.h>
#include <cyclopedia.h>
#include <dice.h>
#include <options.h>

#include <cstdint>

Adndtk::MoneyBag::MoneyBag()
{
	for (auto c : { Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece })
	{
		_money[c] = 0;
	}
}

Adndtk::MoneyBag::MoneyBag(const Defs::character_class_type& clsType)
{
	for (auto c : { Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece })
	{
		_money[c] = 0;
	}
	
	auto type = select_class_type_for_money(clsType);
	auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class_type>(Query::select_starting_money, type);
	auto& startingMoney = rs[0];

	auto number = startingMoney.as<int>("die_number");
	auto faces = startingMoney.as<Defs::die>("die_faces");
	auto base = startingMoney.as<int>("die_base");
	auto mult = startingMoney.as<int>("multiplier");

	if (OptionalRules::get_instance().get_option<bool>(Option::max_starting_money))
	{
		_money[Defs::coin::gold_piece] = (number * to_int(faces) + base) * mult;
	}
	else
	{
		auto score = DiceSet::get_instance().roll(number, faces, base);
		_money[Defs::coin::gold_piece] = score * mult;
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
		total += Coin::convert(m.first, Defs::coin::copper_piece, m.second).value();
    }

	return Coin::convert(currency, Defs::coin::copper_piece, amount) <= total;
}

const uint32_t& Adndtk::MoneyBag::operator[](const Defs::coin& c) const
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
	for (auto ccy : { Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece })
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

Adndtk::Defs::character_class_type Adndtk::MoneyBag::select_class_type_for_money(const Defs::character_class_type& clsType)
{
	auto types = Cyclopedia::get_instance().split<Defs::character_class_type>(clsType);
	Defs::character_class_type selectedType = types[0];
	uint32_t maxAmount{0};
	for (auto& t : types)
	{
		auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class_type>(Query::select_starting_money, t);
		auto& startingMoney = rs[0];

		auto number = startingMoney.as<int>("die_number");
		auto faces = startingMoney.as<int>("die_faces");
		auto base = startingMoney.as<int>("die_base");
		auto mult = startingMoney.as<int>("multiplier");

		uint32_t maxVal = (number * faces + base) * mult;
		if (maxVal > maxAmount)
		{
			selectedType = t;
			maxAmount = maxVal;
		}
	}
	return selectedType;
}