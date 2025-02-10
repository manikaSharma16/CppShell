#ifndef SHELL_H
#define SHELL_H

#define MAX_CMD_LINE 80         // Max length of command line input
#define MAX_CMD_HISTORY_SIZE 10 // Limit of command history size
#define SHELL_PROMPT "manika> "

#include "command_history.h"
#include "system_commands.h"
#include "custom_commands.h"

#define SHELL_PROMPT "manika> "

enum BuiltInCommand
{
    CMD_EXIT, // Command to exit the shell
    CMD_HISTORY, // Command to view the history of commands
    CMD_CD, // Command to change the current directory
    CMD_ECHO, // Command to echo text
    CMD_UNKNOWN // Default command for unknown commands
};

// Convert string command to an enum value for switch statement
BuiltInCommand getCommandType(const std::string &input);

class Shell
{
private:
    CommandHistory command_history; // Object for command history
    SystemCommands system_commands; // Object for handling system commands
    CustomCommands custom_commands; // Object for handling custom commands
    bool should_run;                // Flag to control shell running state

public:
    Shell(); // Constructor to initialize should_run to true
    void run(); // Main shell loop
    bool handleBuiltinCommands(const std::string &input); // Handle built-in commands
    bool handleHistoryCommand(const std::string &input); // Handle history command
    void executeCommand(const std::string &input); // Execute the given command
    // void onManikaSCommands(const std::string &input); // Execute commands made by me
    // void reverseCommand(const std::string &input);
    // void countdownCommand(int input);  // Declare countdownCommand function
    // void countdownTimer(int input);  // Helper function for countdown in background
    // bool isPalindrome(const std::string &text);  // Function to check if a string is a palindrome
    // void palindromeCommand(const std::string &text);  // Function to handle palindrome command
    // void weatherCommand(const std::string &city); // New function to handle weather command
};

#endif // SHELL_H