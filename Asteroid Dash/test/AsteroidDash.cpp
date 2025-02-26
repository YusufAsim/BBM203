#include "AsteroidDash.h"
#include <fstream>
#include <iostream>
#include <sstream>

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
            CelestialObject *current = celestial_objects.at(i - 1);
            CelestialObject *next = nullptr;
            // connecting the rotations to next_celestial_object
            while (current->right_rotation != celestial_objects.at(i - 1))
            {
                next = current->right_rotation;
                next->next_celestial_object = celestial_objects.at(i);
                current = next;
            }
            current = celestial_objects.at(i - 1);
            next = nullptr;
            while (current->left_rotation != celestial_objects.at(i - 1))
            {
                next = current->left_rotation;
                next->next_celestial_object = celestial_objects.at(i);
                current = next;
            }
            {
            }
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

    CelestialObject *current_celestial = celestial_objects_list_head;
    while (current_celestial != nullptr)
    {
        if (current_celestial->shape.size() == 0)
        {
            current_celestial = current_celestial->next_celestial_object;
            continue;
        }

        if (current_celestial->time_of_appearance == game_time)
        {
            current_celestial->top_right_column = space_grid[0].size() - 1;
            int temp = current_celestial->top_right_column;
            for (int i = 0; i < current_celestial->shape.size(); i++)
            {
                for (int j = 0; j < current_celestial->shape[0].size(); j++)
                {
                    if (space_grid[0].size() - temp - 1 == j)
                    {
                        space_grid[i + current_celestial->starting_row][temp] = current_celestial->shape.at(i).at(j);
                    }
                }
            }
            current_celestial->top_right_column--; // decreasing the pointer to the left
        }
        else if (current_celestial->time_of_appearance < game_time)
        {
            // first control the aimed movement whether colliding with the player
            if (CollisionDetect(current_celestial, player))
            {
                current_celestial->shape.clear();
            }
            else
            {
                for (int i = 0; i < current_celestial->shape.size(); i++)
                {
                    for (int j = 0; j < current_celestial->shape[0].size(); j++)
                    {
                        int target_column_shape = current_celestial->top_right_column + j;
                        if (target_column_shape < space_grid[0].size() && i + current_celestial->starting_row < space_grid.size())
                        {
                            space_grid[i + current_celestial->starting_row][target_column_shape] = current_celestial->shape.at(i).at(j);
                        }
                    }
                }
                current_celestial->top_right_column--;
            }
        }
        current_celestial = current_celestial->next_celestial_object;
    }

    CelestialObject *current = bullet_head;
    while (current != nullptr)
    {
        int count = 0;
        for (int i = current->starting_row; i < current->starting_row + current->shape.size(); i++)
        {
            for (int j = current->top_right_column; j < current->top_right_column + current->shape[0].size(); j++)
            {
                if (i >= space_grid.size() || j >= space_grid[0].size())
                {
                    break;
                }
                space_grid[i][j] = current->shape[count / current->shape.size()][count % current->shape[0].size()];
                count++;
            }
        }
        current->top_right_column++;
        current = current->next_celestial_object;
    }

    for (int i = 0; i < player->spacecraft_shape.size(); i++)
    {
        for (int j = 0; j < player->spacecraft_shape[0].size(); j++)
        {
            if (space_grid.at(i + player->position_row).at(j + player->position_col) == 0)
            {
                space_grid.at(i + player->position_row).at(j + player->position_col) = player->spacecraft_shape.at(i).at(j);
            }
        }
    }
}
void AsteroidDash::shootingUpdate(CelestialObject *head_of_projectiles, CelestialObject *obj)
{
    // traversing the linked list and update the positions of the bullets
    CelestialObject *current_bullet = head_of_projectiles;
    while (current_bullet != nullptr)
    {
        // if the bullet hit the obj then delete the bullet and the hitted object' one block
        for (int i = 0; i < obj->shape.size(); i++)
        {
            for (int j = 0; j < obj->shape[0].size(); j++)
            {
                int target_column_shape = obj->top_right_column + j;
                if (target_column_shape < space_grid[0].size())
                {
                    if (i + obj->starting_row == current_bullet->starting_row && target_column_shape == current_bullet->top_right_column && obj->shape[i][j] == 1)
                    {
                        obj->shape[i][j] = 0;
                        current_bullet->shape[0][0] = 0;
                    }
                }
            }
        }
        current_bullet = current_bullet->next_celestial_object;
    }
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot()
{
    // TODO: Your code here
    if (player->current_ammo > 0)
    {
        player->current_ammo--;
        int center_row = player->position_row + player->spacecraft_shape.size() / 2;
        if (bullet_head == nullptr)
        {
            bullet_head = new CelestialObject({{1}}, AMMO, center_row, 1);
            bullet_head->top_right_column = player->position_col + player->spacecraft_shape[0].size();
        }
        else
        {
            CelestialObject *new_bullet = new CelestialObject({{1}}, AMMO, center_row, 1);
            new_bullet->top_right_column = player->position_col + player->spacecraft_shape[0].size();
            // add the last of the linked list
            CelestialObject *current = bullet_head;
            while (current->next_celestial_object != nullptr)
            {
                current = current->next_celestial_object;
            }
            current->next_celestial_object = new_bullet;
        }
    }
}

bool AsteroidDash::CollisionDetect(CelestialObject *obj, Player *player)
{

    for (int i = 0; i < obj->shape.size(); i++)
    {
        for (int j = 0; j < obj->shape[0].size(); j++)
        {
            int target_column_shape = obj->top_right_column + j;
            if (target_column_shape < space_grid[0].size())
            {
                for (int k = 0; k < player->spacecraft_shape.size(); k++)
                {
                    for (int m = 0; m < player->spacecraft_shape[0].size(); m++)
                    {

                        if (k + player->position_row == i + obj->starting_row && m + player->position_col == target_column_shape && player->spacecraft_shape[k][m] == 1 && obj->shape[i][j] == 1)
                        {
                            if (obj->object_type == LIFE_UP)
                            {
                                player->lives++;
                            }
                            else if (obj->object_type == AMMO)
                            {
                                player->current_ammo = player->max_ammo;
                            }
                            else
                            {
                                player->lives--;
                                if (player->lives == 0)
                                {
                                    game_over = true;
                                }
                            }
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash()
{
    // TODO: Your code here

    delete player;

    // deleting celestial objects
    CelestialObject *temp = celestial_objects_list_head;
    CelestialObject *nextnode = nullptr;

    while (temp = nullptr)
    {
        nextnode = temp->next_celestial_object;
        delete temp;
        temp = nextnode;
    }

    // deleting space_grid
    for (int i = 0; i < space_grid.size(); i++)
    {
        space_grid[i].clear();
    }
    space_grid.clear();

    // deleting bullets
    CelestialObject *tempBullet = bullet_head;
    CelestialObject *nextBullet = nullptr;
    while (tempBullet != nullptr)
    {
        nextBullet = tempBullet->next_celestial_object;
        delete tempBullet;
        tempBullet = nextBullet;
    }
}