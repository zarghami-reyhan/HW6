#include "UserManager.hpp"

UserManager::UserManager() : loggedInUser(nullptr) {}

string UserManager::signup(const string &username, const string &password)
{
    if (loggedInUser != nullptr)
    {
        return "Permission Denied";
    }
    if (users.find(username) != users.end())
    {
        return "Bad Request";
    }
   
    users[username] = make_shared<User>(username, password);
    return "OK";
}

string UserManager::login(const string &username, const string &password)
{
    if (loggedInUser != nullptr)
    {
        return "Permission Denied";
    }
    auto it = users.find(username);
    if (it == users.end())
    {
        return "Not Found";
    }
    if (!it->second->checkPassword(password))
    {
        return "Permission Denied";
    }
 

    loggedInUser = it->second;
    return "OK";
}

string UserManager::logout()
{
    if (loggedInUser == nullptr)
    {
        return "Permission Denied"; 
    }
   
    loggedInUser = nullptr;
    return "OK";
}

shared_ptr<User> UserManager::getLoggedInUser() const
{
    return loggedInUser;
}