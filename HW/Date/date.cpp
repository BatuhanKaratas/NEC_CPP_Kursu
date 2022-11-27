//
// Created by batuk on 20.11.2022.
//

#include <cstddef>
#include <chrono>
#include "date.h"

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
    tm utc_tm = *gmtime(&timer);
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
    const std::tm * time_out = std::localtime(&time_temp);

    m_week_day= time_out->tm_wday;
    m_year_day= time_out->tm_yday;
}

void project::Date::checkDateComponentsValueRange(const int & mday, const int & month, const int & year) const
{
    if((mday >= 1 && mday <= 31) ||
       (month >= 1 && month <= 12) ||
       (year < year_base))
    {
        throw DateExcept{"ctor argument(s) value is out of range."};
    }
}


