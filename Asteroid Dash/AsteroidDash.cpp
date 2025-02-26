#include "AsteroidDash.h"
#include <fstream>
#include <iostream>
#include <sstream>

void AsteroidDash::print(std::string s)
{
    std::cout << s << std::endl;
}

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

    : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard())
{

    read_player(player_file_name, player_name);          // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);               // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name); // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file)
{
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file.is_open())
    {
        std::cerr << "File is not openned" << std::endl;
    }
    else
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<int> temp_row;
            for (char c : line)
            {
                if (c == '0')
                {
                    temp_row.push_back(0);
                }
                else if (c == '1')
                {
                    temp_row.push_back(1);
                }
            }
            space_grid.push_back(temp_row);
        }
    }
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name)
{
    // TODO: Your code here
    std::ifstream file(player_file_name);
    if (!file.is_open())
    {
        std::cerr << "player file is not openned" << std::endl;
    }
    else
    {
        std::string line;
        bool is_coordinate_line = true;
        int row, col;
        vector<vector<bool>> space_craft_shape;
        while (std::getline(file, line))
        {
            std::stringstream line_stream(line);
            if (is_coordinate_line)
            {
                line_stream >> row >> col;
                is_coordinate_line = false;
            }
            else
            {

                vector<bool> temp_row;
                for (char c : line)
                {
                    if (c == '1')
                    {
                        temp_row.push_back(true);
                    }
                    else if (c == '0')
                    {
                        temp_row.push_back(false);
                    }
                    else
                    {
                        continue;
                    }
                }
                space_craft_shape.push_back(temp_row);
            }
        }
        this->player = new Player(space_craft_shape, row, col, player_name);
    }
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file)
{
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file.is_open())
    {
        std::cerr << "File is not openned" << std::endl;
        return;
    }

    struct objectInfos
    {
        ObjectType type = ASTEROID;
        std::vector<std::vector<bool>> shape;
        int starting_row;
        int time_of_appearance;
        std::string effect_of_power;
    };

    std::string line;
    std::vector<CelestialObject *> celestial_objects;
    objectInfos current_object_infos;
    bool reading_matrix = false;
    current_object_infos = objectInfos();
    while (std::getline(file, line))
    {
        // end of current object
        if (line.empty())
        {
            if (!current_object_infos.shape.empty())
            {
                CelestialObject *current_object = new CelestialObject(current_object_infos.shape, current_object_infos.type, current_object_infos.starting_row, current_object_infos.time_of_appearance);
                celestial_objects.push_back(current_object);
                current_object_infos = objectInfos();
            }
            reading_matrix = false;
        }
        else if (line.at(0) == '[' || line.at(0) == '{')
        {
            reading_matrix = true;
            if (line.at(line.size() - 1) == ']' || line.at(line.size() - 1) == '}')
            {
                std::vector<bool> temp_row;
                for (char c : line)
                {
                    if (c == '0')
                    {
                        temp_row.push_back(0);
                    }
                    else if (c == '1')
                    {
                        temp_row.push_back(1);
                    }
                }
                current_object_infos.shape.push_back(temp_row);
                reading_matrix = false;
                continue;
            }
            std::vector<bool> temp_row;
            for (char c : line)
            {
                if (c == '0')
                {
                    temp_row.push_back(0);
                }
                else if (c == '1')
                {
                    temp_row.push_back(1);
                }
            }
            current_object_infos.shape.push_back(temp_row);
        }
        else if (line.at(line.size() - 1) == ']' || line.at(line.size() - 1) == '}')
        {
            std::vector<bool> temp_row;
            for (char c : line)
            {
                if (c == '0')
                {
                    temp_row.push_back(0);
                }
                else if (c == '1')
                {
                    temp_row.push_back(1);
                }
            }
            current_object_infos.shape.push_back(temp_row);
            reading_matrix = false;
        }
        else if (reading_matrix)
        {
            std::vector<bool> temp_row;
            for (char c : line)
            {
                if (c == '0')
                {
                    temp_row.push_back(0);
                }
                else if (c == '1')
                {
                    temp_row.push_back(1);
                }
            }
            current_object_infos.shape.push_back(temp_row);
        }
        else if (line.at(0) == 's')
        {
            current_object_infos.starting_row = std::stoi(line.substr(2));
        }
        else if (line.at(0) == 't')
        {
            current_object_infos.time_of_appearance = std::stoi(line.substr(2));
        }
        else if (line.at(0) == 'e')
        {
            current_object_infos.effect_of_power = line.substr(2);
            if (current_object_infos.effect_of_power == "life")
            {
                current_object_infos.type = LIFE_UP;
            }
            else if (current_object_infos.effect_of_power == "ammo")
            {
                current_object_infos.type = AMMO;
            }
        }
    }

    if (!current_object_infos.shape.empty())
    {
        CelestialObject *current_object = new CelestialObject(current_object_infos.shape, current_object_infos.type, current_object_infos.starting_row, current_object_infos.time_of_appearance);
        celestial_objects.push_back(current_object);
    }

    for (int i = 0; i < celestial_objects.size(); i++)
    {
        if (i == 0)
        {
            celestial_objects_list_head = celestial_objects.at(i);
        }
        else
        {
            celestial_objects.at(i - 1)->next_celestial_object = celestial_objects.at(i);
        }
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const
{
    // TODO: Your code here
    for (int i = 0; i < space_grid.size(); i++)
    {
        for (int j = 0; j < space_grid[0].size(); j++)
        {
            if (space_grid[i][j] == 0)
            {
                std::cout << "▒▒";
            }
            else
            {
                std::cout << "██";
            }
        }
        std::cout << std::endl;
    }
}

// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid()
{
    // TODO: Your code here
    for (int i = 0; i < space_grid.size(); i++)
    {
        for (int j = 0; j < space_grid[0].size(); j++)
        {
            space_grid[i][j] = 0;
        }
    }

    int count = 0;
    for (int i = player->position_row; i < player->position_row + player->spacecraft_shape.size(); i++)
    {
        for (int j = player->position_col; j < player->position_col + player->spacecraft_shape[0].size(); j++)
        {
            space_grid[i][j] = player->spacecraft_shape[count / player->spacecraft_shape.size()][count % (player->spacecraft_shape[0].size())];
            count++;
        }
    }
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot()
{
    // TODO: Your code here
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash()
{
    // TODO: Your code here

    delete player;

    CelestialObject *temp = celestial_objects_list_head;
    CelestialObject *nextnode = nullptr;

    while (temp = nullptr)
    {
        nextnode = temp->next_celestial_object;
        delete temp;
        temp = nextnode;
    }

    // delete space_grid
    for (int i = 0; i < space_grid.size(); i++)
    {
        space_grid[i].clear();
    }
    space_grid.clear();
}

void AsteroidDash::printTheList()
{
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr)
    {
        CelestialObject *startRotation = nullptr;
        CelestialObject *currentRotation = current;
        std::cout << "all rotations of the object" << std::endl;
        while (currentRotation != startRotation)
        {
            if (startRotation == nullptr)
            {
                startRotation = currentRotation;
            }

            vector<vector<bool>> shapeToPrint = currentRotation->shape;
            for (int i = 0; i < shapeToPrint.size(); i++)
            {
                for (int j = 0; j < shapeToPrint.at(0).size(); j++)
                {
                    if (shapeToPrint[i][j] == true)
                    {
                        std::cout << occupiedCellChar;
                    }
                    else
                    {
                        std::cout << unoccupiedCellChar;
                    }
                }
                std::cout << std::endl;
            }
            currentRotation = currentRotation->right_rotation;
            std::cout << std::endl;
        }
        current = current->next_celestial_object;
    }
}
