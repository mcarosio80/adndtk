#include <turn_undead.h>
#include <cyclopedia.h>
#include <utility>

bool Adndtk::TurnUndead::_initialised = false;


Adndtk::TurnUndead& Adndtk::TurnUndead::get_instance()
{
    static TurnUndead _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

Adndtk::TurnUndead::~TurnUndead()
{
}

void Adndtk::TurnUndead::init()
{
    auto result = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_turn_undead);

    for (auto& t : result)
    {
        auto level = static_cast<ExperienceLevel>(t.as<int>("level"));
        auto tCode = static_cast<Defs::turnable>(t.as<int>("turnable_code"));
        auto effect = static_cast<Defs::turn_effect>(t.as<int>("effect"));
        
        std::optional<short> value{std::nullopt};
        if (t["value"].has_value())
        {
            value = static_cast<short>(t.as<int>("value"));
        }

        _turnScores[level][tCode] = std::make_pair(effect, value);
    }
}

std::pair<Adndtk::Defs::turn_effect, std::optional<short>> Adndtk::TurnUndead::get(const ExperienceLevel& lvl, const Defs::turnable& t) const
{
    std::pair<Adndtk::Defs::turn_effect, std::optional<short>> p{};

    if (_turnScores.find(lvl) == _turnScores.end())
    {
        auto l = _turnScores.lower_bound(lvl)->first;
        p = _turnScores.at(l).at(t);
    }
    p = _turnScores.at(lvl).at(t);

    return p;
}

std::pair<Adndtk::Defs::turn_effect, bool> Adndtk::TurnUndead::try_turn(const ExperienceLevel& lvl, const Defs::turnable& t, const short& bonusMalus/*=0*/) const
{
    auto tr = get(lvl, t);
    Defs::turn_effect effect = std::get<0>(tr);
    std::pair<Adndtk::Defs::turn_effect, bool> res{effect, true};

    if (effect == Defs::turn_effect::no_effect)
    {
        res = {effect, false};
    }
    else if (effect == Defs::turn_effect::percentage)
    {
        std::optional<short> score = std::get<1>(tr);
        if (!score.has_value())
            res = {effect, false};
        else
        {
            Die d20{Defs::die::d20};
            auto roll = d20;
            bool turnResult = score.value() <= roll + bonusMalus;
            res = {effect, turnResult};
        }
    }

    return res;
}