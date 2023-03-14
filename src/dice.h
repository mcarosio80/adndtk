#ifndef DICE_H
#define DICE_H

#include <random>
#include <iostream>

#include "../generated/defs.h"

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
        
        static int roll(const char *diceExpression);
        static int roll(int numDice, int numFaces, int addendum);

    private:
        Defs::die		m_numFaces;
    };
}

#endif // DICE_H