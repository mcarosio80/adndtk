#include <spell_book.h>
#include <skill_stats.h>
#include <dice.h>
#include <algorithm>
#include <config.h>
#include <cyclopedia.h>

#include <options.h>

Adndtk::SpellBook::SpellBook(const Defs::character_class& cls, Defs::race raceId)
    : _casterLevel{1}, _intelligenceScore{Defs::skill::intelligence, 9}, _casterClass{cls}, _casterRace{raceId}
{
    if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(cls))
    {
        auto races = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class, Defs::race>(
            Query::select_school_of_magic_per_race, _casterClass, _casterRace
        );
        
        bool raceEnabled{true};
        for (auto& rc : races)
        {
            auto availableRace = rc.as<Defs::race>("race_id");
            raceEnabled &= availableRace == raceId;
        }
        if (races.size() == 0 || !raceEnabled)
        {
            ErrorManager::get_instance().error("Class not available for specified race");
        }
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
    if (_casterClass == Defs::character_class::bard
        && _casterLevel == 1 && newLevel == 2)
    {
        auto numSpells = Die::roll(1, 4);
        auto rsSpells = Cyclopedia::get_instance().exec_prepared_statement<ExperienceLevel>(Query::select_wizard_spells_by_level, 1);
        while (numSpells > 0)
        {
            auto id = Die::roll<size_t>(0, rsSpells.size()-1) + Const::spell_offset_level_1;
            auto splId = static_cast<Defs::wizard_spell>(id);
            scribe_scroll(splId);
            --numSpells;
        }
    }
    _casterLevel = newLevel;
    
    Query queryId = get_spell_progression_query();
    auto lvl = std::min<ExperienceLevel>(_casterLevel, Const::max_character_level);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<ExperienceLevel>(queryId, lvl);
    auto& spellProgr = rs[0];

    for (auto& page : _spells)
    {
        std::stringstream ss;
        ss << "spell_level_" << page.first;
        auto spellsAvailable = spellProgr.try_or<short>(ss.str(), 0);

        if (spellsAvailable == 0)
        {
            page.second.clear();
        }
        else
        {
            auto spellsInExcess = std::max<short>(0, used_slots(page.first) - spellsAvailable);
            while (spellsInExcess > 0)
            {
                page.second.erase(page.second.begin());
                --spellsInExcess;
            }
        }
    }
}

void Adndtk::SpellBook::set_caster_intelligence(const short& intelligenceValue)
{
    _intelligenceScore = intelligenceValue;
    
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);
    auto maxLevel = static_cast<SpellLevel>(stats.spell_level.value_or(0));

    for (auto& page : _spells)
    {
        if (page.first > maxLevel)
        {
            page.second.clear();
        }
        else
        {
            auto pageSize = static_cast<short>(stats.max_number_of_spells_per_level.value_or(0));
            auto diff = std::max<short>(0, page.second.size() - pageSize);
            while (diff > 0)
            {
                page.second.erase(page.second.begin());
                --diff;
            }
        }
    }
}

Adndtk::Query Adndtk::SpellBook::get_spell_progression_query() const
{
    Query queryId{Query::select_wizard_spell_progression};
    if (_casterClass == Defs::character_class::bard)
    {
        queryId = Query::select_bard_spell_progression;
    }
    return queryId;
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

    bool scribeAlwaysSucceed = OptionalRules::get_instance().get_option<bool>(Option::scribe_scroll_always_succeeds);
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
    return total_slots(spellLevel) - used_slots(spellLevel);
}

short Adndtk::SpellBook::used_slots(const SpellLevel& spellLevel) const
{
    if (_spells.find(spellLevel) == _spells.end())
    {
        return 0;
    }

    short used{0};
    for (auto& s : _spells.at(spellLevel))
    {
        used += s.second;
    }
    return used;
}

short Adndtk::SpellBook::total_slots(const SpellLevel& spellLevel) const
{
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);
    if (spellLevel > stats.spell_level.value_or(0))
    {
        return 0;
    }

    return get_spells_available(spellLevel);
}

short Adndtk::SpellBook::book_page_size() const
{
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);
    return stats.max_number_of_spells_per_level.value_or(0);
}

bool Adndtk::SpellBook::memorise(const Defs::wizard_spell& spellId)
{
    if (!exists(spellId))
    {
        return false;
    }

    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    auto spellPerLevel = get_spells_available(spellLevel);

    if (spellPerLevel == 0)
    {
        return false;
    }

    auto count = used_slots(spellLevel);
    if (spellPerLevel <= count)
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
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    short spellCount{0};
    if (_spells.find(spellLevel) != _spells.end())
    {
        spellCount = _spells.at(spellLevel).size();
    }
    return book_page_size() > spellCount;
}

bool Adndtk::SpellBook::is_school_allowed(const Defs::wizard_spell& spellId) const
{
    auto rsSchool = Cyclopedia::get_instance().exec_prepared_statement<Defs::wizard_spell>(Query::select_wizard_spell_school, spellId);

    for (auto& r : rsSchool)
    {
        auto school = r.as<Defs::school_of_magic>("school_id");
        auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class, Defs::school_of_magic>(Query::select_school_of_magic_access, _casterClass, school);
        if (rs.size() == 0)
        {
            return false;
        }
    }

    return true;
}

Adndtk::SpellLevel Adndtk::SpellBook::get_spell_level(const Defs::wizard_spell& spellId)
{
    auto rsSpellInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::wizard_spell>(Query::select_wizard_spell, spellId);
    auto& spellInfo = rsSpellInfo[0];
    SpellLevel spellLevel = spellInfo.as<SpellLevel>("level");
    
    return spellLevel;
}

bool Adndtk::SpellBook::is_level_available(const Defs::wizard_spell& spellId) const
{
    SpellLevel spellLevel = SpellBook::get_spell_level(spellId);
    auto stats = SkillStats::get_instance().get_intelligence_stats(_intelligenceScore);

    auto lvl = std::min<ExperienceLevel>(_casterLevel, Const::max_character_level);
    Query queryId = get_spell_progression_query();
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<ExperienceLevel>(queryId, lvl);
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

short Adndtk::SpellBook::get_spells_available(const SpellLevel& spellLvl) const
{
    auto lvl = std::min<ExperienceLevel>(_casterLevel, Const::max_character_level);
    Query queryId = get_spell_progression_query();
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<ExperienceLevel>(queryId, lvl);
    auto& prog = rs[0];

    std::stringstream ss;
    ss << "spell_level_" << spellLvl;
    std::string fieldName = ss.str();
    auto spellCount = prog.try_or<short>(fieldName, 0);
    return spellCount;
}