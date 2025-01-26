#ifndef MONSTER_H
#define MONSTER_H

#include <string>

#include <defs.h>
#include <avatar.h>

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
        Defs::monster       _id;
        std::string         _name;
    };
}

#endif // AVATAR_H