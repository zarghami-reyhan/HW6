#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <ctime>
#include "DateTime.hpp"
using namespace std;
class Event
{
protected:
    static int nextId;
    int id;
    string title;
    string description;
    DateTime::Date date;
    int startTime;
    int duration;

public:
    Event(const string &title, const string &description,
          const DateTime::Date &date, int startTime, int duration);
    virtual ~Event();

    int getId() const;
    string getTitle() const;
    string getDescription() const;
    DateTime::Date getDate() const;
    int getStartTime() const;
    int getDuration() const;
    int getEndTime() const;
    void decreamentid();

    virtual bool isOverlap(Event &other) const = 0;
    virtual bool occursOnDate(const DateTime::Date &date) const;

    virtual string toString();
};

#endif