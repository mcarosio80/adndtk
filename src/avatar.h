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

        virtual const std::string& name() const = 0;
        virtual Type avatar_type() const = 0;
        virtual short roll_initiative() const = 0;
        virtual ~Avatar() {}
        
    private:
    };
}

#endif // __ADNDTK__SRC__AVATAR_H