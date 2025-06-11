#include "Model.hpp"
#include <iostream>
#include <algorithm>

Model::Model() : currentUser(nullptr) {}

AuthStatus Model::signup(const string &username, const string &password)
{
    if (currentUser != nullptr)
    {
        return AuthStatus::PERMISSION_DENIED;
    }
    if (users.find(username) != users.end())
    {
        return AuthStatus::USER_EXISTS;
    }
    users[username] = make_shared<User>(username, password);
    // The user is created but not automatically logged in.
    return AuthStatus::SUCCESS;
}

AuthStatus Model::login(const string &username, const string &password)
{
    if (currentUser != nullptr)
    {
        // This implies a user is already logged in, cannot login again.
        return AuthStatus::PERMISSION_DENIED;
    }
    auto it = users.find(username);
    if (it == users.end())
    {
        return AuthStatus::NOT_FOUND;
    }
    if (!it->second->checkPassword(password))
    {
        return AuthStatus::INVALID_CREDENTIALS;
    }
    currentUser = it->second;
    return AuthStatus::SUCCESS;
}

AuthStatus Model::logout()
{
    if (currentUser == nullptr)
    {
        return AuthStatus::PERMISSION_DENIED;
    }
    currentUser = nullptr;
    return AuthStatus::SUCCESS;
}

shared_ptr<User> Model::getCurrentUser() const
{
    return currentUser;
}

HolidayManager &Model::getHolidayManager()
{
    return holidayManager;
}

EventStatus Model::addNormalEvent(const shared_ptr<NormalEvent> &event, int& out_event_id)
{
    if (!currentUser)
    {
        return EventStatus::PERMISSION_DENIED;
    }

    if (hasOverlapWithEvents(*event))
    {
        return EventStatus::OVERLAP;
    }

    if (isHolidayOverlap(*event))
    {
        return EventStatus::HOLIDAY_FOUND;
    }

    currentUser->addNormalEvent(event);
    out_event_id = event->getId();
    return EventStatus::SUCCESS;
}

EventStatus Model::addPeriodicEvent(const shared_ptr<PeriodicEvent> &event, int& out_event_id)
{
    if (!currentUser)
    {
        return EventStatus::PERMISSION_DENIED;
    }

    if (hasOverlapWithEventsPeriodic(*event))
    {
        return EventStatus::OVERLAP;
    }

    vector<DateTime::Date> holidaysOverlapDays;
    int holiday_check = isHolidayOverlapPeriodic(*event, holidaysOverlapDays);
    // Treat any holiday overlap (partial or full) as a reason to reject for API consistency
    if (holiday_check == 1 || holiday_check == 2)
    {
        return EventStatus::HOLIDAY_FOUND;
    }

    currentUser->addPeriodicEvent(event);
    out_event_id = event->getId(); // Assuming PeriodicEvent also has getId() via Event base class
    return EventStatus::SUCCESS;
}

TaskStatus Model::addTask(const shared_ptr<Task> &task, int& out_task_id)
{
    if (!currentUser)
    {
        return TaskStatus::PERMISSION_DENIED;
    }
    currentUser->addTask(task);
    out_task_id = task->getId();
    return TaskStatus::SUCCESS;
}

const vector<shared_ptr<NormalEvent>> &Model::getNormalEvents() const
{
    if (!currentUser)
        throw runtime_error("No user logged in");
    return currentUser->getNormalEvents();
}

const vector<shared_ptr<PeriodicEvent>> &Model::getPeriodicEvents() const
{
    if (!currentUser)
        throw runtime_error("No user logged in");
    return currentUser->getPeriodicEvents();
}

const vector<shared_ptr<Task>> &Model::getTasks() const
{
    if (!currentUser)
        throw runtime_error("No user logged in");
    return currentUser->getTasks();
}

bool Model::hasOverlapWithEvents(Event &event) const
{
    if (!currentUser)
        return false;
    for (const auto &ev : currentUser->getNormalEvents())
    {
        if (ev->isOverlap(event) && !isHolidayOverlap(event))
            return true;
    }
    for (const auto &ev : currentUser->getPeriodicEvents())
    {
        if (ev->isOverlap(event)&& !isHolidayOverlap(event))
            return true;
    }
    for (const auto &ev : currentUser->getJointEvents())
    {
        if (ev->isConfirmed())
        {
            if (ev->isOverlap(event)&& !isHolidayOverlap(event))
                return true;
        }
    }
    return false;
}

bool Model::hasOverlapWithEventsPeriodic(const PeriodicEvent &event) const
{
    if (!currentUser)
        return false;
    for (const auto &ev : currentUser->getNormalEvents())
    {
        if (event.isOverlap(*ev))
            return true;
    }
    for (const auto &ev : currentUser->getPeriodicEvents())
    {
        if (event.isOverlap(*ev))
            return true;
    }
    return false;
}

bool Model::isHolidayOverlap(const Event &event) const
{
    return holidayManager.isHoliday(event.getDate());
}

