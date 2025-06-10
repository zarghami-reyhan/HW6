#ifndef NORMALEVENT_H
#define NORMALEVENT_H

#include "Event.hpp"
#include "DateTime.hpp"
using namespace std;

class NormalEvent : public Event
{
public:
    NormalEvent( const string &title, const string &description, const DateTime::Date &date, int startTime, int duration);
    bool isOverlap(Event &other) const override;
    string toString()  override;
};

#endif