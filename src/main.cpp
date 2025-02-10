/*
    Author: Manika Sharma
    Project: Shell using C++
    OS: Linux/Unix based
    Course: Operating Systems

    Description: A C++ shell interface that:
                    Accepts user commands as a parent ->
                    Executes each command in a separate child process
                    

    Built-in commands supported:
                1. cal
                3. date
                4. who
                5. top
                6. ls
                7. history
                8. exit
                9. cd
                10. echo
                11. !
                12. !!
                13. 

    External commands supported:

                1. ps
                2. top

    My own added commands supported:
                1. weather
                2. palindrome
                3. reverse
                4. countdown

    "!!" will trigger the history command and will show a list of commands that have been entered. In order to run a command from the list, user must type "!1" where the number is the index from the history list.
*/

#include "shell.h"

int main()
{
    Shell shell; // Create a Shell object
    shell.run(); // Start the shell by calling the run method
    return 0;    // Exit the program
}