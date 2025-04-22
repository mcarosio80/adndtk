#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <optional>
#include <set>

#include <common_types.h>
#include <defs.h>
#include <skills.h>
#include <avatar.h>
#include <dice.h>
#include <cyclopedia.h>

namespace Adndtk
{
    class Monster : public Avatar
    {
    public:
        Monster(const Defs::monster& monsterId);

        Avatar::Type get_avatar_type() const override
        {
            return Avatar::Type::Monster;
        };
        
    private:
        Defs::monster                               _id;
        std::string                                 _name;
        std::optional<Defs::encounter_frequency>    _frequencyId;
        std::optional<SkillValue>                   _intelligence;
        std::set<std::pair<Defs::climate, Defs::terrain>>
                                                    _cliamteTerrain;
        std::set<Defs::plane_of_existence>          _planes;
        std::set<Defs::organisation>                _organisation;
        std::optional<Defs::turnable>               _turnedAs;
        std::set<Defs::activity_cycle>              _activityCycle;
        std::set<Defs::diet>                        _diet;
        std::optional<Defs::moral_alignment>        _alignment;
        std::optional<Defs::element>                _element;
        std::optional<Defs::monster_dragon_type>    _dragonType;

        template <typename _FeatureType>
        std::set<_FeatureType> set_multiple_feature(const Defs::monster& id, const Adndtk::Query& q, const std::string& label)
        {
            auto rs = Adndtk::Cyclopedia::get_instance().exec_prepared_statement(q, id);
            std::set<_FeatureType> set{};
            for (const auto& r : rs)
            {
                auto item = r.as<_FeatureType>(label);
                set.emplace(item);
            }
            return set;
        }

        template <typename _FeatureType>
        std::optional<_FeatureType> set_optional_feature(const Defs::monster& id, const Adndtk::Query& q, const std::string& label)
        {
            auto moralAlignments = Adndtk::Cyclopedia::get_instance().exec_prepared_statement(Query::select_monster_alignment, id).to_vector<_FeatureType>(label);
            if (moralAlignments.size() == 0)
            {
                return std::nullopt;
            }
            auto idx = Die::roll<size_t>(0, moralAlignments.size()-1);
            return moralAlignments[idx];
        }

        static std::optional<SkillValue> get_intelligence_score(const std::optional<short>& value_from, const std::optional<short>& value_to);
    };
}

#endif // AVATAR_H