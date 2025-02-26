#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (int i = 0; i < (int)realmShapers.size(); i++)
    {
        if (realmShapers[i] != nullptr)
        {
            delete realmShapers[i];
            realmShapers[i] = nullptr;
        }
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    for (int i = 0; i < (int)shapers.size(); i++)
    {
        insert(shapers[i]);
    }
    shapers.clear();
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if (index >= 0 && index < (int)realmShapers.size())
    {
        isValid = true;
    }

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    int index = findIndex(shaper);
    if (index == -1)
    {
        return -1;
    }
    delete realmShapers[index];
    realmShapers[index] = nullptr;
    realmShapers.erase(realmShapers.begin() + index);
    return index;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (int i = 0; i < (int)realmShapers.size(); i++)
    {
        if (realmShapers[i] == shaper)
        {
            return i;
        }
    }
    return -1;
}

int ShaperTree::calculateHeight(int index)
{

    // accept nullptr as -1
    // leaf node as 0
    if (index >= (int)realmShapers.size())
    {
        return -1;
    }

    int idxLeft = 2 * index + 1;
    int idxRight = 2 * index + 2;

    return 1 + std::max(calculateHeight(idxLeft), calculateHeight(idxRight));
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    // accept nullptr as -1
    // accept root depth as 0
    if (shaper == nullptr)
    {
        return -1;
    }
    int index = findIndex(shaper);
    if (index == -1)
    {
        return -1;
    }
    return std::floor(std::log2(index + 1));
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    return std::floor(std::log2(realmShapers.size()));
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;
    RealmShaper *parent = getParent(realmShapers[findIndex(challenger)]);
    if (result)
    {
        challenger->gainHonour();
        parent->loseHonour();
        replace(parent, challenger);
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
        if (parent->getHonour() <= 0)
        {
            std::cout << "[Duel] " << parent->getName() << " lost all honour, delete" << std::endl;
            remove(parent);
        }
        return *challenger;
    }
    else
    {
        parent->gainHonour();
        challenger->loseHonour();
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
        if (challenger->getHonour() <= 0)
        {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
            remove(challenger);
        }
        return *parent;
    }
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper

    int index = findIndex(shaper);
    int index_parent = (index - 1) / 2;
    if (index_parent < 0)
    {
        return nullptr;
    }
    parent = realmShapers[index_parent];
    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int index_low = findIndex(player_low);
    int index_high = findIndex(player_high);
    RealmShaper *temp = player_low;
    realmShapers[index_low] = player_high;
    realmShapers[index_high] = temp;
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found
    int index = findIndex(&shaper);
    if (index == -1)
    {
        return nullptr;
    }
    foundShaper = realmShapers[index];
    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (int i = 0; i < (int)realmShapers.size(); i++)
    {
        if (realmShapers[i]->getName() == name)
        {
            foundShaper = realmShapers[i];
            return foundShaper;
        }
    }

    return foundShaper;
}

void recursiveInOrderTraversal(int index, std::vector<RealmShaper *> &realmShapers, std::vector<std::string> &result)
{
    if (index >= (int)realmShapers.size())
    {
        return;
    }
    recursiveInOrderTraversal(2 * index + 1, realmShapers, result);
    result.push_back(realmShapers[index]->getName());
    recursiveInOrderTraversal(2 * index + 2, realmShapers, result);
}
std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    recursiveInOrderTraversal(index, realmShapers, result);
    return result;
}

void recursivePreOrderTraversal(int index, std::vector<RealmShaper *> &realmShapers, std::vector<std::string> &result)
{
    if (index >= (int)realmShapers.size())
    {
        return;
    }
    result.push_back(realmShapers[index]->getName());
    recursivePreOrderTraversal(2 * index + 1, realmShapers, result);
    recursivePreOrderTraversal(2 * index + 2, realmShapers, result);
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    recursivePreOrderTraversal(index, realmShapers, result);
    return result;
}

void recursivePostOrderTraversal(int index, std::vector<RealmShaper *> &realmShapers, std::vector<std::string> &result)
{
    if (index >= (int)realmShapers.size())
    {
        return;
    }
    recursivePostOrderTraversal(2 * index + 1, realmShapers, result);
    recursivePostOrderTraversal(2 * index + 2, realmShapers, result);
    result.push_back(realmShapers[index]->getName());
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    recursivePostOrderTraversal(index, realmShapers, result);
    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation
    std::vector<std::string> result = preOrderTraversal(index);
    for (int i = 0; i < (int)result.size(); i++)
    {
        outFile << result[i] << std::endl;
    }
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
    for (int i = 0; i < (int)realmShapers.size(); i++)
    {
        outFile << realmShapers[i]->getName() << std::endl;
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1;  // TODO: Calculate left index
    int right = 2 * index + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    std::ofstream outFile(filename);
    breadthFirstTraversal(outFile);
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    outFile.close();
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
    std::ofstream outFile(filename);
    preOrderTraversal(0, outFile);
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
    outFile.close();
}
