#ifndef ORDER_MANAGEMENT_HPP
#define ORDER_MANAGEMENT_HPP

// ==========================================================
// TASK 1: ORDER MANAGEMENT MODULE DECLARATIONS
// ==========================================================

#include <string>
using namespace std;

// ----------------------------------------------- [Constants] -----------------------------------------------
#define MAX      999  // maximum pending orders in circular queue
#define MAX_ITEM 100  // maximum items in item list

// ----------------------------------------------- [Structs] -----------------------------------------------
struct Item
{
    string itemID;
    string itemName;
    int    quantity;
    string location;
};

struct Order
{
    int    orderID;       
    string customerName;  
    string itemID;        
    string itemName;      
    string status;        
};

struct Queue
{
    Order arr[MAX];  
    int   front;     
    int   rear;      
    int   size;      
};

void initQueue(Queue& q);
void loadNextOrderID();
void loadItems();
void orderManagementMenu(Queue& q);
Order processOrder(Queue& q);
void updateOrderStatus(int tOrderID, string tItemID);
void loadPendingOrders(Queue& q);

// 'extern' tells the compiler these variables exist, but will be created in exactly ONE .cpp file
extern int  nextOrderID;    
extern Item itemList[MAX_ITEM]; 
extern int  itemCount;      


#endif