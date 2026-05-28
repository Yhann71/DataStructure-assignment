#include "WarehouseLayout.hpp"
#include <iostream>
#include <string>
#include <cctype>

using namespace std;


TreeNode::TreeNode()
{
    name  = "";
    type  = "";
    left  = nullptr;
    right = nullptr;
}

TreeNode::TreeNode(const string& n, const string& t)
{
    name  = n;
    type  = t;
    left  = nullptr;
    right = nullptr;
}


PathResult::PathResult()
{
    stepCount = 0;
    found     = false;
}

void PathResult::display() const
{
    if (!found)
    {
        cout << "  path not found\n";
        return;
    }

    for (int i = 0; i < stepCount; i++)
    {
        cout << "  Step " << (i + 1) << " : " << steps[i];
        if (i == 0) cout << "  (root)";
        if (i == 1) cout << "  (zone)";
        if (i == 2) cout << "  (aisle)";
        if (i == 3) cout << "  (shelf - destination)";
        cout << "\n";
    }
}

string PathResult::getZone() const
{
    if (found && stepCount > 1)
        return steps[1];
    return "";
}

string PathResult::getAisle() const
{
    if (found && stepCount > 2)
        return steps[2];
    return "";
}

string PathResult::getShelf() const
{
    if (found && stepCount > 3)
        return steps[3];
    return "";
}


BFSQueueNode::BFSQueueNode()
{
    node    = nullptr;
    pathLen = 0;
    next    = nullptr;
}

BFSQueueNode::BFSQueueNode(TreeNode* n, const string arr[], int len)
{
    node    = n;
    pathLen = len;
    next    = nullptr;

    for (int i = 0; i < len && i < MAX_PATH; i++)
        path[i] = arr[i];
}


BFSQueue::BFSQueue()
{
    head  = nullptr;
    tail  = nullptr;
    count = 0;
}

BFSQueue::~BFSQueue()
{
    while (!isEmpty())
    {
        BFSQueueNode* temp = dequeue();
        delete temp;
    }
}

void BFSQueue::enqueue(TreeNode* node, const string arr[], int len)
{
    BFSQueueNode* newNode = new BFSQueueNode(node, arr, len);

    if (tail == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail       = newNode;
    }
    count++;
}

BFSQueueNode* BFSQueue::dequeue()
{
    if (isEmpty()) return nullptr;

    BFSQueueNode* temp = head;
    head = head->next;

    if (head == nullptr)
        tail = nullptr;

    temp->next = nullptr;
    count--;
    return temp;
}

bool BFSQueue::isEmpty() const
{
    return count == 0;
}


WarehouseLayout::WarehouseLayout()
{
    root = nullptr;
}

WarehouseLayout::~WarehouseLayout()
{
    deleteTree(root);
    root = nullptr;
}

