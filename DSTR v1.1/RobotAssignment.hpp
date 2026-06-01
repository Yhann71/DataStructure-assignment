#ifndef ROBOT_ASSIGNMENT_HPP
#define ROBOT_ASSIGNMENT_HPP

#include <string>
#include "OrderManagement.hpp"

using namespace std;

#define MAX_ROBOTS 5

struct Robot {
    string robotID;
    string status;       // "Available", "Busy", "Maintenance"
    int tasksCompleted;  // Tracks individual work distribution
};

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
