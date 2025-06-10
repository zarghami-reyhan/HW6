#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <string>
#include <stdexcept>
using namespace std;

class DateTime
{
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;

public:
    DateTime(int year, int month, int day, int hour, int minute);

    struct Date
    {
        int year;
        int month;
        int day;
        int hour;
        int minute;

        Date(int y = 0, int m = 0, int d = 0, int h = 0, int min = 0)
            : year(y), month(m), day(d), hour(h), minute(min) {
            }

        bool operator==(const Date &other) const
        {
            return year == other.year && month == other.month && day == other.day;
        }
        bool operator!=(const Date &other) const
        {
            return !(*this == other);
        }
    };

    DateTime addHours(int hours) const;

    string toString() const;

    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator==(const DateTime& other) const;

    string getDate() const;

    static DateTime fromString(const string& dateTimeStr);

    static void validateHour(int hour);

    static Date parseDate(const string &dateStr);

    static int daysFromBaseDate(const Date &date);
    static int dayOfWeek(const Date &date);
    static string dayOfWeekName(int dayCode);
};

#endif 