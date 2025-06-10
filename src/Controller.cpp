#include "Controller.hpp"
#include "UserManager.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

#include <sstream>
#include <algorithm>
Controller::Controller(Model &m)
    : model(m) {}
Model *Controller::getModel()
{
    return &model;
}
void Controller::handleSignup(string input)
{
    string username;
    string password;
    string order;
    istringstream iss(input);

    iss >> order;
    iss >> order;

    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }

    iss >> order;
    if (order != "username")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss.ignore();
    iss.ignore(1, '\"');
    getline(iss, username, '\"');
    iss >> order;
    if (order != "password")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss.ignore();
    iss.ignore(1, '\"');
    getline(iss, password, '\"');
    model.signup(username, password);
}
void Controller::handleLogin(string input)
{
    string username;
    string password;
    string order;
    istringstream iss(input);

    iss >> order;
    iss >> order;

    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }

    iss >> order;
    if (order != "username")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss.ignore();
    iss.ignore(1, '\"');
    getline(iss, username, '\"');
    iss >> order;
    if (order != "password")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss.ignore();
    iss.ignore(1, '\"');
    getline(iss, password, '\"');
    model.login(username, password);
}
void Controller::handleLogout(string input)
{
string order;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    model.logout();
}
void Controller::handleAddNormalEvent(string input)
{
    string title;
    string description = "";
    DateTime::Date date;
    string datestr;
    int start_time;
    int duration;
    string order;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    while (iss >> order)
    {
        if (order == "date")
        {
            iss >> datestr;
            date = DateTime::parseDate(datestr);
        }
        else if (order == "start_time")
        {
            iss >> start_time;
            if (start_time <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "duration")
        {
            iss >> duration;
            if (duration <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "title")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            getline(iss, title, '\"');
        }
        else if (order == "description")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            getline(iss, description, '\"');
        }
        else
        {
            cout << "Bad Request" << endl;
            return;
        }
    }

    if (title.empty() || start_time <= 0 || duration <= 0)
    {
        cout << "Bad Request" << endl;
        return;
    }

    shared_ptr<NormalEvent> event = make_shared<NormalEvent>(title, description, date, start_time, duration);
    if (!model.addNormalEvent(event))
    {
        event->decreamentid();
        return;
    }
}
void Controller::handleAddRecurringEvent(string input)
{

    string title;
    string description = "";
    DateTime::Date start_date;
    DateTime::Date end_date;
    string start_datestr;
    string end_datestr;
    int start_time;
    int duration;
    string order;
    string type;
    string day = "";
    int Day = 0;
    vector<int> vector_week_days;
    string week_day = "";
    string Sunday, Wednesday, Friday, Monday, Saturday, Thursday, Tuesday;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    while (iss >> order)
    {
        if (order == "start_date")
        {
            iss >> start_datestr;
            start_date = DateTime::parseDate(start_datestr);
        }
        else if (order == "end_date")
        {
            iss >> end_datestr;
            end_date = DateTime::parseDate(end_datestr);
        }
        else if (order == "day")
        {
            iss >> day;
        }
        else if (order == "start_time")
        {
            iss >> start_time;
            if (start_time <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "duration")
        {
            iss >> duration;
            if (duration <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }

        else if (order == "type")
        {
            iss >> type;
        }
        else if (order == "week_days")
        {
            iss >> week_day;
            stringstream ss(week_day);
            string token;
            while (getline(ss, token, ','))
            {
                if (token == "Friday")
                    vector_week_days.push_back(0);
                else if (token == "Saturday")
                    vector_week_days.push_back(1);
                else if (token == "Sunday")
                    vector_week_days.push_back(2);
                else if (token == "Monday")
                    vector_week_days.push_back(3);
                else if (token == "Tuesday")
                    vector_week_days.push_back(4);
                else if (token == "Wednesday")
                    vector_week_days.push_back(5);
                else if (token == "Thursday")
                    vector_week_days.push_back(6);
                else
                {
                    cout << "Bad Request" << endl;
                    return;
                }
            }
        }
        else if (order == "title")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            getline(iss, title, '\"');
        }
        else if (order == "description")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            getline(iss, description, '\"');
        }

        else
        {
            cout << "Bad Request" << endl;
            return;
        }
    }

    if (title.empty() || start_time <= 0 || duration <= 0)
    {
        cout << "Bad Request" << endl;
        return;
    }
    if (!day.empty())
        Day = stoi(day);
    RecurrenceType event_type = stringToRecurrenceType(type);
    shared_ptr<PeriodicEvent> event = make_shared<PeriodicEvent>(title, description, start_date, end_date, start_time, duration, event_type, Day, vector_week_days);
    
    if (!model.addPeriodicEvent(event))
    {
        event->decreamentid();
        return;
    }
}
void Controller::handleAddTask(string input)
{
    string title;
    string description = "";
    DateTime::Date date;
    string datestr;
    int time;
    string order;
    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    while (iss >> order)
    {
        if (order == "date")
        {
            iss >> datestr;
            date = DateTime::parseDate(datestr);
        }
        else if (order == "time")
        {
            iss >> time;
            if (time <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }

        else if (order == "title")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            getline(iss, title, '\"');
        }
        else if (order == "description")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            getline(iss, description, '\"');
        }
        else
        {
            cout << "Bad Request" << endl;
            return;
        }
    }

    if (title.empty() || time <= 0)
    {
        cout << "Bad Request" << endl;
        return;
    }
    shared_ptr<Task> task = make_shared<Task>(title, description, date, time);
    if(!model.addTask(task)){
        task->decreamentid();
        return;
    }
}

