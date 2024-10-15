#include <character_generator.h>
#include <skill_creator.h>
#include <cyclopedia.h>

#include <algorithm>

Adndtk::CharacterGenerator::CharacterGenerator()
{
}

Adndtk::CharacterGenerator::~CharacterGenerator()
{
}

std::vector<Adndtk::Tables::race> Adndtk::CharacterGenerator::available_races(const SkillValue& strength, const SkillValue& dexterity, const SkillValue& constitution, const SkillValue& intelligence, const SkillValue& wisdom, const SkillValue& charisma)
{
    std::map<Defs::skill, SkillValue> skillValues{
        {strength.type(), strength},
        {dexterity.type(), dexterity},
        {constitution.type(), constitution},
        {intelligence.type(), intelligence},
        {wisdom.type(), wisdom},
        {charisma.type(), charisma},
    };

    return available_races(skillValues);
}

std::vector<Adndtk::Tables::race> Adndtk::CharacterGenerator::available_races(const std::map<Defs::skill, SkillValue>& skillValues)
{
    auto query = Query::select_skill_boundaries_race;
    std::vector<Tables::race> availableRaces{};
    auto races = Tables::race::fetch_all();
                                                
    for (auto& r : races)
    {
        std::set<Defs::skill> sklSet{Defs::skill::strength, Defs::skill::dexterity,
                                        Defs::skill::constitution, Defs::skill::intelligence,
                                        Defs::skill::wisdom, Defs::skill::charisma};
        for (auto& s : skillValues)
        {
            int minValue{0};
            int maxValue{20};
            Adndtk::SkillCreator::get_skill_constraints(query, s.first, r.id, minValue, maxValue);
            auto sklValue = skillValues.at(s.first);
            if (sklValue >= minValue && sklValue <= maxValue)
            {
                sklSet.erase(s.first);
                if (sklSet.empty())
                {
                    availableRaces.push_back(r);
                }
            }
        }
    }
    return availableRaces;
}

std::vector<Adndtk::Tables::character_class> Adndtk::CharacterGenerator::available_classes(const SkillValue& strength, const SkillValue& dexterity, const SkillValue& constitution, const SkillValue& intelligence, const SkillValue& wisdom, const SkillValue& charisma, const Defs::race& raceId)
{
    std::map<Defs::skill, SkillValue> skillValues{
        {Defs::skill::strength, strength},
        {Defs::skill::dexterity, dexterity},
        {Defs::skill::constitution, constitution},
        {Defs::skill::intelligence, intelligence},
        {Defs::skill::wisdom, wisdom},
        {Defs::skill::charisma, charisma},
    };

    return available_classes(skillValues, raceId);
}

std::vector<Adndtk::Tables::character_class> Adndtk::CharacterGenerator::available_classes(const std::map<Defs::skill, SkillValue>& skillValues, const Defs::race& raceId)
{
    std::vector<Tables::character_class> availableClasses{};
    auto classes = Tables::character_class::to_map<int>("id");
    auto classesPerRace = Cyclopedia::get_instance().exec_prepared_statement<int>(
                                Query::select_class_availability_per_race,
                                static_cast<int>(raceId)).to_set<int>("class_id");

    for (auto& clsId : classesPerRace)
    {
        std::set<Defs::skill> sklSet{Defs::skill::strength, Defs::skill::dexterity,
                                            Defs::skill::constitution, Defs::skill::intelligence,
                                            Defs::skill::wisdom, Defs::skill::charisma};
        auto& c = classes[clsId];

        for (auto s : skillValues)
        {
            std::vector<int> skillLimitMin{0};
            std::vector<int> skillLimitMax{20};

            auto clsTypeLimits = SkillCreator::get_skill_constraints(Query::select_skill_boundaries_class_type, s.first, c.class_type_id);
            skillLimitMin.push_back(clsTypeLimits.first);
            skillLimitMax.push_back(clsTypeLimits.second);
            
            auto clsLimits = SkillCreator::get_skill_constraints(Query::select_skill_boundaries_class, s.first, c.id);
            skillLimitMin.push_back(clsLimits.first);
            skillLimitMax.push_back(clsLimits.second);
            
            auto raceLimits = SkillCreator::get_skill_constraints(Query::select_skill_boundaries_race, s.first, static_cast<int>(raceId));
            skillLimitMin.push_back(raceLimits.first);
            skillLimitMax.push_back(raceLimits.second);
            
            auto res = Cyclopedia::get_instance().exec_prepared_statement(Query::select_school_of_magic_skill_requisite, c.id);
            if (res.size() > 0)
            {
                auto& schoolLimits = res[0];
                if(schoolLimits.as<Defs::skill>("skill_id") == s.first)
                {
                    skillLimitMin.push_back(schoolLimits.as<int>("skill_value_required"));
                }
            }

            auto minLimit = *std::max_element(skillLimitMin.begin(), skillLimitMin.end());
            auto maxLimit = *std::min_element(skillLimitMax.begin(), skillLimitMax.end());
            if (s.second >= minLimit && s.second <= maxLimit)
            {
                sklSet.erase(s.first);
                if (sklSet.empty())
                {
                    availableClasses.push_back(c);
                }
            }
        }
    }
    return availableClasses;
}

std::vector<Adndtk::Tables::moral_alignment> Adndtk::CharacterGenerator::available_moral_alignments(const Defs::character_class& classId)
{
    auto allAligns = Tables::moral_alignment::fetch_all();
    auto alignsPerClass = Cyclopedia::get_instance().exec_prepared_statement<int>(
                                Query::select_moral_alignments_by_class,
                                static_cast<int>(classId)).to_set<int>("alignment_id");

    std::vector<Adndtk::Tables::moral_alignment> avalableAligns{};
    for (auto& a : allAligns)
    {
        if (alignsPerClass.find(a.id) != alignsPerClass.end())
        {
            avalableAligns.push_back(std::move(a));
        }
    }
    return avalableAligns;
}

std::vector<Adndtk::Tables::sex> Adndtk::CharacterGenerator::available_sex()
{
    return Adndtk::Tables::sex::fetch_all();
}