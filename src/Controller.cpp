#include "Controller.hpp"
#include "UserManager.hpp" // This might not be needed anymore if only used by old handlers
#include <iostream>     // Keep for non-refactored methods that use cout
#include <sstream>      // Keep for non-refactored methods
#include <algorithm>    // For std::sort and std::transform (used in new handleGetReport)
#include <vector>       // For std::vector
#include <memory>       // For std::shared_ptr
// #include "Activity.hpp" // Already included via Controller.hpp

// Note: Old local enum ActivityType and struct ActivityEntry are now removed. They are in Activity.hpp

Controller::Controller(Model &m)
    : model(m) {}
Model *Controller::getModel()
{
    return &model;
}
AuthStatus Controller::handleSignup(const string& username, const string& password)
{
    return model.signup(username, password);
}
AuthStatus Controller::handleLogin(const string& username, const string& password)
{
    return model.login(username, password);
}
AuthStatus Controller::handleLogout()
{
    return model.logout();
}

EventStatus Controller::handleAddNormalEvent(const string& title, const string& description, const DateTime::Date& date, int start_time, int duration, int& out_event_id)
{
    if (title.empty() || start_time <= 0 || duration <= 0) {
        out_event_id = -1;
        return EventStatus::BAD_REQUEST;
    }

    shared_ptr<NormalEvent> event = make_shared<NormalEvent>(title, description, date, start_time, duration);

    return model.addNormalEvent(event, out_event_id);
}

vector<shared_ptr<NormalEvent>> Controller::handleGetNormalEvents(EventStatus& status_code) {
    shared_ptr<User> currentUser = model.getCurrentUser();
    if (!currentUser) {
        status_code = EventStatus::PERMISSION_DENIED;
        return {};
    }

    status_code = EventStatus::SUCCESS;
    return currentUser->getNormalEvents();
}

EventStatus Controller::handleAddPeriodicEvent(
    const string& title, const string& description,
    DateTime::Date startDate,
    const DateTime::Date& endDate,
    int startTime, int duration,
    RecurrenceType recurrenceType,
    int dayOfMonth,
    const vector<int>& weeklyDays,
    int& out_event_id
) {
    if (title.empty() || startTime <= 0 || duration <= 0) {
        out_event_id = -1;
        return EventStatus::BAD_REQUEST;
    }

    shared_ptr<PeriodicEvent> event = make_shared<PeriodicEvent>(
        title, description, startDate, endDate, startTime, duration,
        recurrenceType, dayOfMonth, weeklyDays
    );

    return model.addPeriodicEvent(event, out_event_id);
}

EventStatus Controller::handleAddJointEvent(
    const string& title, const string& description,
    const DateTime::Date& date,
    int startTime, int endTime,
    const vector<string>& guest_usernames,
    int& out_event_id
) {
    shared_ptr<User> current_user_ptr = model.getCurrentUser();
    if (!current_user_ptr) {
        out_event_id = -1;
        return EventStatus::PERMISSION_DENIED;
    }

    if (title.empty() || startTime <= 0 || endTime <= startTime || guest_usernames.empty()) {
        out_event_id = -1;
        return EventStatus::BAD_REQUEST;
    }

    string transmitter_username = current_user_ptr->getUsername();
    shared_ptr<JointEvent> event = make_shared<JointEvent>(
        title, description, date, startTime, endTime, guest_usernames, transmitter_username
    );

    return model.addJointEvent(event, out_event_id);
}

