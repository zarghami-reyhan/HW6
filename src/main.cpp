#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Controller.hpp"
#include "Model.hpp"

#define post "POST"
#define get "GET"
#define put "PUT"
#define delete "DELETE"
#define signup "signup"
#define logout "logout"
#define login_command "login"
#define event "event"
#define periodic_event "periodic_event"
#define task "task"
#define report "report"
#define join_event "join_event"
#define join_event_show "join_event"
#define confirm_join_event "confirm_join_event"
#define reject_join_event "reject_join_event"

using namespace std;
int main(int argc, char *argv[])
{
    Model model;
    Controller controller(model);
    if (argc < 2)
    {
        std::cerr << "Usage: ./UTrello <path/to/holiday/file>" << std::endl;
        return 1;
    }
    HolidayManager manager;

    std::string csvFilePath = argv[1];
    model.getHolidayManager().loadHolidaysFromCSV(csvFilePath);
    string input;

    while (getline(cin, input))
    {
        string order = "";
        string command = "";

        istringstream iss(input);
        iss >> order;
        if (order == post)
        {
            iss >> command;
            if (command == signup)
            {

                controller.handleSignup(input);
            }

            else if (command == login_command)
            {
                controller.handleLogin(input);
                
            }

            else if (command == logout)
            {
                controller.handleLogout(input);
            }

            else if (command == event)
            {
                controller.handleAddNormalEvent(input);
            }

            else if (command == periodic_event)
            {
                controller.handleAddRecurringEvent(input);
            }

            else if (command == task)
            {
                controller.handleAddTask(input);
            }
            else if (command == join_event)
            {
                controller.handleJoinEvent(input);
            }
            else if (command == confirm_join_event)
            {
                controller.handleConfirmJointEvent(input);
            }
            else if (command == reject_join_event)
            {
                controller.handleRejectJointEvent(input);
            }
            else
            {
                cout << "Bad Request" << endl;
            }
        }
        else if (order == delete)
        {
            iss >> command;
            if (command == task)
            {
                controller.handleDeleteTask(input);
            }
        }
        else if (order == put)
        {
            iss >> command;
            if (command == task)
            {
                controller.handleEditTask(input);
            }
            else
            {
                cout << "Bad Request" << endl;
            }
        }
       else if (order == get)
        {
            iss >> command;
            if (command == report)
            {
                controller.handleGetReport(input);
                
            }
            else if (command == join_event_show)
            {
                controller.handleViewJoint(input);
            }
            else
            {
                cout << "Bad Request" << endl;
            }
        }
        else
        {
            cout << "Bad Request" << endl;
        }
    }

    return 0;
}