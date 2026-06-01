#ifndef ITEMMANAGEMENT_HPP
#define ITEMMANAGEMENT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// STRUCT
struct Item1 {
    string itemID;
    string itemName;
    int quantity;
    string location;
};

// GLOBAL ARRAY
extern Item1 items[100];
extern int sizeArr;

// FUNCTION PROTOTYPES
void loadItems1();

void displayItems1();

void insertItem();

void searchItem(string id);

void updateItem();

void deleteItem();

void ItemManagentMenu();

string getItemLocation(string itemID);

#endif