#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "OrderManagement.hpp" 

using namespace std;

// ----------------------------------------------- [Global Variable Instantiation] -----------------------------------------------
int  nextOrderID = 1;    
Item itemList[MAX_ITEM]; 
int  itemCount = 0;      

// =============================================== [Circular Queue Algorithm] =============================================== 
void initQueue(Queue& q) {
    q.front = -1;
    q.rear  = -1;
    q.size  = 0;
}

void enqueue(Queue& q, Order x) {
    if (q.size == MAX) {
        cout << "Queue is full. Cannot add more orders.\n";
        return;
    }
    if (q.size == 0) q.front = 0;          
    q.rear        = (q.rear + 1) % MAX;   
    q.arr[q.rear] = x;                    
    q.size++;
}

Order dequeue(Queue& q) {
    Order v = q.arr[q.front];  
    q.size--;
    if (q.front == q.rear) {
        q.front = q.rear = -1; 
    } else {
        q.front = (q.front + 1) % MAX; 
    }
    return v;
}

Order peek(Queue& q) { return q.arr[q.front]; }

void displayQueue(Queue& q) {
    if (q.size == 0) {
        cout << "No pending orders in queue.\n";
        return;
    }

    cout << "\n------------------------------------------------------------------\n";
    cout << "                     PENDING ORDERS QUEUE                        \n";
    cout << "------------------------------------------------------------------\n";
    cout << "  No. | Order ID | Customer Name   | Item ID  | Item Name | Status\n";
    cout << "------------------------------------------------------------------\n";

    int index = q.front;
    for (int i = 0; i < q.size; i++) {
        cout << right;
        cout << "  " << i + 1 << "   | ";
        cout << setw(4) << setfill('0') << q.arr[index].orderID << "     | ";
        cout << left;
        cout << setw(15) << setfill(' ') << q.arr[index].customerName << " | ";
        cout << setw(8)  << setfill(' ') << q.arr[index].itemID       << " | ";
        cout << setw(9)  << setfill(' ') << q.arr[index].itemName     << " | ";
        cout << q.arr[index].status << "\n";

        index = (index + 1) % MAX; 
    }
    cout << "------------------------------------------------------------------\n";
    cout << "Total pending orders: " << q.size << "\n";
}

// =============================================== [File Operations] =============================================== 
void loadNextOrderID() {
    ifstream file("order.txt");
    if (!file.is_open()) return; 

    int maxID = 0;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int id = stoi(token);
        if (id > maxID) maxID = id;
    }
    file.close();

    if (maxID > 0) nextOrderID = maxID + 1; 
}

void saveToOrder(Order o) {
    ofstream file("order.txt", ios::app); 
    if (file.is_open()) {
        file << setw(4) << setfill('0') << o.orderID << ","
             << o.customerName << "," << o.itemID << ","
             << o.itemName << "," << o.status << "\n";
        file.close();
    }
}

void updateOrderStatus(int tOrderID, string tItemID) {
    Order temp[MAX];
    int count = 0;
    ifstream inFile("order.txt");
    
    if (!inFile.is_open()) {
        cout << "Error: Could not open order.txt to update status.\n";
        return;
    }

    // Read all records into a temporary array
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;

        getline(ss, token,                    ','); temp[count].orderID      = stoi(token);
        getline(ss, temp[count].customerName, ',');
        getline(ss, temp[count].itemID,       ',');
        getline(ss, temp[count].itemName,     ',');
        getline(ss, temp[count].status,       ',');

        // Clean up hidden newline characters (fixes Windows '\r' issues)
        if (!temp[count].itemID.empty() && temp[count].itemID.back() == '\r') temp[count].itemID.pop_back();
        if (!temp[count].status.empty() && temp[count].status.back() == '\r') temp[count].status.pop_back();

        // If this is the exact order and item we are looking for, complete it!
        if (temp[count].orderID == tOrderID && temp[count].itemID == tItemID) {
            temp[count].status = "Completed";
        }
        count++;
    }
    inFile.close();

    // Rewrite the master file with the new Completed statuses
    ofstream outFile("order.txt");
    for (int i = 0; i < count; i++) {
        outFile << right << setw(4) << setfill('0') << temp[i].orderID << ","
                << left << temp[i].customerName << "," << temp[i].itemID << ","
                << temp[i].itemName << "," << temp[i].status << "\n";
    }
    outFile.close();
}

