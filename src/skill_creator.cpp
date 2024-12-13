#include <skill_creator.h>
#include <dice.h>
#include <query_result.h>
#include <cyclopedia.h>

Adndtk::SkillCreator::SkillCreator()
    : _class{std::nullopt}, _race{std::nullopt}
{
}

Adndtk::SkillCreator::SkillCreator(const std::optional<Adndtk::Defs::character_class>& characterClass,
                                    const std::optional<Adndtk::Defs::race>& characterRace)
    : _class{characterClass}, _race{characterRace}
{
}

void Adndtk::SkillCreator::get_skill_constraints(const Adndtk::Query& queryId, const Defs::skill& skillType, const std::optional<int>& object, int& minValue, int& maxValue)
{
    Adndtk::QueryResultSet result;
    
    if (object.has_value())
    {
        int obj = object.value();
        result = Cyclopedia::get_instance().exec_prepared_statement<Defs::skill, int>(queryId, skillType, obj);
    }
    else
    {
        result = Cyclopedia::get_instance().exec_prepared_statement<Defs::skill>(queryId, skillType);
    }

    if (result.size() == 0)
        return;

    if (result[0]["min"].has_value())
    {
        auto v = result[0].as<int>("min");
        if (v > minValue) minValue = v;
    }
    if (result[0]["max"].has_value())
    {
        auto v = result[0].as<int>("max");
        if (v < maxValue) maxValue = v;
    }
}

std::pair<int, int> Adndtk::SkillCreator::get_skill_constraints(const Adndtk::Query& queryId, const Defs::skill& skillType, const std::optional<int>& object)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};
    get_skill_constraints(queryId, skillType, object, minValue, maxValue);
    return std::make_pair(minValue, maxValue);
}

std::pair<int, int> Adndtk::SkillCreator::get_default_boundaries(const Defs::skill &skillType)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};
    Adndtk::Query query = Adndtk::Query::select_skill_boundaries_default;
    get_skill_constraints(query, skillType, std::nullopt, minValue, maxValue);

    return std::make_pair(minValue, maxValue);
}

std::pair<int, int> Adndtk::SkillCreator::get_race_boundaries(const Defs::skill &skillType, const Defs::race& race)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};
    auto query = Adndtk::Query::select_skill_boundaries_race;
    auto raceId = static_cast<int>(race);
    get_skill_constraints(query, skillType, raceId, minValue, maxValue);

    return std::make_pair(minValue, maxValue);
}

short Adndtk::SkillCreator::get_race_adjustments(const Defs::skill &skillType, const Defs::race& race)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};
    auto query = Query::select_skill_modifier;
    auto raceModifier = Cyclopedia::get_instance().exec_prepared_statement<Defs::race, Defs::skill>(query, race, skillType);

    short raceSkillModifier{0};
    if (raceModifier.size() > 0 && raceModifier[0]["value"].has_value())
    {
        raceSkillModifier = raceModifier[0].as<short>("value");
    }
    return raceSkillModifier;
}

std::pair<int, int> Adndtk::SkillCreator::get_class_type_boundaries(const Defs::skill &skillType, const Defs::character_class_type& clsType)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};
    auto clsTypes = Cyclopedia::get_instance().split<Defs::character_class_type>(clsType);
    auto query = Adndtk::Query::select_skill_boundaries_class_type;

    for (auto& t : clsTypes)
    {
        get_skill_constraints(query, skillType, static_cast<int>(t), minValue, maxValue);
    }

    return std::make_pair(minValue, maxValue);
}

