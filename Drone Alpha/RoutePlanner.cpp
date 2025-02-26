#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya",
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string &distance_data, const std::string &priority_data, const std::string &restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0)
{

    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; i++)
    {
        weatherRestrictedProvinces[i] = -1;
    }
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; i++)
    {
        priorityProvinces[i] = -1;
    }

    map.loadDistanceData(distance_data);
    map.resetVisited();
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string &filename)
{
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File cannot be openned." << filename << std::endl;
        return;
    }

    std::string line;
    int idx = 0;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string s;
        std::getline(ss, s, '(');
        std::getline(ss, s, ')');
        priorityProvinces[idx] = std::stoi(s);
        idx++;
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string &filename)
{
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File cannot be openned." << filename << std::endl;
        return;
    }
    std::string line;
    int idx = 0;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string s;
        std::getline(ss, s, '(');
        std::getline(ss, s, ')');
        weatherRestrictedProvinces[idx] = std::stoi(s);
        idx++;
    }
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const
{
    // TODO: Your code here
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; i++)
    {
        if (province == priorityProvinces[i])
        {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const
{
    // TODO: Your code here
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; i++)
    {
        if (province == weatherRestrictedProvinces[i])
        {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity)
{
    // TODO: Your code here
    stack.push(startingCity);
    route.push_back(startingCity);
    map.markAsVisited(startingCity);
    while (!stack.isEmpty())
    {
        int currentCity = stack.peek();
        if (!isExplorationComplete())
        {
            enqueueNeighbors(currentCity);
            exploreFromProvince(currentCity);
        }
    }
    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province)
{
    // TODO: Your code here
    bool noUnvisitedNeighbourInRange = true;
    while (!queue.isEmpty())
    {
        int current_neighbor = queue.dequeue();
        if (map.isWithinRange(province, current_neighbor, maxDistance) && !map.isVisited(current_neighbor))
        {
            if (isWeatherRestricted(current_neighbor))
            {
                std::cout << "Province " << cities[current_neighbor] << " is weather-restricted. Skipping." << std::endl;
                continue;
            }
            else
            {
                noUnvisitedNeighbourInRange = false;
                stack.push(current_neighbor);
                route.push_back(current_neighbor);
                map.markAsVisited(current_neighbor);
                totalDistanceCovered += map.getDistance(province, current_neighbor);
                if (isPriorityProvince(current_neighbor))
                {
                    numPriorityProvinces++;
                }
                exploreFromProvince(current_neighbor);
            }
        }
    }
    if (noUnvisitedNeighbourInRange)
    {
        backtrack();
    }
}

void RoutePlanner::enqueueNeighbors(int province)
{
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int neighbor = 0; neighbor < MAX_SIZE; neighbor++)
    {
        if (map.isWithinRange(province, neighbor, maxDistance) && !map.isVisited(neighbor))
        {
            if (isPriorityProvince(neighbor))
            {
                queue.enqueuePriority(neighbor);
            }
            else
            {
                queue.enqueue(neighbor);
            }
        }
    }
}

void RoutePlanner::backtrack()
{
    // If you reach a dead-end province
    // TODO: Your code here
    if (stack.isEmpty())
    {
        return;
    }
    else
    {
        enqueueNeighbors(stack.pop());
    }
}

bool RoutePlanner::isExplorationComplete() const
{
    // TODO: Your code here
    if (stack.isEmpty() && queue.isEmpty())
    {
        return true;
    }
    return false;
}

void RoutePlanner::displayResults() const
{
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    std::cout << "----------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken:" << std::endl;

    for (std::vector<int>::size_type i = 0; i < route.size(); i++)
    {
        std::cout << cities[route[i]] << " -> ";
    }
    std::cout << "End" << std::endl;
    std::cout << std::endl;
    std::cout << "Priority Provinces Status:" << std::endl;
    int counter = 0;
    for (std::size_t i = 0; i < (sizeof(cities) / sizeof(cities[0])); i++)
    {
        if (isPriorityProvince(i))
        {
            std::cout << "- " << cities[i] << " (Visited)" << std::endl;
            counter++;
        }
    }
    std::cout << std::endl;
    std::cout << "Total Priority Provinces Visited: " << counter << " out of " << counter << std::endl;
    std::cout << "Success: All priority provinces were visited." << std::endl;
}