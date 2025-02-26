#include "GameWorld.h"
#include "RealmShapers.h"
#include "Map.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map &GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree &GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;
    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
    if (shaperTree.findPlayer(realmShaper->getName()) == nullptr)
    {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }
    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalShaperTreeHeight = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();
    int isleDepth = mapTree.getIsleDepth(isle);
    int minMapDepthAccess = mapTree.calculateMinMapDepthAccess(playerDepth, totalShaperTreeHeight, totalMapDepth);
    if (minMapDepthAccess <= isleDepth)
    {
        hasAccess = true;
    }
    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle,
    // collect item,
    // check overcrowding for Isle,
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
    if (hasAccess(realmShaper, isle))
    {
        for (int j = 0; j < (int)realmShaper->getVisitedIsles().size(); j++)
        {
            if (realmShaper->getVisitedIsles()[j] == isle->getName())
            {
                realmShaper->collectItem(isle->getItem());
                std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
                std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
                return;
            }
        }
        if (isle->increaseShaperCount())
        {
            realmShaper->getVisitedIsles().push_back(isle->getName());
            realmShaper->collectItem(isle->getItem());
            std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
            std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
            std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;
            mapTree.remove(isle);
        }
        else
        {
            realmShaper->getVisitedIsles().push_back(isle->getName());
            realmShaper->collectItem(isle->getItem());
            std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
            std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
        }
    }
    else
    {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    }
}
void GameWorld::setSecondItemDrop(bool seconditemdrop)
{
    this->seconditemdrop = seconditemdrop;
}

bool GameWorld::getSecondItemDrop()
{
    return seconditemdrop;
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName)
{
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
    if (shaper->hasEnoughEnergy())
    {
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        Isle *isle = new Isle(isleName);
        mapTree.insert(isle);
        if (seconditemdrop)
        {
            mapTree.populateWithItems();
            mapTree.initialized = true;
            mapTree.dropItemBFS();
        }
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    }
    else
    {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}

void GameWorld::displayGameState()
{
    /* mapTree.displayMap();
    shaperTree.displayTree(); */
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();

    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);
    std::vector<std::string> accessLogsVector;
    std::vector<std::string> duelLogsVector;
    std::string line;
    while (std::getline(accessFile, line))
    {
        accessLogsVector.push_back(line);
    }
    while (std::getline(duelFile, line))
    {
        duelLogsVector.push_back(line);
    }
    // clear the first line in both files
    accessLogsVector.erase(accessLogsVector.begin());
    duelLogsVector.erase(duelLogsVector.begin());
    int duelCount = 0;
    for (int i = 0; i < (int)accessLogsVector.size(); i++)
    {
        if (i == (int)accessLogsVector.size() - 1)
        {
            setSecondItemDrop(true);
        }
        std::string accessLog = accessLogsVector[i];
        std::string duelLog = duelLogsVector[duelCount];
        std::string shaperName = accessLog.substr(0, accessLog.find(" "));
        std::string isleName = accessLog.substr(accessLog.find(" ") + 1);
        RealmShaper *shaper = shaperTree.findPlayer(shaperName);
        Isle *isle = mapTree.findIsle(isleName);
        if (isle != nullptr)
        {
            exploreArea(shaper, isle);
        }
        else
        {
            craft(shaper, isleName);
        }
        if ((i + 1) % 5 == 0)
        {
            std::string dual_offered_from = duelLog.substr(0, duelLog.find(" "));
            int duelResult = std::stoi(duelLog.substr(duelLog.find(" ") + 1));
            RealmShaper *shaper_attack = shaperTree.findPlayer(dual_offered_from);
            RealmShaper *shaper_defend = shaperTree.getParent(shaper_attack);
            if (shaper_attack == nullptr || shaper_defend == nullptr)
            {
                continue;
            }
            shaperTree.duel(shaper_attack, duelResult);
            if (duelResult)
            {
                for (int j = 0; j < (int)shaper_attack->getVisitedIsles().size(); j++)
                {
                    if (!hasAccess(shaper_attack, mapTree.findIsle(shaper_attack->getVisitedIsles()[j])))
                    {
                        mapTree.findIsle(shaper_attack->getVisitedIsles()[j])->decreaseShaperCount();
                        shaper_attack->getVisitedIsles().erase(shaper_attack->getVisitedIsles().begin() + j);
                    }
                }

                for (int j = 0; j < (int)shaper_defend->getVisitedIsles().size(); j++)
                {
                    if (!hasAccess(shaper_defend, mapTree.findIsle(shaper_defend->getVisitedIsles()[j])))
                    {
                        mapTree.findIsle(shaper_defend->getVisitedIsles()[j])->decreaseShaperCount();
                        shaper_defend->getVisitedIsles().erase(shaper_defend->getVisitedIsles().begin() + j);
                    }
                }
            }
            duelCount++;
        }
    }
    duelLogsVector.erase(duelLogsVector.begin(), duelLogsVector.begin() + duelCount);
    for (int i = 0; i < (int)duelLogsVector.size(); i++)
    {
        std::string duelLog = duelLogsVector[i];
        std::string dual_offered_from = duelLog.substr(0, duelLog.find(" "));
        int duelResult = std::stoi(duelLog.substr(duelLog.find(" ") + 1));
        RealmShaper *shaper_attack = shaperTree.findPlayer(dual_offered_from);
        RealmShaper *shaper_defend = shaperTree.getParent(shaper_attack);
        if (shaper_attack == nullptr || shaper_defend == nullptr)
        {
            continue;
        }
        shaperTree.duel(shaper_attack, duelResult);
    }
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
    displayGameState();
}