#include <battleground.h>
#include <character.h>

#include <sstream>
#include <limits>

Adndtk::AvatarId Adndtk::Party::_monsterKeyOffset = 0;
Adndtk::AvatarId Adndtk::Party::_characterKeyOffset = std::numeric_limits<Adndtk::AvatarId>::max() / 2;
bool Adndtk::Battleground::_initialised = false;
Adndtk::BattlefieldPoint Adndtk::Battlefield::origin = {0.0, 0.0};
double Adndtk::Battlefield::defaultRadius = 0.5;

//********************************
//    Party
//

Adndtk::Party::Party(const std::string& name)
    : _name{name}
{
}

Adndtk::AvatarId Adndtk::Party::add_character(const std::string& name, const Adndtk::Defs::character_class& cls,
    const Adndtk::Defs::race& raceId, const Adndtk::Defs::moral_alignment& align, const Adndtk::Defs::sex& sexId,
    const std::optional<Adndtk::Defs::deity>& deityId)
{
    auto nextKey = _characters.size() + _characterKeyOffset;
    _characters.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(nextKey),
        std::forward_as_tuple(name, cls, raceId, align, sexId, deityId));
    return nextKey;
}

Adndtk::AvatarId Adndtk::Party::add_monster(const Adndtk::Defs::monster& monsterId, const std::string& monsterName/*=""*/)
{
    auto nextKey = _monsters.size() + _monsterKeyOffset;
    _monsters.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(nextKey),
        std::forward_as_tuple(monsterId, monsterName));

    return nextKey;
}

void Adndtk::Party::clear()
{
    _characters.clear();
    _monsters.clear();
}

//********************************
//    Battlefield
//

Adndtk::Battlefield::Battlefield(const std::string& key)
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
    const BattlefieldPoint& coords,
    const std::optional<double>& radius/*=std::nullopt*/,
    const std::optional<BattlefieldDirection>& direction/*=std::nullopt*/)
{
    const auto avatarRadius = radius.value_or(Battlefield::defaultRadius);
    const auto avatarDirection = direction.value_or(Battlefield::to_centre(coords));

    if (detect_collision(coords, avatarRadius))
    {
        std::stringstream ss{};
        ss << "Coordinates " << coords.x() << "," << coords.y() << " are currently held";
        throw std::runtime_error(ss.str());
    }
    auto& p = get_party(name + " party");
    auto id = p.add_character(name, cls, raceId, align, sexId, deityId);

    place_avatar(id, coords, avatarRadius, avatarDirection);
    return id;
}

Adndtk::AvatarId Adndtk::Battlefield::add_monster(const Defs::monster& monsterId, const std::string& partyName, const BattlefieldPoint& coords, const std::optional<double>& radius/*=std::nullopt*/, const std::optional<BattlefieldDirection>& direction/*=std::nullopt*/, const std::string& uniqueMonsterName/*=""*/)
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


Adndtk::AvatarId Adndtk::Battlefield::add_monster(const Defs::monster& monsterId, const BattlefieldPoint& coords, const std::optional<double>& radius/*=std::nullopt*/, const std::optional<BattlefieldDirection>& direction/*=std::nullopt*/, const std::string& uniqueMonsterName/*=""*/)
{
    const auto partyName = uniqueMonsterName + " party";
    return add_monster(monsterId, partyName, coords, radius, direction, uniqueMonsterName);
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
}

void Adndtk::Battlefield::step()
{
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

bool Adndtk::Battlefield::detect_collision(const BattlefieldPoint& coord, const double radius) const
{
    for (const auto& c : _coordinates)
    {
        //if (c.first == coord)
        if (Battlefield::intersect(c.first, c.second.avatarRadius, coord, radius))
        {
            return true;
        }
    }
    return false;
}

bool Adndtk::Battlefield::place_avatar(const Adndtk::AvatarId id, const BattlefieldPoint& coord, const double radius, const BattlefieldDirection& direction)
{
    if (detect_collision(coord, radius))
    {
        return false;
    }

    _coordinates.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(coord),
            std::forward_as_tuple(id, direction, radius));
    return true;
}

double Adndtk::Battlefield::distance(const BattlefieldPoint& pt1, const BattlefieldPoint& pt2)
{
    return sqrt((pt1.x() - pt2.x())*(pt1.x() - pt2.x()) + (pt1.y() - pt2.y())*(pt2.y() - pt2.y()));
}

bool Adndtk::Battlefield::intersect(const BattlefieldPoint& centre1, double radius1, const BattlefieldPoint& centre2, double radius2)
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
    p.subtract(centre1).scale(a/d).add(centre1);

    double x3 = p.x() + h*(centre2.y() - centre1.y()) / d;
    double y3 = p.y() - h*(centre2.x() - centre1.x()) / d;
    double x4 = p.x() - h*(centre2.y() - centre1.y()) / d;
    double y4 = p.y() + h*(centre2.x() - centre1.x()) / d;

    return x3 != x4 || y3 != y4;
}

Adndtk::BattlefieldDirection Adndtk::Battlefield::to_centre(const Adndtk::BattlefieldPoint& coords)
{
    Adndtk::BattlefieldDirection direction{-coords.x(), -coords.y()};
    return direction;
}

//********************************
//    Battleground
//

Adndtk::Battleground& Adndtk::Battleground::get_instance()
{
    static Battleground _instance;

    if (!_initialised)
    {
        _initialised = true;
    }
    return _instance;
}

Adndtk::Battleground::Battleground()
{
}

Adndtk::Battleground::~Battleground()
{
}

bool Adndtk::Battleground::has_battlefield(const std::string& fieldName)
{
    return _fields.find(fieldName) != _fields.end();
}

void Adndtk::Battleground::add_battlefield(const std::string& fieldName)
{
    if (!has_battlefield(fieldName))
    {
        _fields.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(fieldName),
            std::forward_as_tuple(fieldName));
    }
}

Adndtk::Battlefield& Adndtk::Battleground::get_battlefield(const std::string& fieldName)
{
    add_battlefield(fieldName);
    return _fields[fieldName];
}

std::optional<Adndtk::Battlefield> Adndtk::Battleground::try_get_battlefield(const std::string& fieldName) const
{
    if (_fields.find(fieldName) == _fields.end())
    {
        return std::nullopt;
    }
    return _fields.at(fieldName);
}

void Adndtk::Battleground::clear()
{
    for (auto& bf : _fields)
    {
        bf.second.clear();
    }
    _fields.clear();
}