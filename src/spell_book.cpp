#include <spell_book.h>
#include <skill_stats.h>
#include <dice.h>
#include <algorithm>
#include <config.h>

#include <options.h>

Adndtk::SpellBook::SpellBook(const Defs::character_class& cls, Defs::race raceId)
    : _casterLevel{1}, _intelligenceScore{Defs::skill::intelligence, 9}, _casterClass{cls}, _casterRace{raceId}
{
    int c = static_cast<int>(_casterClass);
    int r = static_cast<int>(_casterRace);
    auto races = Cyclopedia::get_instance().exec_prepared_statement<int, int>(Query::select_school_of_magic_per_race, c, r);
    
    bool raceEnabled{true};
    for (auto& rc : races)
    {
        auto availableRace = static_cast<Defs::race>(rc.as<short>("race_id"));
        raceEnabled &= availableRace == raceId;
    }
    if (races.size() == 0 || !raceEnabled)
    {
        ErrorManager::get_instance().error("Class not available for specified race");
    }
}

Adndtk::SpellBook::~SpellBook()
{
}

const short& Adndtk::SpellBook::operator[] (Defs::wizard_spell spellId) const
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);

    if (_spells.find(spellLevel) == _spells.end())
    {
        throw std::runtime_error("No spell available at specified level");
    }
    if (_spells.at(spellLevel).find(spellId) == _spells.at(spellLevel).end())
    {
        throw std::runtime_error("Spell not scribed");
    }
    return _spells.at(spellLevel).at(spellId);
}

void Adndtk::SpellBook::set_caster_level(const ExperienceLevel& newLevel)
{
    _casterLevel = newLevel;
}

void Adndtk::SpellBook::set_caster_intelligence(const short& intelligenceValue)
{
    _intelligenceScore = intelligenceValue;
}

Adndtk::AddSpellResult Adndtk::SpellBook::scribe_scroll(const Defs::wizard_spell& spellId)
{   
    if (!is_level_available(spellId))
    {
        return AddSpellResult::level_not_available;
    }

    if (!is_school_allowed(spellId))
    {
        return AddSpellResult::school_not_allowed;
    }

    if (exists(spellId))
    {
        return AddSpellResult::existing;
    }

    if (!has_capacity(spellId))
    {
        return AddSpellResult::no_capacity;
    }

    bool scribeAlwaysSucceed = OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds);
    if (!try_scribe(spellId) && !scribeAlwaysSucceed)
    {
        return AddSpellResult::failure;
    }
    else
    {
        SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
        _spells[spellLevel][spellId] = 0;
        return AddSpellResult::success;
    }

    return AddSpellResult::none;
}

short Adndtk::SpellBook::free_slots(const SpellLevel& spellLevel) const
{
    int lvl = std::min(static_cast<int>(_casterLevel), 20);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell_progression, lvl);
    auto& spellProgression = rs[0];

    std::string label;
    std::stringstream ss;
    ss << "spell_level_" << spellLevel;
    auto spellPerLevel = spellProgression.try_or<short>(ss.str(), 0);

    return spellPerLevel - used_slots(spellLevel);
}

short Adndtk::SpellBook::used_slots(const SpellLevel& spellLevel) const
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

bool Adndtk::SpellBook::memorise(const Defs::wizard_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    
    int lvl = std::min(static_cast<int>(_casterLevel), 20);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell_progression, lvl);
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
    if (spellPerLevel.value() <= count)
    {
        return false;
    }
    _spells[spellLevel][spellId] += 1;
    return true;
}

bool Adndtk::SpellBook::remove(const Defs::wizard_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);

    auto count = _spells[spellLevel][spellId];
    if (count == 0)
    {
        return false;
    }
    
    _spells[spellLevel][spellId]--;
    return true;
}

bool Adndtk::SpellBook::delete_from_book(const Defs::wizard_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    _spells[spellLevel].erase(spellId);
    return true;
}

bool Adndtk::SpellBook::has_capacity(const Defs::wizard_spell& spellId) const
{
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);

    short usedSlots{0};
    if (_spells.find(spellLevel) != _spells.end())
    {
        usedSlots = _spells.at(spellLevel).size();
    }
    return stats.max_number_of_spells_per_level > usedSlots;
}

bool Adndtk::SpellBook::is_school_allowed(const Defs::wizard_spell& spellId) const
{
    int id = static_cast<int>(spellId);
    auto rsSchool = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell_school, id);

    int cls = static_cast<int>(_casterClass);
    for (auto r : rsSchool)
    {
        auto school = r.as<int>("school_id");
        auto rs = Cyclopedia::get_instance().exec_prepared_statement<int, int>(Query::select_school_of_magic_access, cls, school);
        if (rs.size() == 0)
        {
            return false;
        }
    }

    return true;
}

Adndtk::SpellLevel Adndtk::SpellBook::get_spell_level(const Defs::wizard_spell& spellId)
{
    int id = static_cast<int>(spellId);

    auto rsSpellInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell, id);
    auto& spellInfo = rsSpellInfo[0];
    SpellLevel spellLevel = static_cast<SpellLevel>(spellInfo.as<int>("level"));
    
    return spellLevel;
}

bool Adndtk::SpellBook::is_level_available(const Defs::wizard_spell& spellId) const
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);

    int lvl = std::min(static_cast<int>(_casterLevel), 20);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell_progression, lvl);
    auto& prog = rs[0];

    std::stringstream ss;
    ss << "spell_level_" << spellLevel;
    std::string fieldName = ss.str();
    auto spellCount = prog.try_or(fieldName, 0);

    return spellLevel <= stats.spell_level && spellCount > 0;
}

bool Adndtk::SpellBook::exists(const Defs::wizard_spell& spellId) const
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    return _spells.find(spellLevel) != _spells.end()
            && _spells.at(spellLevel).find(spellId) != _spells.at(spellLevel).end();
}

bool Adndtk::SpellBook::try_scribe(const Defs::wizard_spell& spellId)
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);

    Die d{Defs::die::d100};
    return stats.chance_to_learn_spell > d;
}