int Model::isHolidayOverlapPeriodic(const PeriodicEvent &event, vector<DateTime::Date> &holidaysOverlapDays) const
{
    holidaysOverlapDays.clear();
    DateTime::Date start = event.getDate();
    DateTime::Date end = event.getEndDate();

    int startDay = DateTime::daysFromBaseDate(start);
    int endDay = DateTime::daysFromBaseDate(end);
    int i = 0;
    for (int d = startDay; d <= endDay; ++d)
    {
        int y = 1404 + (d / 360);
        int rem = d % 360;
        int m = 1 + (rem / 30);
        int day = 1 + (rem % 30);
        DateTime::Date currentDate(y, m, day);
        if (event.occursOnDate(currentDate))
        {
            i++;
            if (holidayManager.isHoliday(currentDate))
            {

                holidaysOverlapDays.push_back(currentDate);
            }
        }
    }
    if (holidaysOverlapDays.empty())
        return 0;
    else if (holidaysOverlapDays.size() < i ){
        cout << "Holiday Found" << endl;
        return 1;
    }
    else
        return 2;
}

bool Model::deleteTask(int task_id)
{
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    bool result = currentUser->deleteTask(task_id);
    if (result)
        cout << "OK" << endl;
    else
        cout << "Not Found" << endl;
    return result;
}

bool Model::editTask(int task_id, shared_ptr<Task> new_task)
{
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    bool result = currentUser->editTask(task_id, new_task);
    if (result)
        cout << "OK" << endl;
    else
        cout << "Not Found" << endl;
    return result;
}

EventStatus Model::addJointEvent(shared_ptr<JointEvent> &event, int& out_event_id)
{
    if (!currentUser)
    {
        return EventStatus::PERMISSION_DENIED;
    }
    if (hasOverlapWithEvents(*event))
    {
        return EventStatus::OVERLAP;
    }
    if (isHolidayOverlap(*event))
    {
        return EventStatus::HOLIDAY_FOUND;
    }

    for (const auto &guest_username : event->getGuests())
    {
        if (users.find(guest_username) == users.end())
        {
            return EventStatus::GUEST_NOT_FOUND;
        }
    }

    currentUser->addJointEvent(event);
    for (const auto &guest_username : event->getGuests())
    {
        // users map stores shared_ptr<User>, so users[guest_username] is correct
        users[guest_username]->addJointEvent(event);
    }

    out_event_id = event->getId();
    return EventStatus::SUCCESS;
}

void Model::print_joint_event() // Keep old CLI methods for now
{
    if (!currentUser || currentUser->getJointEvents().empty())
    {
        cout << "Empty" << endl;
        return;
    }

    bool foundPending = false;
    for (const auto &eve : currentUser->getJointEvents())
    {
        if (!eve->isConfirmed()) // Assuming isConfirmed means this user has confirmed or it's fully active
        {
            cout << eve->toString() << endl; // Or a more user-friendly format
            foundPending = true;
        }
    }
    if (!foundPending) {
        cout << "Empty" << endl; // No pending invitations for this user
    }
}

JointEventActionStatus Model::confirmJointEventInvitation(int event_id) {
    if (!currentUser) {
        return JointEventActionStatus::PERMISSION_DENIED;
    }

    shared_ptr<JointEvent> jointEventToConfirm = nullptr;
    // Find the event in the current user's list of joint events
    for (const auto& event_ptr : currentUser->getJointEvents()) {
        if (event_ptr->getId() == event_id) {
            jointEventToConfirm = event_ptr; // All users (host, guests) share this instance
            break;
        }
    }

    if (!jointEventToConfirm) {
        return JointEventActionStatus::EVENT_NOT_FOUND; // Event not associated with this user
    }

    // The isConfirmed flag on JointEvent is a simple bool.
    // If it's already true, it means someone (could be this user or another) confirmed it.
    // For simplicity, if it's already confirmed, we can say ALREADY_CONFIRMED.
    // A more complex system would track per-user confirmation status.
    if (jointEventToConfirm->isConfirmed()) {
        return JointEventActionStatus::ALREADY_CONFIRMED;
    }
    
    // Check for overlap only upon confirmation for this user
    if (hasOverlapWithEvents(*jointEventToConfirm)) {
        return JointEventActionStatus::OVERLAP;
    }

    jointEventToConfirm->confirm(); // This sets the shared 'confirmed' flag to true
    return JointEventActionStatus::SUCCESS;
}

JointEventActionStatus Model::rejectJointEventInvitation(int event_id) {
    if (!currentUser) {
        return JointEventActionStatus::PERMISSION_DENIED;
    }

    bool found = false;
    // Check if event is in user's list before trying to remove
    for (const auto& event_ptr : currentUser->getJointEvents()) {
        if (event_ptr->getId() == event_id) {
            found = true;
            // Optional: Check if it's already confirmed. If a user "rejects" an event they previously confirmed,
            // or that became fully confirmed, what should happen? For now, rejection means removal from their list.
            // if (event_ptr->isConfirmed()) {
            //     return JointEventActionStatus::ALREADY_CONFIRMED; // Or a different status like CANNOT_REJECT_CONFIRMED
            // }
            break;
        }
    }

    if (!found) {
        return JointEventActionStatus::EVENT_NOT_FOUND; // Not in this user's list to reject
    }

    currentUser->removeJointEventById(event_id);
    // Note: This only removes it from the current user's list.
    // The event object itself might still exist if other users (host/guests) hold shared_ptrs to it.
    // If the event should be "cancelled" for all, that's different logic (e.g., host cancels).
    return JointEventActionStatus::SUCCESS;
}

