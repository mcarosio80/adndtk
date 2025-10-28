#include <sstream>
#include <cmath>
#include <numeric>

#include <battlefield.h>

Adndtk::Battlefield::Point Adndtk::Battlefield::origin = {0.0, 0.0};
double Adndtk::Battlefield::defaultRadius = 0.5;

Adndtk::Battlefield::Battlefield(std::string_view key)
    : _name{key}
{
}

Adndtk::Party& Adndtk::Battlefield::get_party(const std::string& partyName)
{
    if (!has_party(partyName))
    {
        _parties.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(partyName),
            std::forward_as_tuple(partyName));
    }
    return _parties[partyName];
}

const Adndtk::Party& Adndtk::Battlefield::get_party(const std::string& partyName) const
{
    if (partyName.empty() || !has_party(partyName))
    {
        std::stringstream ss{};
        ss << "Unable to find the specified party: '" << partyName << "'";
        throw std::runtime_error(ss.str());
    }
    return _parties.at(partyName);
}

bool Adndtk::Battlefield::has_party(const std::string& partyName) const
{
    return _parties.find(partyName) != _parties.end();
}

Adndtk::AvatarId Adndtk::Battlefield::add_character(const std::string& name, const Defs::character_class& cls,
    const Defs::race& raceId, const Defs::moral_alignment& align, const Defs::sex& sexId,
    const std::optional<Defs::deity>& deityId,
    const Battlefield::Point& coords,
    const std::optional<double>& radius/*=std::nullopt*/,
    const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/)
{
    return add_character(name, cls, raceId, align, sexId, deityId, name + " party", coords, radius, direction);
}

Adndtk::AvatarId Adndtk::Battlefield::add_character(const std::string& name, const Defs::character_class& cls,
    const Defs::race& raceId, const Defs::moral_alignment& align, const Defs::sex& sexId,
    const std::optional<Defs::deity>& deityId,
    const std::string& partyName,
    const Battlefield::Point& coords,
    const std::optional<double>& radius/*=std::nullopt*/,
    const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/)
{
    const auto avatarRadius = radius.value_or(Battlefield::defaultRadius);
    const auto avatarDirection = direction.value_or(Battlefield::to_centre(coords));

    if (detect_collision(coords, avatarRadius))
    {
        std::stringstream ss{};
        ss << "Coordinates " << coords.x() << "," << coords.y() << " are currently held";
        throw std::runtime_error(ss.str());
    }
    auto& p = get_party(partyName);
    auto id = p.add_character(name, cls, raceId, align, sexId, deityId);

    place_avatar(id, coords, avatarRadius, avatarDirection);
    return id;
}
            
Adndtk::AvatarId Adndtk::Battlefield::add_character(Character&& chr, const Battlefield::Point& coords,
    const std::optional<double>& radius/*=std::nullopt*/,
    const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/)
{
    return add_character(std::move(chr), chr.name() + " party", coords, radius, direction);
}
            
Adndtk::AvatarId Adndtk::Battlefield::add_character(Character&& chr, const std::string& partyName, const Battlefield::Point& coords,
    const std::optional<double>& radius/*=std::nullopt*/,
    const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/)
{
    const auto avatarRadius = radius.value_or(Battlefield::defaultRadius);
    const auto avatarDirection = direction.value_or(Battlefield::to_centre(coords));

    if (detect_collision(coords, avatarRadius))
    {
        std::stringstream ss{};
        ss << "Coordinates " << coords.x() << "," << coords.y() << " are currently held";
        throw std::runtime_error(ss.str());
    }
    auto& p = get_party(partyName);
    auto id = p.add_character(std::move(chr));

    place_avatar(id, coords, avatarRadius, avatarDirection);
    return id;
}

