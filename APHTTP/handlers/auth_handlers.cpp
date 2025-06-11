#include "auth_handlers.hpp"
#include <string>
#include <iostream> // For temporary debugging if needed

// Constructor implementations
UserSignupHandler::UserSignupHandler(Controller& ctrl) : controller(ctrl) {}
UserLoginHandler::UserLoginHandler(Controller& ctrl) : controller(ctrl) {}
UserLogoutHandler::UserLogoutHandler(Controller& ctrl) : controller(ctrl) {}

// UserSignupHandler
Response* UserSignupHandler::callback(Request* req) {
    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");

    // Basic validation (can be expanded)
    if (username.empty() || password.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Username and password cannot be empty\"}");
        return res;
    }

    AuthStatus status = controller.handleSignup(username, password);
    Response* res;
    std::string json_response;

    switch (status) {
        case AuthStatus::SUCCESS:
            res = new Response(Response::Status::created);
            json_response = "{\"status\": \"success\", \"message\": \"User created successfully\"}";
            break;
        case AuthStatus::USER_EXISTS:
            res = new Response(Response::Status::conflict);
            json_response = "{\"status\": \"error\", \"message\": \"User already exists\"}";
            break;
        case AuthStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::forbidden);
            json_response = "{\"status\": \"error\", \"message\": \"Permission denied (perhaps already logged in)\"}";
            break;
        case AuthStatus::BAD_REQUEST: // Or other general errors from model's signup
            res = new Response(Response::Status::badRequest);
            json_response = "{\"status\": \"error\", \"message\": \"Bad request during signup\"}";
            break;
        default:
            res = new Response(Response::Status::internalServerError);
            json_response = "{\"status\": \"error\", \"message\": \"An unexpected error occurred during signup\"}";
            break;
    }
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response);
    return res;
}

// UserLoginHandler
Response* UserLoginHandler::callback(Request* req) {
    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");

    if (username.empty() || password.empty()) {
        Response* res = new Response(Response::Status::badRequest);
        res->setHeader("Content-Type", "application/json");
        res->setBody("{\"status\": \"error\", \"message\": \"Username and password cannot be empty\"}");
        return res;
    }

    AuthStatus status = controller.handleLogin(username, password);
    Response* res;
    std::string json_response;

    switch (status) {
        case AuthStatus::SUCCESS:
            res = new Response(Response::Status::ok);
            json_response = "{\"status\": \"success\", \"message\": \"Login successful\"}";
            // Potentially set a session ID here in a real scenario
            // res->setSessionId("some_session_token");
            break;
        case AuthStatus::NOT_FOUND:
            res = new Response(Response::Status::notFound);
            json_response = "{\"status\": \"error\", \"message\": \"User not found\"}";
            break;
        case AuthStatus::INVALID_CREDENTIALS:
            res = new Response(Response::Status::unauthorized);
            json_response = "{\"status\": \"error\", \"message\": \"Invalid credentials\"}";
            break;
        case AuthStatus::PERMISSION_DENIED:
            res = new Response(Response::Status::forbidden);
            json_response = "{\"status\": \"error\", \"message\": \"Permission denied or already logged in\"}";
            break;
        case AuthStatus::BAD_REQUEST:
             res = new Response(Response::Status::badRequest);
             json_response = "{\"status\": \"error\", \"message\": \"Bad request during login\"}";
             break;
        default:
            res = new Response(Response::Status::internalServerError);
            json_response = "{\"status\": \"error\", \"message\": \"An unexpected error occurred during login\"}";
            break;
    }
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response);
    return res;
}

// UserLogoutHandler
Response* UserLogoutHandler::callback(Request* req) {
    AuthStatus status = controller.handleLogout();
    Response* res;
    std::string json_response;

    switch (status) {
        case AuthStatus::SUCCESS:
            res = new Response(Response::Status::ok);
            json_response = "{\"status\": \"success\", \"message\": \"Logout successful\"}";
            // Potentially clear session ID here
            break;
        case AuthStatus::PERMISSION_DENIED: // e.g., not logged in
            res = new Response(Response::Status::unauthorized);
            json_response = "{\"status\": \"error\", \"message\": \"Logout failed: No user was logged in\"}";
            break;
        default:
            res = new Response(Response::Status::internalServerError);
            json_response = "{\"status\": \"error\", \"message\": \"An unexpected error occurred during logout\"}";
            break;
    }
    res->setHeader("Content-Type", "application/json");
    res->setBody(json_response);
    return res;
}
