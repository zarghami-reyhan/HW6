#include "DateTime.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

DateTime::DateTime(int year, int month, int day, int hour, int minute)
    : year(year), month(month), day(day), hour(hour), minute(minute) 
{
    validateHour(hour);
}

DateTime DateTime::addHours(int hours) const {
    DateTime newDateTime = *this;
    newDateTime.hour += hours;

    while (newDateTime.hour >= 24) {
        newDateTime.hour -= 24;
        newDateTime.day++;
    
    }

    return newDateTime;
}

string DateTime::toString() const {
    ostringstream oss;
    oss << setw(4) << setfill('0') << year << "/"
        << setw(2) << setfill('0') << month << "/"
        << setw(2) << setfill('0') << day << " "
        << setw(2) << setfill('0') << hour << ":"
        << setw(2) << setfill('0') << minute;
    return oss.str();
}

bool DateTime::operator<(const DateTime& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    return minute < other.minute;
}

bool DateTime::operator>(const DateTime& other) const {
    return !(*this < other) && !(*this == other);
}

bool DateTime::operator==(const DateTime& other) const {
    return year == other.year && month == other.month && day == other.day &&
           hour == other.hour && minute == other.minute;
}

string DateTime::getDate() const {
    ostringstream oss;
    oss << setw(4) << setfill('0') << year << "/"
        << setw(2) << setfill('0') << month << "/"
        << setw(2) << setfill('0') << day;
    return oss.str();
}

DateTime DateTime::fromString(const string& dateTimeStr) {
    istringstream iss(dateTimeStr);
    int year, month, day, hour, minute;
    char delim1, delim2, delim3, delim4;

    if (!(iss >> year >> delim1 >> month >> delim2 >> day >> delim3 >> hour >> delim4 >> minute) ||
        delim1 != '/' || delim2 != '/' || delim3 != ' ' || delim4 != ':') {
        throw invalid_argument("Invalid date/time format");
    }

    return DateTime(year, month, day, hour, minute);
}

void DateTime::validateHour(int hour) {
    if (hour < 0 || hour > 23) {
        throw out_of_range("Hour must be between 0 and 23");
    }
}

DateTime::Date DateTime::parseDate(const string &dateStr) {
    istringstream iss(dateStr);
    int y, m, d;
    char delim1, delim2;
    if (!(iss >> y >> delim1 >> m >> delim2 >> d) || delim1 != '/' || delim2 != '/') {
        throw invalid_argument("Invalid date format");
    }
    return Date(y, m, d);
}

int DateTime::daysFromBaseDate(const Date &date) {
    int days = (date.year - 1404) * 360 + (date.month - 1) * 30 + (date.day - 1);
    return days;
}

int DateTime::dayOfWeek(const Date &date) {
    int baseDayCode = 5;
    int daysDiff = daysFromBaseDate(date);
    return (baseDayCode + daysDiff) % 7;
}

string DateTime::dayOfWeekName(int dayCode) {
    static const char* names[] = { "شنبه", "یکشنبه", "دوشنبه", "سه‌شنبه", "چهارشنبه", "پنجشنبه", "جمعه" };
    if (dayCode < 0 || dayCode > 6) return "نامعلوم";
    return names[dayCode];
}