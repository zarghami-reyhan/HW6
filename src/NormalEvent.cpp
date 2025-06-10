#include "NormalEvent.hpp"
#include "PeriodicEvent.hpp"
#include "JointEvent.hpp"
#include <sstream>
#include <iostream>
#include <string>

NormalEvent::NormalEvent(const string &title, const string &description,
                         const DateTime::Date &date, int startTime, int duration)
    : Event(title, description, date, startTime, duration) {}

bool NormalEvent::isOverlap(Event &other) const
{
    const NormalEvent *normalOther = dynamic_cast<const NormalEvent *>(&other);
    if (normalOther)
    {
        if (this->date != normalOther->date)
            return false;
        int start1 = this->startTime;
        int end1 = this->getEndTime();
        int start2 = normalOther->startTime;
        int end2 = normalOther->getEndTime();
        return (start1 < end2 && start2 < end1);
    }
    const PeriodicEvent *periodicOther = dynamic_cast<const PeriodicEvent *>(&other);
    if (periodicOther)
    {
        if (!periodicOther->occursOnDate(this->date))
            return false;
        int start1 = this->startTime;
        int end1 = this->getEndTime();
        int start2 = periodicOther->getStartTime();
        int end2 = periodicOther->getEndTime();
        return (start1 < end2 && start2 < end1);
    }
    
    const JointEvent *jointOther = dynamic_cast<const JointEvent *>(&other);
    if (jointOther)
    {
        if (this->date != jointOther->getDate())
            return false; 
        int start1 = this->startTime;
        int end1 = this->getEndTime();
        int start2 = jointOther->getStartTime();
        int end2 = jointOther->getEndTime();
        return (start1 < end2 && start2 < end1);
    }
    return false;
}

string NormalEvent::toString()
{
    ostringstream oss;
    oss << "event \"" << title << "\" on "
        << date.year << "/" << (date.month < 10 ? "0" : "") << date.month << "/"
        << (date.day < 10 ? "0" : "") << date.day
        << " from " << startTime << " for " << duration << " hours";
    if (!description.empty())
    {
        oss << ": \"" << description << "\"";
    }
    return oss.str();
}