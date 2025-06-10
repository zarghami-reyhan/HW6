#include "Event.hpp"

int Event::nextId = 1;
Event::Event(const string &title, const string &description,
             const DateTime::Date &date, int startTime, int duration)
    : title(title), description(description), date(date),
      startTime(startTime), duration(duration) { id = nextId++; }
Event::~Event() {}
bool Event::isOverlap(Event &other) const {}

int Event::getId() const { return id; }
string Event::getTitle() const { return title; }
string Event::getDescription() const { return description; }
DateTime::Date Event::getDate() const { return date; }
int Event::getStartTime() const { return startTime; }
int Event::getDuration() const { return duration; }
int Event::getEndTime() const
{
  return duration + startTime;
}
void Event::decreamentid()
{
  nextId = nextId - 1;
}
bool Event::occursOnDate(const DateTime::Date &date) const {}
string Event::toString()
{
}