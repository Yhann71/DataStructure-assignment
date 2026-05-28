#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "ItemManagement.hpp"
using namespace std;


// GLOBAL ARRAY
Item1 items[100];
int sizeArr = 0;

// LOAD ITEMS
void loadItems1() {
    ifstream file("items.txt");

    if (!file) {
        cout << "Cannot open items.txt\n";
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        getline(ss, items[sizeArr].itemID, ',');
        getline(ss, items[sizeArr].itemName, ',');

        string qty;
        getline(ss, qty, ',');
        items[sizeArr].quantity = stoi(qty);

        getline(ss, items[sizeArr].location);

        sizeArr++;
    }

    file.close();

    cout << "Items loaded successfully.\n";
}

// DISPLAY
void displayItems1() {

    cout << "\n";
    cout << "=====================================================================\n";

    cout << left
         << setw(8)  << "ID"
         << setw(22) << "Name"
         << setw(8)  << "Qty"
         << setw(20) << "Location"
         << endl;

    cout << "=====================================================================\n";

    for (int i = 0; i < sizeArr; i++) {

        cout << left
             << setw(8)  << items[i].itemID
             << setw(22) << items[i].itemName
             << setw(8)  << items[i].quantity
             << setw(20) << items[i].location
             << endl;
    }

    cout << "=====================================================================\n";
}

// INSERT
void insertItem() {

    cout << "\nEnter Item ID: ";
    cin >> items[sizeArr].itemID;

    cin.ignore();

    cout << "Enter Item Name: ";
    getline(cin, items[sizeArr].itemName);

    cout << "Enter Quantity: ";
    cin >> items[sizeArr].quantity;

    cin.ignore();

    cout << "Enter Location: ";
    getline(cin, items[sizeArr].location);

    sizeArr++;

    cout << "Item added successfully.\n";
}

// SEARCH
void searchItem(string id) {

    bool found = false;

    for (int i = 0; i < sizeArr; i++) {

        if (items[i].itemID == id) {

            cout << "\nItem Found\n";
            cout << "ID: " << items[i].itemID << endl;
            cout << "Name: " << items[i].itemName << endl;
            cout << "Quantity: " << items[i].quantity << endl;
            cout << "Location: " << items[i].location << endl;

            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Item not found.\n";
    }
}

// UPDATE
void updateItem() {

    string id;

    cout << "\nEnter Item ID: ";
    cin >> id;

    for (int i = 0; i < sizeArr; i++) {

        if (items[i].itemID == id) {

            cout << "Enter New Quantity: ";
            cin >> items[i].quantity;

            cin.ignore();

            cout << "Enter New Location: ";
            getline(cin, items[i].location);

            cout << "Item updated successfully.\n";
            return;
        }
    }

    cout << "Item not found.\n";
}

// DELETE
void deleteItem() {

    string id;

    cout << "\nEnter Item ID: ";
    cin >> id;

    for (int i = 0; i < sizeArr; i++) {

        if (items[i].itemID == id) {

            for (int j = i; j < sizeArr - 1; j++) {
                items[j] = items[j + 1];
            }

            sizeArr--;

            cout << "Item deleted successfully.\n";
            return;
        }
    }

    cout << "Item not found.\n";
}

// MENU
void ItemManagentMenu() {

    int opt;

    do {

        cout << "\n===== ITEM SEARCH & MANAGEMENT =====\n";
        cout << "1. Display All Items\n";
        cout << "2. Search Item\n";
        cout << "3. Insert Item\n";
        cout << "4. Update Item\n";
        cout << "5. Delete Item\n";
        cout << "6. Exit\n";
        cout << "Select: ";

        cin >> opt;

        switch (opt) {

        case 1:
            displayItems1();
            break;

        case 2: {
            string id;

            cout << "\nEnter Item ID: ";
            cin >> id;

            searchItem(id);
            break;
        }

        case 3:
            insertItem();
            break;

        case 4:
            updateItem();
            break;

        case 5:
            deleteItem();
            break;

        case 6:
            cout << "Exit program.\n";
            break;

        default:
            cout << "Invalid option.\n";
        }

    } while (opt != 6);
}

string getItemLocation(string itemID)
{
    for (int i = 0; i < sizeArr; i++)
    {
        if (items[i].itemID == itemID)
            return items[i].location;  // returns e.g "Shelf-A1-01"
    }
    return "";  // not found
}
