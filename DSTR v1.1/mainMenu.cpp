#include <iostream>
#include <iomanip>
#include "WarehouseLayout.hpp"
#include "ItemManagement.hpp"
#include "OrderManagement.hpp"
#include "RobotNav_PathTracking.hpp"

using namespace std;

int main() {
    // 1. Initialize the shared memory queue
    loadItems1();
    Queue masterOrderQueue;
    initQueue(masterOrderQueue);

    // 2. Load persistent file records on startup
    loadPendingOrders(masterOrderQueue);
    Order activeOrder = {-1, "", "", "", ""}; 
    
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
                // Opens your interactive menu (Add orders, view queue)
                orderManagementMenu(masterOrderQueue);
                break;

            case 2: 
               
                
                // If we don't have an active order yet, pull one from the queue
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }

                
                if (activeOrder.orderID != -1) {
                    // YOU CAN DELETE THIS LINE BELOW - it's just a confirmation massage 
                    // this is the how you get the orderID [ activeOrder.orderID ]
                    cout << "Passing Order ID " << right << setw(4) << setfill('0') << activeOrder.orderID << " to Robot Assignment Module...\n"; 
                    
                    // function call here:
                }
                break;
            

            case 3: {
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }

                if (activeOrder.orderID != -1) {
                    cout << "Task 3 — Robot Navigation & Path Tracking Module" << endl;
                    
                    //Mock Task 2 for now (until they finish their module)
                    string assignedRobot = "R-01"; 

                    //Task 4 gets the shelf name based on the item ID
                    string targetShelf = getItemLocation(activeOrder.itemID);

                    //Task 5 generate path
                    string rawPath[10];
                    int rawSize = 0;
                    warehouse.getPathArray(targetShelf, rawPath, rawSize);

                    if (rawSize > 0) {
                        // 4. THE BRIDGE: Map Task 5 data to Task 3 struct
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
                        
                        // Convert orderID from int to string for Task 3
                        string orderIDStr = "ORD-" + to_string(activeOrder.orderID);

                        // 5. Execute the Robot Path!
                        nav.startPath(orderIDStr, assignedRobot, rawPath[1], rawPath[2], rawPath[3], "Pack-Station", finalRoute, routeSteps);
                        nav.returnPath();

                        delete[] finalRoute; // Clean up memory
                        
                        // 6. Mark order as complete
                        updateOrderStatus(activeOrder.orderID, activeOrder.itemID);
                        activeOrder.orderID = -1; // Reset for the next order
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

                    ItemManagentMenu();

                    // Task 4 searches for the item
                    searchItem(itemID);

                    // Task 4 returns the shelf location
                    string locationID = getItemLocation(itemID);

                    cout << "\nLocation: " << locationID << endl;
                }
                break;
            

            case 5:
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }

                if (activeOrder.orderID != -1) {

                    //  get shelf name from Task 4 
                    string locationID = getItemLocation(activeOrder.itemID);
                    if (!locationID.empty() && locationID.back() == '\r')
                        locationID.pop_back();
                    
                    cout << "\nItem ID  : " << activeOrder.itemID << endl;
                    cout << "Location : " << locationID << endl;

                    // Task 5 runs BFS for fastest and shortest path using shelf name taken from Task 4
                    // path is generated automatically
                    if (!locationID.empty())
                    {
                        string pathArray[10];
                        int    pathSize = 0;

                        warehouse.getPathArray(locationID, pathArray, pathSize);

                        if (pathSize > 0)
                        {
                            cout << "\n  [Task 5] BFS Path to " << locationID << ":\n";
                            for (int i = 0; i < pathSize; i++)
                                cout << "  Step " << (i + 1) << " : " << pathArray[i] << "\n";
                        }
                        else
                        {
                            cout << "\n  [Task 5] Shelf not found in warehouse tree.\n";
                        }
                    }
                    else
                    {
                        cout << "\n  [Error] Item location not found in item list.\n";
                    }

                    // open full Task 5 menu as normal
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
