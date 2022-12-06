//
// Created by batuk on 20.11.2022.
//

#include <cstddef>
#include <chrono>
#include "date.h"
#include "iostream"

project::DateExcept::DateExcept(const std::string & errMsg) : m_error_message{errMsg}
{
 //blank
}

const char * project::DateExcept::what() const noexcept
{
    return  m_error_message.c_str();
}

project::Date::Date() : m_mon_day{1}, m_mon{1}, m_year{1990}
{
    setWDayAndYearDay();
}

project::Date::Date(int d, int m, int y)
{
    checkDateComponentsValueRange(d, m, y);

    m_mon_day= d;
    m_mon= m;
    m_year= y;

    setWDayAndYearDay();
}

project::Date::Date(const char *p)
{
    const std::string pstr{p};

    if(pstr.length() > 10)
    {
        throw DateExcept{"ctor argument length is wrong."};
    }

    int dateComponent[size_of_date_array]{0};
    size_t last{0};
    size_t index{0};

    for(size_t next = pstr.find('/', last);
        next != std::string::npos;
        next = pstr.find('/', last))
    {
       dateComponent[index]= std::stoi(pstr.substr(last, next - last));
       ++index;
       last= next + 1;
    }

    dateComponent[index]= std::stoi(pstr.substr(last));

    checkDateComponentsValueRange(dateComponent[0],
                                  dateComponent[1],
                                  dateComponent[2]);

    m_mon_day= dateComponent[0];
    m_mon= dateComponent[1];
    m_year= dateComponent[2];

    setWDayAndYearDay();
}

project::Date::Date(std::time_t timer)
{
    tm local_tm = *localtime(&timer);

    checkDateComponentsValueRange(local_tm.tm_mday,
                                  local_tm.tm_mon + 1,
                                  local_tm.tm_year + 1900);

    m_mon_day= local_tm.tm_mday;
    m_mon= local_tm.tm_mon + 1;
    m_year= local_tm.tm_year + 1900;
    m_week_day = local_tm.tm_wday;
    m_year_day= local_tm.tm_yday;
}

int project::Date::get_month_day()const
{
    return m_mon_day;
}

int project::Date::get_month() const
{
    return m_mon;
}
int project::Date::get_year() const
{
    return m_year;
}

int project::Date::get_year_day() const
{
    return m_year_day;
}

int project::Date::get_week_day() const
{
    return m_week_day;
}

project::Date& project::Date::set_month_day(int day)
{
    m_mon_day= day;
    return *this;
}

project::Date& project::Date::set_month(int month)
{
    m_mon= month;
    return *this;
}

project::Date& project::Date::set_year(int year)
{
    m_year= year;
    return *this;
}

project::Date& project::Date::set(int day, int mon, int year)
{
    m_mon_day= day;
    m_mon= mon;
    m_year= year;
    return *this;
}

void project::Date::setWDayAndYearDay()
{
    std::tm time_in = { 0, 0, 0,
                        m_mon_day, m_mon - 1, m_year - 1900 };

    std::time_t time_temp = std::mktime(&time_in);

    const std::tm * time_out = std::gmtime(&time_temp);

    m_year_day= time_out->tm_yday;
    m_week_day= time_out->tm_wday;
}

void project::Date::checkDateComponentsValueRange(const int & mday, const int & month, const int & year) const
{
    if(!(mday >= 1 && mday <= 31) ||
       !(month >= 1 && month <= 12) ||
       !(year_base <= year) )
    {
        throw DateExcept{"ctor argument(s) value is out of range."};
    }
}

project::Date project::Date::operator-(int day) const
{
    std::tm tmDateIn = { 0, 0, 0,
                        m_mon_day, m_mon - 1,
                        m_year - 1900 };

    std::time_t timetDateIn= std::mktime(&tmDateIn);
    std::chrono::time_point tpDate= std::chrono::system_clock::from_time_t(timetDateIn);
    std::chrono::time_point newtpDate= tpDate - std::chrono::hours (day * 24);

    std::time_t timeDateOut= std::chrono::system_clock::to_time_t(newtpDate);

    return Date(timeDateOut);
}

project::Date& project::Date::operator+=(int day)
{
    std::tm tmDateIn = { 0, 0, 0,
                         m_mon_day, m_mon - 1,
                         m_year - 1900 };

    std::time_t timetDateIn= std::mktime(&tmDateIn);
    std::chrono::time_point tpDate= std::chrono::system_clock::from_time_t(timetDateIn);

    std::chrono::time_point newtpDate= tpDate + std::chrono::hours (day * 24);
    std::time_t timeDateOut= std::chrono::system_clock::to_time_t(newtpDate);

    tm localTm = *localtime(&timeDateOut);

    checkDateComponentsValueRange(localTm.tm_mday,
                                  localTm.tm_mon + 1,
                                  localTm.tm_year + 1900);

    m_mon_day= localTm.tm_mday;
    m_mon= localTm.tm_mon + 1;
    m_year= localTm.tm_year + 1900;
    m_week_day = localTm.tm_wday;
    m_year_day= localTm.tm_yday;

    return *this;
}