void Controller::handleDeleteTask(string input)
{
    string order;
    istringstream iss(input);

    iss >> order;
    if (order != "DELETE")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "task")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }

    iss >> order;
    if (order != "task_id")
    {
        cout << "Bad Request" << endl;
        return;
    }

    int task_id;
    if (!(iss >> task_id))
    {
        cout << "Bad Request" << endl;
        return;
    }

    bool result = model.deleteTask(task_id);

    if (result)
        cout << "OK" << endl;
    else
        cout << "Not Found" << endl;
}

void Controller::handleEditTask(string input)
{
    string order;
    istringstream iss(input);

    iss >> order;
    if (order != "PUT")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "task")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }

    iss >> order;
    if (order != "task_id")
    {
        cout << "Bad Request" << endl;
        return;
    }

    int task_id;
    if (!(iss >> task_id))
    {
        cout << "Bad Request" << endl;
        return;
    }

    DateTime::Date date;
    string datestr;
    int time = -1;
    string title;
    string description;

    while (iss >> order)
    {
        if (order == "date")
        {
            if (!(iss >> datestr))
            {
                cout << "Bad Request" << endl;
                return;
            }
            date = DateTime::parseDate(datestr);
        }
        else if (order == "time")
        {
            if (!(iss >> time) || time <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "title")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            if (!getline(iss, title, '\"'))
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "description")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            if (!getline(iss, description, '\"'))
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else
        {
            cout << "Bad Request" << endl;
            return;
        }
    }

    if (title.empty() || time <= 0)
    {
        cout << "Bad Request" << endl;
        return;
    }

    shared_ptr<Task> new_task = make_shared<Task>(title, description, date, time);

    bool result = model.editTask(task_id, new_task);

    if (result)
        cout << "OK" << endl;
    else
    {
        new_task->decreamentid();
        cout << "Not Found" << endl;
    }
}

enum class ActivityType
{
    PeriodicEvent = 1,
    NormalEvent = 2,
    Task = 3,
    JointEvent = 4
};

struct ActivityEntry
{
    DateTime::Date date;
    int hour;
    ActivityType type;
    int id;
    string description;

    bool operator<(const ActivityEntry &other) const
    {
        if (date.year != other.date.year)
            return date.year < other.date.year;
        if (date.month != other.date.month)
            return date.month < other.date.month;
        if (date.day != other.date.day)
            return date.day < other.date.day;
        if (hour != other.hour)
            return hour < other.hour;
        if (type != other.type)
            return type < other.type;
        return id < other.id;
    }
};

