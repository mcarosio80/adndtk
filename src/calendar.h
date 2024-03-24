#ifndef CALENDAR_H
#define CALENDAR_H

#include <defs.h>

#include <string>
#include <map>
#include <vector>
#include <optional>
#include <memory>
#include <chrono>

namespace Adndtk
{
    class Calendar
    {
    public:
        Calendar(){};
        explicit Calendar(const Defs::calendar& calendarId);
        ~Calendar();

        const Defs::calendar& get_id() const { return _id; };
        const std::string& get_name() const { return _name; };
        const Defs::campaign_settings& get_campaign_id() const { return _campaignId; };
        const uint16_t& get_base_year(const Defs::calendar_reckoning& reckoning = Defs::calendar_reckoning::dalereckoning) const;

        std::optional<std::string> is_special_day(const uint16_t& day, const uint16_t& month, const uint16_t& year) const;
        std::optional<std::string> is_special_day() const;
        std::optional<std::string> is_special_day(const uint16_t& dayOfTheYear, const uint16_t& year) const;

        std::optional<std::tuple<uint16_t, uint16_t, uint16_t>> convert_date(const uint16_t& day, const uint16_t& month, const uint16_t& year) const;
        std::optional<std::tuple<uint16_t, uint16_t, uint16_t>> convert_date() const;
        std::optional<std::tuple<uint16_t, uint16_t, uint16_t>> convert_date(const uint16_t& dayOfTheYear, const uint16_t& year) const;

        std::optional<std::string> get_year_name(const uint16_t& year) const;
        const std::string& get_month_name(const Defs::calendar_month& month) const;
        const std::string& get_month_name(const uint16_t& month) const;

        std::string to_string(const uint16_t& day, const uint16_t& month, const uint16_t& year) const;
        std::string to_string() const;
        std::string to_string(const uint16_t& dayOfTheYear, const uint16_t& year) const;

        static bool is_leap_year(uint16_t year);
        static uint16_t get_day_of_the_year();
        static uint16_t get_day_of_the_year(const uint16_t& day, const uint16_t& month, const uint16_t& year);
        
    private:
        Defs::calendar _id;
        std::string _name;
        Defs::campaign_settings _campaignId;

        std::map<Defs::calendar_month, std::pair<std::string, uint16_t>> _months;
        std::map<Defs::calendar_reckoning, std::tuple<std::string, std::string, uint16_t>> _reckonings;
        std::map<Defs::calendar_special_day, std::tuple<std::string, uint16_t, uint16_t>> _specialDays;

        uint16_t count_special_days(const uint16_t& dayOfTheYear, const bool& isLeapYear) const;
    };


    class CalendarProvider
    {
    public:
        static CalendarProvider& get_instance();
        CalendarProvider(Calendar const&) = delete;
        void operator=(CalendarProvider const&) = delete;

        const Calendar& get_calendar(const Defs::calendar& calendarId) const;

    private:
        CalendarProvider();
        ~CalendarProvider();

        void init();

        static bool             _initialised;
        std::map<Defs::calendar, Calendar>
                                _calendars;
    };
}

#endif // CALENDAR_H