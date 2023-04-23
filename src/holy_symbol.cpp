#include <holy_symbol.h>
#include <cyclopedia.h>
#include <skill_stats.h>

Adndtk::HolySymbol::HolySymbol(const Defs::character_class& cls)
    : _casterLevel{1}, _wisdomScore{9}, _priestClass{cls}
{
}

Adndtk::HolySymbol::~HolySymbol()
{
}

void Adndtk::HolySymbol::set_priest_level(const ExperienceLevel& newLevel)
{
    _casterLevel = newLevel;
    enable_levels();
}

void Adndtk::HolySymbol::set_priest_wisdom(const short& wisdomValue)
{
    _wisdomScore = wisdomValue;
}

bool Adndtk::HolySymbol::pray_for_spell(const Defs::priest_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);
    
    int lvl = std::max(static_cast<int>(_casterLevel), 20);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_priest_spell_progression, lvl);
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
    int cls = static_cast<int>(_priestClass);
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

bool Adndtk::HolySymbol::is_level_available(const Defs::priest_spell& spellId)
{
    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);
    auto stats = SkillStats::get_instance().get_intelligence_stats(_wisdomScore);

    return spellLevel <= stats.spell_level;
}

bool Adndtk::HolySymbol::exists(const Defs::priest_spell& spellId)
{
    SpellLevel spellLevel = HolySymbol::get_spell_level(spellId);
    return _spells[spellLevel].find(spellId) != _spells[spellLevel].end();
}

void Adndtk::HolySymbol::enable_levels()
{
}