#ifndef RECURRINGEVENT_H
#define RECURRINGEVENT_H

#include "Event.hpp"
#include <vector>

enum class RecurrenceType
{
    Daily,
    Weekly,
    Monthly

    
};

inline RecurrenceType stringToRecurrenceType(const std::string& str) {
    if (str == "Daily") return RecurrenceType::Daily;
    if (str == "Weekly") return RecurrenceType::Weekly;
    if (str == "Monthly") return RecurrenceType::Monthly;
    
}

class PeriodicEvent : public Event
{
private:
    int day_of_month;
    DateTime::Date endDate;
    RecurrenceType recurrenceType;
    vector<int> weeklyDays; 

public:
    PeriodicEvent(const string &title, const string &description,
                   DateTime::Date &startDate, const DateTime::Date &endDate,
                  int startTime, int duration,
                  RecurrenceType recurrenceType,int day = 0,
                  const vector<int> &weeklyDays = {});

    DateTime::Date getEndDate() const;
    RecurrenceType getRecurrenceType() const;
    vector<int> getWeeklyDays() const;

    bool isOverlap( Event &other) const override;

    bool occursOnDate(const DateTime::Date &date) const override;

    string toString() override;
};

#endif 