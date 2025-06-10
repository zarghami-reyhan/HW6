#include "Task.hpp"
#include <sstream>

int Task::nextId = 1;
Task::Task(const string &title, const string &description, const DateTime::Date &Date, int Time)
    : title(title), description(description), Date(Date), Time(Time)
{
    id = nextId++;
}

Task::~Task()
{
}

int Task::getId() const { return id; }
string Task::getTitle() const { return title; }
string Task::getDescription() const { return description; }
DateTime::Date Task::getDueDate() const { return Date; }
int Task::getDueTime() const { return Time; }

void Task::decreamentid()
{
  nextId--;
}
string Task::toString() const
{
    stringstream ss;
    ss << "task \"" << title << "\" on "
       << Date.year << "/" << (Date.month < 10 ? "0" : "") << Date.month << "/"
       << (Date.day < 10 ? "0" : "") << Date.day
       << " at " << Time;
    if (!description.empty())
    {
        ss << ": \"" << description << "\"";
    }
    return ss.str();
}