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

std::pair<int, int> Adndtk::CharacterGenerator::get_skill_constraints_by_class_type(const Defs::character_class_type& clsType, const Defs::skill& skillId)
{
    auto types = Cyclopedia::get_instance().split<Defs::character_class_type>(clsType);
    std::pair<int, int> clsTypeLimits{1, 20};

    for (auto& t : types)
    {
        auto lim = SkillCreator::get_skill_constraints(Query::select_skill_boundaries_class_type,
                                                            skillId,
                                                            static_cast<int>(t));
        clsTypeLimits.first = std::max(clsTypeLimits.first, lim.first);
        clsTypeLimits.second = std::min(clsTypeLimits.second, lim.second);
    }
    return clsTypeLimits;
}

std::pair<int, int> Adndtk::CharacterGenerator::get_skill_constraints_by_class(const Defs::character_class& clsId, const Defs::skill& skillId)
{
    auto classes = Cyclopedia::get_instance().split<Defs::character_class>(clsId);
    std::pair<int, int> clsLimits{1, 20};

    for (auto& cls : classes)
    {
        auto lim = SkillCreator::get_skill_constraints(Query::select_skill_boundaries_class,
                                                            skillId,
                                                            static_cast<int>(cls));
        clsLimits.first = std::max(clsLimits.first, lim.first);
        clsLimits.second = std::min(clsLimits.second, lim.second);

        if (Cyclopedia::get_instance().is_specialist_wizard(cls))
        {
            auto res = Cyclopedia::get_instance().exec_prepared_statement(Query::select_school_of_magic_skill_requisite, static_cast<int>(cls));
            if (res.size() > 0)
            {
                auto& schoolLimits = res[0];
                if(schoolLimits.as<Defs::skill>("skill_id") == skillId)
                {
                    clsLimits.first = std::max(clsLimits.first, schoolLimits.as<int>("skill_value_required"));
                }
            }
        }
    }
    return clsLimits;
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

            auto clsTypeLimits = get_skill_constraints_by_class_type(
                                        static_cast<Defs::character_class_type>(c.class_type_id),
                                        s.first
            );
            skillLimitMin.push_back(clsTypeLimits.first);
            skillLimitMax.push_back(clsTypeLimits.second);
            
            auto clsLimits = get_skill_constraints_by_class(static_cast<Defs::character_class>(c.id), s.first);
            skillLimitMin.push_back(clsLimits.first);
            skillLimitMax.push_back(clsLimits.second);
            
            auto raceLimits = SkillCreator::get_skill_constraints(Query::select_skill_boundaries_race, s.first, static_cast<int>(raceId));
            skillLimitMin.push_back(raceLimits.first);
            skillLimitMax.push_back(raceLimits.second);
            
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

std::vector<Adndtk::Tables::moral_alignment> Adndtk::CharacterGenerator::available_moral_alignments(const Defs::character_class& classId, const std::optional<Defs::deity>& deityId/*=std::nullopt*/)
{
    std::set<Defs::moral_alignment> avalableAligns{};
    auto allAligns = Tables::moral_alignment::to_map<Defs::moral_alignment>("id");

    auto classes = Cyclopedia::get_instance().split<Defs::character_class>(classId);
    for (auto& c : classes)
    {
        std::set<Defs::moral_alignment> alignsPerClass{};
        if (c == Defs::character_class::preist_of_specific_mythos && deityId.has_value())
        {
            auto id = static_cast<int>(deityId.value());
            alignsPerClass = Cyclopedia::get_instance().exec_prepared_statement<int>(
                                Query::select_worshipper_alignments,
                                id).to_set<Defs::moral_alignment>("moral_alignment");
        }
        else
        {
            alignsPerClass = Cyclopedia::get_instance().exec_prepared_statement<int>(
                                Query::select_moral_alignments_by_class,
                                static_cast<int>(c)).to_set<Defs::moral_alignment>("alignment_id");
        }
        if (avalableAligns.empty())
        {
            avalableAligns.insert(alignsPerClass.begin(), alignsPerClass.end());
        }
        else
        {
            std::set<Defs::moral_alignment> intersect;
            std::set_intersection(alignsPerClass.begin(), alignsPerClass.end(), avalableAligns.begin(), avalableAligns.end(),
                 std::inserter(intersect, intersect.begin()));
            avalableAligns = intersect;
        }
    }

    std::vector<Tables::moral_alignment> res{};
    
    for (auto& a : avalableAligns)
    {
        res.push_back(allAligns[a]);
    }
    return res;
}

std::set<Adndtk::Defs::moral_alignment> Adndtk::CharacterGenerator::available_moral_alignment_ids(const Defs::character_class& cls, const std::optional<Defs::deity>& deityId/*=std::nullopt*/)
{
    std::set<Defs::moral_alignment> alignments{};
    auto aligns = available_moral_alignments(cls, deityId);
    for (auto& a : aligns)
    {
        alignments.emplace(static_cast<Defs::moral_alignment>(a.id));
    }
    return alignments;
}

std::vector<Adndtk::Tables::sex> Adndtk::CharacterGenerator::available_sex()
{
    return Adndtk::Tables::sex::fetch_all();
}

std::vector<Adndtk::Tables::deity> Adndtk::CharacterGenerator::available_deities(const Defs::moral_alignment& align, const bool filterDeadGods/*=false*/)
{
    auto allDeities = Tables::deity::to_map<Defs::deity>("id");
    auto query = Query::select_deities_by_moral_alignment;
    auto alignId = static_cast<int>(align);
    std::vector<Tables::deity> availableDeities{};

    auto deities = Cyclopedia::get_instance().exec_prepared_statement<int>(query, alignId).to_set<Defs::deity>("deity_id");
    for (auto& d : allDeities)
    {
        if (deities.contains(d.first))
        {
            std::cout << d.second.name << " : " << d.second.activity << "\n";
            std::cout << std::boolalpha << "filterDeadGods: " << filterDeadGods << "\n";
            std::cout << "d.second.activity: " << d.second.activity << "\n";
            if (!filterDeadGods || static_cast<Defs::deity_activity>(d.second.activity) == Defs::deity_activity::alive)
            {
                availableDeities.push_back(std::move(d.second));
            }
        }
    }
    return availableDeities;
}

std::set<Adndtk::Defs::deity> Adndtk::CharacterGenerator::available_deity_ids(const Defs::moral_alignment& align)
{
    std::set<Adndtk::Defs::deity> deities;
    auto alignId = static_cast<int>(align);
    auto rsDeities = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_deities_by_moral_alignment, alignId);
    for (auto& d : rsDeities)
    {
        auto deityId = static_cast<Defs::deity>(d.as<int>("deity_id"));
        deities.emplace(deityId);
    }
        
    return deities;
}