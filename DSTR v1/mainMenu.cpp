#include <iostream>
#include <iomanip>
#include "WarehouseLayout.hpp"
#include "ItemManagement.hpp"
#include "OrderManagement.hpp" 

using namespace std;

int main() {
    // 1. Initialize the shared memory queue
    Queue masterOrderQueue;
    initQueue(masterOrderQueue);

    // 2. Load persistent file records on startup
    loadPendingOrders(masterOrderQueue);
    Order activeOrder = {-1, "", "", "", ""}; 

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
            

            case 3:
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }
                if (activeOrder.orderID != -1) {

                    
                    // function call here:

                    // you can move this 2 line to your code file and just call this function when the order is completed in Task 3
                    updateOrderStatus(activeOrder.orderID, activeOrder.itemID);
                    activeOrder.orderID = -1;
                }
                break;

            case 4:
                if (activeOrder.orderID == -1) {
                    activeOrder = processOrder(masterOrderQueue);
                }

                if (activeOrder.orderID != -1) {
                    ItemManagentMenu();// YOU CAN DELETE THIS LINE BELOW - it's just a confirmation massage 
                    // this is the how you get the orderID [ activeOrder.orderID ]
                    cout << "Passing Item ID '" << activeOrder.itemID << "' to Item Search Module...\n";
                    
                }
                break;
            

            case 5:{
                WarehouseLayout warehouse;
                warehouse.buildWarehouse();
                warehouse.runMenu();
                cout << "\n[Task 5 Execution] Warehouse Layout Module is under construction...\n";
                
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

    loadItems();

    WarehouseLayout warehouse;
    // Step 1: Task 4 finds shelf name
    string shelfName = getItemLocation("I001");

    // Step 2: Task 5 finds path
    string pathArray[10];
    int    pathSize = 0;
    warehouse.getPathArray(shelfName, pathArray, pathSize);
    
    return 0;
}