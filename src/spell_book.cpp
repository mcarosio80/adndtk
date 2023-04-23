#include <spell_book.h>
#include <skill_stats.h>
#include <dice.h>
#include <algorithm>

Adndtk::SpellBook::SpellBook(const Defs::character_class& cls)
    : _casterLevel{1}, _intelligenceScore{9}, _casterClass{cls}
{
}

Adndtk::SpellBook::~SpellBook()
{
}

void Adndtk::SpellBook::set_wizard_level(const ExperienceLevel& newLevel)
{
    _casterLevel = newLevel;
}

void Adndtk::SpellBook::set_wizard_intelligence(const short& intelligenceValue)
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

    if (!try_scribe(spellId))
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

bool Adndtk::SpellBook::memorise(const Defs::wizard_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    
    int lvl = std::max(static_cast<int>(_casterLevel), 20);
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

    auto count = _spells[spellLevel][spellId];
    return spellPerLevel.value() > count;
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

bool Adndtk::SpellBook::has_capacity(const Defs::wizard_spell& spellId)
{
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);

    return stats.max_number_of_spells_per_level > _spells[spellLevel].size();
}

bool Adndtk::SpellBook::is_school_allowed(const Defs::wizard_spell& spellId)
{
    int id = static_cast<int>(spellId);
    auto rsSchool = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell_school, id);
    auto school = rsSchool[0].as<int>("school_id");
    int cls = static_cast<int>(_casterClass);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int, int>(Query::select_school_of_magic_access, cls, school);
    auto hasAccess = rs.size() > 0;

    return hasAccess;
}

Adndtk::SpellLevel Adndtk::SpellBook::get_spell_level(const Defs::wizard_spell& spellId)
{
    int id = static_cast<int>(spellId);

    auto rsSpellInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_wizard_spell, id);
    auto& spellInfo = rsSpellInfo[0];
    SpellLevel spellLevel = static_cast<SpellLevel>(spellInfo.as<int>("level"));
    
    return spellLevel;
}

bool Adndtk::SpellBook::is_level_available(const Defs::wizard_spell& spellId)
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);

    return spellLevel <= stats.spell_level;
}

bool Adndtk::SpellBook::exists(const Defs::wizard_spell& spellId)
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    return _spells[spellLevel].find(spellId) != _spells[spellLevel].end();
}

bool Adndtk::SpellBook::try_scribe(const Defs::wizard_spell& spellId)
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);

    Die d{Defs::die::d100};
    return stats.chance_to_learn_spell > d;
}