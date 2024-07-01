#ifndef DICE_H
#define DICE_H

#include <random>
#include <iostream>
#include <regex>

#include <defs.h>

namespace Adndtk
{
    struct Die
    {
        Die();
        Die(const Defs::die& numFaces);

        int roll(int numRolls = 1) const;

        friend std::ostream& operator<< (std::ostream& out, const Die& d)
        {
            out << "d" << static_cast<short>(d.faces());
            return out;
        }
        operator int() const
        {
            return roll();
        }

        Defs::die faces() const { return m_numFaces; }
        
        static int roll(const std::string& diceExpression)
        {
            return roll(diceExpression.c_str());
        }
        static int roll(const char *diceExpression);
        static int roll(int numDice, int numFaces, int addendum);
        static int roll(int minValue, int maxValue);

    private:
        static int match_dice_expression(const std::smatch& matches);
        static int match_range_expression(const std::smatch& matches);

        Defs::die		m_numFaces;
    };

    class DiceSet
    {
    public:
        static DiceSet& get_instance();
        DiceSet(DiceSet const&) = delete;
        void operator=(DiceSet const&) = delete;

        int roll(const Defs::die& dieType, const short diceNumber = 1) const;

    private:
        DiceSet();
        ~DiceSet();

        std::map<Defs::die, Die> _dice;
    };
}

#endif // DICE_H