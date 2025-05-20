#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <map>
#include <array>
#include <string>
#include <optional>
#include <valarray>

#include <defs.h>
#include <party.h>

namespace Adndtk
{
    class Battlefield
    {
    public:
        class Point
        {
        public:
            Point(const double xCoord, const double yCoord)
                : _coords{xCoord, yCoord}
            {}
            const double& x() const { return _coords[XComponent]; }
            const double& y() const { return _coords[YComponent]; }
            double& x() { return _coords[XComponent]; }
            double& y() { return _coords[YComponent]; }

            bool operator==(const Point& lhs) const
            {
                return x() == lhs.x() && y() == lhs.y();
            }

            bool operator<(const Point& lhs) const
            {
                return x() < lhs.x() && y() < lhs.y();
            }

            Point& move(const double xOffset, const double yOffset)
            {
                x() -= xOffset;
                y() -= yOffset;
                return *this;
            }

            Point& scale(const double scaleFactor)
            {
                x() *= scaleFactor;
                y() *= scaleFactor;
                return *this;
            }

        private:
            std::valarray<double> _coords;
        };

        class Direction
        {
        public:
            Direction(const double xOffset, const double yOffser)
                : _offsets{xOffset, yOffser}
            {}
            const double& x() const { return _offsets[XComponent]; }
            const double& y() const { return _offsets[YComponent]; }
            double& x() { return _offsets[XComponent]; }
            double& y() { return _offsets[YComponent]; }

        private:
            std::valarray<double>  _offsets;
        };

        struct Place
        {
            AvatarId avatarId;
            Point avatarCoordinates;
            Direction avatarDirection;
            double avatarRadius;
        };

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
            const Battlefield::Point& coords,
            const std::optional<double>& radius = std::nullopt,
            const std::optional<Battlefield::Direction>& direction = std::nullopt);
        
        AvatarId add_monster(const Defs::monster& monsterId, const std::string& partyName, const Point& coords, const std::optional<double>& radius=std::nullopt, const std::optional<Battlefield::Direction>& direction = std::nullopt, const std::string& uniqueMonsterName = "");
        AvatarId add_monster(const Defs::monster& monsterId, const Point& coords, const std::optional<double>& radius=std::nullopt, const std::optional<Battlefield::Direction>& direction=std::nullopt, const std::string& uniqueMonsterName="");
        
        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId, const std::string& partyName, const std::array<Point, _count>& coords,
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
                        const std::array<Point, _count>& coords,
                        const std::array<double, _count>& radius, const Direction& direction
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
        void add_monsters(const Defs::monster& monsterId, const std::array<Point, _count>& coords,
                        const std::array<double, _count>& radius)
        {
            add_monsters<_count>(monsterId, "", coords, radius, Battlefield::to_centre(coords));
        }

        template <unsigned short _count>
        void add_monsters(const Defs::monster& monsterId, const std::array<Point, _count>& coords,
                        const std::array<double, _count>& radius, const Direction& direction)
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

        static Point origin;
        static double defaultRadius;

    private:
        static constexpr size_t                         XComponent{0};
        static constexpr size_t                         YComponent{1};
        std::string                                     _name;
        std::map<std::string, Party>                    _parties;
        std::map<Point, Place>                          _coordinates;

        bool detect_collision(const Point& coord, const double radius) const;
        bool place_avatar(const Adndtk::AvatarId id, const Point& coord, 
                        const double radius, const Direction& direction);

        static double distance(const Point& pt1, const Point& pt2);
        static bool intersect(const Point& centre1, double radius1, const Point& centre2, double radius2);
        static Direction to_centre(const Point& coords);
    };
}

#endif // BATTLEFIELD_H