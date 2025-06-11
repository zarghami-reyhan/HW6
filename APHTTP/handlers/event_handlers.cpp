#include "event_handlers.hpp"
#include <iostream> // For temporary debugging if needed
#include <vector>   // For parsing title/desc with spaces if needed (not for form params)
#include <iomanip>  // For std::setfill and std::setw
#include <algorithm> // For std::replace

// Constructor
AddNormalEventHandler::AddNormalEventHandler(Controller& ctrl) : controller(ctrl) {}

Response* AddNormalEventHandler::callback(Request* req) {
    // Get parameters from request body
    std::string title = req->getBodyParam("title");
    std::string description = req->getBodyParam("description"); // Optional, can be empty
    std::string date_str = req->getBodyParam("date"); // Expected format: YYYY/MM/DD
    std::string start_time_str = req->getBodyParam("start_time");
    std::string duration_str = req->getBodyParam("duration");

    // Basic validation for presence of required fields
    if (title.empty() || date_str.empty() || start_time_str.empty() || duration_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required fields: title, date, start_time, duration\"}");
        return res;
    }

    int start_time, duration;
    DateTime::Date event_date;

    try {
        start_time = std::stoi(start_time_str);
        duration = std::stoi(duration_str);
        event_date = DateTime::parseDate(date_str); // Assumes YYYY/MM/DD format

        if (start_time <= 0 || duration <= 0) {
             throw std::invalid_argument("Start time and duration must be positive.");
        }
    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        std::ostringstream error_msg;
        error_msg << "{\"status\": \"error\", \"message\": \"Invalid number format for start_time/duration or date format: " << e.what() << "\"}";
        res->setBody(error_msg.str());
        return res;
    }
    // Removed catch for DateTime::Date::InvalidDate as it's not defined in DateTime.hpp
    // std::invalid_argument should catch issues from DateTime::parseDate if it throws that,
    // or if it throws a more specific custom exception that inherits from std::exception,
    // a more generic catch (const std::exception& e) could be used if needed later.

    int event_id = -1;
    EventStatus status = controller.handleAddNormalEvent(title, description, event_date, start_time, duration, event_id);

    Response* res;
    std::ostringstream json_response_body;
    json_response_body << "{";

    switch (status) {
        case EventStatus::SUCCESS:
            res = new Response(Response::Status::created);
            json_response_body << "\"status\": \"success\", \"message\": \"Normal event added successfully\", \"event_id\": " << event_id;
            break;
        case EventStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::unauthorized); // Or forbidden
            json_response_body << "\"status\": \"error\", \"message\": \"Permission denied. User not logged in.\"";
            break;
        case EventStatus::OVERLAP:
            res = new Response(Response::Status::conflict);
            json_response_body << "\"status\": \"error\", \"message\": \"Event overlaps with an existing event.\"";
            break;
        case EventStatus::HOLIDAY_FOUND:
            res = new Response(Response::Status::conflict); // Or badRequest depending on desired strictness
            json_response_body << "\"status\": \"error\", \"message\": \"Event falls on a holiday.\"";
            break;
        case EventStatus::BAD_REQUEST: // From controller's own validation
            res = new Response(Response::Status::badRequest);
            json_response_body << "\"status\": \"error\", \"message\": \"Invalid event data provided (e.g., empty title, non-positive time/duration).\"";
            break;
        default:
            res = new Response(Response::Status::internalServerError);
            json_response_body << "\"status\": \"error\", \"message\": \"An unexpected error occurred.\"";
            break;
    }
    json_response_body << "}";
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response_body.str());
    return res;
}

// ConfirmJointEventHandler
ConfirmJointEventHandler::ConfirmJointEventHandler(Controller& ctrl) : controller(ctrl) {}

Response* ConfirmJointEventHandler::callback(Request* req) {
    std::string event_id_str = req->getBodyParam("event_id");
    if (event_id_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required body parameter: event_id\"}");
        return res;
    }

    int event_id;
    try {
        event_id = std::stoi(event_id_str);
    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Invalid event_id format.\"}");
        return res;
    }

    JointEventActionStatus status = controller.handleConfirmJointEvent(event_id);
    Response* res;
    std::ostringstream json_body;
    json_body << "{";

    switch (status) {
        case JointEventActionStatus::SUCCESS:
            res = new Response(Response::Status::ok);
            json_body << "\"status\": \"success\", \"message\": \"Joint event confirmed.\"}";
            break;
        case JointEventActionStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::unauthorized);
            json_body << "\"status\": \"error\", \"message\": \"Permission denied.\"}";
            break;
        case JointEventActionStatus::EVENT_NOT_FOUND:
            res = new Response(Response::Status::notFound);
            json_body << "\"status\": \"error\", \"message\": \"Event not found or not associated with user.\"}";
            break;
        case JointEventActionStatus::ALREADY_CONFIRMED:
            res = new Response(Response::Status::conflict);
            json_body << "\"status\": \"error\", \"message\": \"Event already confirmed.\"}";
            break;
        case JointEventActionStatus::OVERLAP:
            res = new Response(Response::Status::conflict);
            json_body << "\"status\": \"error\", \"message\": \"Confirming event causes an overlap.\"}";
            break;
        default: // EVENT_IS_NOT_A_JOINT_EVENT, ACTION_FAILED
            res = new Response(Response::Status::internalServerError);
            json_body << "\"status\": \"error\", \"message\": \"Action failed or invalid event type.\"}";
            break;
    }
    // The single closing brace "}" is added by the ostringstream after the switch cases.
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_body.str());
    return res;
}

