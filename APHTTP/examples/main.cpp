#include <iostream>
#include <string> // For std::string

// HW6 Backend Includes
#include "Model.hpp"
#include "Controller.hpp"

#include "../server/server.hpp"
#include "handlers.hpp"
#include "../handlers/auth_handlers.hpp" // Added for auth handlers

void mapServerPaths(Server& server, Controller& controller) { // Added controller argument
    server.setNotFoundErrPage("static/404.html");
    server.get("/", new ShowPage("static/home.html"));
    server.get("/home.png", new ShowImage("static/home.png"));
    server.get("/rand", new RandomNumberHandler());
    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler());
    server.get("/up", new ShowPage("static/upload_form.html"));
    server.post("/up", new UploadHandler());
    server.get("/colors", new ColorHandler("template/colors.html"));
    server.get("/music", new ShowPage("static/music.html"));
    server.get("/music/moonlight.mp3", new ShowFile("static/moonlight.mp3", "audio/mpeg"));

    // HW6 API Auth Routes
    server.post("/api/signup", new UserSignupHandler(controller));
    server.post("/api/login", new UserLoginHandler(controller));
    server.post("/api/logout", new UserLogoutHandler(controller));
}

int main(int argc, char** argv) {
    try {
        // Instantiate Model and Controller for HW6 Backend
        Model model;
        Controller controller(model);

        // Load holiday data
        std::string holiday_csv_path = "src/Holiday.csv"; // Path relative to where executable runs (e.g., root)
        if (!model.getHolidayManager().loadHolidaysFromCSV(holiday_csv_path)) {
            std::cerr << "Error: Failed to load holiday data from " << holiday_csv_path << std::endl;
            // For now, just print an error and continue.
        } else {
            std::cout << "Successfully loaded holiday data from " << holiday_csv_path << std::endl;
        }

        int port = argc > 1 ? std::stoi(argv[1]) : 5000;
        Server server(port);
        mapServerPaths(server, controller); // Updated call
        std::cout << "Server running on port: " << port << std::endl;
        server.run();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const Server::Exception& e) {
        std::cerr << e.getMessage() << std::endl;
    }
    return 0;
}