TaskStatus Controller::handleAddTask(
    const string& title, const string& description,
    const DateTime::Date& dueDate,
    int dueTime,
    int& out_task_id
) {
    if (title.empty() || dueTime < 0 || dueTime > 23) {
        out_task_id = -1;
        return TaskStatus::BAD_REQUEST;
    }

    shared_ptr<Task> task = make_shared<Task>(title, description, dueDate, dueTime);
    return model.addTask(task, out_task_id);
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

// New handleGetReport implementation
vector<ActivityEntry> Controller::handleGetReport(
    const DateTime::Date& fromDate,
    const DateTime::Date& toDate,
    const string& filterTypeStr,
    EventStatus& status_code
) {
    shared_ptr<User> currentUser = model.getCurrentUser();
    if (!currentUser) {
        status_code = EventStatus::PERMISSION_DENIED;
        return {};
    }

    status_code = EventStatus::SUCCESS;
    vector<ActivityEntry> activities;
    string lowerFilterType = filterTypeStr;
    std::transform(lowerFilterType.begin(), lowerFilterType.end(), lowerFilterType.begin(), ::tolower);

    auto isTypeAllowed = [&](ActivityType t) -> bool {
        if (lowerFilterType.empty()) return true;
        // Allow "event" or "normal_event" for NormalEvent type
        if ((lowerFilterType == "event" || lowerFilterType == "normal_event") && t == ActivityType::NormalEvent) return true;
        if (lowerFilterType == "periodic_event" && t == ActivityType::PeriodicEvent) return true;
        if (lowerFilterType == "task" && t == ActivityType::Task) return true;
        if (lowerFilterType == "joint_event" && t == ActivityType::JointEvent) return true;
        return false;
    };

    int fromDateDays = DateTime::daysFromBaseDate(fromDate);
    int toDateDays = DateTime::daysFromBaseDate(toDate);

    // Normal Events
    if (isTypeAllowed(ActivityType::NormalEvent)) {
        for (const auto& event_ptr : currentUser->getNormalEvents()) {
            int eventDay = DateTime::daysFromBaseDate(event_ptr->getDate());
            if (eventDay >= fromDateDays && eventDay <= toDateDays) {
                std::string details = "Duration: " + std::to_string(event_ptr->getDuration()) + " hours";
                activities.push_back({event_ptr->getDate(), event_ptr->getStartTime(), ActivityType::NormalEvent, event_ptr->getId(), event_ptr->getDescription(), event_ptr->getTitle(), details});
            }
        }
    }

    // Periodic Events
    if (isTypeAllowed(ActivityType::PeriodicEvent)) {
        for (const auto& event_ptr : currentUser->getPeriodicEvents()) {
            for (int d = fromDateDays; d <= toDateDays; ++d) {
                int y = 1404 + (d / 360);
                int rem = d % 360;
                int m = 1 + (rem / 30);
                int day_val = 1 + (rem % 30);
                DateTime::Date currentDate(y, m, day_val);
                if (currentDate.day > 0 && currentDate.month > 0 && currentDate.year >0 && event_ptr->occursOnDate(currentDate)) {
                    std::string recurrenceStr;
                    switch (event_ptr->getRecurrenceType()) {
                        case RecurrenceType::Daily: recurrenceStr = "Daily"; break;
                        case RecurrenceType::Weekly: recurrenceStr = "Weekly"; break;
                        case RecurrenceType::Monthly: recurrenceStr = "Monthly"; break;
                        default: recurrenceStr = "Unknown"; break;
                    }
                    std::string details = "Duration: " + std::to_string(event_ptr->getDuration()) + " hours, Type: " + recurrenceStr;
                    activities.push_back({currentDate, event_ptr->getStartTime(), ActivityType::PeriodicEvent, event_ptr->getId(), event_ptr->getDescription(), event_ptr->getTitle(), details});
                }
            }
        }
    }

    // Tasks
    if (isTypeAllowed(ActivityType::Task)) {
        for (const auto& task_ptr : currentUser->getTasks()) {
            int taskDay = DateTime::daysFromBaseDate(task_ptr->getDueDate());
            if (taskDay >= fromDateDays && taskDay <= toDateDays) {
                 activities.push_back({task_ptr->getDueDate(), task_ptr->getDueTime(), ActivityType::Task, task_ptr->getId(), task_ptr->getDescription(), task_ptr->getTitle(), ""});
            }
        }
    }

    // Joint Events
    if (isTypeAllowed(ActivityType::JointEvent)) {
        for (const auto& event_ptr : currentUser->getJointEvents()) {
            if (event_ptr->isConfirmed()){
                int eventDay = DateTime::daysFromBaseDate(event_ptr->getDate());
                if (eventDay >= fromDateDays && eventDay <= toDateDays) {
                    std::string details = "Duration: " + std::to_string(event_ptr->getDuration()) + " hours. Guests: ";
                    const auto& guests = event_ptr->getGuests();
                    for(size_t i=0; i < guests.size(); ++i){
                        details += guests[i] + (i < guests.size()-1 ? ", " : "");
                    }
                    activities.push_back({event_ptr->getDate(), event_ptr->getStartTime(), ActivityType::JointEvent, event_ptr->getId(), event_ptr->getDescription(), event_ptr->getTitle(), details});
                }
            }
        }
    }

    std::sort(activities.begin(), activities.end());
    return activities;
}

JointEventActionStatus Controller::handleConfirmJointEvent(int event_id) {
    return model.confirmJointEventInvitation(event_id);
}

JointEventActionStatus Controller::handleRejectJointEvent(int event_id) {
    return model.rejectJointEventInvitation(event_id);
}

void Controller::handleViewJoint(string input) // Keep old signature for non-refactored methods
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
