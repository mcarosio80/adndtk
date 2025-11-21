#include <adnd_errors.h>
#include <dictionary.h>

/*  ********************************
 *  AdndException
 *  *******************************/
Adndtk::AdndException::AdndException(const std::string& mesg)
    : std::exception{}, _errorMsg{mesg}
{
}

const char* Adndtk::AdndException::what() const noexcept
{
    return _errorMsg.c_str();
}

/*  ********************************
 *  InvalidClassException
 *  *******************************/
Adndtk::InvalidClassException::InvalidClassException(const Adndtk::Defs::character_class& cls)
    : AdndException("Invalid character class specified: " + as_string(cls)),
    _cls{cls}
{
}

// const char* Adndtk::InvalidClassException::what() const noexcept
// {
//     return AdndException::what();
// }

/*  ********************************
 *  CalendarDayException
 *  *******************************/
Adndtk::CalendarDayException::CalendarDayException(const uint16_t& dayOfTheYear, const uint16_t& year)
    : AdndException("Invalid day " + std::to_string(dayOfTheYear) + " of the year " + std::to_string(year)),
    _dayOfTheYear{dayOfTheYear},
    _year{year}
{
}

// const char* Adndtk::CalendarDayException::what() const noexcept
// {
//     return AdndException::what();
// }

// const char* Adndtk::InvalidClassException::what() const noexcept
// {
//     return AdndException::what();
// }

/*  ********************************
 *  MarketException
 *  *******************************/
Adndtk::MarketException::MarketException(const std::string& mesg, const std::string& storeName)
    : AdndException(mesg + ", store " + storeName),
    _storeName{storeName}
{
}

Adndtk::MarketException::MarketException(const std::string& mesg, const std::string& storeName,
                                const Defs::equipment& equipmentId)
    : AdndException(mesg + ", store '" + storeName + "', equipment '"
            + Dictionary::to_string(equipmentId) + "'"),
    _storeName{storeName}
{
}

/*  ********************************
 *  MonsterException
 *  *******************************/
Adndtk::MonsterException::MonsterException(const std::string& mesg, const Defs::monster& id)
    : AdndException(mesg + ", id: " + as_string(id)),
    _id{id}
{
}

/*  ********************************
 *  DiceException
 *  *******************************/
Adndtk::DiceException::DiceException(const std::string& mesg, const char *expr)
    : AdndException(mesg + ", '" + expr + "'"), _expression{expr}
{
}

/*  ********************************
 *  SkillException
 *  *******************************/
Adndtk::SkillException::SkillException(const std::string& mesg)
    : AdndException(mesg)
{
}

Adndtk::SkillException::SkillException(const std::string& mesg, const Defs::skill& skl, const short& val)
    : AdndException(mesg + ": Skill " + Dictionary::to_string(skl) + ", value " + std::to_string(val)),
        _skl{skl}, _val{val}
{
}

/*  ********************************
 *  CharacterException
 *  *******************************/
Adndtk::CharacterException::CharacterException(const std::string& mesg,
                        const Defs::character_class& cls, const Defs::moral_alignment& align)
    : AdndException(mesg + ": class '" + Dictionary::to_string(cls) + "', alignment '" + Dictionary::to_string(align) + "'"),
                _cls{cls}, _align{align}
{
}

Adndtk::CharacterException::CharacterException(const std::string& mesg,
                        const Defs::character_class& cls, const Defs::deity& deityId)
    : AdndException(mesg + ": class '" + Dictionary::to_string(cls) + "', deity '" + Dictionary::to_string(deityId) + "'"),
                _cls{cls}, _deityId{deityId}
{
}

Adndtk::CharacterException::CharacterException(const std::string& mesg,
                        const Defs::character_class& cls)
    : AdndException(mesg + ": class '" + Dictionary::to_string(cls) + "'"),
                _cls{cls}
{
}

/*  ********************************
 *  RaceException
 *  *******************************/
Adndtk::RaceException::RaceException(const std::string& mesg,
                        const Defs::race& raceId, const short& age, const short& ageLimit)
    : AdndException(mesg + ": race '" + Dictionary::to_string(raceId)
                + "', age " + std::to_string(age) + ", limit " + std::to_string(ageLimit)),
                _race{raceId}, _age{age}, _limit{ageLimit}
{
}

/*  ********************************
 *  EquipmentException
 *  *******************************/
Adndtk::EquipmentException::EquipmentException(const std::string& mesg)
    : AdndException(mesg)
{
}

Adndtk::EquipmentException::EquipmentException(const std::string& mesg,
                        const Defs::equipment& equipmentId)
    : AdndException(mesg + ": equipment '" + Dictionary::to_string(equipmentId) + "'"),
    _equipmentId{equipmentId}
{
}

Adndtk::EquipmentException::EquipmentException(const std::string& mesg,
                        const Defs::equipment_type& equipmentType)
    : AdndException(mesg + ": equipment '" + Dictionary::to_string(equipmentType) + "'"),
    _equipmentType{equipmentType}
{
}

Adndtk::EquipmentException::EquipmentException(const std::string& mesg,
                        const Defs::equipment& equipmentId, const Coin& price)
    : AdndException(mesg + ": equipment '" + Dictionary::to_string(equipmentId)
                + "', price is " + std::to_string(price.value()) + " "
                + Dictionary::to_string(price.currency())),
    _equipmentId{equipmentId}
{
}

/*  ********************************
 *  DataException
 *  *******************************/
Adndtk::DataException::DataException(const std::string& mesg)
    : AdndException(mesg)
{
}

Adndtk::DataException::DataException(const std::string& mesg, const std::string& key)
    : AdndException(mesg + ": key " + key)
{
}

Adndtk::DataException::DataException(const std::string& mesg, const size_t& index)
    : AdndException(mesg + ": index " + std::to_string(index))
{
}

Adndtk::DataException::DataException(const std::string& mesg, const Query& queryId)
    : AdndException(mesg + ": query ID " + std::to_string(static_cast<int>(queryId)))
{
}

/*  ********************************
 *  MoneyException
 *  *******************************/
Adndtk::MoneyException::MoneyException(const std::string& mesg)
    : AdndException(mesg)
{
}

/*  ********************************
 *  InventoryException
 *  *******************************/
Adndtk::InventoryException::InventoryException(const std::string& mesg)
    : AdndException(mesg)
{
}