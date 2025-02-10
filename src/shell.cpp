#include "shell.h"
#include <iostream>
#include <unistd.h>    // For fork(), execvp(), chdir(), etc.
#include <sys/types.h> // For wait() function
#include <sys/wait.h>
#include <cstring> // For strerror() and string functions
#include <cerrno>
#include <cstdlib> // For system()
#include <errno.h>    // For errno and error handling
#include <sys/stat.h> // For file system functions (e.g., chdir())
#include <string>
#include <sstream> // Include this to use std::istringstream
#include <random>

#include <curl/curl.h> // Includes the cURL library, which is used for network-related tasks (e.g., making HTTP requests)
// #include <nlohmann/json.hpp> // For parsing JSON responses
#include <thread>    // For countdown functionality
#include <algorithm> // For reversing text and palindrome check

#include "command_history.h" // Declare CommandHistory class
#include "system_commands.h" // Declare SystemCommands class

using namespace std; // use namespace std for cout, cerr, etc

// Convert string command to an enum value for switch statement
// getCommandType function, which converts a string command into a corresponding BuiltInCommand enum value
BuiltInCommand getCommandType(const string &input)
{
    if (input == "exit")
        return CMD_EXIT;
    if (input == "history")
        return CMD_HISTORY;
    if (input.substr(0, 3) == "cd ")
        return CMD_CD;
    if (input.substr(0, 4) == "echo")
        return CMD_ECHO;
    return CMD_UNKNOWN; // For unknown or unsupported commands
}

// Constructor
Shell::Shell() : should_run(true) {}

// Run the shell
void Shell::run()
{
    while (should_run)
    {
        cout << SHELL_PROMPT; // Display shell prompt
        string input;
        getline(cin, input); // Get input from the user

        if (input.empty()) // Ignore empty input
            continue;

        command_history.add(input); // Add command to history

        if (handleBuiltinCommands(input)) // If input is a built-in command, directly run
            continue;

        executeCommand(input); // If input is not a built-in command, execute it
    }
}

// Handle built-in shell commands
bool Shell::handleBuiltinCommands(const string &input)
{
    string dir; // Initialize variables here
    string text;

    if (input[0] == '!')
    { // Handle history command (e.g., !! or !<number>)
        return handleHistoryCommand(input);
    }

    // Get the type of the command
    BuiltInCommand cmdType = getCommandType(input);

    switch (cmdType)
    {
    case CMD_EXIT:
        cout << "Exiting the shell...\n";
        should_run = false; // Set should_run to false to stop the shell loop
        return true;

    case CMD_HISTORY:
        command_history.print(); // Print command history
        return true;

    case CMD_CD:
        dir = input.substr(3); // Get directory path after 'cd '
        if (chdir(dir.c_str()) == -1) // Change directory and handle errors
        {
            cerr << "Error: " << strerror(errno) << endl;
        }
        return true;

    case CMD_ECHO:
        text = input.substr(5); // Get text after 'echo'
        cout << text << endl;   // Print the text
        return true;

    default:
        return false; // Return false if it's not a built-in command
    }
}

// Handle commands to retrieve and execute commands from history
bool Shell::handleHistoryCommand(const string &input)
{
    if (command_history.empty())
    {
        cerr << "No commands in history.\n";
        return true;
    }

    string newInput;
    if (input == "!!") // "!!" means repeat the last command
    {
        newInput = command_history.lastCommand();
    }
    else
    {
        try
        {
            int cmd_index = stoi(input.substr(1));            // Extract index after "!"
            newInput = command_history.getCommand(cmd_index); // Get the specific command
        }
        catch (const exception &)
        {
            cerr << "Invalid history command format.\n";
            return true;
        }
    }

    executeCommand(newInput); // Execute the retrieved command
    return true;
}

// Execute a command
void Shell::executeCommand(const string &input)
{
    if (input == "ps")
    { // Handle "ps" command
        system_commands.listProcesses();
        return;
    }
    if (input == "who")
    { // Handle "who" command
        system_commands.listUsers();
        return;
    }
    if (input == "ls")
    { // Handle "ls" command
        system_commands.listFiles("."); // current directory
        return;
    }

    if (input.substr(0, 7) == "reverse")
    {
        custom_commands.reverseCommand(input.substr(8)); // Pass the text after "reverse "
        return;
    }

    if (input.substr(0, 9) == "countdown")
    {
        custom_commands.countdownCommand(stoi(input.substr(10))); // Call countdownCommand with the seconds value
        return;
    }

    if (input.substr(0, 10) == "palindrome")
    {
        custom_commands.palindromeCommand(input.substr(11)); // Call palindromeCommand with the text after "palindrome "
        return;
    }

    // Check if the input is the "weather" command
    if (input.substr(0, 7) == "weather") // If weather command
    {
        custom_commands.weatherCommand(input.substr(8)); // Pass city name (after "weather")
        return;
    }

    // Handle other external commands that are not defined in the shell like date, top, pwd, etc

    // Split the input command into tokens (e.g., command and its arguments): "ls -l -a", the tokens vector will contain ["ls", "-l", "-a"].
    vector<string> tokens; // vector of strings token
    istringstream iss(input); // convert input string into stream
    string token;

    // Add each token in input to the vector tokens
    while (iss >> token)
    {
        tokens.push_back(token);
    }

    // Convert the tokens into a char array for execvp
    char *args[MAX_CMD_LINE / 2 + 1]; // To estimate the number of arguments that can fit into the args array and ensure null fits for execvp
    int argc = tokens.size();

    // Convert string vector tokens to char* args
    for (int i = 0; i < argc; i++)
    {
        args[i] = strdup(tokens[i].c_str());
    }
    args[argc] = NULL; // Null-terminate the argument array

    // Fork the process to execute the command
    pid_t pid = fork();
    if (pid < 0)
    {
        cerr << "Fork failed.\n"; // Handle fork failure
        exit(EXIT_FAILURE);            // Exit on error
    }
    else if (pid == 0)
    {
        // Child process executes the command
        if (execvp(args[0], args) == -1)
        {
            cerr << "Error executing command: " << strerror(errno) << endl;
            exit(EXIT_FAILURE); // Exit with failure if execvp fails
        }
    }
    else
    {
        wait(NULL); // Parent process waits for child process to finish
    }

    // Free allocated memory for arguments
    for (int i = 0; i < argc; i++)
    {
        free(args[i]);
    }
}