//
// Created by batuk on 20.11.2022.
//

#include <cstddef>
#include <chrono>
#include <iostream>
#include <utility>
#include "date.h"

project::DateExcept::DateExcept(std::string errMsg) : m_error_message{std::move(errMsg)} {
    //blank
}

const char *project::DateExcept::what() const noexcept {
    return m_error_message.c_str();
}

project::Date::Date() : m_mon_day{1}, m_mon{1}, m_year{YEAR_BASE} {
    setWDayAndYearDay();
}

project::Date::Date(int d, int m, int y) : m_mon_day{d}, m_mon{m}, m_year{y} {
    checkDateComponentsValueRange(d, m, y);
    setWDayAndYearDay();
}

project::Date::Date(const char *p) {
    const std::string pstr{p};

    if (pstr.length() > MAX_NUMBER_OF_CHAR_IN_DATE) {
        throw DateExcept{"ctor argument length is wrong."};
    }

    std::array<int, SIZE_OF_DATE_ARRAY> dateComponent{0};
    size_t last{0};
    size_t index{0};

    for (size_t next = pstr.find('/', last);
         next != std::string::npos;
         next = pstr.find('/', last)) {
        dateComponent[index] = std::stoi(pstr.substr(last, next - last));
        ++index;
        last = next + 1;
    }

    dateComponent[index] = std::stoi(pstr.substr(last));

    checkDateComponentsValueRange(dateComponent[0],
                                  dateComponent[1],
                                  dateComponent[2]);

    m_mon_day = dateComponent.at(0);
    m_mon = dateComponent.at(1);
    m_year = dateComponent.at(2);

    setWDayAndYearDay();
}

project::Date::Date(std::time_t timer) {
    const std::tm utcTM = *gmtime(&timer);

    checkDateComponentsValueRange(utcTM.tm_mday,
                                  utcTM.tm_mon + 1,
                                  utcTM.tm_year + YEAR_BASE);

    m_mon_day = utcTM.tm_mday;
    m_mon = utcTM.tm_mon + 1;
    m_year = utcTM.tm_year + YEAR_BASE;
    m_week_day = utcTM.tm_wday;
    m_year_day = utcTM.tm_yday;
}

int project::Date::get_month_day() const {
    return m_mon_day;
}

int project::Date::get_month() const {
    return m_mon;
}

int project::Date::get_year() const {
    return m_year;
}

int project::Date::get_year_day() const {
    return m_year_day;
}

int project::Date::get_week_day() const {
    return m_week_day;
}

project::Date &project::Date::set_month_day(int day) {
    m_mon_day = day;
    return *this;
}

project::Date &project::Date::set_month(int month) {
    m_mon = month;
    return *this;
}

project::Date &project::Date::set_year(int year) {
    m_year = year;
    return *this;
}

project::Date &project::Date::set(int day, int mon, int year) {
    m_mon_day = day;
    m_mon = mon;
    m_year = year;
    return *this;
}

void project::Date::setWDayAndYearDay() {
    std::tm timeTM = {0, 0, 0,
                       m_mon_day, m_mon - 1, m_year - YEAR_BASE};

    std::time_t timeT = std::mktime(&timeTM);

    if(const std::tm *time_out = std::gmtime(&timeT))
    {
        m_year_day = time_out->tm_yday;
        m_week_day = time_out->tm_wday;
    }
    else
    {
        throw DateExcept{"Date components are not proper."};
    }
}

void project::Date::checkDateComponentsValueRange(const int &mday, const int &month, const int &year) {
    if ((mday < 1 || mday > 31) ||
        (month < 1 || month > 12) ||
        (YEAR_BASE > year)) {
        throw DateExcept{"ctor argument(s) value is out of range."};
    }
}

project::Date project::Date::operator-(int day) const {
    std::time_t timeDateOut = std::chrono::system_clock::to_time_t(convertTmToTp() -
                                                                      std::chrono::hours(day * 24));

    return Date(timeDateOut);
}

project::Date &project::Date::operator+=(int day) {
    std::time_t timeDateOut = std::chrono::system_clock::to_time_t(convertTmToTp() +
                                                                      std::chrono::hours(day * 24));

    if(std::tm * utcTm = gmtime(&timeDateOut)) {

        checkDateComponentsValueRange(utcTm->tm_mday,
                                      utcTm->tm_mon + 1,
                                      utcTm->tm_year + YEAR_BASE);

        m_mon_day = utcTm->tm_mday;
        m_mon = utcTm->tm_mon + 1;
        m_year = utcTm->tm_year + YEAR_BASE;
        m_week_day = utcTm->tm_wday;
        m_year_day = utcTm->tm_yday;

        return *this;
    }
    else
    {
        throw DateExcept{"Date components are not proper."};
    }
}

