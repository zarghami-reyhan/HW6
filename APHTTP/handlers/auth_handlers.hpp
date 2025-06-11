#ifndef AUTH_HANDLERS_HPP_INCLUDE
#define AUTH_HANDLERS_HPP_INCLUDE

#include "../server/server.hpp" // Path relative to auth_handlers.hpp
#include "../../src/Controller.hpp" // Path relative to auth_handlers.hpp
#include "../../src/Model.hpp"      // For AuthStatus enum
#include <string>

class UserSignupHandler : public RequestHandler {
private:
    Controller& controller;
public:
    UserSignupHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

class UserLoginHandler : public RequestHandler {
private:
    Controller& controller;
public:
    UserLoginHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

class UserLogoutHandler : public RequestHandler {
private:
    Controller& controller;
public:
    UserLogoutHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

#endif // AUTH_HANDLERS_HPP_INCLUDE
