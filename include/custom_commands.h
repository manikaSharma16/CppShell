#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

#include <string>
#include <cstring>

class CustomCommands
{
public:
    void reverseCommand(const std::string &input);
    void countdownTimer(int input);
    void countdownCommand(int input);
    bool isPalindrome(const std::string &text);
    void palindromeCommand(const std::string &text);
    void weatherCommand(const std::string &city);
};

#endif // CUSTOM_COMMANDS_H