// RejectJointEventHandler
RejectJointEventHandler::RejectJointEventHandler(Controller& ctrl) : controller(ctrl) {}

Response* RejectJointEventHandler::callback(Request* req) {
    std::string event_id_str = req->getBodyParam("event_id");
    if (event_id_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required body parameter: event_id\"}");
        return res;
    }

    int event_id;
    try {
        event_id = std::stoi(event_id_str);
    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Invalid event_id format.\"}");
        return res;
    }

    JointEventActionStatus status = controller.handleRejectJointEvent(event_id);
    Response* res;
    std::ostringstream json_body;
    json_body << "{";

    switch (status) {
        case JointEventActionStatus::SUCCESS:
            res = new Response(Response::Status::ok);
            json_body << "\"status\": \"success\", \"message\": \"Joint event rejected/removed from your list.\"}";
            break;
        case JointEventActionStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::unauthorized);
            json_body << "\"status\": \"error\", \"message\": \"Permission denied.\"}";
            break;
        case JointEventActionStatus::EVENT_NOT_FOUND:
            res = new Response(Response::Status::notFound);
            json_body << "\"status\": \"error\", \"message\": \"Event not found or not associated with user.\"}";
            break;
        case JointEventActionStatus::ALREADY_CONFIRMED:
            res = new Response(Response::Status::conflict);
            json_body << "\"status\": \"error\", \"message\": \"Cannot reject: Event already confirmed (or state prevents rejection).\"}";
            break;
        default: // EVENT_IS_NOT_A_JOINT_EVENT, ACTION_FAILED
            res = new Response(Response::Status::internalServerError);
            json_body << "\"status\": \"error\", \"message\": \"Action failed or invalid event type.\"}";
            break;
    }
    // The single closing brace "}" is added by the ostringstream after the switch cases.
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_body.str());
    return res;
}

// Constructor for AddJointEventHandler
AddJointEventHandler::AddJointEventHandler(Controller& ctrl) : controller(ctrl) {}

Response* AddJointEventHandler::callback(Request* req) {
    // --- Parameter Extraction ---
    std::string title = req->getBodyParam("title");
    std::string description = req->getBodyParam("description"); // Optional
    std::string date_str = req->getBodyParam("date"); // Expected format: YYYY/MM/DD
    std::string start_time_str = req->getBodyParam("start_time");
    std::string end_time_str = req->getBodyParam("end_time"); // JointEvent uses end_time
    std::string guests_str = req->getBodyParam("guests");   // Comma-separated usernames

    // --- Basic Validation for Presence of Core Fields ---
    if (title.empty() || date_str.empty() || start_time_str.empty() ||
        end_time_str.empty() || guests_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required fields: title, date, start_time, end_time, guests\"}");
        return res;
    }

    // --- Data Conversion and Detailed Validation ---
    DateTime::Date event_date;
    int start_time, end_time;
    std::vector<std::string> guest_usernames_vec;

    try {
        start_time = std::stoi(start_time_str);
        end_time = std::stoi(end_time_str);
        event_date = DateTime::parseDate(date_str);

        if (start_time < 0 || end_time <= 0) { // start_time can be 0 (e.g. midnight)
            throw std::invalid_argument("Start time must be non-negative, end time must be positive.");
        }
        if (end_time <= start_time) {
            throw std::invalid_argument("End time must be after start time.");
        }

        // Parse guests string
        std::stringstream ss_guests(guests_str);
        std::string guest_item;
        while (std::getline(ss_guests, guest_item, ',')) {
            // Basic trim whitespace from guest_item if necessary, though usernames usually don't have spaces
            // For now, assuming usernames are clean or Model/User handles it.
            if (!guest_item.empty()) { // Avoid empty guest names if string is "user1,,user2"
               guest_usernames_vec.push_back(guest_item);
            }
        }
        if (guest_usernames_vec.empty()) {
            throw std::invalid_argument("Guests list cannot be empty or contain only empty names.");
        }

    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        std::ostringstream error_msg;
        error_msg << "{\"status\": \"error\", \"message\": \"Invalid data format or value: " << e.what() << "\"}";
        res->setBody(error_msg.str());
        return res;
    }

    // --- Call Controller ---
    int event_id = -1;
    EventStatus status = controller.handleAddJointEvent(
        title, description, event_date, start_time, end_time,
        guest_usernames_vec, event_id
    );

    // --- Construct Response ---
    Response* res;
    std::ostringstream json_response_body;
    json_response_body << "{";

    switch (status) {
        case EventStatus::SUCCESS:
            res = new Response(Response::Status::created);
            json_response_body << "\"status\": \"success\", \"message\": \"Joint event added successfully\", \"event_id\": " << event_id;
            break;
        case EventStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::unauthorized);
            json_response_body << "\"status\": \"error\", \"message\": \"Permission denied. User not logged in.\"";
            break;
        case EventStatus::OVERLAP:
            res = new Response(Response::Status::conflict);
            json_response_body << "\"status\": \"error\", \"message\": \"Event overlaps with an existing event.\"";
            break;
        case EventStatus::HOLIDAY_FOUND:
            res = new Response(Response::Status::conflict);
            json_response_body << "\"status\": \"error\", \"message\": \"Event falls on a holiday.\"";
            break;
        case EventStatus::GUEST_NOT_FOUND:
            res = new Response(Response::Status::notFound);
            json_response_body << "\"status\": \"error\", \"message\": \"One or more guest users not found.\"";
            break;
        case EventStatus::BAD_REQUEST:
            res = new Response(Response::Status::badRequest);
            json_response_body << "\"status\": \"error\", \"message\": \"Invalid event data provided (controller validation).\"";
            break;
        default:
            res = new Response(Response::Status::internalServerError);
            json_response_body << "\"status\": \"error\", \"message\": \"An unexpected error occurred.\"";
            break;
    }
    json_response_body << "}";
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response_body.str());
    return res;
}

