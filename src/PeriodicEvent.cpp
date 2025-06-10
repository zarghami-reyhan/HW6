#include "PeriodicEvent.hpp"
#include "NormalEvent.hpp"
#include"JointEvent.hpp"
#include "DateTime.hpp"
#include <sstream>

PeriodicEvent::PeriodicEvent(const string& title, const string& description,
                            DateTime::Date& startDate, const DateTime::Date& endDate,
                             int startTime, int duration,
                             RecurrenceType recurrenceType,int day,
                             const vector<int>& weeklyDays)
    : Event(title, description, startDate, startTime, duration),
      endDate(endDate), recurrenceType(recurrenceType),day_of_month(day), weeklyDays(weeklyDays) {
        if (day_of_month >0){
            if(day>startDate.day){
                if (startDate.month<12){
                    startDate.month ++;
                }
                else{
                    startDate.year ++;
                    startDate.month = 1;
                }
                startDate.day=day_of_month;

            }
        }
      }

DateTime::Date PeriodicEvent::getEndDate() const {
    return endDate;
}

RecurrenceType PeriodicEvent::getRecurrenceType() const {
    return recurrenceType;
}

vector<int> PeriodicEvent::getWeeklyDays() const {
    return weeklyDays;
}

bool PeriodicEvent::occursOnDate(const DateTime::Date& date) const {
    int startDays = DateTime::daysFromBaseDate(this->date);
    int endDays = DateTime::daysFromBaseDate(endDate);
    int targetDays = DateTime::daysFromBaseDate(date);

    if (targetDays < startDays || targetDays > endDays) return false;

    switch(recurrenceType) {
        case RecurrenceType::Daily:
            return true;
        case RecurrenceType::Weekly: {
            int dayCode = DateTime::dayOfWeek(date);
            for(int wd : weeklyDays) {
                if(dayCode == wd) return true;
            }
            return false;
        }
        case RecurrenceType::Monthly:
            return (date.day == this->date.day);
    }
    return false;
}

bool PeriodicEvent::isOverlap( Event& other) const {
    const NormalEvent* normalOther = dynamic_cast<const NormalEvent*>(&other);
    if(normalOther) {
        if(!this->occursOnDate(normalOther->getDate())) return false;
        int start1 = this->startTime;
        int end1 = this->getEndTime();
        int start2 = normalOther->getStartTime();
        int end2 = start2 + normalOther->getDuration();
        return (start1 < end2 && start2 < end1);
    }
    const PeriodicEvent* periodicOther = dynamic_cast<const PeriodicEvent*>(&other);
    if(periodicOther) {
        int start1 = DateTime::daysFromBaseDate(this->date);
        int end1 = DateTime::daysFromBaseDate(this->endDate);
        int start2 = DateTime::daysFromBaseDate(periodicOther->date);
        int end2 = DateTime::daysFromBaseDate(periodicOther->endDate);

        int overlapStart = max(start1, start2);
        int overlapEnd = min(end1, end2);
        if(overlapStart > overlapEnd) return false;

        for(int d = overlapStart; d <= overlapEnd; ++d) {
            int y = 1404 + (d / 360);
            int rem = d % 360;
            int m = 1 + (rem / 30);
            int day = 1 + (rem % 30);
            DateTime::Date currentDate(y, m, day);
            
            if(this->occursOnDate(currentDate) && periodicOther->occursOnDate(currentDate)) {
                int startTime1 = this->startTime;
                int endTime1 = this->getEndTime();
                int startTime2 = periodicOther->startTime;
                int endTime2 = periodicOther->getEndTime();
                if(startTime1 < endTime2 && startTime2 < endTime1)
                    return true;
            }
        }
        return false;
    }
      const JointEvent* jointOther = dynamic_cast<const JointEvent*>(&other);
    if(jointOther) {
        if(this->date != jointOther->getDate()) return false; 
        int start1 = this->startTime;
        int end1 = this->getEndTime();
        int start2 = jointOther->getStartTime();
        int end2 = jointOther->getEndTime();
        return (start1 < end2 && start2 < end1);
    }
    return false;
}

string PeriodicEvent::toString()  {
    ostringstream oss;
    oss << "periodic_event \"" << title << "\" on "
        << date.year << "/" << (date.month < 10 ? "0" : "") << date.month << "/"
        << (date.day < 10 ? "0" : "") << date.day
        << " to "
        << endDate.year << "/" << (endDate.month < 10 ? "0" : "") << endDate.month << "/"
        << (endDate.day < 10 ? "0" : "") << endDate.day
        << " from " << startTime << " for " << duration << " hours";
    if(!description.empty()) {
        oss << ": \"" << description << "\"";
    }
    return oss.str();
}
