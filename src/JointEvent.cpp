#include "JointEvent.hpp"
#include "NormalEvent.hpp"
#include "PeriodicEvent.hpp"
#include "Event.hpp"
#include <iostream>
#include <string>

JointEvent::JointEvent(const string &title, const string &description, const DateTime::Date &date, int startTime, int endTime, const vector<string> &guests, string transmiter)
    : Event(title, description, date, startTime, 0), guests(guests), confirmed(false), transmiter(transmiter)
{
    duration = endTime - startTime;
}

const vector<string> &JointEvent::getGuests() const
{
    return guests;
}

bool JointEvent::isConfirmed() const
{
    return confirmed;
}

bool JointEvent::confirm()
{

    confirmed = true;
    return true;
}

string JointEvent::getTransmiter()
{
    return transmiter;
}

bool JointEvent::isOverlap(Event &other) const
{
    const NormalEvent *normalOther = dynamic_cast<const NormalEvent *>(&other);
    if (normalOther)
    {
        if (this->date != normalOther->getDate())
            return false;
        int start1 = this->startTime;
        int end1 = this->getEndTime();
        int start2 = normalOther->getStartTime();
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

string JointEvent::toString()
{
    string guestList;
    for (const auto &guest : guests)
    {
        guestList += "\"" + guest + "\", ";
    }
    if (!guestList.empty())
    {
        guestList = guestList.substr(0, guestList.length() - 2);
    }
    string month = to_string(getDate().month);
    string day = to_string(getDate().day);
    if (getDate().month < 10)
    {
        month="0" + month;
    }
    if (getDate().day < 10)
    {
        day="0" + day;
    }

    return to_string(getId()) + ": " + "\"" + getTitle() + "\" - " + to_string(getDate().year) + "/" + month + "/" + day +
           " - " + to_string(getStartTime()) + " - " + to_string(getEndTime());
}

string JointEvent::toPrint()
{

    string guestList;
    for (const auto &guest : guests)
    {
        guestList += "\"" + guest + "\", ";
    }
    if (!guestList.empty())
    {
        guestList = guestList.substr(0, guestList.length() - 2);
    }
    string month = to_string(getDate().month);
    string day = to_string(getDate().day);
    if (getDate().month < 10)
    {
        month="0" + month;
    }
    if (getDate().day < 10)
    {
        day="0" + day;
    }

    return  "join_event \"" + getTitle() + "\"on " + to_string(getDate().year) + "/" + month + "/" + day +
           " from " + to_string(getStartTime()) + " to " + to_string(getEndTime()) + "hosted by \"" +
            transmiter+"\" : " + "\""+getDescription()+"\"" ;
}
