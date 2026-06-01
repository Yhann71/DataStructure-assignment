#ifndef WAREHOUSELAYOUT_HPP
#define WAREHOUSELAYOUT_HPP

#include <string>

const int MAX_PATH  = 10;
const int QUEUE_MAX = 32;


// one node in the binary tree
// left  = first child  (one level down)
// right = next sibling (same level)
class TreeNode
{
public:
    std::string name;
    std::string type;
    TreeNode* left;
    TreeNode* right;

    TreeNode();
    TreeNode(const std::string& n, const std::string& t);
};


// stores the result of a BFS search
// Task 2 reads getZone() for zone assignment
// Task 3 reads steps[] to move the robot
class PathResult
{
public:
    std::string steps[MAX_PATH];
    int stepCount;
    bool found;

    PathResult();

    void display()const;
    std::string getZone()const;
    std::string getAisle()const;
    std::string getShelf()const;
};

// one slot inside the BFS queue
// holds the tree node and the path taken to reach it
class BFSQueueNode
{
public:
    TreeNode*node;
    std::string path[MAX_PATH];
    int pathLen;
    BFSQueueNode*next;

    BFSQueueNode();
    BFSQueueNode(TreeNode* n, const std::string arr[], int len);
};

// self-made linked FIFO queue used by BFS
// no STL allowed so this is built from scratch
class BFSQueue
{
public:
    BFSQueue();
    ~BFSQueue();

    void enqueue(TreeNode* node, const std::string arr[], int len);
    BFSQueueNode*dequeue();
    bool isEmpty() const;

private:
    BFSQueueNode*head;
    BFSQueueNode*tail;
    int count;
};

// main class for Task 5
// builds and manages the binary tree
// contains BFS (path finding) and DFS (traversal)
class WarehouseLayout
{
public:
    WarehouseLayout();
    ~WarehouseLayout();

    void buildWarehouse();

    void displayLayout()const;
    void displayAllShelves()const;
    void displayZoneContents(const std::string& zoneName)const;
    void displayAisleContent(const std::string& aisleName)const;

    PathResult findPath(const std::string& shelfName) const;

    // for Task 3 integration
    // outArray[] = path steps: [0]=Warehouse [1]=Zone [2]=Aisle [3]=Shelf
    // outSize    = how many steps are in the array
    void getPathArray(const std::string& shelfName, std::string outArray[], int& outSize) const;

    void preOrderTraversal()const;

    bool nodeExists(const std::string& name) const;
    TreeNode* findNode(const std::string& name) const;

    void runMenu();

private:
    TreeNode* root;

    void deleteTree(TreeNode* node);
    TreeNode* makeNode(const std::string& name, const std::string& type);

    PathResult bfsSearch(const std::string& target)const;
    void dfsPreOrder(TreeNode* node, int depth)const;
    void printShelvesList(TreeNode* node)const;
    TreeNode*searchNode(TreeNode* node, const std::string& name)const;

    void showAllLocations()const;
    void showZone()const;
    void showAislesInZone (const std::string& zone)const;
    void showShelvesInAisle(const std::string& aisle)const;
    void printTreeView(TreeNode* node, int depth)const;

    std::string fixInput(const std::string& input)const;

    void printHeader(const std::string& title)const;
    void printLine()const;
};

#endif
