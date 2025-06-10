#ifndef MODEL_HPP
#define MODEL_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include "User.hpp"
#include "HolidayManager.hpp"
#include "JointEvent.hpp"
using namespace std;

class Model
{
private:
    unordered_map<string, shared_ptr<User>> users;
    HolidayManager holidayManager;
    shared_ptr<User> currentUser;
    vector<shared_ptr<JointEvent>> jointEvents;

public:
    Model();

    bool signup(const string &username, const string &password);
    bool login(const string &username, const string &password);
    bool logout();

    shared_ptr<User> getCurrentUser() const;

    HolidayManager &getHolidayManager();
    bool addNormalEvent(const shared_ptr<NormalEvent> &event);

    bool addPeriodicEvent(const shared_ptr<PeriodicEvent> &event);

    bool addTask(const shared_ptr<Task> &task);

    const vector<shared_ptr<NormalEvent>> &getNormalEvents() const;
    const vector<shared_ptr<PeriodicEvent>> &getPeriodicEvents() const;
    const vector<shared_ptr<Task>> &getTasks() const;

    bool hasOverlapWithEvents(Event &event) const;
    bool hasOverlapWithEventsPeriodic(const PeriodicEvent &event) const;

    bool isHolidayOverlap(const Event &event) const;
    int isHolidayOverlapPeriodic(const PeriodicEvent &event, vector<DateTime::Date> &holidaysOverlapDays) const;

    bool deleteTask(int task_id);
    bool editTask(int task_id, shared_ptr<Task> new_task);
    bool addJointEvent(shared_ptr<JointEvent> &event) const;
    void print_joint_event();
    bool Taiid(int id,shared_ptr<User> user,int i);
    void rad(int id);
};

#endif