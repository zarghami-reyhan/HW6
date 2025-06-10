#ifndef TASK_H
#define TASK_H

#include <string>
#include "DateTime.hpp"
using namespace std;

class Task
{
private:
    static int nextId;
    int id;
    string title;
    string description;
    DateTime::Date Date;
    int Time;

public:
    Task(const string &title, const string &description, const DateTime::Date &Date, int Time);
    virtual ~Task();
    int getId() const;
    string getTitle() const;
    string getDescription() const;
    DateTime::Date getDueDate() const;
    int getDueTime() const;
    void decreamentid();
    string toString() const;
};

#endif