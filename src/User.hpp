#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <memory>
#include "NormalEvent.hpp"
#include "PeriodicEvent.hpp"
#include "JointEvent.hpp"
#include "Task.hpp"
#include <iostream>
using namespace std;

class User
{
private:
    string username;
    string password;
    int normalEventIdCounter;
    int recurringEventIdCounter;
    int taskIdCounter;

    vector<shared_ptr<NormalEvent>> normalEvents;
    vector<shared_ptr<PeriodicEvent>> periodicEvents;
    vector<shared_ptr<Task>> tasks;
    vector<shared_ptr<JointEvent>> jointEvents;
    vector<int> invitations;

public:
    User(const string &username, const string &password);

    string getUsername() const;
    bool checkPassword(const string &password) const;

    int getNextNormalEventId();
    int getNextRecurringEventId();
    int getNextTaskId();

    void addNormalEvent(shared_ptr<NormalEvent> event);
    bool addPeriodicEvent(const shared_ptr<PeriodicEvent> event);
    void addTask(shared_ptr<Task> task);

    const vector<shared_ptr<NormalEvent>> &getNormalEvents() const;
    const vector<shared_ptr<PeriodicEvent>> &getPeriodicEvents() const;
    const vector<shared_ptr<Task>> &getTasks() const;

    bool deleteTask(int task_id);
    bool editTask(int task_id, shared_ptr<Task> new_task);
    bool addJointEvent(shared_ptr<JointEvent> event);
    void addInvitation(int invitationId); 
    void confirmJointEvent(int invitationId);
    void rejectJointEvent(int invitationId);
    vector<shared_ptr<JointEvent>>& getJointEvents() ;
    const vector<int>& getInvitations() const; 
    void removeJointEventById(int event_id);
};

#endif