#ifndef REPORT_HANDLERS_HPP_INCLUDE
#define REPORT_HANDLERS_HPP_INCLUDE

#include "../server/server.hpp"
#include "../../src/Controller.hpp" // For Controller and EventStatus
#include "../../src/Activity.hpp"   // For ActivityEntry, ActivityType
#include "../../src/DateTime.hpp"   // For DateTime::Date and DateTime::parseDate
#include <string>
#include <vector>
#include <sstream>   // For std::ostringstream
#include <iomanip>   // For std::setfill, std::setw

class GetReportHandler : public RequestHandler {
private:
    Controller& controller;
public:
    GetReportHandler(Controller& ctrl);
    Response* callback(Request* req) override; // Handles GET requests
};

#endif // REPORT_HANDLERS_HPP_INCLUDE