void displayAllOrders() {
    ifstream file("order.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open order.txt\n";
        return;
    }

    cout << "\n------------------------------------------------------------------\n";
    cout << "                      ALL ORDERS RECORD                          \n";
    cout << "------------------------------------------------------------------\n";
    cout << "  Order ID | Customer Name   | Item ID  | Item Name | Status     \n";
    cout << "------------------------------------------------------------------\n";

    string line;
    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        Order o;

        getline(ss, token,          ','); o.orderID = stoi(token);
        getline(ss, o.customerName, ',');
        getline(ss, o.itemID,       ',');
        getline(ss, o.itemName,     ',');
        getline(ss, o.status,       ',');

        cout << "  " << right << setw(4) << setfill('0') << o.orderID << "     | ";
        cout << left  << setw(15) << setfill(' ') << o.customerName << " | ";
        cout << left  << setw(8)  << setfill(' ') << o.itemID       << " | ";
        cout << left  << setw(9)  << setfill(' ') << o.itemName     << " | ";
        cout << o.status << "\n";
        count++;
    }
    file.close();
    cout << "------------------------------------------------------------------\n";
    cout << "Total records: " << count << "\n";
}

// =============================================== [Item List Operations] =============================================== 
void loadItems() {
    ifstream file("Items.txt");
    if (!file.is_open()) return;

    string line;
    itemCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;

        getline(ss, itemList[itemCount].itemID,   ',');
        getline(ss, itemList[itemCount].itemName, ',');
        getline(ss, token, ','); itemList[itemCount].quantity = stoi(token);
        getline(ss, itemList[itemCount].location, ',');
        itemCount++;
    }
    file.close();
}

void displayItems() {
    cout << "\n------------------------------------------------------------------\n";
    cout << "                     AVAILABLE ITEMS LIST                        \n";
    cout << "------------------------------------------------------------------\n";
    
    bool anyAvailable = false;
    for (int i = 0; i < itemCount; i++) {
        if (itemList[i].quantity > 0) {
            cout << "  " << setw(8)  << setfill(' ') << left << itemList[i].itemID   << " | ";
            cout << setw(15) << setfill(' ') << left << itemList[i].itemName          << " | ";
            cout << setw(8)  << setfill(' ') << left << itemList[i].quantity          << " | ";
            cout << itemList[i].location << "\n";
            anyAvailable = true;
        }
    }
    if (!anyAvailable) cout << "No items available.\n";
    cout << "------------------------------------------------------------------\n";
}

int findItem(string targetID) {
    for (int i = 0; i < itemCount; i++) {
        if (itemList[i].itemID == targetID) return i;
    }
    return -1;
}

// =============================================== [Order Management Functions] =============================================== 
void addOrder(Queue& q) {
    if (q.size == MAX) {
        cout << "Order queue is full. Cannot accept new orders.\n";
        return;
    }

    string customerName;
    cout << "\nEnter Customer Name : ";
    cin.ignore();
    getline(cin, customerName);

    int currentOrderID = nextOrderID++;  
    int itemsAdded = 0;

    while (true) {
        displayItems(); 
        cout << "\nEnter Item ID to add (or 0 to finish) : ";
        string selectedItemID;
        getline(cin, selectedItemID);

        if (selectedItemID == "0") break;

        int idx = findItem(selectedItemID);
        if (idx == -1) {
            cout << "Item ID not found. Please try again.\n";
            continue;
        }
        if (itemList[idx].quantity == 0) {
            cout << "Item is out of stock. Please select another item.\n";
            continue;
        }

        bool duplicate = false;
        int checkIndex = q.front;
        for (int i = 0; i < q.size; i++) {
            if (q.arr[checkIndex].orderID == currentOrderID &&
                q.arr[checkIndex].itemID  == itemList[idx].itemID) {
                duplicate = true;
                break;
            }
            checkIndex = (checkIndex + 1) % MAX;
        }

        if (duplicate) {
            cout << "Item " << itemList[idx].itemID << " already added to this order.\n";
            continue;
        }

        Order o;
        o.orderID      = currentOrderID;
        o.customerName = customerName;
        o.itemID       = itemList[idx].itemID;
        o.itemName     = itemList[idx].itemName;
        o.status       = "Pending";

        enqueue(q, o);   
        saveToOrder(o);

        itemList[idx].quantity--;
        ofstream itemFile("Items.txt");
        for (int i = 0; i < itemCount; i++) {
            itemFile << itemList[i].itemID << "," << itemList[i].itemName << ","
                     << itemList[i].quantity << "," << itemList[i].location << "\n";
        }
        itemFile.close();

        cout << "Item " << o.itemID << " (" << o.itemName << ") added to Order "
             << right << setw(4) << setfill('0') << o.orderID << "\n";
        itemsAdded++;

        if (q.size == MAX) {
            cout << "Queue is now full. Cannot add more items.\n";
            break;
        }
    }

    if (itemsAdded == 0) {
        cout << "No items selected. Order cancelled.\n";
        nextOrderID--; 
    } else {
        cout << "\nOrder " << right << setw(4) << setfill('0') << currentOrderID << " created.\n";
    }
}

