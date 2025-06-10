#ifndef JOINTEVENT_H
#define JOINTEVENT_H

#include <string>
#include <vector>
#include "DateTime.hpp"
#include "Event.hpp"

class JointEvent : public Event
{
private:
    vector<string> guests;
    bool confirmed;
    string transmiter;

public:
    JointEvent(const string &title, const string &description, const DateTime::Date &date, int startTime, int endTime, const vector<string> &guests, string transmiter);

    const vector<string> &getGuests() const;
    bool isConfirmed() const;
    bool confirm();
    string getTransmiter();
    bool isOverlap(Event &other) const override;
    string toString() override;
    string toPrint();
};

#endif