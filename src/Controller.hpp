#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <vector>
#include "Model.hpp"
#include "DateTime.hpp"
#include "User.hpp"
#include "NormalEvent.hpp"
#include "PeriodicEvent.hpp"
#include "Task.hpp"
#include "Activity.hpp" // Include the new header
using namespace std;

class Controller
{
private:
    Model &model;
    

    bool validateHour(int hour);

    // void printReport(shared_ptr<User> user, const DateTime::Date &fromDate, const DateTime::Date &toDate, const string &filterType = ""); // Removed

public:
    Controller(Model &m);

    Model* getModel();
    AuthStatus handleSignup(const string& username, const string& password);
    AuthStatus handleLogin(const string& username, const string& password);
    AuthStatus handleLogout();
    EventStatus handleAddNormalEvent(const string& title, const string& description, const DateTime::Date& date, int start_time, int duration, int& out_event_id);
    vector<shared_ptr<NormalEvent>> handleGetNormalEvents(EventStatus& status_code);
    EventStatus handleAddPeriodicEvent(
        const string& title, const string& description,
        DateTime::Date startDate, // Passed by value
        const DateTime::Date& endDate,
        int startTime, int duration,
        RecurrenceType recurrenceType,
        int dayOfMonth,
        const vector<int>& weeklyDays,
        int& out_event_id
    );
    EventStatus handleAddJointEvent(
        const string& title, const string& description,
        const DateTime::Date& date,
        int startTime, int endTime,
        const vector<string>& guest_usernames,
        int& out_event_id
    );
    TaskStatus handleAddTask(
        const string& title, const string& description,
        const DateTime::Date& dueDate,
        int dueTime,
        int& out_task_id
    );
    void handleDeleteTask(string input);
    void handleEditTask(string input);
    vector<ActivityEntry> handleGetReport(
        const DateTime::Date& fromDate,
        const DateTime::Date& toDate,
        const string& filterType, // "event", "periodic_event", "task", "joint_event", or empty
        EventStatus& status_code // To return PERMISSION_DENIED or SUCCESS
    );
    JointEventActionStatus handleConfirmJointEvent(int event_id);
    JointEventActionStatus handleRejectJointEvent(int event_id);
    void handleViewJoint(string input); // Keep old signature for non-refactored methods
};

#endif