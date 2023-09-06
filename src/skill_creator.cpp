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

void Adndtk::SkillCreator::get_skill_constraints(const Adndtk::Query& queryId, const int& skillType, const std::optional<int>& object, int& minValue, int& maxValue)
{
    Adndtk::QueryResultSet result;
    
    if (object.has_value())
    {
        int obj = object.value();
        result = Cyclopedia::get_instance().exec_prepared_statement<int, int>(queryId, skillType, obj);
    }
    else
    {
        result = Cyclopedia::get_instance().exec_prepared_statement<int>(queryId, skillType);
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

Adndtk::SkillValue Adndtk::SkillCreator::create(const Defs::skill &skillType, const std::optional<Defs::character_class>& cls,
                                const std::optional<Defs::race>& race, const SkillGenerationMethod &method/* = SkillGenerationMethod::standard*/)
{
    int sklValue = static_cast<int>(skillType);
    int minValue{0};
    int maxValue{20};

    // Check default constraints
    Adndtk::Query query = Adndtk::Query::select_skill_boundaries_default;
    get_skill_constraints(query, sklValue, std::nullopt, minValue, maxValue);

    Defs::character_class_type clsType;

    if (cls.has_value())
    {
        auto classes = Cyclopedia::get_instance().split<Defs::character_class>(cls.value());
        for (auto& c : classes)
        {
            int clsId = static_cast<int>(c);

            query = Adndtk::Query::select_character_class;
            auto result = Cyclopedia::get_instance().exec_prepared_statement<int>(query, clsId);

            if (result.size() > 0 && result[0]["class_type_id"].has_value())
            {
                clsType = static_cast<Defs::character_class_type>(result[0].as<int>("class_type_id"));
            }

            auto clsTypes = Cyclopedia::get_instance().split<Defs::character_class_type>(clsType);
            query = Adndtk::Query::select_skill_boundaries_class_type;
            for (auto& t : clsTypes)
            {
                get_skill_constraints(query, sklValue, static_cast<int>(t), minValue, maxValue);
            }

            // check class boundaries
            query = Adndtk::Query::select_skill_boundaries_class;
            get_skill_constraints(query, sklValue, clsId, minValue, maxValue);

            // // check school of magic requisites
            // query = Adndtk::Query::select_school_of_magic_skill_requisite;
            // auto somRequisite = Cyclopedia::get_instance().exec_prepared_statement<int, int>(query, sklValue, clsId);
            // if (somRequisite.size() > 0 && somRequisite[0]["skill_value_required"].has_value())
            // {
            //     auto somReqVal = somRequisite[0].as<int>("skill_value_required");
            //     minValue = std::max(minValue, somReqVal);
            // }
        }
    }

    short raceSkillModifier = 0;
    if (race.has_value())
    {
        // check race boundaries
        query = Adndtk::Query::select_skill_boundaries_race;
        int raceId = static_cast<int>(race.value());
        get_skill_constraints(query, sklValue, raceId, minValue, maxValue);

        // get race modifier
        query = Query::select_skill_modifier;
        auto raceModifier = Cyclopedia::get_instance().exec_prepared_statement<int, int>(query, raceId, sklValue);

        if (raceModifier.size() > 0 && raceModifier[0]["value"].has_value())
        {
            raceSkillModifier = raceModifier[0].as<short>("value");
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

    if (val == 18
        && skillType == Defs::skill::strength
        && Cyclopedia::get_instance().is_type_of<Defs::character_class_type::warrior>(clsType)
        && race != Defs::race::halfling
        )
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