void Controller::printReport(shared_ptr<User> user, const DateTime::Date &fromDate, const DateTime::Date &toDate, const string &filterType)
{
    if (!user)
    {
        cout << "User not logged in." << endl;
        return;
    }

    vector<ActivityEntry> activities;

    auto isTypeAllowed = [&](ActivityType t) -> bool
    {
        if (filterType.empty())
            return true;
        if (filterType == "event" && t == ActivityType::NormalEvent)
            return true;
        if (filterType == "periodic_event" && t == ActivityType::PeriodicEvent)
            return true;
        if (filterType == "task" && t == ActivityType::Task)
            return true;
        return false;
    };

    for (const auto &periodicEvent : user->getPeriodicEvents())
    {
        if (!isTypeAllowed(ActivityType::PeriodicEvent))
            continue;

        int startDay = DateTime::daysFromBaseDate(fromDate);
        int endDay = DateTime::daysFromBaseDate(toDate);

        for (int d = startDay; d <= endDay; ++d)
        {
            int y = 1404 + (d / 360);
            int rem = d % 360;
            int m = 1 + (rem / 30);
            int day = 1 + (rem % 30);
            DateTime::Date currentDate(y, m, day);

            if (periodicEvent->occursOnDate(currentDate))
            {
                activities.push_back({currentDate,
                                      periodicEvent->getStartTime(),
                                      ActivityType::PeriodicEvent,
                                      periodicEvent->getId(),
                                      periodicEvent->toString()});
            }
        }
    }

    for (const auto &normalEvent : user->getNormalEvents())
    {
        if (!isTypeAllowed(ActivityType::NormalEvent))
            continue;
        int eventDay = DateTime::daysFromBaseDate(normalEvent->getDate());
        if (eventDay >= DateTime::daysFromBaseDate(fromDate) && eventDay <= DateTime::daysFromBaseDate(toDate))
        {
            activities.push_back(ActivityEntry{
                normalEvent->getDate(),
                normalEvent->getStartTime(),
                ActivityType::NormalEvent,
                normalEvent->getId(),
                normalEvent->toString()});
        }
    }

    for (const auto &task : user->getTasks())
    {
        if (!isTypeAllowed(ActivityType::Task))
            continue;
        int dueDay = DateTime::daysFromBaseDate(task->getDueDate());
        if (dueDay >= DateTime::daysFromBaseDate(fromDate) && dueDay <= DateTime::daysFromBaseDate(toDate))
        {
            activities.push_back({task->getDueDate(),
                                  task->getDueTime(),
                                  ActivityType::Task,
                                  task->getId(),
                                  task->toString()});
        }
    }



    sort(activities.begin(), activities.end());

    for (const auto &activity : activities)
    {
        cout << activity.description << endl;
    }
}

