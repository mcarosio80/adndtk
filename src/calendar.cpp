#include <calendar.h>
#include <cyclopedia.h>
#include <tables.h>

#include <algorithm>

bool Adndtk::CalendarProvider::_initialised = false;



Adndtk::Calendar::Calendar(const Defs::calendar& calendarId)
    : _id{calendarId}
{
    auto calInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::calendar>(Query::select_calendar, _id);
    {
        auto& r = calInfo[0];
        _name = r.as<std::string>("name");
        _campaignId = static_cast<Defs::campaign_settings>(r.as<int>("campaign_settings_id"));
    }

    auto reckoningsInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::calendar>(Query::select_calendar_reckonings, _id);
    {
        for (auto& r : reckoningsInfo)
        {
            auto reckId = static_cast<Defs::calendar_reckoning>(r.as<int>("id"));
            auto reckName = r.as<std::string>("name");
            auto acronym = r.as<std::string>("acronym");
            auto baseYear = r.as<uint16_t>("base_year");
            _reckonings[reckId] = std::make_tuple(reckName, acronym, baseYear);
        }
    }

    auto monthsInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::calendar>(Query::select_calendar_months, _id);
    {
        for (auto& r : monthsInfo)
        {
            auto monthId = static_cast<Defs::calendar_month>(r.as<int>("id"));
            auto monthName = r.as<std::string>("name");
            auto monthDuration = r.as<uint16_t>("duration");
            _months[monthId] = std::make_pair(monthName, monthDuration);
        }
    }

    auto specDaysInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::calendar>(Query::select_calendar_special_days, _id);
    {
        for (auto& r : specDaysInfo)
        {
            auto specDayId = static_cast<Defs::calendar_special_day>(r.as<int>("id"));
            auto dayName = r.as<std::string>("name");
            auto yearDay = r.try_or<uint16_t>("year_day", 0);
            auto leapYearDay = r.as<uint16_t>("leap_year_day");
            _specialDays[specDayId] = std::make_tuple(dayName, yearDay, leapYearDay);
        }
    }
}

Adndtk::Calendar::~Calendar()
{
}

const uint16_t& Adndtk::Calendar::get_base_year(const Defs::calendar_reckoning& reckoning/*=Defs::calendar_reckoning::dalereckoning*/) const
{
    return std::get<2>(_reckonings.at(reckoning));
}

std::optional<std::string> Adndtk::Calendar::is_special_day(const uint16_t& dayOfTheYear, const uint16_t& year) const
{
    bool isLeapYear = Calendar::is_leap_year(year);

    for (auto& sp : _specialDays)
    {
        auto yearDay = (isLeapYear) ? std::get<2>(sp.second) : std::get<1>(sp.second);
        if (dayOfTheYear == yearDay)
        {
            return std::get<0>(sp.second);
        }
    }
    return std::nullopt;
}

std::optional<std::string> Adndtk::Calendar::is_special_day(const uint16_t& day, const uint16_t& month, const uint16_t& year) const
{
    auto dayOfTheYear = get_day_of_the_year(day, month, year);
    return is_special_day(dayOfTheYear, year);
}

std::optional<std::string> Adndtk::Calendar::is_special_day() const
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = std::localtime(&t);
    return is_special_day(now->tm_mday, now->tm_mon+1, now->tm_year);
}

std::optional<std::tuple<uint16_t, uint16_t, uint16_t>> Adndtk::Calendar::convert_date(const uint16_t& day, const uint16_t& month, const uint16_t& year) const
{
    auto dayOfTheYear = get_day_of_the_year(day, month, year);
    return convert_date(dayOfTheYear, year);
}

std::optional<std::tuple<uint16_t, uint16_t, uint16_t>> Adndtk::Calendar::convert_date() const
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = std::localtime(&t);
    return convert_date(now->tm_mday, now->tm_mon+1, now->tm_year);
}

uint16_t Adndtk::Calendar::count_special_days(const uint16_t& dayOfTheYear, const bool& isLeapYear) const
{
    uint16_t daysLeft{dayOfTheYear};
    bool stopSearch{false};
    auto it = _specialDays.cbegin();
    uint16_t count{0};

    while (it != _specialDays.cend() && !stopSearch)
    {
        auto yearDay = (isLeapYear) ? std::get<2>(it->second) : std::get<1>(it->second);
        if (daysLeft < yearDay)
        {
            stopSearch = true;
        }
        else if (yearDay != 0)
        {
            ++count;
        }
        ++it;
    }

    return count;
}

