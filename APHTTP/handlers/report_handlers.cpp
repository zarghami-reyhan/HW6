#include "report_handlers.hpp"
#include <algorithm> // For std::replace for basic JSON string escaping

// Constructor
GetReportHandler::GetReportHandler(Controller& ctrl) : controller(ctrl) {}

// Helper to convert ActivityType to string for JSON
std::string activityTypeToString(ActivityType type) {
    switch (type) {
        case ActivityType::NormalEvent: return "Normal Event";
        case ActivityType::PeriodicEvent: return "Periodic Event";
        case ActivityType::JointEvent: return "Joint Event";
        case ActivityType::Task: return "Task";
        default: return "Unknown";
    }
}

Response* GetReportHandler::callback(Request* req) {
    // --- Parameter Extraction from Query ---
    std::string from_date_str = req->getQueryParam("from_date");
    std::string to_date_str = req->getQueryParam("to_date");
    std::string filter_type_str = req->getQueryParam("filter_type"); // Optional

    // --- Validation for Presence of Required Date Fields ---
    if (from_date_str.empty() || to_date_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required query parameters: from_date, to_date\"}");
        return res;
    }

    // --- Data Conversion and Detailed Validation for Dates ---
    DateTime::Date from_date, to_date;
    try {
        from_date = DateTime::parseDate(from_date_str);
        to_date = DateTime::parseDate(to_date_str);

        // Basic check: to_date should not be before from_date
        if (to_date.year < from_date.year ||
            (to_date.year == from_date.year && to_date.month < from_date.month) ||
            (to_date.year == from_date.year && to_date.month == from_date.month && to_date.day < from_date.day)) {
            throw std::invalid_argument("to_date cannot be before from_date.");
        }

    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        std::ostringstream error_msg;
        error_msg << "{\"status\": \"error\", \"message\": \"Invalid date format or range: " << e.what() << "\"}";
        res->setBody(error_msg.str());
        return res;
    }

    // Validate filter_type if provided
    std::string effective_filter_type = filter_type_str; // Use original if no alias needed or keep empty
    if (!filter_type_str.empty()) {
        if (filter_type_str == "event") {
             effective_filter_type = "normal_event"; // Alias for controller
        } else if (filter_type_str != "normal_event" &&
                   filter_type_str != "periodic_event" &&
                   filter_type_str != "task" &&
                   filter_type_str != "joint_event") {
            Response* res = new Response(Response::Status::badRequest);
            res->setHeader("Content-Type", "application/json");
            res->setBody("{\"status\": \"error\", \"message\": \"Invalid filter_type. Allowed values: event (for normal_event), normal_event, periodic_event, task, joint_event.\"}");
            return res;
        }
    }


    // --- Call Controller ---
    EventStatus status_code; // Using EventStatus for PERMISSION_DENIED or SUCCESS
    std::vector<ActivityEntry> report_data = controller.handleGetReport(from_date, to_date, effective_filter_type, status_code);

    // --- Construct Response ---
    Response* res;
    std::ostringstream json_body_stream;

    if (status_code == EventStatus::PERMISSION_DENIED) {
        res = new Response(Response::Status::unauthorized);
        json_body_stream << "{\"status\": \"error\", \"message\": \"Permission denied. User not logged in.\"}";
    } else if (status_code == EventStatus::SUCCESS) {
        res = new Response(Response::Status::ok);
        json_body_stream << "[";
        for (size_t i = 0; i < report_data.size(); ++i) {
            const auto& entry = report_data[i];
            DateTime::Date entry_date = entry.date;

            std::string title = entry.title;
            std::replace(title.begin(), title.end(), '\"', '\'');
            std::string description = entry.description;
            std::replace(description.begin(), description.end(), '\"', '\'');
            std::string details = entry.event_specific_details;
            std::replace(details.begin(), details.end(), '\"', '\'');

            json_body_stream << "{";
            json_body_stream << "\"id\": " << entry.id << ",";
            json_body_stream << "\"type\": \"" << activityTypeToString(entry.type) << "\",";
            json_body_stream << "\"title\": \"" << title << "\",";
            json_body_stream << "\"description\": \"" << description << "\",";
            json_body_stream << "\"date\": \"" << entry_date.year << "/"
                             << std::setfill('0') << std::setw(2) << entry_date.month << "/"
                             << std::setfill('0') << std::setw(2) << entry_date.day << "\",";
            json_body_stream << "\"hour\": " << entry.hour << ",";
            json_body_stream << "\"details\": \"" << details << "\"";
            json_body_stream << "}";
            if (i < report_data.size() - 1) {
                json_body_stream << ",";
            }
        }
        json_body_stream << "]";
    } else {
        res = new Response(Response::Status::internalServerError);
        json_body_stream << "{\"status\": \"error\", \"message\": \"An unexpected error occurred retrieving report data.\"}";
    }

    res->setHeader("Content-Type", "application/json");
    res->setBody(json_body_stream.str());
    return res;
}