void Controller::handleGetReport(string input)
{
    string order;
    istringstream iss(input);

    iss >> order;
    if (order != "GET")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "report")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }

    DateTime::Date fromDate(1404, 1, 1);
    DateTime::Date toDate;
    bool hasFrom = false;
    bool hasTo = false;
    string filterType = "";

    while (iss >> order)
    {
        if (order == "from")
        {
            string from_str;
            if (!(iss >> from_str))
            {
                cout << "Bad Request" << endl;
                return;
            }
            fromDate = DateTime::parseDate(from_str);
            hasFrom = true;
        }
        else if (order == "to")
        {
            string to_str;
            if (!(iss >> to_str))
            {
                cout << "Bad Request" << endl;
                return;
            }
            toDate = DateTime::parseDate(to_str);
            hasTo = true;
        }
        else if (order == "type")
        {
            if (!(iss >> filterType))
            {
                cout << "Bad Request" << endl;
                return;
            }
            transform(filterType.begin(), filterType.end(), filterType.begin(), ::tolower);
            if (filterType != "task" && filterType != "event" && filterType != "periodic_event" && filterType != "event_join")
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else
        {
            cout << "Bad Request" << endl;
            return;
        }
    }

    if (!hasTo)
    {
        cout << "Bad Request" << endl;
        return;
    }

    shared_ptr<User> currentUser = model.getCurrentUser();
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return;
    }

    vector<ActivityEntry> activities;

    auto isTypeAllowed = [&](ActivityType t) -> bool
    {
        if (filterType.empty())
            return true;
        if (filterType == "event" && t == ActivityType::NormalEvent)
            return true;
        if (filterType == "periodic_event" && t == ActivityType::PeriodicEvent)
            return true;
        if (filterType == "task" && t == ActivityType::Task)
            return true;
        if (filterType == "JointEvent" && t == ActivityType::JointEvent)
            return true;
        return false;
    };

    int startDay = DateTime::daysFromBaseDate(fromDate);
    int endDay = DateTime::daysFromBaseDate(toDate);

    for (const auto &periodicEvent : currentUser->getPeriodicEvents())
    {
        if (!isTypeAllowed(ActivityType::PeriodicEvent))
            continue;

        for (int d = startDay; d <= endDay; ++d)
        {
            int y = 1404 + (d / 360);
            int rem = d % 360;
            int m = 1 + (rem / 30);
            int day = 1 + (rem % 30);
            DateTime::Date currentDate(y, m, day);

            if (periodicEvent->occursOnDate(currentDate))
            {
                string recurrenceStr;
                switch (periodicEvent->getRecurrenceType())
                {
                case RecurrenceType::Daily:
                    recurrenceStr = "Daily";
                    break;
                case RecurrenceType::Weekly:
                    recurrenceStr = "Weekly";
                    break;
                case RecurrenceType::Monthly:
                    recurrenceStr = "Monthly";
                    break;
                default:
                    recurrenceStr = "Unknown";
                    break;
                }

                stringstream ss;
                ss << "periodic_event \"" << periodicEvent->getTitle()
                   << "\" on " << currentDate.year << "/"
                   << (currentDate.month < 10 ? "0" : "") << currentDate.month << "/"
                   << (currentDate.day < 10 ? "0" : "") << currentDate.day
                   << " from " << periodicEvent->getStartTime()
                   << " for " << periodicEvent->getDuration()
                   << " hours " << recurrenceStr << ": \"" << periodicEvent->getDescription() << "\"";

                activities.push_back({currentDate,
                                      periodicEvent->getStartTime(),
                                      ActivityType::PeriodicEvent,
                                      periodicEvent->getId(),
                                      ss.str()});
            }
        }
    }

    for (const auto &normalEvent : currentUser->getNormalEvents())
    {
        if (!isTypeAllowed(ActivityType::NormalEvent))
            continue;
        int eventDay = DateTime::daysFromBaseDate(normalEvent->getDate());
        if (eventDay >= startDay && eventDay <= endDay)
        {
            activities.push_back(ActivityEntry{
                normalEvent->getDate(),
                normalEvent->getStartTime(),
                ActivityType::NormalEvent,
                normalEvent->getId(),
                normalEvent->toString()});
        }
    }

    for (const auto &jointEvent : currentUser->getJointEvents())
    {
        if (!isTypeAllowed(ActivityType::JointEvent))
            continue;
        int eventDay = DateTime::daysFromBaseDate(jointEvent->getDate());
        if (eventDay >= startDay && eventDay <= endDay)
        {
            activities.push_back(ActivityEntry{
                jointEvent->getDate(),
                jointEvent->getStartTime(),
                ActivityType::JointEvent,
                jointEvent->getId(),
                jointEvent->toString()});
        }
    }

    for (const auto &task : currentUser->getTasks())
    {
        if (!isTypeAllowed(ActivityType::Task))
            continue;
        int dueDay = DateTime::daysFromBaseDate(task->getDueDate());
        if (dueDay >= startDay && dueDay <= endDay)
        {
            activities.push_back({task->getDueDate(),
                                  task->getDueTime(),
                                  ActivityType::Task,
                                  task->getId(),
                                  task->toString()});
        }
    }

    sort(activities.begin(), activities.end());

    if (activities.empty())
    {
        cout << "Empty" << endl;
        return;
    }

    for (const auto &activity : activities)
    {
        cout << activity.description << endl;
    }
}
void Controller::handleJoinEvent(string input)
{
    string title, description;
    DateTime::Date date;
    string date_str;
    int start_time;
    int end_time;
    string guests_str;
    vector<string> guests;
    string order;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    while (iss >> order)
    {
        if (order == "guests")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            if (!getline(iss, guests_str, '\"'))
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "date")
        {
            iss >> date_str;
            date = DateTime::parseDate(date_str);
        }
        else if (order == "start_time")
        {

            if (!(iss >> start_time) || start_time <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "end_time")
        {
            if (!(iss >> end_time) || end_time <= 0)
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "title")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            if (!getline(iss, title, '\"'))
            {
                cout << "Bad Request" << endl;
                return;
            }
        }
        else if (order == "description")
        {
            iss.ignore();
            iss.ignore(1, '\"');
            if (!getline(iss, description, '\"'))
            {
                cout << "Bad Request" << endl;
                return;
            }
        }

        else
        {
            cout << "Bad Request" << endl;
            return;
        }
    }

    size_t pos = 0;
    while ((pos = guests_str.find(',')) != string::npos)
    {
        string guest = guests_str.substr(0, pos);
        guests.push_back(guest);
        guests_str.erase(0, pos + 1);
    }
    guests.push_back(guests_str);

    shared_ptr<JointEvent> jointEvent = make_shared<JointEvent>(title, description, date, start_time, end_time, guests, model.getCurrentUser()->getUsername());
    if (!model.addJointEvent(jointEvent))
    {
        jointEvent->decreamentid();
        return;
    }
}

void Controller::handleConfirmJointEvent(string input)
{
    string order;
    int join_event_id;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "invitation_id")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> join_event_id;
    if (!model.getCurrentUser())
    {
        cout << "Permission Denied" << endl;
    }
    model.Taiid(join_event_id,model.getCurrentUser(),0);
  
}

void Controller::handleRejectJointEvent(string input)
{
     string order;
    int join_event_id;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> order;
    if (order != "invitation_id")
    {
        cout << "Bad Request" << endl;
        return;
    }
    iss >> join_event_id;
    model.rad(join_event_id);
}

void Controller::handleViewJoint(string input)
{
    string order;

    istringstream iss(input);
    iss >> order;
    iss >> order;
    iss >> order;
    if (order != "?")
    {
        cout << "Bad Request" << endl;
        return;
    }
    model.print_joint_event();
}