std::optional<std::tuple<uint16_t, uint16_t, uint16_t>> Adndtk::Calendar::convert_date(const uint16_t& dayOfTheYear, const uint16_t& year) const
{
    auto specialDay = is_special_day(dayOfTheYear, year);
    if (specialDay.has_value())
    {
        return std::nullopt;
    }

    bool isLeapYear = Calendar::is_leap_year(year);
    uint16_t daysLeft{dayOfTheYear};
    daysLeft -= count_special_days(dayOfTheYear, isLeapYear);

    uint16_t calendarDay{0};
    uint16_t calendarMonth{1};

    for (auto& m : _months)
    {
        auto monthDuration = std::get<1>(m.second);
        if (daysLeft <= monthDuration)
        {
            calendarDay += daysLeft;
            return std::make_tuple(calendarDay, calendarMonth, year);
        }
        daysLeft -= monthDuration;
        ++calendarMonth;
    }
    
    return std::nullopt;
}

bool Adndtk::Calendar::is_leap_year(uint16_t year)
{
    return ((year % 4 == 0) || (year % 100 == 0)) && (year % 400 != 0);
}

uint16_t Adndtk::Calendar::get_day_of_the_year()
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = std::localtime(&t);
    return get_day_of_the_year(now->tm_mday, now->tm_mon+1, now->tm_year);
}

uint16_t Adndtk::Calendar::get_day_of_the_year(const uint16_t& day, const uint16_t& month, const uint16_t& year)
{
    uint16_t daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (Calendar::is_leap_year(year))
    {
        daysInMonth[1]++;
    }

    uint16_t dayOfTheYear{0};
    uint16_t monthNumber = std::min<uint16_t>(month, sizeof(daysInMonth));

    for (uint16_t m=0; m<monthNumber-1; ++m)
    {
        dayOfTheYear += daysInMonth[m];
    }
    dayOfTheYear += std::min(day,daysInMonth[monthNumber-1]);
    return dayOfTheYear;
}

std::optional<std::string> Adndtk::Calendar::get_year_name(const uint16_t& year) const
{
    auto info = Cyclopedia::get_instance().exec_prepared_statement<uint16_t>(Query::select_roll_of_the_year, year);
    if (info.size() >= 1)
    {
        return info[0].as<std::string>("name");
    }
    return std::nullopt;
}

const std::string& Adndtk::Calendar::get_month_name(const Defs::calendar_month& month) const
{
    return _months.at(month).first;
}

const std::string& Adndtk::Calendar::get_month_name(const uint16_t& month) const
{
    auto monthId = static_cast<Defs::calendar_month>(month);
    return get_month_name(monthId);
}

std::string Adndtk::Calendar::to_string(const uint16_t& day, const uint16_t& month, const uint16_t& year) const
{
    auto dayOfTheYear = get_day_of_the_year(day, month, year);
    return to_string(dayOfTheYear, year);
}

std::string Adndtk::Calendar::to_string() const
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = std::localtime(&t);
    auto dayOfTheYear = get_day_of_the_year();
    return to_string(dayOfTheYear, now->tm_year);
}

std::string Adndtk::Calendar::to_string(const uint16_t& dayOfTheYear, const uint16_t& year) const
{
    auto specDay = is_special_day(dayOfTheYear, year);
    std::stringstream ss{""};

    if (specDay.has_value())
    {
        ss << specDay.value() << ", " << year;
    }
    else
    {
        auto dmy = convert_date(dayOfTheYear, year);
        if (!dmy.has_value())
        {
            throw std::runtime_error("Invaid day of the year");
        }
        ss << std::get<0>(dmy.value()) << " "
            << get_month_name(std::get<1>(dmy.value())) << ", "
            << year;
    }

    auto rollOfTheYear = get_year_name(year);
    if (rollOfTheYear.has_value())
    {
        ss << ", " << rollOfTheYear.value();
    }

    return ss.str();
}


////
//// CalendarProvider
////
Adndtk::CalendarProvider::CalendarProvider()
{
}

Adndtk::CalendarProvider::~CalendarProvider()
{
}

Adndtk::CalendarProvider& Adndtk::CalendarProvider::get_instance()
{
    static CalendarProvider _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

const Adndtk::Calendar& Adndtk::CalendarProvider::get_calendar(const Defs::calendar& calendarId) const
{
    return _calendars.at(calendarId);
}

void Adndtk::CalendarProvider::init()
{
    auto calIds = Tables::calendar::to_set<Defs::calendar>("id");

    for (auto& id : calIds)
    {
        Calendar cal{id};
        _calendars[id] = cal;
    }
}