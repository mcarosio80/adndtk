#ifndef __ADNDTK__SRC__ADND_ERRORS_H
#define __ADNDTK__SRC__ADND_ERRORS_H

#include <string>
#include <exception>

#include <defs.h>
// #include <cyclopedia.h>
#include <coin.h>
#include <dictionary.h>
#include <queries.h>
#include <common_types.h>

namespace Adndtk
{
    class AdndException : public std::exception {
    public:
        AdndException(const std::string& mesg);
        virtual ~AdndException() = default;

        virtual const char* what() const noexcept override;

    private:
        std::string     _errorMsg;
    };

    class InvalidClassException : public AdndException {
    public:
        InvalidClassException(const Defs::character_class& cls);
        virtual ~InvalidClassException() = default;

        const Defs::character_class& get_class_id() const { return _cls; }

    private:
        Defs::character_class   _cls;
    };

    class CalendarDayException : public AdndException {
    public:
        CalendarDayException(const uint16_t& dayOfTheYear, const uint16_t& year);
        virtual ~CalendarDayException() = default;

        // virtual const char* what() const noexcept override;
        const uint16_t& get_day_of_the_year() const { return _dayOfTheYear; }
        const uint16_t& get_year() const { return _year; }

    private:
        uint16_t        _dayOfTheYear;
        uint16_t        _year;
    };

    class MarketException : public AdndException {
    public:
        MarketException(const std::string& mesg, const std::string& storeName);
        MarketException(const std::string& mesg, const std::string& storeName,
                        const Defs::equipment& equipmentId);

        virtual ~MarketException() = default;

        const std::string& get_store_name() const { return _storeName; }

    private:
        std::string        _storeName;
    };

    class MonsterException : public AdndException {
    public:
        MonsterException(const std::string& mesg, const Defs::monster& storeName);
        virtual ~MonsterException() = default;

        const Defs::monster& get_monster_id() const { return _id; }

    private:
        Defs::monster       _id;
    };

    class DiceException : public AdndException {
    public:
        DiceException(const std::string& mesg, const char *expr);
        virtual ~DiceException() = default;

        const std::string& get_dice_expression() const { return _expression; }

    private:
        std::string         _expression;
    };

    class SkillException : public AdndException {
    public:
        SkillException(const std::string& mesg);
        SkillException(const std::string& mesg, const Defs::skill& skl, const short& val);
        virtual ~SkillException() = default;

    private:
        Defs::skill     _skl;
        short           _val;
    };

    class CharacterException : public AdndException {
    public:
        CharacterException(const std::string& mesg, const Defs::character_class& cls,
                                const Defs::moral_alignment& align);
        CharacterException(const std::string& mesg, const Defs::character_class& cls,
                                const Defs::deity& deityId);
        CharacterException(const std::string& mesg, const Defs::character_class& cls);

        virtual ~CharacterException() = default;

        const Defs::character_class& get_character_class() const { return _cls; }

    private:
        Defs::character_class                       _cls;
        std::optional<Defs::moral_alignment>        _align;
        std::optional<Defs::deity>                  _deityId;
    };

    class RaceException : public AdndException {
    public:
        RaceException(const std::string& mesg, const Defs::race& raceId,
                                const short& age, const short& ageLimit);

        virtual ~RaceException() = default;

        const Defs::race& get_race() const { return _race; }
        const short& get_age() const { return _age; }
        const short& get_age_limit() const { return _limit; }

    private:
        Defs::race          _race;
        short               _age;
        short               _limit;
    };

    class EquipmentException : public AdndException {
    public:
        EquipmentException(const std::string& mesg);
        EquipmentException(const std::string& mesg, const Defs::equipment& equipmentId);
        EquipmentException(const std::string& mesg, const Defs::equipment_type& equipmentType);
        EquipmentException(const std::string& mesg, const Defs::equipment& equipmentId, const Coin& price);

        virtual ~EquipmentException() = default;

    private:
        Defs::equipment          _equipmentId;
        Defs::equipment_type     _equipmentType;
    };

    class DataException : public AdndException {
    public:
        DataException(const std::string& mesg);
        DataException(const std::string& mesg, const std::string& key);
        DataException(const std::string& mesg, const size_t& index);
        DataException(const std::string& mesg, const Query& queryId);

        virtual ~DataException() = default;

    private:
    };

    class MoneyException : public AdndException {
    public:
        MoneyException(const std::string& mesg);

        virtual ~MoneyException() = default;

    private:
    };

    class InventoryException : public AdndException {
    public:
        InventoryException(const std::string& mesg);

        virtual ~InventoryException() = default;

    private:
    };

    template <typename _SpellType>
    class SpellException : public AdndException {
    public:
        SpellException(const std::string& error, const _SpellType& spellId)
            : AdndException(error + ": spell " + Dictionary::to_string(spellId)),
            // _level{Cyclopedia::get_instance().get_spell_level(spellId)},
            _spellId{spellId}
        {
        }
        SpellException(const std::string& error, const Defs::character_class& cls, const Defs::race& raceId)
            : AdndException(error + ": class " + Dictionary::to_string(cls)
                        + ", race " + Dictionary::to_string(raceId))
        {
        }

        virtual ~SpellException() = default;

        // virtual const char* what() const noexcept override
        // {
        //     return (std::string(AdndException::what())
        //             + "\n\tSpell ID : "
        //             + as_string(_spellId) + ", level : " + std::to_string(_level)).c_str();
        // }
        // const SpellLevel& get_level() const { return _level; }
        // const _SpellType& get_spell_id() const { return _spellId; }

    private:
        // SpellLevel      _level;
        _SpellType      _spellId;
    };
}

#endif // __ADNDTK__SRC__ADND_ERRORS_H