void WarehouseLayout::deleteTree(TreeNode* node)
{
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

TreeNode* WarehouseLayout::makeNode(const string& name, const string& type)
{
    return new TreeNode(name, type);
}

void WarehouseLayout::buildWarehouse()
{
    deleteTree(root);
    root = nullptr;

    root = makeNode("Warehouse", "Root");

    TreeNode* zA = makeNode("Zone-A", "Zone");
    TreeNode* zB = makeNode("Zone-B", "Zone");
    TreeNode* zC = makeNode("Zone-C", "Zone");

    root->left = zA;
    zA->right  = zB;
    zB->right  = zC;

    TreeNode* aA1 = makeNode("Aisle-A1", "Aisle");
    TreeNode* aA2 = makeNode("Aisle-A2", "Aisle");
    TreeNode* aB1 = makeNode("Aisle-B1", "Aisle");
    TreeNode* aB2 = makeNode("Aisle-B2", "Aisle");
    TreeNode* aC1 = makeNode("Aisle-C1", "Aisle");
    TreeNode* aC2 = makeNode("Aisle-C2", "Aisle");

    zA->left = aA1;   aA1->right = aA2;
    zB->left = aB1;   aB1->right = aB2;
    zC->left = aC1;   aC1->right = aC2;

    TreeNode* sA101 = makeNode("Shelf-A1-01", "Shelf");
    TreeNode* sA102 = makeNode("Shelf-A1-02", "Shelf");
    TreeNode* sA201 = makeNode("Shelf-A2-01", "Shelf");
    TreeNode* sA202 = makeNode("Shelf-A2-02", "Shelf");
    TreeNode* sB101 = makeNode("Shelf-B1-01", "Shelf");
    TreeNode* sB102 = makeNode("Shelf-B1-02", "Shelf");
    TreeNode* sB201 = makeNode("Shelf-B2-01", "Shelf");
    TreeNode* sB202 = makeNode("Shelf-B2-02", "Shelf");
    TreeNode* sC101 = makeNode("Shelf-C1-01", "Shelf");
    TreeNode* sC102 = makeNode("Shelf-C1-02", "Shelf");
    TreeNode* sC201 = makeNode("Shelf-C2-01", "Shelf");
    TreeNode* sC202 = makeNode("Shelf-C2-02", "Shelf");

    aA1->left = sA101;  sA101->right = sA102;
    aA2->left = sA201;  sA201->right = sA202;
    aB1->left = sB101;  sB101->right = sB102;
    aB2->left = sB201;  sB201->right = sB202;
    aC1->left = sC101;  sC101->right = sC102;
    aC2->left = sC201;  sC201->right = sC202;

    cout << "\n  Warehouse tree built successfully.\n";
    cout << "  Structure : Binary Tree (Left-Child Right-Sibling)\n";
    cout << "  Zones     : 3  (Zone-A, Zone-B, Zone-C)\n";
    cout << "  Aisles    : 6  (2 per zone)\n";
    cout << "  Shelves   : 12 (2 per aisle)\n";
    cout << "  Total nodes in tree: 22\n";
}


// ==========================================================
// BFS - Breadth First Search
// used to find the shortest path from Warehouse to a shelf
//
// how it works:
//   uses a queue (FIFO) to explore the tree level by level
//   level 0 = Warehouse (root)
//   level 1 = Zones
//   level 2 = Aisles
//   level 3 = Shelves 
//
//   each node in the queue carries the path taken to reach it
//   when the target shelf is found, that path is returned
//   because BFS explores level by level, the first match
//   found is always the shortest path (fewest steps)
// ==========================================================
PathResult WarehouseLayout::bfsSearch(const string& target) const
{
    PathResult result;

    if (root == nullptr) return result;

    BFSQueue queue;

    string startPath[MAX_PATH];
    startPath[0] = root->name;
    queue.enqueue(root, startPath, 1);

    while (!queue.isEmpty())
    {
        BFSQueueNode* slot = queue.dequeue();
        TreeNode*     curr = slot->node;

        string currPath[MAX_PATH];
        int currLen = slot->pathLen;

        for (int i = 0; i < currLen; i++)
        currPath[i] = slot->path[i];

        delete slot;

        if (curr->name == target)
        {
            result.found     = true;
            result.stepCount = currLen;
            for (int i = 0; i < currLen; i++)
            result.steps[i] = currPath[i];
            return result;
        }

        if (curr->left != nullptr)
        {
            string childPath[MAX_PATH];
            for (int i = 0; i < currLen; i++)
            childPath[i] = currPath[i];
            childPath[currLen] = curr->left->name;
            queue.enqueue(curr->left, childPath, currLen + 1);

            TreeNode* sib = curr->left->right;
            while (sib != nullptr)
            {
                string sibPath[MAX_PATH];
                for (int i = 0; i < currLen; i++)
                sibPath[i] = currPath[i];
                sibPath[currLen] = sib->name;
                queue.enqueue(sib, sibPath, currLen + 1);
                sib = sib->right;
            }
        }
    }

    return result;
}

PathResult WarehouseLayout::findPath(const string& shelfName) const
{
    return bfsSearch(shelfName);
}


// getPathArray - for Task 3 integration
// fills outArray[] with path steps and outSize with how many steps
// Task 3 reads: outArray[1]=Zone, outArray[2]=Aisle, outArray[3]=Shelf
void WarehouseLayout::getPathArray(const string& shelfName,string outArray[],int& outSize) const
{
    PathResult path = bfsSearch(shelfName);

    if (path.found)
    {
        outSize = path.stepCount;
        for (int i = 0; i < path.stepCount; i++)
        outArray[i] = path.steps[i];
    }
    else
    {
        outSize = 0;
    }
}


// ==========================================================
// DFS (Pre-Order)
// used to traverse every node in the warehouse tree
//
// pre-order = visit parent BEFORE children
//   1. print current node
//   2. recurse left  (go deeper into children)
//   3. recurse right (go across to siblings)
//
// why pre-order:
//   Zone prints before its Aisles,
//   Aisle prints before its Shelves
//   this matches how a warehouse layout is read naturally
//   if BFS was used for display, all Zones would print
//   before any Aisle which looks wrong and confusing
// ==========================================================
void WarehouseLayout::preOrderTraversal() const
{
    printHeader("DFS PRE-ORDER TRAVERSAL");
    cout << "\n";
    cout << "  Visits parent before children at every level.\n";
    cout << "  left ptr  = go deeper (child)\n";
    cout << "  right ptr = go across (sibling, same level)\n\n";
    dfsPreOrder(root, 0);
    printLine();
}

void WarehouseLayout::dfsPreOrder(TreeNode* node, int depth) const
{
    if (node == nullptr) return;

    for (int i = 0; i < depth; i++) cout << "    ";
    cout << "[" << node->type << "]  " << node->name << "\n";

    dfsPreOrder(node->left,  depth + 1);
    dfsPreOrder(node->right, depth);
}


TreeNode* WarehouseLayout::findNode(const string& name) const
{
    return searchNode(root, name);
}

TreeNode* WarehouseLayout::searchNode(TreeNode* node, const string& name) const
{
    if (node == nullptr)    return nullptr;
    if (node->name == name) return node;

    TreeNode* found = searchNode(node->left, name);
    if (found != nullptr)   return found;

    return searchNode(node->right, name);
}

bool WarehouseLayout::nodeExists(const string& name) const
{
    return findNode(name) != nullptr;
}


void WarehouseLayout::displayLayout() const
{
    printHeader("WAREHOUSE LAYOUT  (Binary Tree)");
    cout << "\n";
    cout << "  left  = child (one level down)\n";
    cout << "  right = sibling (same level)\n\n";
    cout << "  [Root]  = whole warehouse\n";
    cout << "  [Zone]  = large section  e.g. Zone-A\n";
    cout << "  [Aisle] = corridor inside a zone\n";
    cout << "  [Shelf] = storage unit inside an aisle\n\n";
    printTreeView(root, 0);
    printLine();
}

void WarehouseLayout::printTreeView(TreeNode* node, int depth) const
{
    if (node == nullptr) return;

    for (int i = 0; i < depth; i++) cout << "    ";
    if (depth > 0) cout << "|-- ";
    cout << "[" << node->type << "]  " << node->name << "\n";

    printTreeView(node->left,  depth + 1);
    printTreeView(node->right, depth);
}

void WarehouseLayout::displayAllShelves() const
{
    printHeader("ALL SHELVES");
    cout << "\n  All shelf locations a robot can be sent to:\n\n";
    printShelvesList(root);
    printLine();
}

void WarehouseLayout::printShelvesList(TreeNode* node) const
{
    if (node == nullptr) return;
    if (node->type == "Shelf")
        cout << "    >> " << node->name << "\n";
    printShelvesList(node->left);
    printShelvesList(node->right);
}

void WarehouseLayout::displayZoneContents(const string& zoneName) const
{
    printHeader("ZONE: " + zoneName);
    TreeNode* zone = findNode(zoneName);

    if (zone == nullptr || zone->type != "Zone")
    {
        cout << "  Zone not found.\n";
        printLine();
        return;
    }

    TreeNode* aisle = zone->left;
    while (aisle != nullptr)
    {
        cout << "\n    [Aisle]  " << aisle->name << "\n";
        TreeNode* shelf = aisle->left;
        while (shelf != nullptr)
        {
            cout << "        >> " << shelf->name << "\n";
            shelf = shelf->right;
        }
        aisle = aisle->right;
    }
    printLine();
}

void WarehouseLayout::displayAisleContents(const string& aisleName) const
{
    printHeader("AISLE: " + aisleName);
    TreeNode* aisle = findNode(aisleName);

    if (aisle == nullptr || aisle->type != "Aisle")
    {
        cout << "  Aisle not found.\n";
        printLine();
        return;
    }

    TreeNode* shelf = aisle->left;
    while (shelf != nullptr)
    {
        cout << "    >> " << shelf->name << "\n";
        shelf = shelf->right;
    }
    printLine();
}


void WarehouseLayout::showAllLocations() const
{
    cout << "\n  Locations in warehouse:\n\n";
    TreeNode* zone = (root ? root->left : nullptr);
    while (zone != nullptr)
    {
        cout << "  [" << zone->name << "]\n";
        TreeNode* aisle = zone->left;
        while (aisle != nullptr)
        {
            cout << "    +--> " << aisle->name << "\n";
            TreeNode* shelf = aisle->left;
            while (shelf != nullptr)
            {
                cout << "           >> " << shelf->name << "\n";
                shelf = shelf->right;
            }
            aisle = aisle->right;
        }
        zone = zone->right;
    }
    cout << "\n";
}

void WarehouseLayout::showZones() const
{
    cout << "\n  Zones available:\n";
    TreeNode* zone = (root ? root->left : nullptr);
    int n = 1;
    while (zone != nullptr)
    {
        cout << "    " << n++ << ") " << zone->name << "\n";
        zone = zone->right;
    }
}

void WarehouseLayout::showAislesInZone(const string& zone) const
{
    TreeNode* z = findNode(zone);
    if (z == nullptr) return;
    cout << "\n  Aisles in " << zone << ":\n";
    TreeNode* aisle = z->left;
    while (aisle != nullptr)
    {
        cout << "    -> " << aisle->name << "\n";
        aisle = aisle->right;
    }
}

void WarehouseLayout::showShelvesInAisle(const string& aisle) const
{
    TreeNode* a = findNode(aisle);
    if (a == nullptr) return;
    cout << "\n  Shelves in " << aisle << ":\n";
    TreeNode* shelf = a->left;
    while (shelf != nullptr)
    {
        cout << "    -> " << shelf->name << "\n";
        shelf = shelf->right;
    }
}


string WarehouseLayout::fixInput(const string& input) const
{
    if (input.empty()) return input;

    string result = input;
    bool   capNext = true;

    for (int i = 0; i < (int)result.size(); i++)
    {
        if (result[i] == '-')
        {
            capNext = true;
        }
        else if (capNext)
        {
            result[i] = toupper(result[i]);
            capNext   = false;
        }
        else
        {
            result[i] = tolower(result[i]);
        }
    }
    return result;
}


void WarehouseLayout::printHeader(const string& title) const
{
    cout << "\n";
    cout << "=====================================================\n";
    cout << "  " << title << "\n";
    cout << "=====================================================\n";
}

void WarehouseLayout::printLine() const
{
    cout << "-----------------------------------------------------\n";
}


void WarehouseLayout::runMenu()
{
    int choice = -1;

    do
    {
        cout << "\n";
        cout << "=====================================================\n";
        cout << "  WAREHOUSE LAYOUT AND NAVIGATION MODULE  (Task 5)\n";
        cout << "  Data Structure : Binary Tree\n";
        cout << "  Algorithm 1    : BFS  (path finding)\n";
        cout << "  Algorithm 2    : DFS  (traversal)\n";
        cout << "=====================================================\n";
        cout << "  --- DISPLAY ---\n";
        cout << "  1. Display Warehouse Layout (Binary Tree View)\n";
        cout << "  2. Display All Shelves\n";
        cout << "  3. Display Zone Contents\n";
        cout << "  4. Display Aisle Contents\n";
        cout << "\n";
        cout << "  --- ALGORITHM: BFS (Breadth-First Search) ---\n";
        cout << "  5. BFS - Find Shortest Path to a Shelf\n";
        cout << "\n";
        cout << "  --- ALGORITHM: DFS (Depth-First Search) ---\n";
        cout << "  6. DFS - Pre-Order Traversal of Entire Warehouse\n";
        cout << "\n";
        cout << "  0. Exit\n";
        cout << "=====================================================\n";
        cout << "  Enter choice (0-6): ";

        cin >> choice;
        cin.ignore(1000, '\n');

        switch (choice)
        {
            case 1:
                displayLayout();
                break;

            case 2:
                displayAllShelves();
                break;

            case 3:
            {
                printHeader("VIEW ZONE CONTENTS");
                showZones();
                cout << "\n  Enter zone name (e.g. zone-a): ";
                string input;
                getline(cin, input);
                displayZoneContents(fixInput(input));
                break;
            }

            case 4:
            {
                printHeader("VIEW AISLE CONTENTS");
                cout << "\n  Available aisles:\n\n";
                TreeNode* zone = (root ? root->left : nullptr);
                while (zone != nullptr)
                {
                    cout << "  [" << zone->name << "]\n";
                    TreeNode* aisle = zone->left;
                    while (aisle != nullptr)
                    {
                        cout << "    -> " << aisle->name << "\n";
                        aisle = aisle->right;
                    }
                    zone = zone->right;
                }
                cout << "\n  Enter aisle name (e.g. aisle-b2): ";
                string input;
                getline(cin, input);
                displayAisleContents(fixInput(input));
                break;
            }

            case 5:
            {
                printHeader("BFS - FIND SHORTEST PATH TO A SHELF");
                cout << "\n";
                cout << "  BFS explores level by level:\n";
                cout << "  Level 0 -> Warehouse\n";
                cout << "  Level 1 -> Zones\n";
                cout << "  Level 2 -> Aisles\n";
                cout << "  Level 3 -> Shelves (target)\n";
                cout << "\n  First match found = shortest path.\n";

                showAllLocations();

                cout << "  Enter shelf name (e.g. shelf-b2-01): ";
                string input;
                getline(cin, input);
                string target = fixInput(input);

                PathResult res = findPath(target);

                if (res.found)
                {
                    cout << "\n  Shortest path to " << target << ":\n\n";
                    res.display();
                    cout << "\n";
                    cout << "  Zone  : " << res.getZone()  << "\n";
                    cout << "  Aisle : " << res.getAisle() << "\n";
                }
                else
                {
                    cout << "\n  Not found. Please enter a valid shelf name.\n";
                }
                printLine();
                break;
            }

            case 6:
            {
                cout << "\n";
                cout << "  DFS goes deep into one branch before\n";
                cout << "  moving on to the next branch.\n\n";
                cout << "  Pre-order: parent is visited before its children.\n";
                cout << "  Zone prints before its Aisles,\n";
                cout << "  Aisle prints before its Shelves.\n\n";
                preOrderTraversal();
                break;
            }

            case 0:
                cout << "\n  Exiting Warehouse Layout Module.\n";
                break;

            default:
                cout << "\n  Invalid choice. Please enter 0 to 6.\n";
                break;
        }

    } while (choice != 0);
}


