#ifndef ACTIVITY_HPP
#define ACTIVITY_HPP

#include "DateTime.hpp" // For DateTime::Date
#include <string>
#include <vector> // Included for potential future use, not strictly for these structs

// Matches enum in Controller.cpp, moved here for broader access
enum class ActivityType {
    PeriodicEvent = 1, // Value matches original for sorting consistency if needed
    NormalEvent = 2,
    Task = 3,
    JointEvent = 4 // Assuming JointEvent was intended to be part of reports
};

// Matches struct in Controller.cpp, moved here
struct ActivityEntry {
    DateTime::Date date; // For events, this is event start date. For tasks, due date.
    int hour;            // For events, start time. For tasks, due time.
    ActivityType type;
    int id;
    std::string description; // This was previously the output of event->toString() or task->toString()
    std::string title;       // Add title for better JSON structure
    std::string event_specific_details; // e.g., duration for events, recurrence for periodic

    // Define a less-than operator for sorting
    bool operator<(const ActivityEntry &other) const {
        if (date.year != other.date.year) return date.year < other.date.year;
        if (date.month != other.date.month) return date.month < other.date.month;
        if (date.day != other.date.day) return date.day < other.date.day;
        if (hour != other.hour) return hour < other.hour;
        // Match original enum values for type sorting if that was intended
        // Otherwise, could sort by a more logical display order or type name
        if (static_cast<int>(type) != static_cast<int>(other.type)) {
            return static_cast<int>(type) < static_cast<int>(other.type);
        }
        return id < other.id;
    }
};

#endif // ACTIVITY_HPP
