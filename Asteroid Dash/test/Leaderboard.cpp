#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename)
{
    // TODO: Your code here
}

// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename)
{
    // TODO: Your code here
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard()
{
    // TODO: Your code here
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry)
{
    // TODO: Your code here

    // the first entry should be the greatest score

    if (head_leaderboard_entry == nullptr)
    {
        head_leaderboard_entry = new_entry;
    }
    LeaderboardEntry *current = head_leaderboard_entry;
    LeaderboardEntry *prev = nullptr;
    int number_of_entries = 0;
    while (current != nullptr)
    {
        if (new_entry->score > current->score)
        {
            if (prev == nullptr)
            {
                new_entry->next = current;
                head_leaderboard_entry = new_entry;
            }
            else
            {
                prev->next = new_entry;
                new_entry->next = current;
            }
            break;
        }
        prev = current;
        number_of_entries++;
        current = current->next;
    }

    // if the number of entries is greater than 10
    if (number_of_entries > 10)
    {
        LeaderboardEntry *current = head_leaderboard_entry;
        LeaderboardEntry *prev = nullptr;
        while (current != nullptr)
        {
            if (current->next == nullptr)
            {
                prev->next = nullptr;
                delete current;
                break;
            }
            prev = current;
            current = current->next;
        }
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard()
{
    // TODO: Your code here
    LeaderboardEntry *current = head_leaderboard_entry;
    LeaderboardEntry *next = nullptr;
    while (current != nullptr)
    {
        next = current->next;
        delete current;
        current = next;
    }
}