project::Date &project::Date::operator-=(int day) {

    std::time_t timeDateOut = std::chrono::system_clock::to_time_t(convertTmToTp() -
                                                                   std::chrono::hours(day * 24));

    if(std::tm * utcTm = gmtime(&timeDateOut)) {

        checkDateComponentsValueRange(utcTm->tm_mday,
                                      utcTm->tm_mon + 1,
                                      utcTm->tm_year + YEAR_BASE);

        m_mon_day = utcTm->tm_mday;
        m_mon = utcTm->tm_mon + 1;
        m_year = utcTm->tm_year + YEAR_BASE;
        m_week_day = utcTm->tm_wday;
        m_year_day = utcTm->tm_yday;

        return *this;
    }
    else
    {
        throw DateExcept{"Date components are not proper."};
    }
}

project::Date &project::Date::operator++() {
    return (*this += 1);
}

project::Date project::Date::operator++(int) {
    Date unchangedDate = *this;

    (*this) += 1;

    return unchangedDate;
}

project::Date &project::Date::operator--() {
    return (*this -= 1);
}

project::Date project::Date::operator--(int) {
    Date unchangedDate = *this;

    (*this) -= 1;

    return unchangedDate;
}

project::Date project::Date::random_date() {
    const auto lowerBoundDate = std::chrono::year{RANDOM_MIN_YEAR} / 1 / 1;
    const auto upperBoundDate = std::chrono::year{RANDOM_MAX_YEAR} / 12 / 31;
    const auto maxDayNumber = (std::chrono::sys_days{upperBoundDate} - std::chrono::sys_days{lowerBoundDate}).count();
    const auto randomDayNumber = std::rand() % (maxDayNumber + 1 - 0) + 0;

    Date random{1, 1, RANDOM_MIN_YEAR};
    return random += static_cast<int>(randomDayNumber);
}

constexpr bool project::Date::isleap(int y) {
    if (std::chrono::year chronoYear{y}; chronoYear.ok() && chronoYear.is_leap()) {
        return true;
    }

    return false;
}

auto project::Date::convertTmToTp() const ->std::chrono::time_point<std::chrono::system_clock>
{
    std::tm tmDateIn = {0, 0, 0,
                        m_mon_day, m_mon - 1,
                        m_year - YEAR_BASE};

    std::time_t timetDateIn = std::mktime(&tmDateIn);
    return std::chrono::system_clock::from_time_t(timetDateIn);
}


auto project::Date::convertToDayNumber() const{
    return std::chrono::duration_cast<std::chrono::days>(convertTmToTp().time_since_epoch()).count();
}

bool project::operator<(const Date &d1, const Date &d2) {
    return d1.convertToDayNumber() < d2.convertToDayNumber();
}

bool project::operator==(const Date &d1, const Date &d2) {
    return d1.convertToDayNumber() == d2.convertToDayNumber();
}

std::ostream &project::operator<<(std::ostream &os, const Date &date) {
    return os << std::to_string(date.m_mon_day) << ' ' <<
              Date::MonthStr[date.m_mon] << ' ' << std::to_string(date.m_year) << ' '
              << Date::WeekdayStr[date.m_week_day] << '\n';
}

std::istream &project::operator>>(std::istream &is, Date &date) {
    is >> date.m_mon_day;
    char dash{};
    is >> dash;
    if (dash != '/')
        is.setstate(std::ios::failbit);
    is >> date.m_mon;
    is >> dash;
    if (dash != '/')
        is.setstate(std::ios::failbit);
    is >> date.m_year;
    return is;
}

bool project::operator>(const Date &d1, const Date &d2) {
    return d2 < d1;
}

bool project::operator<=(const Date &d1, const Date &d2) {
    return !(d1 > d2);
}

bool project::operator>=(const Date &d1, const Date &d2) {
    return !(d1 < d2);
}

bool project::operator!=(const Date &d1, const Date &d2) {
    return !(d1 == d2);
}

int project::operator-(const Date &d1, const Date &d2) {
    return std::abs(static_cast<int>(d1.convertToDayNumber() - d2.convertToDayNumber()));
}

project::Date project::operator+(const Date &date, int n) {
    return Date{date} += n;
}

project::Date project::operator+(int n, const Date &date) {
    return date + n;
}

project::Date::Weekday &project::operator++(Date::Weekday &wd) {
    auto weekDayNum = static_cast<unsigned int>(wd);
    weekDayNum = (weekDayNum == 6) ? 0 : weekDayNum++;

    wd = static_cast<Date::Weekday>(weekDayNum);
    return wd;
}

project::Date::Weekday project::operator++(project::Date::Weekday &wd, int dayNum) {
    for (auto count{0}; count < dayNum; count++) {
        operator++(wd);
    }

    return Date::Weekday{wd};
}

project::Date::Weekday &project::operator--(Date::Weekday &wd) {
    auto weekDayNum = static_cast<unsigned int>(wd);
    weekDayNum = (weekDayNum == 0) ? 6 : weekDayNum--;

    wd = static_cast<Date::Weekday>(weekDayNum);
    return wd;
}

project::Date::Weekday project::operator--(Date::Weekday &wd, int dayNum) {
    for (auto count{0}; count < dayNum; count++) {
        operator--(wd);
    }

    return Date::Weekday{wd};
}
