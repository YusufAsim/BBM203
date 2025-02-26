#include "CelestialObject.h"
#include <iostream>
using namespace std;
// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
    : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance)
{
    next_celestial_object = nullptr;
    createCircularLinkedList();
}

void CelestialObject::createCircularLinkedList()
{
    CelestialObject *current = this;
    CelestialObject *next = nullptr;

    while (!checkMatrixEquality(current->Rotate_right(), this->shape))
    {
        next = new CelestialObject(current);
        next->shape = next->Rotate_right();
        current->right_rotation = next;
        next->left_rotation = current;
        current = next;
    }

    current->right_rotation = this;
    this->left_rotation = current;
}

bool CelestialObject::checkMatrixEquality(std::vector<std::vector<bool>> matrix1, std::vector<std::vector<bool>> matrix2)
{
    if (matrix1.size() != matrix2.size())
    {
        return false;
    }
    for (int i = 0; i < matrix1.size(); i++)
    {
        if (matrix1[i].size() != matrix2[i].size())
        {
            return false;
        }
        for (int j = 0; j < matrix1[i].size(); j++)
        {
            if (matrix1[i][j] != matrix2[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

vector<vector<bool>> CelestialObject::Rotate_right()
{

    int rows = this->shape.size();
    int cols = this->shape[0].size();

    vector<vector<bool>> temp(cols, vector<bool>(rows));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp[j][rows - 1 - i] = this->shape[i][j];
        }
    }

    return temp;
}

vector<vector<bool>> CelestialObject::Rotate_left(vector<vector<bool>> matrix)
{

    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<vector<bool>> temp(cols, vector<bool>(rows));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp[cols - 1 - j][i] = this->shape[i][j];
        }
    }

    return temp;
}

// Default constructor for CelestialObject
CelestialObject::CelestialObject() {};

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
    : shape(other->shape),                          // Copy the 2D vector shape
      object_type(other->object_type),              // Copy the object type
      starting_row(other->starting_row),            // Copy the starting row
      time_of_appearance(other->time_of_appearance) // Copy the time of appearance
{
    // TODO: Your code here
    next_celestial_object = nullptr;
    /*     CelestialObject *temp(this);
        std::vector<std::vector<bool>> current_shape = this->shape;
        CelestialObject *last_of_the_list = nullptr;
        CelestialObject *first(this);
        do
        {
            CelestialObject *currentObject = new CelestialObject(*temp);
            currentObject->shape = currentObject->Rotate_left(temp->shape);
            current_shape = currentObject->shape;

            temp->left_rotation = currentObject;  // set the left rotation of the current object
            currentObject->right_rotation = temp; // set the right rotation of the rotated object
            last_of_the_list = temp;
            temp = temp->left_rotation; // move to the next object
        } while (!checkMatrixEquality(current_shape, this->shape));
        last_of_the_list->left_rotation = first;  // set the left rotation of the last object
        first->right_rotation = last_of_the_list; // set the right rotation of the first object
        delete temp; */
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target)
{

    if (target->left_rotation == nullptr && target->right_rotation == nullptr)
    {
        return;
    }

    CelestialObject *current = target->right_rotation;
    CelestialObject *next_node = nullptr;

    while (current != target)
    {
        next_node = current->right_rotation;
        delete current;
        current = next_node;
    }
    target->right_rotation = target;
    target->left_rotation = target;

    // print the linked list
    /*     CelestialObject *temp = target;
        do
        {
            temp->visualizematrix(temp->shape);
            std::cout << "-----------------" << std::endl;
            temp = temp->right_rotation;
        } while (temp != target); */
}

void CelestialObject::visualizematrix(std::vector<std::vector<bool>> matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (matrix[i][j])
            {
                cout << "▒▒";
            }
            else
            {
                cout << "██";
            }
        }
        cout << endl;
    }

    cout << endl;
}