ListNormalEventsHandler::ListNormalEventsHandler(Controller& ctrl) : controller(ctrl) {}

Response* ListNormalEventsHandler::callback(Request* req) {
    EventStatus status_code;
    vector<shared_ptr<NormalEvent>> events = controller.handleGetNormalEvents(status_code);

    Response* res;
    std::ostringstream json_body_stream;

    if (status_code == EventStatus::PERMISSION_DENIED) {
        res = new Response(Response::Status::unauthorized);
        json_body_stream << "{\"status\": \"error\", \"message\": \"Permission denied. User not logged in.\"}";
    } else if (status_code == EventStatus::SUCCESS) {
        res = new Response(Response::Status::ok);
        json_body_stream << "[";
        for (size_t i = 0; i < events.size(); ++i) {
            const auto& event = events[i];
            DateTime::Date date = event->getDate(); // Get Date object

            json_body_stream << "{";
            json_body_stream << "\"id\": " << event->getId() << ",";

            std::string title = event->getTitle();
            std::replace(title.begin(), title.end(), '\"', '\''); // Replace " with '
            std::string description = event->getDescription();
            std::replace(description.begin(), description.end(), '\"', '\'');

            json_body_stream << "\"title\": \"" << title << "\",";
            json_body_stream << "\"description\": \"" << description << "\",";
            // Format date as YYYY/MM/DD string
            json_body_stream << "\"date\": \"" << date.year << "/"
                             << std::setfill('0') << std::setw(2) << date.month << "/"
                             << std::setfill('0') << std::setw(2) << date.day << "\",";
            json_body_stream << "\"start_time\": " << event->getStartTime() << ",";
            json_body_stream << "\"duration\": " << event->getDuration();
            json_body_stream << "}";
            if (i < events.size() - 1) {
                json_body_stream << ",";
            }
        }
        json_body_stream << "]";
    } else {
        // Should not happen if controller logic is correct (only SUCCESS or PERMISSION_DENIED)
        res = new Response(Response::Status::internalServerError);
        json_body_stream << "{\"status\": \"error\", \"message\": \"An unexpected error occurred.\"}";
    }

    res->setHeader("Content-Type", "application/json");
    res->setBody(json_body_stream.str());
    return res;
}

// Constructor for AddPeriodicEventHandler
AddPeriodicEventHandler::AddPeriodicEventHandler(Controller& ctrl) : controller(ctrl) {}

