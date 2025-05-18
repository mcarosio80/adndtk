#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H

#include <map>
#include <array>
#include <vector>
#include <string>
#include <optional>
#include <functional>

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

    class BattlefieldPoint
    {
    public:
        BattlefieldPoint(double x_coord, double y_coord)
            : _x{x_coord}, _y{y_coord}
        {}
        const double& x() const { return _x; }
        const double& y() const { return _y; }

        bool operator==(const BattlefieldPoint& lhs) const
        {
            return _x == lhs.x() && _y == lhs.y();
        }

        bool operator<(const BattlefieldPoint& lhs) const
        {
            return _x < lhs.x() && _y < lhs.y();
        }

        BattlefieldPoint& subtract(const BattlefieldPoint& pt)
        {
            _x -= pt.x();
            _y -= pt.y();
            return *this;
        }

        BattlefieldPoint& add(const BattlefieldPoint& pt)
        {
            _x += pt.x();
            _y += pt.y();
            return *this;
        }

        BattlefieldPoint& scale(double scaleFactor)
        {
            _x *= scaleFactor;
            _y *= scaleFactor;
            return *this;
        }

    private:
        double  _x;
        double  _y;
    };

    class BattlefieldDirection
    {
    public:
        BattlefieldDirection(const double xCoord, const double yCoord)
            : _x{xCoord}, _y{yCoord}
        {}
        const double& x() const { return _x; }
        const double& y() const { return _y; }

    private:
        double  _x;
        double  _y;
    };

    struct BattlefieldPlace
    {
        AvatarId avatarId;
        BattlefieldDirection avatarDirection;
        double avatarRadius;
    };

    class Party
    {
    public:
        Party() = default;
        explicit Party(const std::string& partyName);

        const std::string& get_name() const { return _name; }

        AvatarId add_character(const std::string& name, const Defs::character_class& cls,
            const Defs::race& raceId, const Defs::moral_alignment& align, const Defs::sex& sexId,
            const std::optional<Defs::deity>& deityId);

        AvatarId add_monster(const Defs::monster& monsterId, const std::string& monsterName = "");

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

    class Battlefield
    {
    public:
        Battlefield() = default;
        explicit Battlefield(const std::string& key);

        const std::string& get_name() const { return _name; }
        Party& get_party(const std::string& partyName);
        const Party& get_party(const std::string& partyName) const;
        bool has_party(const std::string& partyName) const;

        size_t count_parties() const { return _parties.size(); }

        AvatarId add_character(const std::string& name, const Defs::character_class& cls,
            const Defs::race& raceId, const Defs::moral_alignment& align, const Defs::sex& sexId,
            const std::optional<Defs::deity>& deityId,
            const BattlefieldPoint& coords,
            const std::optional<double>& radius = std::nullopt,
            const std::optional<BattlefieldDirection>& direction = std::nullopt);
        
        AvatarId add_monster(const Defs::monster& monsterId, const std::string& partyName, const BattlefieldPoint& coords, const std::optional<double>& radius=std::nullopt, const std::optional<BattlefieldDirection>& direction = std::nullopt, const std::string& uniqueMonsterName = "");
        AvatarId add_monster(const Defs::monster& monsterId, const BattlefieldPoint& coords, const std::optional<double>& radius=std::nullopt, const std::optional<BattlefieldDirection>& direction=std::nullopt, const std::string& uniqueMonsterName="");
        
        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId, const std::string& partyName, const std::array<BattlefieldPoint, _count>& coords,
                        const std::array<double, _count>& radius)
        {
            for (unsigned short c=0; c<_count; ++c)
            {
                if (detect_collision(coords[c], radius[c]))
                {
                    std::stringstream ss{};
                    ss << "Coordinates " << coords[c].x() << "," << coords[c].y() << " are currently held";
                    throw std::runtime_error(ss.str());
                }
            }

            auto& p = get_party(partyName);
            for (unsigned short c=0; c<_count; ++c)
            {
                auto id = p.add_monster(monsterId);
                place_avatar(id, coords[c], radius[c], Battlefield::to_centre(coords[c]));
            }
        }

        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId, const std::string& partyName,
                        const std::array<BattlefieldPoint, _count>& coords,
                        const std::array<double, _count>& radius, const BattlefieldDirection& direction
                        )
        {
            for (unsigned short c=0; c<_count; ++c)
            {
                if (detect_collision(coords[c], radius[c]))
                {
                    std::stringstream ss{};
                    ss << "Coordinates " << coords[c].x() << "," << coords[c].y() << " are currently held";
                    throw std::runtime_error(ss.str());
                }
            }

            auto& p = get_party(partyName);
            for (unsigned short c=0; c<_count; ++c)
            {
                auto id = p.add_monster(monsterId, coords[c]);
                place_avatar(id, coords[c], radius[c], direction);
            }
        }

        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId, const std::array<BattlefieldPoint, _count>& coords,
                        const std::array<double, _count>& radius)
        {
            add_monsters<_count>(monsterId, "", coords, radius, Battlefield::to_centre(coords));
        }

        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId, const std::array<BattlefieldPoint, _count>& coords,
                        const std::array<double, _count>& radius, const BattlefieldDirection& direction)
        {
            add_monsters<_count>(monsterId, "", coords, radius, direction);
        }

        size_t count_avatars() const;
        size_t count_avatars(const Adndtk::Avatar::Type& type) const;
        size_t count_avatars(const std::string& partyName) const;
        size_t count_avatars(const std::string& partyName, const Adndtk::Avatar::Type& type) const;


        void clear();
        void step();

        template <class _ElementType>
        AvatarList<_ElementType> select(const AvatarFilter<_ElementType>& cbk) const
        {
            AvatarList<_ElementType> filteredElements{};
            for (auto& p : _parties)
            {
                if constexpr(std::is_same_v<_ElementType, Monster>)
                {
                    auto m = p.second.monsters(cbk);
                    filteredElements.insert(filteredElements.end(),
                            std::make_move_iterator(m.begin()),
                            std::make_move_iterator(m.end()));
                }
                else if constexpr(std::is_same_v<_ElementType, Character>)
                {
                    auto m = p.second.characters(cbk);
                    filteredElements.insert(filteredElements.end(),
                            std::make_move_iterator(m.begin()),
                            std::make_move_iterator(m.end()));
                }
            }
            return filteredElements;
        }

        PartyList select() const;
        PartyList select(const PartyFilter& cbk) const;

        static BattlefieldPoint origin;
        static double defaultRadius;

    private:
        std::string                                     _name;
        std::map<std::string, Party>                    _parties;
        std::map<BattlefieldPoint, BattlefieldPlace>    _coordinates;

        bool detect_collision(const BattlefieldPoint& coord, const double radius) const;
        bool place_avatar(const Adndtk::AvatarId id, const BattlefieldPoint& coord, 
                        const double radius, const BattlefieldDirection& direction);

        static double distance(const BattlefieldPoint& pt1, const BattlefieldPoint& pt2);
        static bool intersect(const BattlefieldPoint& centre1, double radius1, const BattlefieldPoint& centre2, double radius2);
        static BattlefieldDirection to_centre(const BattlefieldPoint& coords);
    };

    class Battleground
    {
    public:
        static Battleground& get_instance();
        Battleground(Battleground const&) = delete;
        void operator=(Battleground const&) = delete;

        bool has_battlefield(const std::string& fieldName);
        void add_battlefield(const std::string& fieldName);
        Battlefield& get_battlefield(const std::string& fieldName);
        std::optional<Battlefield> try_get_battlefield(const std::string& fieldName) const;
        size_t size() const { return _fields.size(); };
        void clear();

    private:
        Battleground();
        ~Battleground();

		static bool                         _initialised;
        std::map<std::string, Battlefield>  _fields;
    };
}

#endif // CALENDAR_H