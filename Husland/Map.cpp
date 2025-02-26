#include "Map.h"
#include <algorithm>
#include <queue>
#include <stack>

void freeMap(MapNode *node)
{
    if (node != nullptr)
    {
        freeMap(node->left);
        freeMap(node->right);
        delete node;
    }
}

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    freeMap(root);
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items

    for (int i = 0; i < (int)isles.size(); i++)
    {
        insert(isles[i]);
    }
    populateWithItems();
    rebalanceCount = 0;
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (findNode(*(current->isle)) == nullptr)
    {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    }
    else
    {
        MapNode *temp = current->left;
        current->left = temp->right;
        temp->right = current;
        current->height = height(current);
        temp->height = height(temp);
        return temp;
    }
    return nullptr;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (findNode(*(current->isle)) == nullptr)
    {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    }
    else
    {
        MapNode *temp = current->right;
        current->right = temp->left;
        temp->left = current;
        current->height = height(current);
        temp->height = height(temp);
        return temp;
    }
    return nullptr;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node

    // accept nullptr as -1
    if (node == nullptr)
    {
        return -1;
    }
    if (node->left == nullptr && node->right == nullptr)
    {
        return 0;
    }
    else
    {
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        return std::max(leftHeight, rightHeight) + 1;
    }
}

int Map::getBalance(MapNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return height(node->left) - height(node->right);
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    if (node == nullptr)
    {
        return new MapNode(isle);
    }

    MapNode *newNode = nullptr;

    if (isle->getName() < node->isle->getName())
    {
        newNode = insert(node->left, isle);
        node->left = newNode;
    }
    else if (isle->getName() > node->isle->getName())
    {
        newNode = insert(node->right, isle);
        node->right = newNode;
    }
    else
    {
        return node;
    }

    node->height = height(node);

    int balance = getBalance(node);

    if (balance > 1 && isle->getName() < node->left->isle->getName())
    {
        rebalanceCount++;
        dropItemBFS();
        return rotateRight(node);
    }

    if (balance < -1 && isle->getName() > node->right->isle->getName())
    {
        rebalanceCount++;
        dropItemBFS();
        return rotateLeft(node);
    }

    if (balance > 1 && isle->getName() > node->left->isle->getName())
    {
        rebalanceCount++;
        dropItemBFS();
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && isle->getName() < node->right->isle->getName())
    {
        rebalanceCount++;
        dropItemBFS();
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    node->height = height(node);

    return node;
}

void Map::insert(Isle *isle)
{
    if (root == nullptr)
    {
        root = new MapNode(isle);
        root->height = height(root);
        return;
    }
    root = insert(root, isle);
    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{

    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;

    if (root == nullptr)
    {
        std::cout << "[Remove] " << "Tree is Empty" << std::endl;
        return nullptr;
    }

    if (isle->getName() < node->isle->getName())
    {
        node->left = remove(node->left, isle);
    }
    else if (isle->getName() > node->isle->getName())
    {
        node->right = remove(node->right, isle);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            node = nullptr;
        }
        else if (node->right == nullptr)
        {
            MapNode *temp = node->left;
            if (temp->isle != nullptr)
            {
                node->isle = new Isle(*temp->isle);
            }
            else
            {
                node->isle = nullptr;
            }
            delete temp;
            node->left = nullptr;
            temp = nullptr;
        }
        else if (node->left == nullptr)
        {
            MapNode *temp = node->right;
            if (temp->isle != nullptr)
            {
                node->isle = new Isle(*temp->isle);
            }
            else
            {
                node->isle = nullptr;
            }
            delete temp;
            node->right = nullptr;
            temp = nullptr;
        }
        else
        {
            MapNode *temp = node->left;
            while (temp->right != nullptr)
            {
                temp = temp->right;
            }
            if (temp->isle != nullptr)
            {
                node->isle = new Isle(*temp->isle);
            }
            else
            {
                node->isle = nullptr;
            }
            node->left = remove(node->left, temp->isle);
        }
    }

    if (node == nullptr)
    {
        return node;
    }

    node->height = height(node);
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
    {
        rebalanceCount++;
        dropItemBFS();
        return rotateRight(node);
    }
    if (balance > 1 && getBalance(node->left) < 0)
    {
        rebalanceCount++;
        dropItemBFS();
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0)
    {
        rebalanceCount++;
        dropItemBFS();
        return rotateLeft(node);
    }
    if (balance < -1 && getBalance(node->right) > 0)
    {
        rebalanceCount++;
        dropItemBFS();
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}
bool isInTree(MapNode *node, Isle *isle)
{
    if (node == nullptr)
    {
        return false;
    }
    if (*(node->isle) == *isle)
    {
        return true;
    }
    return isInTree(node->left, isle) || isInTree(node->right, isle);
}
void Map::remove(Isle *isle)
{
    // be sure that isle is in the tree

    if (!isInTree(root, isle))
    {
        return;
    }

    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr)
    {
        return;
    }

    count++;
    if (count % 5 == 0)
    {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (current == nullptr)
    {
        return;
    }
    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);
    count++;
    if (count % 3 == 0)
    {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item

    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->isle->getItem() == EMPTY)
    {
        return node;
    }
    else
    {
        MapNode *left = findFirstEmptyIsle(node->left);
        MapNode *right = findFirstEmptyIsle(node->right);
        if (left != nullptr)
        {
            return left;
        }
        else if (right != nullptr)
        {
            return right;
        }
    }
    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    if (rebalanceCount % 3 != 0 || !initialized)
    {
        return;
    }
    MapNode *targetNode = findFirstEmptyIsle(root);

    if (targetNode != nullptr)
    {
        targetNode->isle->setItem(AMAZONITE);
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    }
    else
    {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // depth of the root accepted as 0
    // accept nullptr as -1

    if (node == nullptr)
    {
        return -1;
    }
    if (findNode(*(node->isle)) == nullptr)
    {
        return -1;
    }
    MapNode *temp = root;
    int depth = 0;
    while (temp != nullptr)
    {
        if (*(node->isle) == *(temp->isle))
        {
            return depth;
        }
        else if (*(node->isle) > *(temp->isle))
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
        depth++;
    }
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else

    // accept nullptr as -1
    // accept root depth as 0

    if (isle == nullptr)
    {
        return -1;
    }
    MapNode *isleTemp = findNode(*isle);
    if (isleTemp == nullptr)
    {
        return -1;
    }
    return getDepth(isleTemp);
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    return height(root);
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM

    int count = 0;
    postOrderItemDrop(root, count);
    count = 0;
    preOrderItemDrop(root, count);
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    MapNode *current = root;
    while (current != nullptr)
    {
        if (isle == *(current->isle))
        {
            return current->isle;
        }
        else if (isle > *(current->isle))
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    MapNode *current = root;
    while (current != nullptr)
    {
        if (current->isle->getName() == name)
        {
            return current->isle;
        }
        else if (current->isle->getName() > name)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode *current = root;
    while (current != nullptr)
    {
        if (isle == *(current->isle))
        {
            return current;
        }
        else if (isle > *(current->isle))
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode *current = root;
    while (current != nullptr)
    {
        if (current->isle->getName() == name)
        {
            return current;
        }
        else if (current->isle->getName() > name)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File could not be opened!" << std::endl;
        return;
    }
    std::queue<MapNode *> q;
    q.push(root);
    bool is_nulls_flag = true;
    while (!q.empty() && is_nulls_flag)
    {
        is_nulls_flag = false;
        bool is_there_null_currentL = false;
        int level_size = q.size();
        std::string tree_level_str;
        for (int i = 0; i < level_size; ++i)
        {
            MapNode *temp = q.front();
            q.pop();

            if (temp != nullptr)
            {
                tree_level_str += temp->isle->getName() + " ";
                q.push(temp->left);
                q.push(temp->right);

                is_nulls_flag = true;
                is_there_null_currentL = true;
            }
            else
            {
                tree_level_str += "NULL ";
            }
        }

        if (is_there_null_currentL)
        {
            file << tree_level_str << std::endl;
        }
    }
    file.close();
}

void inOrderTraversal(MapNode *current, std::ofstream &file)
{
    if (current == nullptr)
    {
        return;
    }
    inOrderTraversal(current->left, file);
    file << current->isle->getName() << std::endl;
    inOrderTraversal(current->right, file);
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File could not be opened!" << std::endl;
    }
    else
    {
        // inorder successortraversal
        inOrderTraversal(root, file);
        std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
        file.close();
    }
}