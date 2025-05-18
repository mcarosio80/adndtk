//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <calendar.h>

using namespace Adndtk;

TEST_CASE("[TC-CALE.001] Day of the year may change if the year is leap", "[calendar]" )
{
    int year{1367};
    
    REQUIRE_FALSE(Calendar::is_leap_year(year));
    REQUIRE(Calendar::get_day_of_the_year(1, 1, year) == 1);
    REQUIRE(Calendar::get_day_of_the_year(25, 5, year) == 145);
    REQUIRE(Calendar::get_day_of_the_year(30, 12, year) == 364);
    REQUIRE(Calendar::get_day_of_the_year(31, 12, year) == 365);
    
    ++year;
    REQUIRE(Calendar::is_leap_year(year));
    REQUIRE(Calendar::get_day_of_the_year(1, 1, year) == 1);
    REQUIRE(Calendar::get_day_of_the_year(25, 5, year) == 146);
    REQUIRE(Calendar::get_day_of_the_year(30, 12, year) == 365);
    REQUIRE(Calendar::get_day_of_the_year(31, 12, year) == 366);
}

TEST_CASE("[TC-CALE.002] Base year is 1367 in DR", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::dalereckoning) == 1367);
    REQUIRE(cal.get_base_year() == 1367);
}

TEST_CASE("[TC-CALE.003] Base year is 1342 in CR", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::cormyr_reckoning) == 1342);
}

TEST_CASE("[TC-CALE.004] Base year is 2455 in WY", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::waterdeep_years) == 2455);
}

TEST_CASE("[TC-CALE.005] Base year is 1570 in TR", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::tethyreckoning) == 1570);
}

TEST_CASE("[TC-CALE.006] Base year is 335 in NR", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::northreckoning) == 335);
}

TEST_CASE("[TC-CALE.007] Base year is 3501 in MC", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::mulhorand_calendar) == 3501);
}

TEST_CASE("[TC-CALE.008] Base year is 5217 in NY", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::netheril_year_calendar) == 5217);
}

TEST_CASE("[TC-CALE.009] Base year is 7 in PR", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    REQUIRE(cal.get_base_year(Defs::calendar_reckoning::present_reckoning) == 7);
}

TEST_CASE("[TC-CALE.010] Leap years can ve divided by 4 and 100 but not by 400", "[calendar]" )
{
    REQUIRE_FALSE(Calendar::is_leap_year(1200));
    REQUIRE(Calendar::is_leap_year(1204));
    REQUIRE(Calendar::is_leap_year(1368));
    REQUIRE(Calendar::is_leap_year(1400));
    REQUIRE_FALSE(Calendar::is_leap_year(1367));
    REQUIRE_FALSE(Calendar::is_leap_year(1600));
}

