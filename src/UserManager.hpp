#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "User.hpp"
using namespace std;

class UserManager {
private:
    unordered_map<string, shared_ptr<User>> users;
    shared_ptr<User> loggedInUser;

public:
    UserManager();
    string signup(const string& username, const string& password);
    string login(const string& username, const string& password);
    string logout();

    shared_ptr<User> getLoggedInUser() const;
};

#endif 