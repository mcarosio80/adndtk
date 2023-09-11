#include <holy_symbol.h>
#include <cyclopedia.h>
#include <skill_stats.h>

Adndtk::HolySymbol::HolySymbol(const Defs::character_class& cls, std::optional<Defs::deity> deityId/*=std::nullptr*/)
    : _casterLevel{1}, _wisdomScore{Defs::skill::wisdom, 9}, _casterClass{cls}, _deityId{deityId},
        _actualCasterLevel{_casterLevel}
{
    if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(cls))
    {
        enable_levels();
    }
}

Adndtk::HolySymbol::~HolySymbol()
{
}

const short& Adndtk::HolySymbol::operator[] (Defs::priest_spell spellId) const
{
    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);

    if (_spells.find(spellLevel) == _spells.end())
    {
        throw std::runtime_error("Level not available");
    }
    if (_spells.at(spellLevel).find(spellId) == _spells.at(spellLevel).end())
    {
        throw std::runtime_error("Spell not available");
    }
    return _spells.at(spellLevel).at(spellId);
}

void Adndtk::HolySymbol::set_caster_level(const ExperienceLevel& newLevel)
{
    _casterLevel = newLevel;
    enable_levels();
}

void Adndtk::HolySymbol::set_caster_wisdom(const short& wisdomValue)
{
    bool bonusSpellsLost = wisdomValue < _wisdomScore && _wisdomScore >= 13 && wisdomValue >= 12;

    _wisdomScore = wisdomValue;

    if (bonusSpellsLost)
    {
        for (SpellLevel sl=1; sl<= Const::holy_symbol_limit; ++sl)
        {
            auto currentSpellsCount = used_slots(sl);
            auto maxSpellsCount = total_slots(sl);
            if (currentSpellsCount > total_slots(sl))
            {
                auto it = _spells[sl].begin();
                while (it != _spells[sl].end() && currentSpellsCount > maxSpellsCount)
                {
                    if (it->second > 0)
                    {
                        it->second -= 1;
                        --currentSpellsCount;
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }
}

bool Adndtk::HolySymbol::pray_for_spell(const Defs::priest_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);
    
    int lvl = std::min(static_cast<int>(_casterLevel), 20);
    Query queryId = get_spell_progression_query_id();
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(queryId, lvl);
    auto& prog = rs[0];

    std::string label;
    std::stringstream ss;
    ss << "spell_level_" << spellLevel;
    ss >> label;
    auto spellPerLevel = prog.try_as<int>(label);

    if (!spellPerLevel.has_value())
    {
        return false;
    }

    auto count = used_slots(spellLevel);
    if (spellPerLevel.value() + get_bonus_spells(spellLevel) <= count)
    {
        return false;
    }
    _spells[spellLevel][spellId] += 1;
    return true;
}

bool Adndtk::HolySymbol::remove(const Defs::priest_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);

    auto count = _spells[spellLevel][spellId];
    if (count == 0)
    {
        return false;
    }
    
    _spells[spellLevel][spellId]--;
    return true;
}

bool Adndtk::HolySymbol::is_sphere_allowed(const Defs::priest_spell& spellId)
{
    int id = static_cast<int>(spellId);
    auto rsSphere = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_priest_spell_sphere, id);
    auto sphere = rsSphere[0].as<int>("sphere_id");
    int cls = static_cast<int>(_casterClass);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int, int>(Query::select_sphere_access_per_class, cls, sphere);
    auto hasAccess = rs.size() > 0;

    return hasAccess;
}

Adndtk::SpellLevel Adndtk::HolySymbol::get_spell_level(const Defs::priest_spell& spellId)
{
    int id = static_cast<int>(spellId);

    auto rsSpellInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_priest_spell, id);
    auto& spellInfo = rsSpellInfo[0];
    SpellLevel spellLevel = static_cast<SpellLevel>(spellInfo.as<int>("level"));
    
    return spellLevel;
}

// bool Adndtk::HolySymbol::is_level_available(const Defs::priest_spell& spellId)
// {
//     SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);
    
//     int lvl = std::min(static_cast<int>(_casterLevel), 20);
//     auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_priest_spell_progression, lvl);
//     auto& prog = rs[0];

//     std::stringstream ss;
//     ss << "spell_level_" << spellLevel;
//     auto spellCount = prog.try_or(ss.str(), 0);

//     return spellCount > 0;
// }