project::Date& project::Date::operator-=(int day)
{
    std::tm tmDateIn = { 0, 0, 0,
                         m_mon_day, m_mon - 1,
                         m_year - 1900 };

    std::time_t timetDateIn= std::mktime(&tmDateIn);
    std::chrono::time_point tpDate= std::chrono::system_clock::from_time_t(timetDateIn);

    std::chrono::time_point newtpDate= tpDate - std::chrono::hours (day * 24);
    std::time_t timeDateOut= std::chrono::system_clock::to_time_t(newtpDate);

    tm localTm = *localtime(&timeDateOut);

    checkDateComponentsValueRange(localTm.tm_mday,
                                  localTm.tm_mon + 1,
                                  localTm.tm_year + 1900);

    m_mon_day= localTm.tm_mday;
    m_mon= localTm.tm_mon + 1;
    m_year= localTm.tm_year + 1900;
    m_week_day = localTm.tm_wday;
    m_year_day= localTm.tm_yday;

    return *this;
}

project::Date& project::Date::operator++()
{
    return (*this += 1);
}

project::Date project::Date::operator++(int)
{
    Date unchangedDate = *this;

    (*this) += 1;

    return unchangedDate;
}

project::Date& project::Date::operator--()
{
    return (*this -= 1);
}

project::Date project::Date::operator--(int)
{
    Date unchangedDate = *this;

    (*this) -= 1;

    return unchangedDate;
}

project::Date project::Date::random_date()
{
    using namespace std::chrono;
    auto lowerBoundDate = year{random_min_year}/1/1;
    auto upperBoundDate = year{random_max_year}/12/31;
    int maxDayNumber= (sys_days{upperBoundDate} - sys_days{lowerBoundDate}).count();
    auto randomDayNumber= rand() % (maxDayNumber + 1 - 0) + 0;

    Date random{1, 1, 1940};
    return random += randomDayNumber;
}

constexpr bool project::Date::isleap(int y)
{
    std::chrono::year chronoYear{y};

    if(chronoYear.ok() && chronoYear.is_leap())
    {
        return true;
    }

    return false;
}

auto project::Date::convertToDayNumber() const -> int
{
    std::tm tmDateIn = { 0, 0, 0,
                           m_mon_day, m_mon - 1,
                           m_year - 1900 };

    std::time_t timetDateIn= std::mktime(&tmDateIn);
    std::chrono::time_point tpDate= std::chrono::system_clock::from_time_t(timetDateIn);
    return std::chrono::duration_cast<std::chrono::days>
            (tpDate.time_since_epoch()).count();
}

bool project::operator<(const Date& d1, const Date& d2)
{
    return d1.convertToDayNumber() < d2.convertToDayNumber();
}

bool project::operator==(const Date& d1, const Date& d2)
{
    return d1.convertToDayNumber() == d2.convertToDayNumber();
}

std::ostream& project::operator<<(std::ostream& os, const Date& date)
{
    return os << std::to_string(date.m_mon_day) << ' ' <<
                 Date::MonthStr[date.m_mon] << ' ' << std::to_string(date.m_year) << ' '
                 << Date::WeekdayStr[date.m_week_day] << '\n';
}

std::istream& project::operator>>(std::istream& is, Date& date)
{
    is >> date.m_mon_day;
    char dash;
    is >> dash;
    if(dash != '/')
        is.setstate(std::ios::failbit);
    is >> date.m_mon;
    is >> dash;
    if(dash != '/')
        is.setstate(std::ios::failbit);
    is >> date.m_year;
    return is;
}

bool project::operator>(const Date& d1, const Date& d2)
{
    return d2 < d1;
}

bool project::operator<=(const Date& d1, const Date& d2)
{
    return !(d1 > d2);
}

bool project::operator>=(const Date& d1, const Date& d2)
{
    return !(d1 < d2);
}

bool project::operator!=(const Date& d1, const Date& d2)
{
    return !(d1 == d2);
}

int project::operator-(const Date& d1, const Date& d2)
{
    return std::abs(d1.convertToDayNumber() - d2.convertToDayNumber());
}

project::Date project::operator+(const Date& date, int n)
{
    return Date{date} + n;

}

project::Date project::operator+(int n, const Date& date)
{
    return date + n;
}

project::Date::Weekday& project::operator++(Date::Weekday& wd)
{
    auto weekDayNum= static_cast<unsigned int>(wd);
    weekDayNum = (weekDayNum == 6) ? 0 : weekDayNum++;

    wd= static_cast<Date::Weekday>(weekDayNum);
    return wd;
}

project::Date::Weekday project::operator++(project::Date::Weekday& wd, int dayNum)
{
    for (auto count{0}; count < dayNum; count++)
    {
        operator++(wd);
    }

    return Date::Weekday{wd};
}

project::Date::Weekday& project::operator--(Date::Weekday& wd)
{
    auto weekDayNum= static_cast<unsigned int>(wd);
    weekDayNum = (weekDayNum == 0) ? 6 : weekDayNum--;

    wd= static_cast<Date::Weekday>(weekDayNum);
    return wd;
}

project::Date::Weekday project::operator--(Date::Weekday& wd, int dayNum)
{
    for (auto count{0}; count < dayNum; count++)
    {
        operator--(wd);
    }

    return Date::Weekday{wd};
}