Response* AddPeriodicEventHandler::callback(Request* req) {
    // --- Parameter Extraction ---
    std::string title = req->getBodyParam("title");
    std::string description = req->getBodyParam("description"); // Optional
    std::string start_date_str = req->getBodyParam("start_date");
    std::string end_date_str = req->getBodyParam("end_date");
    std::string start_time_str = req->getBodyParam("start_time");
    std::string duration_str = req->getBodyParam("duration");
    std::string recurrence_type_str = req->getBodyParam("recurrence_type"); // "Daily", "Weekly", "Monthly"

    std::string day_of_month_str = req->getBodyParam("day_of_month"); // Optional, for Monthly
    std::string weekly_days_str = req->getBodyParam("weekly_days");   // Optional, for Weekly (e.g., "0,1,6")

    // --- Basic Validation for Presence of Core Fields ---
    if (title.empty() || start_date_str.empty() || end_date_str.empty() ||
        start_time_str.empty() || duration_str.empty() || recurrence_type_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required fields: title, start_date, end_date, start_time, duration, recurrence_type\"}");
        return res;
    }

    // --- Data Conversion and Detailed Validation ---
    DateTime::Date start_date, end_date;
    int start_time, duration, day_of_month = 0; // Default day_of_month
    RecurrenceType recurrence_type;
    std::vector<int> weekly_days_vec;

    try {
        start_time = std::stoi(start_time_str);
        duration = std::stoi(duration_str);
        start_date = DateTime::parseDate(start_date_str);
        end_date = DateTime::parseDate(end_date_str);

        if (start_time <= 0 || duration <= 0) {
            throw std::invalid_argument("Start time and duration must be positive.");
        }

        // Recurrence Type
        if (recurrence_type_str == "Daily") recurrence_type = RecurrenceType::Daily;
        else if (recurrence_type_str == "Weekly") recurrence_type = RecurrenceType::Weekly;
        else if (recurrence_type_str == "Monthly") recurrence_type = RecurrenceType::Monthly;
        else throw std::invalid_argument("Invalid recurrence_type. Must be Daily, Weekly, or Monthly.");

        // Day of Month (for Monthly)
        if (recurrence_type == RecurrenceType::Monthly) {
            if (day_of_month_str.empty()) throw std::invalid_argument("day_of_month is required for Monthly recurrence.");
            day_of_month = std::stoi(day_of_month_str);
            if (day_of_month < 1 || day_of_month > 30) { // Assuming 30-day months as per DateTime.hpp logic
                throw std::invalid_argument("day_of_month must be between 1 and 30.");
            }
        }

        // Weekly Days (for Weekly)
        if (recurrence_type == RecurrenceType::Weekly) {
            if (weekly_days_str.empty()) throw std::invalid_argument("weekly_days is required for Weekly recurrence (e.g., '0,1,6').");
            std::stringstream ss(weekly_days_str);
            std::string item;
            while (std::getline(ss, item, ',')) {
                int day_code = std::stoi(item);
                if (day_code < 0 || day_code > 6) throw std::invalid_argument("Invalid day code in weekly_days. Must be 0-6.");
                weekly_days_vec.push_back(day_code);
            }
            if (weekly_days_vec.empty()) throw std::invalid_argument("weekly_days cannot result in an empty list for Weekly recurrence.");
        }

    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        std::ostringstream error_msg;
        error_msg << "{\"status\": \"error\", \"message\": \"Invalid data format or value: " << e.what() << "\"}";
        res->setBody(error_msg.str());
        return res;
    }
    // Note: DateTime::parseDate might throw its own specific exceptions if defined, or std::invalid_argument.
    // The current catch block handles std::invalid_argument.

    // --- Call Controller ---
    int event_id = -1;
    // Pass startDate by value to controller as PeriodicEvent constructor might modify it.
    EventStatus status = controller.handleAddPeriodicEvent(
        title, description, start_date, end_date, start_time, duration,
        recurrence_type, day_of_month, weekly_days_vec, event_id
    );

    // --- Construct Response ---
    Response* res;
    std::ostringstream json_response_body;
    json_response_body << "{";

    switch (status) {
        case EventStatus::SUCCESS:
            res = new Response(Response::Status::created);
            json_response_body << "\"status\": \"success\", \"message\": \"Periodic event added successfully\", \"event_id\": " << event_id;
            break;
        case EventStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::unauthorized);
            json_response_body << "\"status\": \"error\", \"message\": \"Permission denied. User not logged in.\"";
            break;
        case EventStatus::OVERLAP:
            res = new Response(Response::Status::conflict);
            json_response_body << "\"status\": \"error\", \"message\": \"Event overlaps with an existing event.\"";
            break;
        case EventStatus::HOLIDAY_FOUND:
            res = new Response(Response::Status::conflict);
            json_response_body << "\"status\": \"error\", \"message\": \"Event falls on a holiday.\"";
            break;
        case EventStatus::BAD_REQUEST:
            res = new Response(Response::Status::badRequest);
            json_response_body << "\"status\": \"error\", \"message\": \"Invalid event data provided (controller validation).\"";
            break;
        default:
            res = new Response(Response::Status::internalServerError);
            json_response_body << "\"status\": \"error\", \"message\": \"An unexpected error occurred.\"";
            break;
    }
    json_response_body << "}";
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response_body.str());
    return res;
}
