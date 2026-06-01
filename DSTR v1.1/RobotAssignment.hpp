#ifndef ROBOT_ASSIGNMENT_HPP
#define ROBOT_ASSIGNMENT_HPP

#include <string>
#include "OrderManagement.hpp" // Required to access the Order data structure

using namespace std;

// Maximum size constraint for the custom Robot array queue
#define MAX_ROBOTS 5

struct Robot {
    string robotID;
    string status;       // Track states: "Available", "Busy", "Maintenance"
    int tasksCompleted;  // tracks individual work count to show fair distribution
};

// Hand-built Circular Queue matching Task requirements
struct RobotQueue {
    Robot arr[MAX_ROBOTS];
    int front;
    int rear;
    int size;
};

// Core Function Prototypes
void initRobotQueue(RobotQueue& rq);
void populateDefaultFleet(RobotQueue& rq);
bool assignRobotToOrder(RobotQueue& rq, int orderID, string& assignedRobotID);
void releaseRobotFromTask(RobotQueue& rq, string robotID);
void displayRobotFleetStatus(RobotQueue& rq);
void robotAssignmentMenu(RobotQueue& rq, Order& activeOrder);

#endif
