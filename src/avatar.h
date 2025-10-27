#ifndef __ADNDTK__SRC__AVATAR_H
#define __ADNDTK__SRC__AVATAR_H

namespace Adndtk
{
    class Avatar
    {
    public:
        enum class Type
        {
            PlayerCharacter,
            NonPlayerCharacter,
            Monster
        };

        virtual Type get_avatar_type() const = 0;
        virtual ~Avatar() {}
        
    private:
    };
}

#endif // __ADNDTK__SRC__AVATAR_H