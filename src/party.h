#ifndef PARTY_H
#define PARTY_H

#include <map>
#include <vector>
#include <string>
#include <optional>
#include <functional>
#include <string_view>

#include <defs.h>
#include <avatar.h>
#include <character.h>
#include <monster.h>

namespace Adndtk
{
    using AvatarId = uint32_t;

    template <class _ElementType>
    using AvatarFilter = std::function<bool(const _ElementType&)>;

    template <class _ElementType>
    struct AvatarEntry
    {
        std::reference_wrapper<_ElementType> avatar;
        AvatarId id;
    };
    
    template <class _ElementType>
    using AvatarList = std::vector<AvatarEntry<_ElementType>>;

    class Party
    {
    public:
        Party() = default;
        explicit Party(std::string_view partyName);

        const std::string& get_name() const { return _name; }

        AvatarId add_character(std::string_view name, const Defs::character_class& cls,
            const Defs::race& raceId, const Defs::moral_alignment& align, const Defs::sex& sexId,
            const std::optional<Defs::deity>& deityId);

        AvatarId add_monster(const Defs::monster& monsterId, std::string_view monsterName = "");

        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId)
        {
            for (unsigned short c=0; c<_count; ++c)
            {
                add_monster(monsterId);
            }
        }

        inline size_t count_characters() const { return _characters.size(); }
        inline size_t count_monsters() const { return _monsters.size(); }
        inline size_t size() const { return count_characters() + count_monsters(); }
        void clear();

        AvatarList<Monster> monsters() const
        {
            auto f = [](const Monster& m){ return true; };
            return monsters(f);
        }

        AvatarList<Monster> monsters(const AvatarFilter<Monster>& cbk) const
        {
            AvatarList<Monster> res{};
            for (auto& m : _monsters)
            {
                if (cbk(m.second))
                {
                    std::reference_wrapper<Monster> r = const_cast<Monster&>(m.second);
                    res.push_back({r, m.first});
                }
            }
            return std::move(res);
        }

        AvatarList<Character> characters() const
        {
            auto f = [](const Character& m){ return true; };
            return characters(f);
        }

        AvatarList<Character> characters(const AvatarFilter<Character>& cbk) const
        {
            AvatarList<Character> res{};
            for (auto& m : _characters)
            {
                if (cbk(m.second))
                {
                    std::reference_wrapper<Character> r = const_cast<Character&>(m.second);
                    res.push_back({r, m.first});
                }
            }
            return std::move(res);
        }

    private:
        static AvatarId                 _monsterKeyOffset;
        static AvatarId                 _characterKeyOffset;
        std::string                     _name;
        std::map<AvatarId, Character>   _characters;
        std::map<AvatarId, Monster>     _monsters;
    };

    using PartyFilter = std::function<bool(const Party&)>;
    using PartyList = std::vector<std::reference_wrapper<Party>>;
}

#endif // PARTY_H