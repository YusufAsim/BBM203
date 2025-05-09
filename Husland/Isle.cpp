#include "Isle.h"

Isle::Isle(std::string name) : name(name)
{
}

const std::string &Isle::getName() const
{
    return this->name;
}
Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item item)
{
    this->item = item;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    bool isFull = false;

    // TODO: Increase shaperCount if necessary
    // return isFull, True if capacity is exceded, false otherwise
    if (shaperCount < capacity)
    {
        shaperCount++;
    }
    else
    {
        isFull = true;
    }

    return isFull;
}

bool Isle::decreaseShaperCount()
{
    bool isEmpty = true;

    // TODO: Decrease shaperCount if necessary
    // return isEmpty, True if shaper count less and equal to 0, false otherwise
    if (shaperCount > 0)
    {
        shaperCount--;
        isEmpty = false;
    }

    return isEmpty;
}

bool Isle::operator==(const Isle &other) const
{
    // TODO: Compare by name, return true if same
    if (name == other.name)
    {
        return true;
    }
    return false;
}

bool Isle::operator<(const Isle &other) const
{
    // TODO: Compare by name
    // std::string.compare() use

    if (std::string(this->name).compare(other.name) > 0)
    {
        return true;
    }
    return false;
}

bool Isle::operator>(const Isle &other) const
{
    // TODO: Compare by name
    // std::string.compare() use
    if (std::string(name).compare(other.name) < 0)
    {
        return true;
    }
    return false;
}

// Implementation of readFromFile
std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;
    // TODO: Read isles from the file,
    // add them to vector
    // return the vector
    // Input format: isleName
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File can not be opened!" << std::endl;
        return isles;
    }
    else
    {
        std::string line;
        while (std::getline(file, line))
        {
            Isle *isle = new Isle(line);
            isles.push_back(isle);
        }
    }
    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Prints to terminal with color
    // This function might cause some problems in terminals that are not Linux based
    // If so, comment out the colors while testing on local machine
    // But open them back up while submitting or using TurBo

    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}