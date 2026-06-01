#include <iostream>
#include <iomanip>
#include "WarehouseLayout.hpp"
#include "ItemManagement.hpp"
#include "OrderManagement.hpp"
#include "RobotNav_PathTracking.hpp"
#include "RobotAssignment.hpp" 

using namespace std;

int main() {
    // Initialize the shared memory queue
    loadItems1();
    Queue masterOrderQueue;
    initQueue(masterOrderQueue);

    // Initialize Task 2 Robot Fleet Queue Memory Structures
    RobotQueue masterRobotQueue;
    populateDefaultFleet(masterRobotQueue);

    // Load persistent file records on startup
    loadPendingOrders(masterOrderQueue);
    Order activeOrder = {-1, "", "", "", ""}; 
    string assignedRobot = ""; 

    WarehouseLayout warehouse;
    warehouse.buildWarehouse();
    NavigationSystem nav;

    int choice = 0;
    do {
        cout << "\n=====================================\n";
        cout << "     CENTRAL WAREHOUSE SYSTEM MENU    \n";
        cout << "=====================================\n\n";
        cout << "1. TASK 1: ORDER MANAGEMENT MODULE\n";
        cout << "2. TASK 2: ROBOT ASSIGNMENT MODULE\n";
        cout << "3. TASK 3: ROBOT NAVIGATION AND PATH TRACKING MODULE\n"; 
        cout << "4. TASK 4: ITEM SEARCH AND MANAGEMENT MODULE\n";
        cout << "5. TASK 5: WAREHOUSE LAYOUT AND NAVIGATION MODULE\n";
        cout << "6. EXIT SYSTEM\n";
        cout << "Select Option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                orderManagementMenu(masterOrderQueue);
                break;

            case 2: 
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }
                if (activeOrder.orderID != -1) {
                    robotAssignmentMenu(masterRobotQueue, activeOrder);
                }
                break;

            case 3: {
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }

                if (activeOrder.orderID != -1) {
                    cout << "\nTask 3 — Robot Navigation & Path Tracking Module" << endl;
                    
                    // Call live allocation tracking from Task 2 dynamically
                    if (assignedRobot.empty()) {
                        cout << "Assigning a robot from the fleet sequence front...\n";
                        if (!assignRobotToOrder(masterRobotQueue, activeOrder.orderID, assignedRobot)) {
                            cout << "[Alert] Navigation Aborted: Entire fleet is busy or offline!\n";
                            break; 
                        }
                        cout << ">>> DEPLOYMENT SUCCESS: Assigned to [ " << assignedRobot << " ] <<<\n";
                    } else {
                        cout << "Using pre-allocated asset for this order: [ " << assignedRobot << " ]\n";
                    }

                    string targetShelf = getItemLocation(activeOrder.itemID);

                    // Task 5 generate path
                    string rawPath[10];
                    int rawSize = 0;
                    warehouse.getPathArray(targetShelf, rawPath, rawSize);

                    if (rawSize > 0) {
                        int routeSteps = rawSize - 1; 
                        stepMovement* finalRoute = new stepMovement[routeSteps];

                        for (int i = 0; i < routeSteps; i++) {
                            if (i == 0) {
                                finalRoute[i].zone = rawPath[1];
                                finalRoute[i].aisle = "";
                                finalRoute[i].shelf = "";
                            } 
                            else if (i == 1) {
                                finalRoute[i].zone = rawPath[1];
                                finalRoute[i].aisle = rawPath[2];
                                finalRoute[i].shelf = "";
                            } 
                            else if (i == 2) {
                                finalRoute[i].zone = rawPath[1];
                                finalRoute[i].aisle = rawPath[2];
                                finalRoute[i].shelf = rawPath[3];
                            }
                        }
                        
                        string orderIDStr = "ORD-" + to_string(activeOrder.orderID);

                        // Execute the Stack-driven path journeys
                        nav.startPath(orderIDStr, assignedRobot, rawPath[1], rawPath[2], rawPath[3], "Pack-Station", finalRoute, routeSteps);
                        nav.returnPath();

                        delete[] finalRoute; 
                        
                        updateOrderStatus(activeOrder.orderID, activeOrder.itemID);
                        
                        // Fire a complete release signal to Task 2 circular queue
                        releaseRobotFromTask(masterRobotQueue, assignedRobot);

                        activeOrder.orderID = -1; 
                        assignedRobot = ""; 
                    } else {
                        cout << "Error: Task 5 could not find a path to " << targetShelf << endl;
                    }
                } else {
                    cout << "\nNo pending orders to process." << endl;
                }
                break;
            }

            case 4:
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }
                if (activeOrder.orderID != -1) {
                    string itemID = activeOrder.itemID;
                    searchItem(itemID);
                    string locationID = getItemLocation(itemID);
                    cout << "\nLocation: " << locationID << endl;
                }
                break;
            
            case 5:
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }

                if (activeOrder.orderID != -1) {
                    string locationID = getItemLocation(activeOrder.itemID);
                    if (!locationID.empty() && locationID.back() == '\r')
                        locationID.pop_back();
                    
                    cout << "\nItem ID  : " << activeOrder.itemID << endl;
                    cout << "Location : " << locationID << endl;

                    if (!locationID.empty()) {
                        string pathArray[10];
                        int    pathSize = 0;

                        warehouse.getPathArray(locationID, pathArray, pathSize);

                        if (pathSize > 0) {
                            cout << "\n  [Task 5] BFS Path to " << locationID << ":\n";
                            for (int i = 0; i < pathSize; i++)
                                cout << "  Step " << (i + 1) << " : " << pathArray[i] << "\n";
                        } else {
                            cout << "\n  [Task 5] Shelf not found in warehouse tree.\n";
                        }
                    } else {
                        cout << "\n  [Error] Item location not found in item list.\n";
                    }

                    warehouse.runMenu();
                }
                break;

            case 6:
                cout << "\nShutting down Central Warehouse System. Goodbye!\n";
                break;

            default:
                cout << "\nInvalid selection. Please enter a number between 1 and 6.\n";
                break;
        }

    } while (choice != 6);
    return 0;
}
