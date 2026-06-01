#include <iostream>
#include <iomanip>
#include "RobotAssignment.hpp"

using namespace std;

// Initialize circular queue variables
void initRobotQueue(RobotQueue& rq) {
    rq.front = -1;
    rq.rear  = -1;
    rq.size  = 0;
}

// Internal worker function to add configured entities into the custom structure
void enqueueRobot(RobotQueue& rq, Robot r) {
    if (rq.size == MAX_ROBOTS) {
        return;
    }
    if (rq.size == 0) rq.front = 0;
    rq.rear = (rq.rear + 1) % MAX_ROBOTS;
    rq.arr[rq.rear] = r;
    rq.size++;
}

// Provision the warehouse fleet database on system initialization
void populateDefaultFleet(RobotQueue& rq) {
    initRobotQueue(rq);
    Robot r1 = {"ROBOT_A", "Available", 0};
    Robot r2 = {"ROBOT_B", "Available", 0};
    Robot r3 = {"ROBOT_C", "Maintenance", 0}; // Initialized to test exception-skipping logic
    Robot r4 = {"ROBOT_D", "Available", 0};
    Robot r5 = {"ROBOT_E", "Available", 0};

    enqueueRobot(rq, r1);
    enqueueRobot(rq, r2);
    enqueueRobot(rq, r3);
    enqueueRobot(rq, r4);
    enqueueRobot(rq, r5);
}

// Core Rotation Algorithm: Cycles fairly to assign an automated available asset
bool assignRobotToOrder(RobotQueue& rq, int orderID, string& assignedRobotID) {
    if (rq.size == 0) {
        cout << "System Error: Robot database empty.\n";
        return false;
    }

    int currentScanIndex = rq.front;

    // Scan through all slots in the circular structure starting at the current sequence front
    for (int i = 0; i < rq.size; i++) {
        if (rq.arr[currentScanIndex].status == "Available") {
            // Update state attributes
            rq.arr[currentScanIndex].status = "Busy";
            rq.arr[currentScanIndex].tasksCompleted++;
            assignedRobotID = rq.arr[currentScanIndex].robotID;

            // Shift the tracking front index to the next adjacent slot to distribute work evenly next time
            rq.front = (currentScanIndex + 1) % MAX_ROBOTS;
            return true;
        }
        // Exception step: Skip if Busy or in Maintenance, advance scan index circularly
        currentScanIndex = (currentScanIndex + 1) % MAX_ROBOTS;
    }

    // All registered fleet assets are currently deployed or offline
    return false;
}

// Reset function invoked when Task 3 complete/return signal fires
void releaseRobotFromTask(RobotQueue& rq, string robotID) {
    int index = rq.front;
    for (int i = 0; i < rq.size; i++) {
        if (rq.arr[index].robotID == robotID) {
            if (rq.arr[index].status == "Busy") {
                rq.arr[index].status = "Available";
                cout << "\n[System Event] " << robotID << " returned safely to station. Status changed to Available.\n";
            }
            return;
        }
        index = (index + 1) % MAX_ROBOTS;
    }
}

// Output display rendering real-time operational dashboard logs
void displayRobotFleetStatus(RobotQueue& rq) {
    cout << "\n-------------------------------------------------\n";
    cout << "             WAREHOUSE FLEET SYSTEM DASHBOARD    \n";
    cout << "-------------------------------------------------\n";
    cout << "  Robot ID   | Status        | Tasks Handled     \n";
    cout << "-------------------------------------------------\n";

    int index = rq.front;
    for (int i = 0; i < rq.size; i++) {
        cout << "  " << setw(10) << left << rq.arr[index].robotID << " | "
             << setw(13) << left << rq.arr[index].status << " | "
             << setw(13) << right << rq.arr[index].tasksCompleted << "\n";
        index = (index + 1) % MAX_ROBOTS;
    }
    cout << "-------------------------------------------------\n";
    cout << "Next Fleet Allocation Tracker Head: " << rq.arr[rq.front].robotID << "\n";
}

// Clean Submenu interface fulfilling individual presentation standards
void robotAssignmentMenu(RobotQueue& rq, Order& activeOrder) {
    int choice = 0;
    do {
        cout << "\n=====================================\n";
        cout << "     ROBOT ASSIGNMENT MODULE MENU    \n";
        cout << "=====================================\n\n";
        cout << "1. Display Fleet Status Overview\n";
        cout << "2. Dispatch Next Available Robot to Active Order\n";
        cout << "3. Toggle Maintenance Mode on Robot\n";
        cout << "4. Return to Central Main Menu\n";
        cout << "Select Option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayRobotFleetStatus(rq);
                break;
                
            case 2: {
                if (activeOrder.orderID == -1) {
                    cout << "\n[Alert] Order instance missing. Extract an incoming order via Task 1 first.\n";
                } else {
                    string designatedRobot = "";
                    cout << "\nProcessing assignment sequence for Order ID " 
                         << setfill('0') << setw(4) << activeOrder.orderID << "...\n";
                         
                    if (assignRobotToOrder(rq, activeOrder.orderID, designatedRobot)) {
                        cout << "\n>>> DEPLOYMENT SUCCESS: Assigned to [ " << designatedRobot << " ] <<<\n";
                    } else {
                        cout << "\n[Alert] Warning: Operation failed. Entire fleet is busy or offline.\n";
                    }
                }
                break;
            }
            
            case 3: {
                cout << "\nEnter Target Robot ID to service (e.g., ROBOT_C): ";
                string inputID;
                cin >> inputID;
                
                int index = rq.front;
                bool clear = false;
                for (int i = 0; i < rq.size; i++) {
                    if (rq.arr[index].robotID == inputID) {
                        clear = true;
                        if (rq.arr[index].status == "Maintenance") {
                            rq.arr[index].status = "Available";
                            cout << inputID << " diagnostics clear. Asset online.\n";
                        } else if (rq.arr[index].status == "Available") {
                            rq.arr[index].status = "Maintenance";
                            cout << inputID << " taken offline for routine servicing.\n";
                        } else {
                            cout << "Operation Denied: Cannot modify a robot running active navigation logic.\n";
                        }
                        break;
                    }
                    index = (index + 1) % MAX_ROBOTS;
                }
                if (!clear) cout << "Invalid Selection: Identifier match failed.\n";
                break;
            }
            
            case 4:
                cout << "Exiting Assignment Interface...\n";
                break;
                
            default:
                cout << "Error: Selection boundaries exceeded.\n";
        }
    } while (choice != 4);
}
