#include "HolidayManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

HolidayManager::HolidayManager() {}

bool HolidayManager::loadHolidaysFromCSV(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Cannot open holidays file: " << filename << endl;
        return false;
    }

    holidays.clear();
    string line;
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string dayStr, monthStr, yearStr;

        if (!getline(ss, dayStr, ','))
            continue;
        if (!getline(ss, monthStr, ','))
            continue;
        if (!getline(ss, yearStr, ','))
            continue;

        int day = stoi(dayStr);
        int month = stoi(monthStr);
        int year = stoi(yearStr);

        holidays.push_back(DateTime::Date(year, month, day));
    }

    file.close();

    return true;
}

bool HolidayManager::isHoliday(const DateTime::Date &date) const
{
    for (const auto &h : holidays)
    {
        if (h == date)
            return true;
    }
    return false;
}

const vector<DateTime::Date> &HolidayManager::getHolidays() const
{
    return holidays;
}

void HolidayManager::printHolidays()
{
     for (const auto &holiday : holidays)
    {
        cout << holiday.year << "/"
             << (holiday.month < 10 ? "0" : "") << holiday.month << "/"
             << (holiday.day < 10 ? "0" : "") << holiday.day
             << endl;
    }
}