Adndtk::AvatarId Adndtk::Battlefield::add_monster(const Defs::monster& monsterId, const std::string& partyName, const Point& coords, const std::optional<double>& radius/*=std::nullopt*/, const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/, const std::string& uniqueMonsterName/*=""*/)
{
    const auto avatarRadius = radius.value_or(Battlefield::defaultRadius);
    const auto avatarDirection = direction.value_or(Battlefield::to_centre(coords));

    if (detect_collision(coords, avatarRadius))
    {
        std::stringstream ss{};
        ss << "Coordinates " << coords.x() << "," << coords.y() << " are currently held";
        throw std::runtime_error(ss.str());
    }

    auto& p = get_party(partyName);
    auto id = p.add_monster(monsterId, uniqueMonsterName);

    place_avatar(id, coords, avatarRadius, avatarDirection);
    return id;
}


Adndtk::AvatarId Adndtk::Battlefield::add_monster(const Defs::monster& monsterId, const Point& coords, const std::optional<double>& radius/*=std::nullopt*/, const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/, const std::string& uniqueMonsterName/*=""*/)
{
    const auto partyName = uniqueMonsterName + " party";
    return add_monster(monsterId, partyName, coords, radius, direction, uniqueMonsterName);
}


Adndtk::AvatarId Adndtk::Battlefield::add_monster(Monster&& monster, const std::string& partyName, const Point& coords, const std::optional<double>& radius/*=std::nullopt*/, const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/)
{
    const auto avatarRadius = radius.value_or(Battlefield::defaultRadius);
    const auto avatarDirection = direction.value_or(Battlefield::to_centre(coords));

    if (detect_collision(coords, avatarRadius))
    {
        std::stringstream ss{};
        ss << "Coordinates " << coords.x() << "," << coords.y() << " are currently held";
        throw std::runtime_error(ss.str());
    }

    auto& p = get_party(partyName);
    auto id = p.add_monster(std::move(monster));

    place_avatar(id, coords, avatarRadius, avatarDirection);
    return id;
}

Adndtk::AvatarId Adndtk::Battlefield::add_monster(Monster&& monster, const Point& coords, const std::optional<double>& radius/*=std::nullopt*/, const std::optional<Battlefield::Direction>& direction/*=std::nullopt*/)
{
    const auto partyName = monster.get_unique_name() + " party";
    return add_monster(std::move(monster), partyName, coords, radius, direction);
}        

size_t Adndtk::Battlefield::count_avatars() const
{
    size_t total{0};
    for (const auto& p : _parties)
    {
        total += p.second.count_characters() + p.second.count_monsters();
    }
    return total;
}

size_t Adndtk::Battlefield::count_avatars(const Adndtk::Avatar::Type& type) const
{
    size_t total{0};
    for (const auto& p : _parties)
    {
        total += (type == Avatar::Type::Monster) ? p.second.count_monsters() : p.second.count_characters();
    }
    return total;
}

size_t Adndtk::Battlefield::count_avatars(const std::string& partyName) const
{
    size_t total{0};
    if (has_party(partyName))
    {
        auto& p = get_party(partyName);
        total = p.count_monsters() + p.count_characters();
    }
    return total;
}

size_t Adndtk::Battlefield::count_avatars(const std::string& partyName, const Adndtk::Avatar::Type& type) const
{
    size_t total{0};
    if (has_party(partyName))
    {
        auto& p = get_party(partyName);
        total = (type == Avatar::Type::Monster) ? p.count_monsters() : p.count_characters();
    }
    return total;
}

void Adndtk::Battlefield::clear()
{
    for (auto& p : _parties)
    {
        p.second.clear();
    }
    _parties.clear();
    _coordinates.clear();
    _battleTurn.clear();
}

