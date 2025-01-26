#include <monster.h>
#include <tables.h>

Adndtk::Monster::Monster(const Adndtk::Defs::monster& monsterId)
    : _id{monsterId}
{
    auto info = Tables::monster::select_one<Defs::monster>("id", _id);
    if (info.has_value())
    {
        _name = info.value().name;
    }
}