Order processOrder(Queue& q) {
    if (q.size == 0) {
        cout << "No pending orders to process.\n";
        Order emptyOrder = {-1, "", "", "", ""}; 
        return emptyOrder;
    }

    Order o = dequeue(q);  
    o.status = "Processing"; 

    cout << "\n--------------------------------------------------\n";
    cout << " [Task 1 Dequeue] Order Extracted Successfully!\n";
    cout << "--------------------------------------------------\n";
    
    // FIX: Add 'right' before printing the ID so the zeros pad the front!
    cout << "  Order ID  : " << right << setw(4) << setfill('0') << o.orderID << "\n";
    
    // It's good practice to switch back to left for the text fields just in case
    cout << left; 
    cout << "  Customer  : " << o.customerName << "\n";
    cout << "  Item ID   : " << o.itemID << "\n";
    cout << "  Item Name : " << o.itemName << "\n";
    cout << "--------------------------------------------------\n";
    
    return o;
}

void viewCurrentOrder(Queue& q) {
    if (q.size == 0) {
        cout << "No orders in queue.\n";
        return;
    }
    Order o = peek(q); 
    cout << "\nCurrent Order (Front of Queue):\n";
    cout << "  Order ID  : " << setw(4) << setfill('0') << o.orderID << "\n";
    cout << "  Customer  : " << o.customerName << "\n";
    cout << "  Item ID   : " << o.itemID << "\n";
    cout << "  Item Name : " << o.itemName << "\n";
}

void loadPendingOrders(Queue& q) {
    ifstream file("order.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        Order o;

        getline(ss, token,          ','); o.orderID = stoi(token);
        getline(ss, o.customerName, ',');
        getline(ss, o.itemID,       ',');
        getline(ss, o.itemName,     ',');
        getline(ss, o.status,       ',');

        // Clean up hidden Windows carriage returns
        if (!o.status.empty() && o.status.back() == '\r') o.status.pop_back();

        // Push it back into the live queue!
        if (o.status == "Pending") {
            enqueue(q, o);
        }
    }
    file.close();
}

// =============================================== [Menu] =============================================== 
void orderManagementMenu(Queue& q) {
    loadNextOrderID();
    loadItems();

    int opt = 0;
    do {
        cout << "\n=====================================\n";
        cout << "     ORDER MANAGEMENT MODULE         \n";
        cout << "=====================================\n\n";
        cout << "1. Add New Order\n";
        cout << "2. View Current Order (Peek)\n";
        cout << "3. Display Pending Orders\n";
        cout << "4. Display All Orders Record\n";
        cout << "5. Return to Main Warehouse Menu\n";
        cout << "Select: ";
        cin >> opt;

        switch (opt) {
            case 1: addOrder(q); break;
            case 2: viewCurrentOrder(q); break;
            case 3: displayQueue(q); break;
            case 4: displayAllOrders(); break;
            case 5: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid selection.\n";
        }
    } while (opt != 5);
}