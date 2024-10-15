#include <character_generator.h>
#include <skill_creator.h>
#include <cyclopedia.h>

Adndtk::CharacterGenerator::CharacterGenerator()
{
}

Adndtk::CharacterGenerator::~CharacterGenerator()
{
}

std::vector<Adndtk::Tables::race> Adndtk::CharacterGenerator::available_races(const SkillValue& strength, const SkillValue& dexterity, const SkillValue& constitution, const SkillValue& intelligence, const SkillValue& wisdom, const SkillValue& charisma)
{
    std::map<Defs::skill, SkillValue> skillValues{
        {Defs::skill::strength, strength},
        {Defs::skill::dexterity, dexterity},
        {Defs::skill::constitution, constitution},
        {Defs::skill::intelligence, intelligence},
        {Defs::skill::wisdom, wisdom},
        {Defs::skill::charisma, charisma},
    };

    return available_races(skillValues[Defs::skill::strength], skillValues[Defs::skill::dexterity],
                        skillValues[Defs::skill::constitution], skillValues[Defs::skill::intelligence],
                        skillValues[Defs::skill::wisdom], skillValues[Defs::skill::charisma]);
}

std::vector<Adndtk::Tables::race> Adndtk::CharacterGenerator::available_races(const std::map<Defs::skill, SkillValue>& skillValues)
{
    auto query = Query::select_skill_boundaries_class;
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

    return available_classes(skillValues[Defs::skill::strength], skillValues[Defs::skill::dexterity],
                        skillValues[Defs::skill::constitution], skillValues[Defs::skill::intelligence],
                        skillValues[Defs::skill::wisdom], skillValues[Defs::skill::charisma],
                        raceId);
}

std::vector<Adndtk::Tables::character_class> Adndtk::CharacterGenerator::available_classes(const std::map<Defs::skill, SkillValue>& skillValues, const Defs::race& raceId)
{
    auto query = Adndtk::Query::select_skill_boundaries_class;
    std::vector<Adndtk::Tables::character_class> availableClasses{};
    auto classes = Tables::character_class::fetch_all();
    auto classesPerRace = Cyclopedia::get_instance().exec_prepared_statement<int>(
                                Query::select_class_availability_per_race,
                                static_cast<int>(raceId)).to_set<int>("class_id");

    for (auto& c : classes)
    {
        std::set<Defs::skill> sklSet{Defs::skill::strength, Defs::skill::dexterity,
                                            Defs::skill::constitution, Defs::skill::intelligence,
                                            Defs::skill::wisdom, Defs::skill::charisma};
        if (classesPerRace.find(c.id) != classesPerRace.end())
        {
            for (auto s : skillValues)
            {
                int minValue{0};
                int maxValue{20};
                SkillCreator::get_skill_constraints(query, s.first, c.id, minValue, maxValue);
                if (s.second >= minValue && s.second <= maxValue)
                {
                    sklSet.erase(s.first);
                    if (sklSet.empty())
                    {
                        availableClasses.push_back(c);
                    }
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
                                static_cast<int>(classId)).to_set<int>("id");

    std::vector<Adndtk::Tables::moral_alignment> avalableAligns{};
    for (auto& a : allAligns)
    {
        if (alignsPerClass.find(a.id) == alignsPerClass.end())
        {
            avalableAligns.push_back(std::move(a));
        }
    }
    return avalableAligns;
}