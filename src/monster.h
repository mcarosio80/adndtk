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

        constexpr Avatar::Type get_avatar_type() const override
        {
            return Avatar::Type::Monster;
        };
        const auto& get_id() const { return _id; }
        const auto& get_name() const { return _name; }
        const auto& get_full_name() const { return _fullName; }
        const auto& get_frequency() const { return _frequencyId; }
        const auto& get_intelligence() const { return _intelligence; }

        const auto& get_climate_terrain() const { return _cliamteTerrain; }
        bool lives_in(const Defs::climate& climateId) const
        {
            for (const auto& p : _cliamteTerrain)
            {
                if (p.first == climateId)
                {
                    return true;
                }
            }
            return false;
        }
        bool lives_in(const Defs::terrain& terrainId) const
        {
            for (const auto& p : _cliamteTerrain)
            {
                if (p.second == terrainId)
                {
                    return true;
                }
            }
            return false;
        }

        const auto& get_planes() const { return _planes; }
        bool lives_in(const Defs::plane_of_existence& id) const
        {
            return _planes.find(id) != _planes.end();
        }

        const auto& get_organisation() const { return _organisation; }
        bool lives_in(const Defs::organisation& id) const
        {
            return _organisation.find(id) != _organisation.end();
        }

        const auto& get_activity_cycle() const { return _activityCycle; }
        bool active_at(const Defs::activity_cycle& id) const
        {
            return _activityCycle.find(id) != _activityCycle.end();
        }

        const auto& get_diet() const { return _diet; }
        bool eats(const Defs::diet& id) const
        {
            return _diet.find(id) != _diet.end();
        }

        bool is_undead() const { return _turnedAs.has_value(); }
        std::optional<Defs::turnable> turned_as() const { return _turnedAs; }
        std::optional<Defs::moral_alignment> moral_alignment() const { return _alignment; }
        bool is_elemental() const { return _element.has_value(); }
        std::optional<Defs::element> element() const { return _element; }
        bool is_dragon() const { return _dragonType.has_value(); }
        std::optional<Defs::monster_dragon_type> dragon_type() const { return _dragonType; }
        
    private:
        Defs::monster                               _id;
        std::string                                 _name;
        std::string                                 _fullName;
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
        std::set<_FeatureType> set_multiple_feature(const Defs::monster& id, const Adndtk::Query& queryId, const std::string& label)
        {
            auto rs = Adndtk::Cyclopedia::get_instance().exec_prepared_statement<Defs::monster>(queryId, id);
            std::set<_FeatureType> set{};
            for (const auto& r : rs)
            {
                auto item = r.as<_FeatureType>(label);
                set.emplace(item);
            }
            return set;
        }

        template <typename _FeatureType>
        std::optional<_FeatureType> set_optional_feature(const Defs::monster& id, const Adndtk::Query& queryId, const std::string& label)
        {
            auto rs = Adndtk::Cyclopedia::get_instance().exec_prepared_statement<Defs::monster>(queryId, id).to_vector<_FeatureType>(label);
            if (rs.size() == 0)
            {
                return std::nullopt;
            }
            auto idx = Die::roll<size_t>(0, rs.size()-1);
            return rs[idx];
        }

        static double get_treasure_multiplier(const std::optional<double>& multiplier, const std::optional<short>& diceNumber, const std::optional<Defs::die>& dieFaces);
        static std::optional<SkillValue> get_intelligence_score(const std::optional<short>& value_from, const std::optional<short>& value_to);
    };
}

#endif // AVATAR_H