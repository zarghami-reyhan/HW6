#include "User.hpp"
#include <algorithm>

User::User(const std::string& username, const std::string& password)
    : username(username), password(password),
      normalEventIdCounter(1), recurringEventIdCounter(1), taskIdCounter(1) {}

std::string User::getUsername() const {
    return username;
}

bool User::checkPassword(const std::string& password) const {
    return password == this->password;
}

int User::getNextNormalEventId() {
    return normalEventIdCounter++;
}

void User::addNormalEvent(std::shared_ptr<NormalEvent> event) {
    normalEvents.push_back(event);
}

const std::vector<std::shared_ptr<NormalEvent>>& User::getNormalEvents() const {
    return normalEvents;
}

int User::getNextRecurringEventId() {
    return recurringEventIdCounter++;
}

const std::vector<std::shared_ptr<PeriodicEvent>>& User::getPeriodicEvents() const {
    return periodicEvents;
}

int User::getNextTaskId() {
    return taskIdCounter++;
}

void User::addTask(std::shared_ptr<Task> task) {
    tasks.push_back(task);
}

const std::vector<std::shared_ptr<Task>>& User::getTasks() const {
    return tasks;
}
bool User::addPeriodicEvent(const shared_ptr<PeriodicEvent> event){
        periodicEvents.push_back(event);
        return true;
}


bool User::deleteTask(int task_id) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if ((*it)->getId() == task_id) {
            tasks.erase(it);
            return true;
        }
    }
    return false; 
}

bool User::editTask(int task_id, shared_ptr<Task> new_task) {
    for (auto& task : tasks) {
        if (task->getId() == task_id) {
            task = new_task;
            return true;
        }
    }
    return false; 
}
bool User::addJointEvent(shared_ptr<JointEvent> event) {

    jointEvents.push_back(event);
    for (const auto& guest : event->getGuests()) {

    }
    return true;
}

void User::addInvitation(int invitationId) {
    invitations.push_back(invitationId);
}

void User::confirmJointEvent(int invitationId) {
    auto it = std::find(invitations.begin(), invitations.end(), invitationId);
    if (it != invitations.end()) {
        invitations.erase(it);

    }
}

void User::rejectJointEvent(int invitationId) {
    auto it = find(invitations.begin(), invitations.end(), invitationId);
    if (it != invitations.end()) {
        invitations.erase(it);
    }
}

vector<shared_ptr<JointEvent>>& User::getJointEvents() {
    return jointEvents;
}

const vector<int>& User::getInvitations() const {
    return invitations;
}