void Adndtk::Battlefield::step()
{
    _battleTurn.clear();
    auto monsters = select<Monster>([](const Monster& m){ return true; });
    auto characters = select<Character>([](const Character& c){ return true; });

    std::map<short, BattleTurnList> reactOrder{};

    for (const auto& c : characters)
    {
        auto initiative = c.avatar.get().roll_initiative();
        reactOrder[initiative].push_back(std::make_pair(initiative, c.avatar));
    }
    for (const auto& m : monsters)
    {
        auto initiative = m.avatar.get().roll_initiative();
        reactOrder[initiative].push_back(std::make_pair(initiative, m.avatar));
    }

    for (const auto r : reactOrder)
    {
        _battleTurn.insert(_battleTurn.end(), r.second.begin(), r.second.end());
    }
}

Adndtk::PartyList Adndtk::Battlefield::select() const
{
    auto f = [](const Party& p){ return true; };
    return select(f);
}

Adndtk::PartyList Adndtk::Battlefield::select(const PartyFilter& cbk) const
{
    PartyList res{};
    for (auto& p : _parties)
    {
        if (cbk(p.second))
        {
            std::reference_wrapper<Party> r = const_cast<Party&>(p.second);
            res.push_back(r);
        }
    }
    return std::move(res);
}

bool Adndtk::Battlefield::detect_collision(const Point& coord, const double radius) const
{
    for (const auto& c : _coordinates)
    {
        if (Battlefield::intersect(c.first, c.second.avatarRadius, coord, radius))
        {
            return true;
        }
    }
    return false;
}

bool Adndtk::Battlefield::place_avatar(const Adndtk::AvatarId id, const Point& coord, const double radius, const Direction& direction)
{
    if (detect_collision(coord, radius))
    {
        return false;
    }

    _coordinates.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(coord),
            std::forward_as_tuple(id, coord, direction, radius));
    return true;
}

double Adndtk::Battlefield::inner_product(const Adndtk::Battlefield::Direction& dir1, const Adndtk::Battlefield::Direction& dir2)
{
    return std::inner_product(dir1.begin(), dir1.end(), dir2.begin(), 0);
}

double Adndtk::Battlefield::normal(const Adndtk::Battlefield::Direction& dir)
{
    auto pred = [] (const double sum, const double v) { return sum + std::pow(v, 2); };
    const auto normSquare = std::accumulate(dir.begin(), dir.end(), 0, pred);
    return std::sqrt(normSquare);
}

double Adndtk::Battlefield::distance(const Point& pt1, const Point& pt2)
{
    double res{};
    for (auto idx=0; idx<pt1.size(); ++idx)
    {
        res += std::pow(pt1[idx] - pt2[idx], 2);
    }
    return sqrt(res);
}

bool Adndtk::Battlefield::intersect(const Point& centre1, double radius1, const Point& centre2, double radius2)
{
    double d = distance(centre1, centre2);

    if (d == 0.0)
    {
        return true;
    }
    // if d > r0 + r1 then the circles are separate -> no collision
    if (d > radius1 + radius2)
    {
        return false;
    }
    // if d < |r0 - r1| then there one circle is contained within the other -> collision
    if (d < fabs(radius1 - radius2))
    {
        return true;
    }
    double radius1Squared = radius1*radius1;
    double a = (radius1Squared - radius2*radius2+ d*d)/(2*d);
    double h = sqrt(radius1Squared - a*a);
    auto p = centre2;
    p.move(-centre1.x(), -centre1.y()).scale(a/d).move(centre1.x(), centre1.y());

    double x3 = p.x() + h*(centre2.y() - centre1.y()) / d;
    double y3 = p.y() - h*(centre2.x() - centre1.x()) / d;
    double x4 = p.x() - h*(centre2.y() - centre1.y()) / d;
    double y4 = p.y() + h*(centre2.x() - centre1.x()) / d;

    return x3 != x4 || y3 != y4;
}

Adndtk::Battlefield::Direction Adndtk::Battlefield::to_centre(const Adndtk::Battlefield::Point& coords)
{
    Battlefield::Direction direction{-coords.x(), -coords.y()};
    direction.normalise();
    return direction;
}