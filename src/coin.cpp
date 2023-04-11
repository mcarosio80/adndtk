#include <coin.h>

#include "../generated/config.h"

// #include <cstdlib>
// #include <math.h>


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
		_amount = amount;
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
		_amount += amount;
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
		val = amount;
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