#include "Model.hpp"
#include <iostream>
#include <algorithm>

Model::Model() : currentUser(nullptr) {}

bool Model::signup(const string &username, const string &password)
{
    if (currentUser != nullptr)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    if (users.find(username) != users.end())
    {
        cout << "Bad Request" << endl;
        return false;
    }
    users[username] = make_shared<User>(username, password);
    login(username,password);
    return true;
}

bool Model::login(const string &username, const string &password)
{
    if (currentUser != nullptr)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    auto it = users.find(username);
    if (it == users.end())
    {
        cout << "Not Found" << endl;
        return false;
    }
    if (!it->second->checkPassword(password))
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    currentUser = it->second;
    cout << "OK" << endl;
    return true;
}

bool Model::logout()
{

    if (currentUser == nullptr)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    currentUser = nullptr;
    cout << "OK" << endl;
    return true;
}

shared_ptr<User> Model::getCurrentUser() const
{
    return currentUser;
}

HolidayManager &Model::getHolidayManager()
{
    return holidayManager;
}

bool Model::addNormalEvent(const shared_ptr<NormalEvent> &event)
{

    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return false;
    }

    if (hasOverlapWithEvents(*event))
    {
        cout << "Overlap" << endl;
        return false;
    }

    if (isHolidayOverlap(*event))
    {
        cout << "Holiday Found" << endl;
        return false;
    }

    currentUser->addNormalEvent(event);
    cout << "OK" << endl;
    return true;
}

bool Model::addPeriodicEvent(const shared_ptr<PeriodicEvent> &event)
{
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return false;
    }

    if (hasOverlapWithEventsPeriodic(*event))
    {
        cout << "Overlap" << endl;
        return false;
    }

    vector<DateTime::Date> holidaysOverlapDays;
    int check = isHolidayOverlapPeriodic(*event, holidaysOverlapDays);
    if (check == 2)
    {
        cout << "Holiday Found" << endl;
        return false;
    }
    currentUser->addPeriodicEvent(event);
    if(check == 0)
        cout << "OK" << endl;
    return true;
}

bool Model::addTask(const shared_ptr<Task> &task)
{
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    currentUser->addTask(task);
    cout << "OK" << endl;
    return true;
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
bool Model::addJointEvent(shared_ptr<JointEvent> &event) const
{
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
        return false;
    }
    if (hasOverlapWithEvents(*event))
    {
        cout << "Overlap" << endl;
        return false;
    }
    if (isHolidayOverlap(*event))
    {
        cout << "Holiday Found" << endl;
        return false;
    }
    for (const auto &guset : event->getGuests())
    {
        if (users.find(guset) == users.end())
        {
            cout << "Not Found" << endl;

            return false;
        }
    }
    currentUser->addJointEvent(event);
    for (const auto &guset : event->getGuests())
    {
        auto it = users.find(guset);
        if (it != users.end())
        {
            it->second->addJointEvent(event);
        }
    }
    cout << "OK" << endl;
    return true;
}

void Model::print_joint_event()

{
    if (currentUser->getJointEvents().empty())
    {
        cout << "Empty" << endl;
        return;
    }

    for (const auto &eve : currentUser->getJointEvents())
    {
        if (!eve->isConfirmed())
        {
            cout << eve->toString() << endl;
            return;
        }
    }
    cout << "Empty" << endl;
}
bool Model::Taiid(int id,shared_ptr<User> user,int i)
{
    
    for (auto &event : user->getJointEvents())
    {
        if (event->getId() == id)
        {
            if (hasOverlapWithEvents(*event))
            {
                cout << "Overlap" << endl;
                return false;
            }
            event->confirm();
            if (i ==0){
                Taiid(id,users[event->getTransmiter()],1);
                cout << "OK" << endl;
            }
            
            return true;
        }
    }

    cout << "Not Found" << endl;
    return false;
}
void Model::rad(int id)
{
    if (!currentUser)
    {
        cout << "Permission Denied" << endl;
    }
    


    auto& events = currentUser->getJointEvents();
    
    for (auto it = events.begin(); it != events.end(); ) {
        if ((*it)->getId() == id && !(*it)->isConfirmed()) {
            it = events.erase(it);  
            std::cout << "OK" << std::endl;
            return;
        } else {
            ++it;  
        }
    }



    cout << "Not Found" << endl;
}

