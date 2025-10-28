#include <limits>

#include <party.h>

Adndtk::AvatarId Adndtk::Party::_monsterKeyOffset = 0;
Adndtk::AvatarId Adndtk::Party::_characterKeyOffset = std::numeric_limits<Adndtk::AvatarId>::max() / 2;

Adndtk::Party::Party(std::string_view name)
    : _name{name}
{
}

Adndtk::AvatarId Adndtk::Party::add_character(std::string_view name, const Adndtk::Defs::character_class& cls,
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

Adndtk::AvatarId Adndtk::Party::add_character(Character&& chr)
{
    auto nextKey = _characters.size() + _characterKeyOffset;
    _characters.emplace(nextKey, std::move(chr));
    return nextKey;
}

Adndtk::AvatarId Adndtk::Party::add_monster(const Adndtk::Defs::monster& monsterId, std::string_view monsterName/*=""*/)
{
    auto nextKey = _monsters.size() + _monsterKeyOffset;
    _monsters.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(nextKey),
        std::forward_as_tuple(monsterId, monsterName));

    return nextKey;
}

Adndtk::AvatarId Adndtk::Party::add_monster(Monster&& monster)
{
    auto nextKey = _monsters.size() + _monsterKeyOffset;
    _monsters.emplace(nextKey, std::move(monster));

    return nextKey;
}

void Adndtk::Party::clear()
{
    _characters.clear();
    _monsters.clear();
}