std::pair<int, int> Adndtk::SkillCreator::get_class_boundaries(const Defs::skill &skillType, const Defs::character_class& cls)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};

    auto query = Adndtk::Query::select_character_class;
    auto result = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(query, cls);
    Defs::character_class_type clsType{};
    if (result.size() > 0 && result[0]["class_type_id"].has_value())
    {
        clsType = result[0].as<Defs::character_class_type>("class_type_id");
    }

    auto clsTypLimits = get_class_type_boundaries(skillType, clsType);
    minValue = std::max(minValue, clsTypLimits.first);
    maxValue = std::max(minValue, clsTypLimits.second);

    // check class boundaries
    query = Adndtk::Query::select_skill_boundaries_class;
    get_skill_constraints(query, skillType, static_cast<int>(cls), minValue, maxValue);

    query = Adndtk::Query::select_school_of_magic_skill_requisite;
    auto res = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(query, cls);
    if (res.size() > 0)
    {
        auto& schoolLimits = res[0];
        if(schoolLimits.as<Defs::skill>("skill_id") == skillType)
        {
            auto somReqVal = schoolLimits.as<int>("skill_value_required");
            minValue = std::max(minValue, somReqVal);
        }
    }

    return std::make_pair(minValue, maxValue);
}

Adndtk::SkillValue Adndtk::SkillCreator::create(const Defs::skill &skillType, const std::optional<Defs::character_class>& cls,
                                const std::optional<Defs::race>& race, const SkillGenerationMethod &method/* = SkillGenerationMethod::standard*/)
{
    int minValue{Const::min_extended_skill_value};
    int maxValue{Const::max_extended_skill_value};

    // Check default constraints
    auto defLimits = SkillCreator::get_default_boundaries(skillType);
    minValue = std::max(minValue, defLimits.first);
    maxValue = std::max(minValue, defLimits.second);

    short raceSkillModifier = 0;
    if (race.has_value())
    {
        // check race boundaries
        auto raceLimits = get_race_boundaries(skillType, race.value());
        minValue = std::max(minValue, raceLimits.first);
        maxValue = std::max(minValue, raceLimits.second);

        // get race modifier
        raceSkillModifier = get_race_adjustments(skillType, race.value());
    }

    if (cls.has_value())
    {
        auto classes = Cyclopedia::get_instance().split<Defs::character_class>(cls.value());
        for (auto& c : classes)
        {
            auto clsLimits = get_class_boundaries(skillType, c);
            minValue = std::max(minValue, clsLimits.first);
            maxValue = std::max(minValue, clsLimits.second);
        }
    }

    // generate exceptional strength
    std::optional<short> excValue = std::nullopt;
    short val = minValue;
    do
    {
        val = generate_value(method);
    }
    while (val < minValue || val > maxValue);
    val += raceSkillModifier;

    if (cls.has_value() && race.has_value()
        && Cyclopedia::get_instance().can_have_exceptional_strength(cls.value(), race.value(), skillType, val))
    {
        Adndtk::Die d{Defs::die::d100};
        excValue = d.roll();
    }

    SkillValue skillValue{skillType, val, excValue};
    return skillValue;
}

Adndtk::SkillValue Adndtk::SkillCreator::create(const Adndtk::Defs::skill& skillType, const Adndtk::SkillGenerationMethod& method/*= Adndtk::SkillGenerationMethod::standard*/)
{
    return create(skillType, _class, _race, method);
}

short Adndtk::SkillCreator::generate_value(const Adndtk::SkillGenerationMethod& method/* = SkillGenerationMethod::standard*/)
{
    Die d{Defs::die::d6};
    short skillValue = 0;

    switch (method)
    {
    case SkillGenerationMethod::standard:
    {
        skillValue = d.roll(3);
        break;
    }
    case SkillGenerationMethod::best_of_each:
    {
        short r1 = d.roll(3);
        short r2 = d.roll(3);
        skillValue = std::max<short>(r1, r2);
        break;
    }
    case SkillGenerationMethod::best_of_four:
    {
        int tmpValue = 0;
        int minValue = 19;
        for (int i=0; i<4; ++i)
        {
            auto v = d.roll();
            if (v < minValue)
            {
                minValue = v;
            }
            tmpValue += v;
        }
        skillValue = tmpValue - minValue;
        break;
    }
    
    default:
        break;
    }

    return skillValue;
}