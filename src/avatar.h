#ifndef AVATAR_H
#define AVATAR_H

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

#endif // AVATAR_H