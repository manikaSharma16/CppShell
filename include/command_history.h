#ifndef COMMAND_HISTORY_H
#define COMMAND_HISTORY_H

#include <vector>
#include <string>

#define MAX_CMD_HISTORY_SIZE 10

using namespace std;

class CommandHistory
{
private:
    vector<string> history; // Vector to store command history

public:
    void add(const string &command); // Add command to history vector
    void print() const; // Print the last 10 command history
    string getCommand(int index) const; // Return command of index
    bool empty() const; // Check if command history is empty
    string lastCommand() const; // Return the last command
};

#endif // COMMAND_HISTORY_H
