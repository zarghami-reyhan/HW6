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
using namespace std;

class Controller
{
private:
    Model &model;
    

    bool validateHour(int hour);

    void printReport(shared_ptr<User> user, const DateTime::Date &fromDate, const DateTime::Date &toDate, const string &filterType = "");

public:
    Controller(Model &m);

    Model* getModel();
    AuthStatus handleSignup(const string& username, const string& password);
    AuthStatus handleLogin(const string& username, const string& password);
    AuthStatus handleLogout();
    void handleAddNormalEvent(string input);
    void handleAddRecurringEvent(string input);
    void handleAddTask(string input);
    void handleDeleteTask(string input);
    void handleEditTask(string input);
    void handleGetReport(string input);
    void handleJoinEvent(string input);
    void handleConfirmJointEvent(string input);
    void handleRejectJointEvent(string input);
    void handleViewJoint(string input);
};

#endif