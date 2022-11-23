
//
// Created by batuk on 20.11.2022.
//

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
    //blank
}

project::Date::Date(int d, int m, int y)
{
    if( year_base > y)
    {
        throw DateExcept{"Out of Range year value"};
    }

    m_mon_day= d;
    m_mon= m;
    m_year= y;
}

project::Date::Date(const char *p)
{
    //TODO: Construct a date object with using the arg in gg/aa/yil format.
}

project::Date::Date(std::time_t timer)
{
    //TODO: Construct a date object with using the timer object.
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

project::Date::Weekday project::Date::get_week_day() const
{
    return m_week_day;
}