bool Adndtk::HolySymbol::exists(const Defs::priest_spell& spellId)
{
    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);
    return _spells.find(spellLevel) != _spells.end()
            && _spells.at(spellLevel).find(spellId) != _spells.at(spellLevel).end();
}

void Adndtk::HolySymbol::enable_levels() 
{
    auto lvl = std::min<short>(static_cast<short>(_casterLevel), 20);

    Query queryId = get_spell_progression_query_id();
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<short>(queryId, lvl);
    auto& prog = rs[0];

    _actualCasterLevel = prog.try_or<short>("casting_level", _casterLevel);

    for (SpellLevel sl=1; sl<=Const::holy_symbol_limit; ++sl)
    {
        std::stringstream ss;
        ss << "spell_level_" << sl;
        if (prog.try_as<short>(ss.str()).has_value())
        {
            fill_level(sl);
        }
        else
        {
            erase_level(sl);
        }
    }
}

short Adndtk::HolySymbol::free_slots(const SpellLevel& spellLevel) const
{
    return total_slots(spellLevel) - used_slots(spellLevel);
}

short Adndtk::HolySymbol::used_slots(const SpellLevel& spellLevel) const
{
    if (_spells.find(spellLevel) == _spells.end())
    {
        return 0;
    }

    short used{0};
    for (auto s : _spells.at(spellLevel))
    {
        used += s.second;
    }
    return used;
}

short Adndtk::HolySymbol::get_bonus_spells(const SpellLevel& spellLevel) const
{
    if (_casterClass != Defs::character_class::cleric && _casterClass != Defs::character_class::preist_of_specific_mythos)
    {
        return 0;
    }

    auto stats = SkillStats::get_instance().get_wisdom_stats(_wisdomScore);
    std::optional<short> bonus{0};

    if (spellLevel == 1)
    {
        bonus = stats.bonus_spell_level_1;
    }
    else if (spellLevel == 2)
    {
        bonus = stats.bonus_spell_level_2;
    }
    else if (spellLevel == 2)
    {
        bonus = stats.bonus_spell_level_2;
    }
    else if (spellLevel == 3)
    {
        bonus = stats.bonus_spell_level_3;
    }
    else if (spellLevel == 4)
    {
        bonus = stats.bonus_spell_level_4;
    }
    else if (spellLevel == 6)
    {
        bonus = stats.bonus_spell_level_6;
    }
    else if (spellLevel == 7)
    {
        bonus = stats.bonus_spell_level_7;
    }

    return bonus.value_or<short>(0);
}

short Adndtk::HolySymbol::total_slots(const SpellLevel& spellLevel) const
{
    int lvl = std::min(static_cast<int>(_casterLevel), 20);
    Query queryId = get_spell_progression_query_id();
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(queryId, lvl);
    auto& prog = rs[0];

    std::stringstream ss;
    ss << "spell_level_" << spellLevel;
    std::string fieldName = ss.str();
    auto spellCount = prog.try_or(fieldName, 0);

    if (spellCount > 0)
    {
        spellCount += get_bonus_spells(spellLevel);
    }
    return spellCount;
}

void Adndtk::HolySymbol::fill_level(const SpellLevel& spellLevel)
{
    QueryResultSet spells;
    if (_deityId.has_value())
    {
        auto deityId = static_cast<short>(_deityId.value());
        auto lvl = static_cast<short>(spellLevel);
        spells = Cyclopedia::get_instance().exec_prepared_statement<short, short>(Query::select_priest_spells_per_level_deity, lvl, deityId);
    }
    else
    {
        auto cls = static_cast<short>(_casterClass);
        auto lvl = static_cast<short>(spellLevel);
        spells = Cyclopedia::get_instance().exec_prepared_statement<short, short>(Query::select_priest_spells_per_class_level, cls, lvl);
    }

    for (auto& s : spells)
    {
        auto spellId = static_cast<Defs::priest_spell>(s.as<short>("id"));
        _spells[spellLevel][spellId] = 0;
    }
}

void Adndtk::HolySymbol::erase_level(const SpellLevel& spellLevel)
{
    if (_spells.find(spellLevel) != _spells.end())
    {
        _spells.erase(spellLevel);
    }
}

Adndtk::Query Adndtk::HolySymbol::get_spell_progression_query_id() const
{
    Query queryId = Query::select_priest_spell_progression;
    if (_casterClass == Defs::character_class::paladin)
    {
        queryId = Query::select_paladin_spell_progression;
    }
    else  if (_casterClass == Defs::character_class::ranger)
    {
        queryId = Query::select_ranger_spell_progression;
    }
    return queryId;
}