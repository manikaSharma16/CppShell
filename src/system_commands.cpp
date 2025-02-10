#include "system_commands.h"
#include <iostream> // For input output, error
#include <unistd.h> // For process management using POSIX system calls (fork(), execlp(), wait())
#include <dirent.h> // To read directories (opendir(), readdir(), and closedir())
#include <cstring> // For string manipulation

using namespace std; // We are using namespace std for cout, cin, etc

// void SystemCommands::listProcesses() const
// {
//     system("ps aux");
// }

void SystemCommands::listProcesses() const
{
    pid_t pid = fork();  // Fork a new process

    if (pid == 0) {
        // Child process executes the 'ps aux' command
        /*
            a: Show processes for all users.
            u: Display the user/owner of the process.
            x: Show background processes.
        */
        execlp("ps", "ps", "aux", nullptr); // Replace child process with ps command and run the binary of ps in PATH
    } else {
        // Parent process: wait for the child to finish
        wait(nullptr);
    }
}

void SystemCommands::listUsers() const
{
    system("who"); // system() runs a shell command, same as execlp
}

void SystemCommands::listFiles(const std::string &path) const
{
    DIR *dir = opendir(path.c_str()); // open the directory of the given path

    // Check if opendir() failed to open the directory
    if (!dir)
    {
        cerr << "Cannot open directory." << endl; // Error message using cerr
        return;
    }

    // Declare a pointer to a struct dirent, which will store directory entries
    struct dirent *entry; // file no., length of this record, etc

    // Loop through each entry in the directory
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) // Skip the current directory (".") and the parent directory ("..")
            cout << entry->d_name << "  ";
    }

    cout << endl;

    closedir(dir); // Close the current directory
}
