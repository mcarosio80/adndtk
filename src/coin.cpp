#include <coin.h>

#include <config.h>

#include <cmath>
#include <vector>


Adndtk::Coin::Coin()
	: _amount(0), _currency(Defs::coin::gold_piece)
{
}

Adndtk::Coin::Coin(const Defs::coin& c, const uint32_t& amount)
	: _amount(amount), _currency(c)
{
}

Adndtk::Coin::~Coin()
{
}

// Amount is assumed to be expressed in the same currency as the left-side operand
Adndtk::Coin& Adndtk::Coin::operator= (const uint32_t amount)
{
	_amount = amount;
	return (*this);
}

// Amount is assumed to be expressed in the same currency as the left-side operand
Adndtk::Coin& Adndtk::Coin::operator+= (const uint32_t amount)
{
	_amount += amount;
	return (*this);
}

// Amount is assumed to be expressed in the same currency as the left-side operand
Adndtk::Coin& Adndtk::Coin::operator-= (const uint32_t amount)
{
	if (_amount - amount < 0)
    {
		ErrorManager::get_instance().error("Amount not available");
	    return (*this);
    }

	_amount -= amount;
	return (*this);
}

// Amount is assumed to be expressed in the same currency as the left-side operand
bool Adndtk::Coin::operator== (const uint32_t& amount) const
{
	return _amount == amount;
}

// Amount is assumed to be expressed in the same currency as the left-side operand
bool Adndtk::Coin::operator< (const uint32_t& amount) const
{
	return _amount < amount;
}

// Amount is assumed to be expressed in the same currency as the left-side operand
bool Adndtk::Coin::operator<= (const uint32_t& amount) const
{
	return _amount <= amount;
}

// Amount is assumed to be expressed in the same currency as the left-side operand
bool Adndtk::Coin::operator>= (const uint32_t& amount) const
{
	return _amount >= amount;
}

// Amount is assumed to be expressed in the same currency as the left-side operand
bool Adndtk::Coin::operator> (const uint32_t& amount) const
{
	return _amount > amount;
}

// Amount is assumed to be expressed in the same currency as the left-side operand
bool Adndtk::Coin::operator!= (const uint32_t& amount) const
{
	return _amount != amount;
}

// If expressed in a different currency, amount is adjusted accordingly
Adndtk::Coin& Adndtk::Coin::operator= (const Coin& amount)
{
	if (_currency == amount.currency())
		_amount = amount.value();
	else
	{
        double ratio = CoinExchange::get_instance().get_conversion_ratio(amount.currency(), _currency);
		_amount = static_cast<uint32_t>(amount.value() * ratio);
	}
	return (*this);
}

// If expressed in a different currency, amount is adjusted accordingly
Adndtk::Coin& Adndtk::Coin::operator+= (const Coin& amount)
{
	if (_currency == amount.currency())
		_amount += amount.value();
	else
	{
        double ratio = CoinExchange::get_instance().get_conversion_ratio(amount.currency(), _currency);
		_amount += static_cast<uint32_t>(amount.value() * ratio);
	}
	return (*this);
}

// If expressed in a different currency, amount is adjusted accordingly
Adndtk::Coin& Adndtk::Coin::operator-= (const Coin& amount)
{
	uint32_t val{0};
	if (_currency == amount.currency())
		val = amount.value();
	else
	{
        double ratio = CoinExchange::get_instance().get_conversion_ratio(amount.currency(), _currency);
		val = static_cast<uint32_t>(amount.value() * ratio);
	}
	if (_amount - val < 0)
    {
		ErrorManager::get_instance().error("Amount not available");
	    return (*this);
    }

	_amount -= val;
	return (*this);
}

/**	If expressed in a different currency, amount is adjusted accordingly
  *	Comparison occurs in the least segnificant currency so as an integer conversion is always possible
  */
bool Adndtk::Coin::operator== (const Coin& amount) const
{
    auto c1 = convert<Defs::coin::copper_piece>(*this);
	auto c2 = convert<Defs::coin::copper_piece>(amount);
	return c1.value() == c2.value();
}

/**	If expressed in a different currency, amount is adjusted accordingly
  *	Comparison occurs in the least segnificant currency so as an integer conversion is always possible
  */
