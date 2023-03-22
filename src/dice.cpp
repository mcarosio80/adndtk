#include <dice.h>
#include <typeinfo>

Adndtk::Die::Die()
    : m_numFaces{Adndtk::Defs::die::d6}
{
}

Adndtk::Die::Die(const Adndtk::Defs::die& numFaces)
    : m_numFaces{numFaces}
{
}

int Adndtk::Die::roll(int numRolls) const
{
    int result = 0;
    
    std::random_device rd;
    std::mt19937 generator(rd());	//mersenne_twister_engine
    std::uniform_int_distribution<int> distribution(1, static_cast<int>(m_numFaces));

    for (int r = 0; r < numRolls; ++r)
    {
        int roll = distribution(generator);
        result += roll;
    }
    return result;
}

int Adndtk::Die::match_dice_expression(const std::smatch& matches)
{
    int numDice = std::stoi(matches[1]);
    int numFaces = std::stoi(matches[2]);

    int addendum = 0;
    if (matches.length() > 3)
    {
        char sign = matches[4].str().at(0);
        addendum = (sign == '-') ? -1 : 1 * std::stoi(matches[5]);
    }
    
    return roll(numDice, numFaces, addendum);
}

int Adndtk::Die::match_range_expression(const std::smatch& matches)
{
    int rangeFrom = std::stoi(matches[1]);
    int rangeTo = std::stoi(matches[2]);
    
    return Die::roll(rangeFrom, rangeTo);
}

int Adndtk::Die::roll(const char *diceExpression)
{
    std::string text{diceExpression};
	std::regex patternDiceExpr(R"(^(\d+)\s*d\s*(\d*)(\s*([\+\-])\s*(\d+))?$)",
                    std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex patternRangeExpr(R"(^(\d+)\s*-\s*(\d+)$)",
                    std::regex_constants::ECMAScript | std::regex_constants::icase);
	
    int result = 0;
	std::smatch matches;
	if (std::regex_search(text, matches, patternDiceExpr))
    {
        result = match_dice_expression(matches);
    }
    else if (std::regex_search(text, matches, patternRangeExpr))
    {
        result = match_range_expression(matches);
    }
    else
    {
        throw std::runtime_error("Invalid expression");
    }
    
    return result;
}

int Adndtk::Die::roll(int numDice, int numFaces, int addendum)
{
    Defs::die dieType = static_cast<Defs::die>(numFaces);
    Die d{dieType};
    return d.roll(numDice) + addendum;
}

int Adndtk::Die::roll(int minValue, int maxValue)
{
    std::random_device rd;
    std::mt19937 generator(rd());	//mersenne_twister_engine
    std::uniform_int_distribution<int> distribution(1, static_cast<int>(maxValue - minValue + 1));

    int roll = distribution(generator) + (minValue - 1);

    return roll;
}