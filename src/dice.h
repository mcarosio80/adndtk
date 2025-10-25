#ifndef DICE_H
#define DICE_H

#include <random>
#include <iostream>
#include <array>
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
            out << "d" << d.faces();
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
        //static int roll(int minValue, int maxValue);

        template<typename _NumberType = int>
        static _NumberType roll(const _NumberType minValue, const _NumberType maxValue)
        {
            std::random_device rd;
            std::mt19937 generator(rd());	//mersenne_twister_engine
            std::uniform_int_distribution<_NumberType> distribution(1, static_cast<_NumberType>(maxValue - minValue + 1));

            auto roll = distribution(generator) + (minValue - 1);

            return roll;
        }

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

        int roll(const short diceNumber, const Defs::die& dieType, const short base = 0) const;

    private:
        DiceSet();
        ~DiceSet();

        std::map<Defs::die, Die> _dice;
    };

    template<Defs::die _dieFaces, short _numDice>
    class DiceRoll
    {
    public:
        DiceRoll()
            : _rolls{}
        {}

        int roll()
        {
            for (int n{0}; n<_numDice; ++n) 
            {
                _rolls[n] = DiceSet::get_instance().roll(1, _dieFaces);
            }
            return total();
        }

        friend std::ostream& operator<< (std::ostream& out, const DiceRoll& d)
        {
            out << "(";
            auto len = d._rolls.size();
            for (const auto& r : d._rolls)
            {
                out << r;
                if (len > 1) out << ", ";
                --len;
            }
            out << ")";

            return out;
        }

        int& operator[](int index)
        {
            return _rolls[index];
        }

        const int& operator[](int index) const
        {
            return _rolls.at(index);
        }

        int total() const
        {
            int total{};
            for (const auto& r : _rolls)
            {
                total += r;
            }

            return total;
        }

    private:
        std::array<int, _numDice> _rolls;
    };
}

#endif // DICE_H