#ifndef HOLIDAYMANAGER_HPP
#define HOLIDAYMANAGER_HPP

#include <vector>
#include <string>
#include "DateTime.hpp"
using namespace std;

class HolidayManager {
private:
    vector<DateTime::Date> holidays;

public:
    HolidayManager();
    bool loadHolidaysFromCSV(const string& filename);
    bool isHoliday(const DateTime::Date& date) const;
    const vector<DateTime::Date>& getHolidays() const;
    void printHolidays()  ;

};

#endif