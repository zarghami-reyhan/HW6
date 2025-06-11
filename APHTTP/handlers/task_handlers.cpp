#include "task_handlers.hpp"
#include <iostream> // For temporary debugging if needed
#include <vector>

// Constructor
AddTaskHandler::AddTaskHandler(Controller& ctrl) : controller(ctrl) {}

Response* AddTaskHandler::callback(Request* req) {
    // --- Parameter Extraction ---
    std::string title = req->getBodyParam("title");
    std::string description = req->getBodyParam("description"); // Optional
    std::string date_str = req->getBodyParam("date"); // Expected format: YYYY/MM/DD
    std::string time_str = req->getBodyParam("time");   // Expected: integer for hour (0-23)

    // --- Basic Validation for Presence of Core Fields ---
    if (title.empty() || date_str.empty() || time_str.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Missing required fields: title, date, time\"}");
        return res;
    }

    // --- Data Conversion and Detailed Validation ---
    DateTime::Date due_date;
    int due_time;

    try {
        due_time = std::stoi(time_str);
        due_date = DateTime::parseDate(date_str); // Assumes YYYY/MM/DD format

        if (due_time < 0 || due_time > 23) { // Task dueTime is an hour
             throw std::invalid_argument("Time must be an hour between 0 and 23.");
        }
    } catch (const std::invalid_argument& e) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        std::ostringstream error_msg;
        error_msg << "{\"status\": \"error\", \"message\": \"Invalid number format for time or date format: " << e.what() << "\"}";
        res->setBody(error_msg.str());
        return res;
    }
    // DateTime::parseDate might throw its own specific exceptions or std::invalid_argument.

    // --- Call Controller ---
    int task_id = -1;
    TaskStatus status = controller.handleAddTask(title, description, due_date, due_time, task_id);

    // --- Construct Response ---
    Response* res;
    std::ostringstream json_response_body;
    json_response_body << "{";

    switch (status) {
        case TaskStatus::SUCCESS:
            res = new Response(Response::Status::created);
            json_response_body << "\"status\": \"success\", \"message\": \"Task added successfully\", \"task_id\": " << task_id;
            break;
        case TaskStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::unauthorized);
            json_response_body << "\"status\": \"error\", \"message\": \"Permission denied. User not logged in.\"";
            break;
        case TaskStatus::BAD_REQUEST: // From controller's own validation or here
            res = new Response(Response::Status::badRequest);
            json_response_body << "\"status\": \"error\", \"message\": \"Invalid task data provided.\"}"; // Corrected: removed extra brace
            break;
        default: // Should not happen with current TaskStatus definition
            res = new Response(Response::Status::internalServerError);
            json_response_body << "\"status\": \"error\", \"message\": \"An unexpected error occurred.\"}"; // Corrected: removed extra brace
            break;
    }
    json_response_body << "}";
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response_body.str());
    return res;
}
