#include "custom_commands.h"
#include <iostream> // For input output, error
#include <unistd.h> // For process management using POSIX system calls (fork(), execlp(), wait())
#include <dirent.h> // To read directories (opendir(), readdir(), and closedir())
#include <cstring>  // For string manipulation
#include <chrono>   // For std::this_thread::sleep_for
#include <thread>   // Include for std::thread
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

// Reverse the input string
void CustomCommands::reverseCommand(const string &input)
{
    string reversedText = input;
    reverse(reversedText.begin(), reversedText.end()); // Reverse the string
    cout << reversedText << endl;                      // Print the reversed string
}

// Function that performs the countdown and prints the message
void CustomCommands::countdownTimer(int input)
{
    for (int i = input; i > 0; --i)
    {
        // std::cout << "Counting down from " << i << "...\n";
        this_thread::sleep_for(chrono::seconds(1)); // Wait for 1 second
    }
    cout << "Time's up!\n";
}

// Function to handle the countdown command: Starts the countdown n a background thread
void CustomCommands::countdownCommand(int input)
{
    // Starts a new thread that runs the countdownTimer function in the background
    thread countdownThread(&CustomCommands::countdownTimer, this, input);
    countdownThread.detach(); // Detach the thread to run in the background
}

// Function to check if a string is a palindrome
bool CustomCommands::isPalindrome(const string &input)
{
    string cleanedText;

    // Remove non-alphanumeric characters and convert to lowercase
    for (char c : input)
    {
        if (isalnum(c)) // Only include alphanumeric characters
        {
            cleanedText += std::tolower(c); // Convert to lowercase for case-insensitive comparison
        }
    }

    // Compare the string with its reverse
    string reversedText = cleanedText;
    reverse(reversedText.begin(), reversedText.end());

    return cleanedText == reversedText; // Return true if the string is the same forward and backward
}

// Function to handle palindrome command
void CustomCommands::palindromeCommand(const string &text)
{
    if (isPalindrome(text))
    {
        cout << "\"" << text << "\" is a palindrome." << endl;
    }
    else
    {
        cout << "\"" << text << "\" is not a palindrome." << endl;
    }
}

// // Function to handle the "weather" command
// void CustomCommands::weatherCommand(const string &city)
// {
//     // In this version, we'll simulate weather information for a city
//     // You can later replace this part with actual API calls like OpenWeatherMap API

//     // Simulate weather info based on city (for fun)
//     cout << "Weather in " << city << ": ";

//     // Create a simple random weather generator
//     srand(time(0));
//     int temperature = rand() % 50 + 50; // Random temperature between 50°F and 100°F
//     string weatherConditions[] = {"Sunny", "Cloudy", "Rainy", "Windy", "Snowy"};
//     string condition = weatherConditions[rand() % 5];
//     int chanceOfRain = rand() % 100; // Random chance of rain

//     cout << temperature << "°F, " << condition << ", " << chanceOfRain << "% chance of rain" << endl;
// }

// Simple URL encode function to replace ' ' with +
// Todo make it more detailed
string urlEncode(const string &str)
{
    string encoded = str;
    replace(encoded.begin(), encoded.end(), ' ', '+');
    return encoded;
}

// Callback function to write data into a string
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void CustomCommands::weatherCommand(const string &city)
{
    string apiKey = "afa8d373260b8cf51798bf3be77b0121";  // Replace with OpenWeatherMap API key from https://home.openweathermap.org/api_keys
    string encodedCity = urlEncode(city);  // URL encode the city name
    string url = "https://api.openweathermap.org/data/2.5/weather?q=" + encodedCity + "&appid=" + apiKey + "&units=imperial";

    CURL *curl; // A pointer to a CURL object. This object will be used to execute the HTTP request
    CURLcode res;
    string readBuffer;

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT); // Initializes the global CURL environment
    curl = curl_easy_init(); // Initializes the CURL object to prepare it for making requests

    if (curl) // If CURL initialization succeeded
    {
        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Sets the URL for the request
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Specify WriteCallback to handle the incoming response data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // Tells CURL to store the response data in the readBuffer

        // Performs the HTTP request to the OpenWeatherMap API and stores the response code in res
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            cerr << "CURL request failed: " << curl_easy_strerror(res) << endl;
        }
        else
        {
            // Debugging: Print the raw API response
            // std::cout << "API Response: " << readBuffer << std::endl;

            try
            {
                // Parse the JSON response
                json weatherData = json::parse(readBuffer);

                // Check if "name" and "weather" exist in the response
                if (weatherData.contains("name") && weatherData.contains("weather") && !weatherData["weather"].empty())
                {
                    // Extract data
                    string cityName = weatherData["name"];
                    float temperature = weatherData["main"]["temp"];
                    string condition = weatherData["weather"][0]["description"];
                    int chanceOfRain = 0; // Default to no rain

                    // Check if "rain" exists in the response and get the rain chance
                    if (weatherData.contains("rain") && weatherData["rain"].contains("1h"))
                    {
                        chanceOfRain = static_cast<int>(weatherData["rain"]["1h"]);
                    }

                    // Display formatted weather information
                    cout << "\n----------------------------\n";
                    cout << "Weather Information for " << cityName << ":\n";
                    cout << "----------------------------\n";
                    cout << "Temperature: " << fixed << setprecision(1) << temperature << "°F\n";
                    cout << "Condition: " << condition << "\n";
                    cout << "Humidity: " << weatherData["main"]["humidity"] << "%\n";
                    cout << "Wind Speed: " << weatherData["wind"]["speed"] << " m/s\n";
                    cout << "Cloud Coverage: " << weatherData["clouds"]["all"] << "%\n";

                    if (chanceOfRain > 0) {
                        cout << "Chance of Rain: " << chanceOfRain << "%\n";
                    } else {
                        cout << "No Rain Expected.\n";
                    }

                    cout << "----------------------------\n";
                }
                else
                {
                    cerr << "Error: Missing expected data in the response." << endl;
                }
            }
            catch (const json::exception& e)
            {
                cerr << "Error parsing JSON response: " << e.what() << endl;
            }
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