bool Adndtk::Coin::operator< (const Coin& amount) const
{
    auto c1 = convert<Defs::coin::copper_piece>(*this);
	auto c2 = convert<Defs::coin::copper_piece>(amount);
	return c1.value() < c2.value();
}

/**	If expressed in a different currency, amount is adjusted accordingly
  *	Comparison occurs in the least segnificant currency so as an integer conversion is always possible
  */
bool Adndtk::Coin::operator<= (const Coin& amount) const
{
    auto c1 = convert<Defs::coin::copper_piece>(*this);
	auto c2 = convert<Defs::coin::copper_piece>(amount);
	return c1.value() <= c2.value();
}

/**	If expressed in a different currency, amount is adjusted accordingly
  *	Comparison occurs in the least segnificant currency so as an integer conversion is always possible
  */
bool Adndtk::Coin::operator>= (const Coin& amount) const
{
    auto c1 = convert<Defs::coin::copper_piece>(*this);
	auto c2 = convert<Defs::coin::copper_piece>(amount);
	return c1.value() >= c2.value();
}

/**	If expressed in a different currency, amount is adjusted accordingly
  *	Comparison occurs in the least segnificant currency so as an integer conversion is always possible
  */
bool Adndtk::Coin::operator> (const Coin& amount) const
{
    auto c1 = convert<Defs::coin::copper_piece>(*this);
	auto c2 = convert<Defs::coin::copper_piece>(amount);
	return c1.value() > c2.value();
}

/**	If expressed in a different currency, amount is adjusted accordingly
  *	Comparison occurs in the least segnificant currency so as an integer conversion is always possible
  */
bool Adndtk::Coin::operator!= (const Coin& amount) const
{
    auto c1 = convert<Defs::coin::copper_piece>(*this);
	auto c2 = convert<Defs::coin::copper_piece>(amount);
	return c1.value() != c2.value();
}

uint32_t Adndtk::Coin::value() const
{
	return _amount;
}

const Adndtk::Defs::coin& Adndtk::Coin::currency() const
{
	return _currency;
}

std::map<Adndtk::Defs::coin, uint32_t> Adndtk::Coin::split(const Defs::coin& currency, const double& value)
{
	double currencyValue = value;
	std::map<Defs::coin, uint32_t> result;

	if (currencyValue >= 1.0)
	{
		double intPart = 0.0;
		double fracPart = std::modf(currencyValue, &intPart);

		Coin amt{currency, static_cast<uint32_t>(intPart)};
		result.emplace(currency, amt.value());
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
				result.emplace(cny, amt.value());
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


// Normalise a fractional amount of money converting it into an integer amount of a lesser value currency
// (i.e. 1.5 gold pieces become 3 electrum pieces)
Adndtk::Coin Adndtk::Coin::normalise_fractional(const Defs::coin& currency, const double& value)
{
	for (auto cny : { Defs::coin::platinum_piece, Defs::coin::gold_piece, Defs::coin::electrum_piece, Defs::coin::silver_piece, Defs::coin::copper_piece })
	{
		if (static_cast<short>(cny) < static_cast<short>(currency))
		{
			double newValue = CoinExchange::get_instance().get_conversion_ratio(currency, cny) * value;
			double intPart = 0.0;
			if (std::modf(newValue, &intPart) == 0)
			{
				return Coin(cny, static_cast<uint32_t>(intPart));
			}
		}
	}
	ErrorManager::get_instance().error("Unable to normalise");
	return Coin{currency, static_cast<uint32_t>(value)};
}

std::vector<Adndtk::Coin> Adndtk::Coin::normalise(const Defs::coin& currency, const uint32_t& value)
{
	std::vector<Adndtk::Coin> result;
	uint32_t amt = value;

	for (auto ccy : { Defs::coin::platinum_piece, Defs::coin::gold_piece, Defs::coin::electrum_piece, Defs::coin::silver_piece, Defs::coin::copper_piece })
	{
		double rate = CoinExchange::get_instance().get_conversion_ratio(currency, ccy);
		if (rate < 1 && amt >= 1/rate)
		{
			ldiv_t res = std::ldiv(amt, static_cast<long>(1/rate));
			if (res.quot > 0)
			{
				result.push_back(Coin(ccy, res.quot));
				amt = res.rem;
			}
		}
	}
	if (amt > 0)
	{
		result.push_back(Coin(currency, amt));
	}
	return result;
}