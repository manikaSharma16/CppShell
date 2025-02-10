#include "command_history.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>

using namespace std;

// Add command to command history
void CommandHistory::add(const std::string &command)
{
    history.push_back(command);

    if (history.size() > MAX_CMD_HISTORY_SIZE)
    {
        history.erase(history.begin());
    }
}

// Print the command history
void CommandHistory::print() const
{
    int index = 1;
    for (auto it = history.rbegin(); it != history.rend(); ++it)
    {
        cout << index++ << ": " << *it << endl;
    }
}

// Return command at index of history vector
string CommandHistory::getCommand(int index) const
{
    if (index <= 0 || index > (int)history.size())
    {
        throw out_of_range("No such command in history.");
    }

    return history[history.size() - index];
}

// If the history is empty
bool CommandHistory::empty() const
{
    return history.empty();
}

// Get the last executed command from history vector
std::string CommandHistory::lastCommand() const
{
    if (history.empty())
        throw out_of_range("No commands in history.");

    return history.back();
}
