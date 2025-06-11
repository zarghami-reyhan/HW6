#ifndef TASK_HANDLERS_HPP_INCLUDE
#define TASK_HANDLERS_HPP_INCLUDE

#include "../server/server.hpp"
#include "../../src/Controller.hpp"
#include "../../src/Model.hpp"      // For TaskStatus enum
#include "../../src/DateTime.hpp"   // For DateTime::Date and DateTime::parseDate
#include <string>
#include <sstream> // Required for std::ostringstream

class AddTaskHandler : public RequestHandler {
private:
    Controller& controller;
public:
    AddTaskHandler(Controller& ctrl);
    Response* callback(Request* req) override;
};

#endif // TASK_HANDLERS_HPP_INCLUDE
