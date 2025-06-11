#ifndef EVENT_HANDLERS_HPP_INCLUDE
#define EVENT_HANDLERS_HPP_INCLUDE

#include "../server/server.hpp"
#include "../../src/Controller.hpp"
#include "../../src/Model.hpp"      // For EventStatus enum
#include "../../src/DateTime.hpp"   // For DateTime::Date and DateTime::parseDate
#include <string>
#include <sstream> // Required for std::ostringstream

class AddNormalEventHandler : public RequestHandler {
private:
    Controller& controller;
public:
    AddNormalEventHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

class ListNormalEventsHandler : public RequestHandler {
private:
    Controller& controller;
public:
    ListNormalEventsHandler(Controller& ctrl);
    Response* callback(Request* req) override; // Handles GET requests
};

class AddPeriodicEventHandler : public RequestHandler {
private:
    Controller& controller;
public:
    AddPeriodicEventHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

class AddJointEventHandler : public RequestHandler {
private:
    Controller& controller;
public:
    AddJointEventHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

class ConfirmJointEventHandler : public RequestHandler {
private:
    Controller& controller;
public:
    ConfirmJointEventHandler(Controller& ctrl);
    Response* callback(Request* req) override; // Handles POST
};

class RejectJointEventHandler : public RequestHandler {
private:
    Controller& controller;
public:
    RejectJointEventHandler(Controller& ctrl);
    Response* callback(Request* req) override; // Handles POST
};

#endif // EVENT_HANDLERS_HPP_INCLUDE
