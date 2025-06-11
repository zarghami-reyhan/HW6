#ifndef MODEL_HPP
#define MODEL_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include "User.hpp"
#include "HolidayManager.hpp"
#include "JointEvent.hpp"
using namespace std;

// Enum for Authentication Status
enum class AuthStatus {
    SUCCESS,
    PERMISSION_DENIED, // User already logged in, or no user logged in for logout
    USER_EXISTS,       // For signup: username taken
    BAD_REQUEST,       // For signup/login: other validation error (e.g. empty username/password)
    NOT_FOUND,         // For login: user not found
    INVALID_CREDENTIALS // For login: password incorrect
};

// Enum for Event Status
enum class EventStatus {
    SUCCESS,
    PERMISSION_DENIED, // No user logged in
    OVERLAP,
    HOLIDAY_FOUND,
    BAD_REQUEST,        // Invalid input parameters
    GUEST_NOT_FOUND     // New status for addJointEvent
};

// Enum for Task Status
enum class TaskStatus {
    SUCCESS,
    PERMISSION_DENIED, // No user logged in
    BAD_REQUEST        // Invalid input parameters (e.g., empty title)
};

// Enum for Joint Event Action Status
enum class JointEventActionStatus {
    SUCCESS,
    PERMISSION_DENIED, // No user logged in, or not authorized for this action
    EVENT_NOT_FOUND,
    ALREADY_CONFIRMED,
    EVENT_IS_NOT_A_JOINT_EVENT,
    OVERLAP,           // If confirming causes an overlap for the current user
    ACTION_FAILED      // Generic failure
};

class Model
{
private:
    unordered_map<string, shared_ptr<User>> users;
    HolidayManager holidayManager;
    shared_ptr<User> currentUser;
    vector<shared_ptr<JointEvent>> jointEvents;

public:
    Model();

    AuthStatus signup(const string &username, const string &password);
    AuthStatus login(const string &username, const string &password);
    AuthStatus logout();

    shared_ptr<User> getCurrentUser() const;

    HolidayManager &getHolidayManager();
    EventStatus addNormalEvent(const shared_ptr<NormalEvent> &event, int& out_event_id);

    EventStatus addPeriodicEvent(const shared_ptr<PeriodicEvent> &event, int& out_event_id);

    TaskStatus addTask(const shared_ptr<Task> &task, int& out_task_id);

    const vector<shared_ptr<NormalEvent>> &getNormalEvents() const;
    const vector<shared_ptr<PeriodicEvent>> &getPeriodicEvents() const;
    const vector<shared_ptr<Task>> &getTasks() const;

    bool hasOverlapWithEvents(Event &event) const;
    bool hasOverlapWithEventsPeriodic(const PeriodicEvent &event) const;

    bool isHolidayOverlap(const Event &event) const;
    int isHolidayOverlapPeriodic(const PeriodicEvent &event, vector<DateTime::Date> &holidaysOverlapDays) const;

    bool deleteTask(int task_id);
    bool editTask(int task_id, shared_ptr<Task> new_task);
    EventStatus addJointEvent(shared_ptr<JointEvent> &event, int& out_event_id);
    void print_joint_event(); // Keep for now, might be used by old CLI parts
    JointEventActionStatus confirmJointEventInvitation(int event_id);
    JointEventActionStatus rejectJointEventInvitation(int event_id);
};

#endif