TEST_CASE("[TC-CALE.011] Leap years have 6 special days", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1368;

    REQUIRE_FALSE(cal.is_special_day(1, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(30, year).has_value());
    REQUIRE(cal.is_special_day(31, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(32, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(61, year).has_value());
    REQUIRE(cal.is_special_day(62, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(63, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(122, year).has_value());
    REQUIRE(cal.is_special_day(123, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(124, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(213, year).has_value());
    REQUIRE(cal.is_special_day(214, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(215, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(274, year).has_value());
    REQUIRE(cal.is_special_day(275, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(276, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(335, year).has_value());
    REQUIRE(cal.is_special_day(336, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(337, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(365, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(366, year).has_value());
}

TEST_CASE("[TC-CALE.012] Non-leap years have 5 special days", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1367;

    REQUIRE_FALSE(cal.is_special_day(1, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(30, year).has_value());
    REQUIRE(cal.is_special_day(31, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(32, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(61, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(62, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(63, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(121, year).has_value());
    REQUIRE(cal.is_special_day(122, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(123, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(212, year).has_value());
    REQUIRE(cal.is_special_day(213, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(214, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(273, year).has_value());
    REQUIRE(cal.is_special_day(274, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(275, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(334, year).has_value());
    REQUIRE(cal.is_special_day(335, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(336, year).has_value());
    REQUIRE_FALSE(cal.is_special_day(365, year).has_value());
}

TEST_CASE("[TC-CALE.013] Gregorian calendar dates matching special days cannot be converted (leap years)", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t leapYear = 1368;

    REQUIRE(cal.convert_date(1,1,leapYear).has_value());
    REQUIRE(cal.convert_date(15,1,leapYear).has_value());

    REQUIRE(cal.convert_date(30,1,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(31,1,leapYear).has_value());
    REQUIRE(cal.convert_date(1,2,leapYear).has_value());

    REQUIRE(cal.convert_date(1,3,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(2,3,leapYear).has_value());
    REQUIRE(cal.convert_date(3,3,leapYear).has_value());

    REQUIRE(cal.convert_date(1,5,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(2,5,leapYear).has_value());
    REQUIRE(cal.convert_date(3,5,leapYear).has_value());

    REQUIRE(cal.convert_date(31,7,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(1,8,leapYear).has_value());
    REQUIRE(cal.convert_date(2,8,leapYear).has_value());

    REQUIRE(cal.convert_date(30,9,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(1,10,leapYear).has_value());
    REQUIRE(cal.convert_date(2,10,leapYear).has_value());

    REQUIRE(cal.convert_date(30,11,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(1,12,leapYear).has_value());
    REQUIRE(cal.convert_date(2,12,leapYear).has_value());

    REQUIRE(cal.convert_date(18,12,leapYear).has_value());
    REQUIRE(cal.convert_date(25,12,leapYear).has_value());
    REQUIRE(cal.convert_date(31,12,leapYear).has_value());
}

TEST_CASE("[TC-CALE.014] Gregorian calendar dates matching special days cannot be converted (non-leap years)", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t leapYear = 1367;

    REQUIRE(cal.convert_date(1,1,leapYear).has_value());
    REQUIRE(cal.convert_date(15,1,leapYear).has_value());

    REQUIRE(cal.convert_date(30,1,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(31,1,leapYear).has_value());
    REQUIRE(cal.convert_date(1,2,leapYear).has_value());

    REQUIRE(cal.convert_date(2,3,leapYear).has_value());
    REQUIRE(cal.convert_date(3,3,leapYear).has_value());
    REQUIRE(cal.convert_date(4,3,leapYear).has_value());

    REQUIRE(cal.convert_date(1,5,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(2,5,leapYear).has_value());
    REQUIRE(cal.convert_date(3,5,leapYear).has_value());

    REQUIRE(cal.convert_date(31,7,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(1,8,leapYear).has_value());
    REQUIRE(cal.convert_date(2,8,leapYear).has_value());

    REQUIRE(cal.convert_date(30,9,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(1,10,leapYear).has_value());
    REQUIRE(cal.convert_date(2,10,leapYear).has_value());

    REQUIRE(cal.convert_date(30,11,leapYear).has_value());
    REQUIRE_FALSE(cal.convert_date(1,12,leapYear).has_value());
    REQUIRE(cal.convert_date(2,12,leapYear).has_value());

    REQUIRE(cal.convert_date(18,12,leapYear).has_value());
    REQUIRE(cal.convert_date(25,12,leapYear).has_value());
    REQUIRE(cal.convert_date(31,12,leapYear).has_value());
}

TEST_CASE("[TC-CALE.015] The first special day in Hapthos Calendar is Midwinter", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t nonLeapYear = 1367;
    uint16_t leapYear = 1368;

    REQUIRE(cal.is_special_day(31, leapYear).has_value());
    REQUIRE(cal.is_special_day(31, leapYear).value() == "Midwinter");
    REQUIRE(cal.is_special_day(31, nonLeapYear).has_value());
    REQUIRE(cal.is_special_day(31, nonLeapYear).value() == "Midwinter");
}

TEST_CASE("[TC-CALE.015] The second special day in Hapthos Calendar is Shieldmeet", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t nonLeapYear = 1367;
    uint16_t leapYear = 1368;

    REQUIRE(cal.is_special_day(62, leapYear).has_value());
    REQUIRE(cal.is_special_day(62, leapYear).value() == "Shieldmeet");
    REQUIRE_FALSE(cal.is_special_day(62, nonLeapYear).has_value());
}

TEST_CASE("[TC-CALE.016] The third special day in Hapthos Calendar is Greengrass", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t nonLeapYear = 1367;
    uint16_t leapYear = 1368;

    REQUIRE(cal.is_special_day(123, leapYear).has_value());
    REQUIRE(cal.is_special_day(123, leapYear).value() == "Greengrass");
    REQUIRE(cal.is_special_day(122, nonLeapYear).has_value());
    REQUIRE(cal.is_special_day(122, nonLeapYear).value() == "Greengrass");
}

TEST_CASE("[TC-CALE.017] The fourth special day in Hapthos Calendar is Midsummer", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t nonLeapYear = 1367;
    uint16_t leapYear = 1368;

    REQUIRE(cal.is_special_day(214, leapYear).has_value());
    REQUIRE(cal.is_special_day(214, leapYear).value() == "Midsummer");
    REQUIRE(cal.is_special_day(213, nonLeapYear).has_value());
    REQUIRE(cal.is_special_day(213, nonLeapYear).value() == "Midsummer");
}

TEST_CASE("[TC-CALE.018] The fifth special day in Hapthos Calendar is Higharvestide", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t nonLeapYear = 1367;
    uint16_t leapYear = 1368;

    REQUIRE(cal.is_special_day(275, leapYear).has_value());
    REQUIRE(cal.is_special_day(275, leapYear).value() == "Higharvestide");
    REQUIRE(cal.is_special_day(274, nonLeapYear).has_value());
    REQUIRE(cal.is_special_day(274, nonLeapYear).value() == "Higharvestide");
}

TEST_CASE("[TC-CALE.019] The sixth special day in Hapthos Calendar is Feast of the moon", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t nonLeapYear = 1367;
    uint16_t leapYear = 1368;

    REQUIRE(cal.is_special_day(336, leapYear).has_value());
    REQUIRE(cal.is_special_day(336, leapYear).value() == "Feast of the moon");
    REQUIRE(cal.is_special_day(335, nonLeapYear).has_value());
    REQUIRE(cal.is_special_day(335, nonLeapYear).value() == "Feast of the moon");
}

TEST_CASE("[TC-CALE.020] Year 1367 is the Year of the Shield", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1367;

    REQUIRE(cal.get_year_name(year) == "Year of the Shield");
}

TEST_CASE("[TC-CALE.021] Date October 2nd, 1367 translates to 1 Marpenoth, 1367, Year of the Shield", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1367;

    REQUIRE(cal.to_string(2, 10, year) == "1 Marpenoth, 1367, Year of the Shield");
}

TEST_CASE("[TC-CALE.022] Date January 1st, 1367 translates to Midwinter, 1367, Year of the Shield", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1367;

    REQUIRE(cal.to_string(31, 1, year) == "Midwinter, 1367, Year of the Shield");
}

TEST_CASE("[TC-CALE.023] Date March 3rd, 1368 translates to Shieldmeet, 1368, Year of the Banner", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1368;

    REQUIRE(cal.to_string(2, 3, year) == "Shieldmeet, 1368, Year of the Banner");
}

TEST_CASE("[TC-CALE.024] Date March 1st, 1367 translates to 29 Alturiak, 1367, Year of the Shield", "[calendar]" )
{
    auto cal = CalendarProvider::get_instance().get_calendar(Defs::calendar::hapthos_calendar);
    uint16_t year = 1367;

    REQUIRE(cal.to_string(1, 3, year) == "29 Alturiak, 1367, Year of the Shield");
}