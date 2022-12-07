//
// Created by batuk on 20.11.2022.
//

#ifndef DATE_H
#define DATE_H

#include <iosfwd>
#include <ctime>
#include <stdexcept>
#include <string>
#include <array>

namespace project {
    class DateExcept : public std::exception
    {
    public:
        explicit DateExcept(std::string );
        [[nodiscard]] const char * what() const noexcept override;

    private:
        std::string m_error_message{};
    };

    class Date {
    public:
        static constexpr int YEAR_BASE = 1900;  //1
        static constexpr int RANDOM_MIN_YEAR = 1940;  //2
        static constexpr int RANDOM_MAX_YEAR = 2020;  //3
        enum class Weekday : unsigned int { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday }; //4

        Date();
        Date(int d, int m, int y);  //6
        explicit Date(const char* p);  //7
        explicit Date(std::time_t timer); //8

        [[nodiscard]] int get_month_day()const; //9
        [[nodiscard]] int get_month()const; //10
        [[nodiscard]] int get_year()const; //11
        [[nodiscard]] int get_year_day()const; //12
        [[nodiscard]] int get_week_day()const; //13

        Date& set_month_day(int day); //14
        Date& set_month(int month); //15
        Date& set_year(int year); //16
        Date& set(int day, int mon, int year); //17

        Date operator-(int day)const; //18
        Date& operator+=(int day); //19
        Date& operator-=(int day); //20
        Date& operator++(); //21
        Date operator++(int); //22
        Date& operator--(); //23
        Date operator--(int); //24

        static Date random_date(); //25
        static constexpr bool isleap(int y); //26
        [[nodiscard]]auto convertToDayNumber() const;
        [[nodiscard]]auto convertTmToTp() const ->std::chrono::time_point<std::chrono::system_clock>;


        friend bool operator<(const Date&, const Date&); //27
        friend bool operator==(const Date&, const Date&); //27
        friend std::ostream& operator<<(std::ostream&, const Date& date); //31
        friend std::istream& operator>>(std::istream&, Date& date); //32

    private:
        static void checkDateComponentsValueRange(const int & mday, const int & month, const int & year);
        void setWDayAndYearDay();

        int m_mon_day{};
        int m_mon{};
        int m_year{};

        int m_year_day{};
        int m_week_day{};

        static constexpr uint8_t SIZE_OF_DATE_ARRAY{3};
        static constexpr uint8_t MAX_NUMBER_OF_CHAR_IN_DATE{10};

        static const inline std::array<std::string, 7> WeekdayStr{"Pazar", "Pazartesi", "Sali",
                                                                  "Carsamba", "Persembe", "Cuma",
                                                                  "Cumartesi"};

        static const inline std::array<std::string, 12> MonthStr{"Ocak", "Subat", "Mart",
                                                                 "Nisan", "Mayis","Haziran",
                                                                 "Temmuz", "Agustos", "Eylul",
                                                                 "Ekim", "Kasim", "Aralik"};

    };

    bool operator<(const Date&, const Date&); //27
    bool operator>(const Date&, const Date&); //27
    bool operator<=(const Date&, const Date&); //27
    bool operator>=(const Date&, const Date&); //27
    bool operator==(const Date&, const Date&); //27
    bool operator!=(const Date&, const Date&); //27

    int operator-(const Date& d1, const Date& d2); //28
    Date operator+(const Date& date, int n); //29
    Date operator+(int n, const Date&); //29
    Date::Weekday& operator++(Date::Weekday&); //30
    Date::Weekday operator++(Date::Weekday&, int); //30
    Date::Weekday& operator--(Date::Weekday&); //30
    Date::Weekday operator--(Date::Weekday&, int); //30

    std::ostream& operator<<(std::ostream&, const Date& date); //31
    std::istream& operator>>(std::istream&, Date& date); //32
}
#endif
