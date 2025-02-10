#ifndef SYSTEM_COMMANDS_H // Header guard mechanism: To ensure contents of SYSTEM_COMMANDS_H are included only once in the .cpp file
#define SYSTEM_COMMANDS_H // Let us define SYSTEM_COMMANDS_H. The contents of SYSTEM_COMMANDS_H go here:

#include <string>

class SystemCommands
{
public:
    void listProcesses() const;
    void listUsers() const;
    void listFiles(const std::string &path) const;
};

#endif // SYSTEM_COMMANDS_H
