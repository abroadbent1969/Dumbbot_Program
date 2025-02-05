#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <regex>
#include <thread>
#include <chrono>
#include <sapi.h>


void speak(const std::string& text) {
    ISpVoice* pVoice = NULL;

    if (FAILED(::CoInitialize(NULL))) {
        std::cerr << "Failed to initialize COM library." << std::endl;
        return;
    }

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    if (SUCCEEDED(hr)) {
        std::wstring wtext(text.begin(), text.end());
        pVoice->Speak(wtext.c_str(), 0, NULL);
        pVoice->Release();
        pVoice = NULL;
    }
    else {
        std::cerr << "Failed to create voice instance." << std::endl;
    }

    ::CoUninitialize();
}

/* int main() {
    std::string text = "Hello, this is a test.";
    speak(text);
    return 0;
}
*/

// Function to print text with a typing effect
void typeEffect(const std::string& text, int delay = 15) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Delay in milliseconds
    }
    std::cout << std::endl; // Move to the next line after typing is done
}

// Variable to store the user's name and color
std::string userName = "";
std::string userColor = "";
std::string userEmot = "";
std::string userCar = "";

// Global variables
int totalChips = 100;

// Function declarations
void numberGame();
std::string getResponse(const std::string& input);
int rollDie();
bool playCraps();
void crapsStart();
int generateRandomNumber(int min, int max);
void playNumberGame();
bool containsNumber(const std::string& input);
bool containsSubstring(const std::string& str, const std::string& subStr);
int guessNumber(int low, int high);
std::string toLower(const std::string& str);

int main() {
    std::cout << "Welcome to the Dumbbot Program!" << std::endl;
    typeEffect("You can start interacting with me. Type 'hello' to start and 'bye' to exit.");
    std::string text = "Welcome to the Dumbbot Program!, Type 'hello' to start and 'bye' to exit.";
    speak(text);

    while (true) {
        std::string userInput;
        std::cout << "You: ";
        std::getline(std::cin, userInput);

        std::string lowercaseInput = toLower(userInput);

        if (lowercaseInput == "bye" || lowercaseInput == "bye bye" || lowercaseInput == "exit") {
            typeEffect("Dumbbot: " + userName + " please be gental. I have Crashed and Burned!");
            std::string text = "" + userName + ", please be gental. I have Crashed and Burned!";
            speak(text);
            break;
        }

        std::string response = getResponse(userInput);
        std::cout << "Dumbbot: " << response << std::endl;
    }

    return 0;
}

// Function to check if any of the phrases in the list match the user's input
bool containsPhrase(const std::string& input, const std::vector<std::string>& phrases) {
    for (const std::string& phrase : phrases) {
        if (input.find(phrase) != std::string::npos) {
            return true;
        }
    }
    return false;
}

// Function to generate a random number between 1 and 6 (inclusive)
int rollDie() {
    return rand() % 6 + 1;
}

// Function to simulate a single round of craps
bool playCraps() {
    int dice1 = rollDie();
    int dice2 = rollDie();
    int sum = dice1 + dice2;
    int bet = 5;

    std::cout << "You rolled: " << dice1 << " + " << dice2 << " = " << sum << std::endl;

    if (sum == 7 || sum == 11) {
        totalChips += bet;
        std::cout << "You win! Chip Count: " << totalChips << std::endl;
        std::string text = "You win " + userName + "!";
        speak(text);
        return true;
    }
    else if (sum == 2 || sum == 3 || sum == 12) {
        totalChips -= bet;
        std::cout << "You lose " + userName + "! Chip Count is now: " << totalChips << std::endl;
        std::string text = "You lose " + userName + "!";
        speak(text);
        return false;
    }
    else {
        int point = sum;
        std::cout << "Point is " << point << std::endl;

        while (true) {
            dice1 = rollDie();
            dice2 = rollDie();
            int newSum = dice1 + dice2;
            std::cout << "You rolled: " << dice1 << " + " << dice2 << " = " << newSum << std::endl;

            if (newSum == point) {
                totalChips += bet;
                std::cout << "You win! Chip Count: " << totalChips << std::endl;
                std::string text = "You win.";
                speak(text);
                return true;
            }
            else if (newSum == 7) {
                totalChips -= bet;
                std::cout << "You lose " + userName + "! Chip Count: " << totalChips << std::endl;
                std::string text = "You lose.";
                speak(text);
                return false;
            }
        }
    }
}

void crapsStart() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    char playAgain;
    do {
        std::cout << "Welcome to the game of Craps!" << std::endl;
        playCraps();

        std::cout << "Do you want to roll again? (y/n): ";
        std::cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    typeEffect("Thanks for playing " + userName + "! Scram!");
    std::string text = "Thanks for playing " + userName + "! Scram!";
    speak(text);
}

// Function to generate a random number between min and max
int generateRandomNumber(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(0)));
        initialized = true;
    }
    return min + rand() % (max - min + 1);
}

// Function to play the number guessing game
void playNumberGame() {
    const int minNumber = 1;
    const int maxNumber = 100;
    const int secretNumber = generateRandomNumber(minNumber, maxNumber);
    int guess;
    int attempts = 0;

    typeEffect("Welcome to the Number Guessing Game!");
    std::string text = "Welcome to the Number Guessing Game!";
    speak(text);
    std::cout << "I have selected a number between " << minNumber << " and " << maxNumber << ". Try to guess it!" << std::endl;

    do {
        std::cout << "Enter your guess: ";
        std::cin >> guess;

        if (std::cin.fail()) {
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << minNumber << " and " << maxNumber << "." << std::endl;
            continue;
        }

        attempts++;

        if (guess < secretNumber) {
            std::cout << "Too low! Try again." << std::endl;
        }
        else if (guess > secretNumber) {
            std::cout << "Too high! Try again." << std::endl;
        }
        else {
            std::cout << "Congratulations " + userName + "! You guessed the number " << secretNumber << " in " << attempts << " attempts." << std::endl;
            return;
        }

    } while (guess != secretNumber);
}

// Function to check if a string contains a specific substring (case insensitive)
bool containsSubstring(const std::string& str, const std::string& subStr) {
    std::string strLower = str;
    std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
    std::string subStrLower = subStr;
    std::transform(subStrLower.begin(), subStrLower.end(), subStrLower.begin(), ::tolower);
    return strLower.find(subStrLower) != std::string::npos;
}

// Function to implement the AI guessing logic
int guessNumber(int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        std::cout << "Is your number " << mid << "? (y/n): ";
        std::string response;
        std::cin >> response;

        if (response == "y") {
            std::cout << "I just guessed your number." << std::endl;
            return mid;
        }
        else if (response == "n") {
            std::cout << "Is your number less than " << mid << "? (y/n): ";
            std::cin >> response;
            if (response == "y") {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }
        else {
            std::cout << "Please enter 'y' or 'n'." << std::endl;
        }
    }
    return -1; // Indicates failure to guess the number
}

void numberGame() {
    typeEffect("Number Guessing Game!");
    std::string text = "Number Guessing Game!";
    speak(text);
    std::cout << "Think of a number between 1 and 1000." << std::endl;

    int low = 1;
    int high = 1000;

    int guessedNumber = guessNumber(low, high);

    if (guessedNumber != -1) {
        std::cout << "The number you were thinking of " + userName + " is: " << guessedNumber << std::endl;
        return ;
    }
    else {
        std::cout << "Are you sure about this " + userName + "?" << std::endl;
    }
}

// Function to convert a string to lowercase
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Function to get a response from the chatbot
std::string getResponse(const std::string& input) {
    std::string lowercaseInput = toLower(input);

    // Predefined responses
    std::map<std::string, std::string> responses = {
        {"yes", "Thanks!"},
        {"no", "Are you sure?"},
        {"y", "Nice!"},
        {"n", "Cool"},
        {"nice to meet you too", "Thank you!"},
        {"thanks", "You are welcome " + userName + "!"},
        {"how are you?", "I'm just a dumb AI program, so I don't have feelings. How are you " + userName + "?"},
        {"i am well", "Great! But I really do not care " + userName + "."},
        {"what is your name", "I'm a dumb AI program created to help you with none of your tasks."},
        {"bye", "Take care, Have a great day!"},
        {"bye bye", "Have a super day!"},
        {"what is my name?", "I don't know because I haven't asked for it yet. Say hello first"},
        {"what is your favorite color?", "I'm a dumb AI program so color is not my thing"},
        {"exit", "Ciao Bella!"},
        {"games", "OK, Craps, Numbers Game or Guessing Game?"},
    };

    // List of phrases that should trigger the same response
    std::vector<std::string> feelQuestions = {
        "how do i feel",
        "how are you",
        "how do you feel",
        "how am i feeling",
        "how do you do",
        "you feel ok?",
        "what is up",
        "what is my mood"
    };

    // Check if the input matches any of the phrases in the list
    if (containsPhrase(lowercaseInput, feelQuestions)) {
        if (userEmot.empty()) {
            typeEffect("Dumbbot: I do not know, I'm just a dumb AI program, so I don't have feelings. How are you?");
            std::cout << "You: ";
            std::getline(std::cin, userEmot);
            return userName + " feels " + userEmot + " today? That is so sad!";
        }
        else {
            return "We did this already. " + userName + " feels " + userEmot + " today. Right?";
        }
    }

    // Check if the input is "hello" and ask for the name if not already stored
    if (lowercaseInput == "hello") {
        if (userName.empty()) {
            typeEffect("Dumbbot: Hello! What is your name?");
            std::cout << "You: ";
            std::getline(std::cin, userName);
            return "Nice to meet you, " + userName + "!";
        }
        else {
            return "Hello again, " + userName + "!";
        }
    }
    if ((lowercaseInput == "what is my favorite color?") || (lowercaseInput == "color") || (lowercaseInput == "my color") || (lowercaseInput == "what is your favorite color?")) {
        if (userColor.empty()) {
            typeEffect("Dumbbot:Not sure, I'm a dumb AI program so color is not my thing. What is your favorite color?");
            std::cout << "You: ";
            std::getline(std::cin, userColor);
            return "" + userColor + "? That is an ugly color " + userName + "!";
        }
        else {
            return "You favorite color is " + userColor + " right?";
        }
    }
    if ((lowercaseInput == "what is my name?") || (lowercaseInput == "name") || (lowercaseInput == "my name")) {
        if (userName.empty()) {
            typeEffect("Dumbbot:I don't know because I haven't asked for it yet. What is your name?");
            std::cout << "You: ";
            std::getline(std::cin, userName);
            return "" + userName + "? That is an odd name!";
        }
        else {
            return "You name is " + userName + " right?";
        }
    }
    if ((lowercaseInput == "what do you drive?") || (lowercaseInput == "do you have a car?") || (lowercaseInput == "what sort of car do you have?") || (lowercaseInput == "car") || (lowercaseInput == "truck")) {
        if (userCar.empty()) {
            typeEffect("Dumbbot:I do not know, I'm just a dumb AI program, so I don't have a car. What kind of vehicle do you have?");
            std::cout << "You: ";
            std::getline(std::cin, userCar);
            return "" + userName + " has a " + userCar + " type of vehicle? That is so sad!";
        }
        else {
            typeEffect("We did this already. " + userName + " has a " + userCar + " type of vehicle. Right?");
            return "";
        }
    }
    if ((lowercaseInput == userColor) && !userColor.empty()) {
        {
            typeEffect("Color again? " + userName + " Really?");
            return "";
        }
    }
    if ((lowercaseInput == userCar) && !userCar.empty()) {
        {
            typeEffect("I don't like a " + userCar + " " + userName + " Really?");
            return "";
        }
    }
    if ((lowercaseInput == userName) && !userName.empty()) {
        {
            typeEffect("" + userName + " is so cool!");
            return "";
        }
    }
    if ((lowercaseInput == "shall we play a game?") || (lowercaseInput == "lets play a game") || (lowercaseInput == "games")) {
        {
            typeEffect("OK, " + userName + " Craps, Numbers Game or Guessing Game?");
            std::string text = "shall we play a game " + userName + "?";
            speak(text);
            return "";
        }
    }
    if ((lowercaseInput == "what do you know?") || (lowercaseInput == "facts") || (lowercaseInput == "tell me your facts")) {
        if (userEmot.empty() && userColor.empty() && userName.empty() && userCar.empty()) {
            typeEffect("I don't know anything.");
            std::string text = "I don't know anything.";
            speak(text);
            return "";
        }
        if (userEmot.empty() && userColor.empty() && userCar.empty()) {
            typeEffect("Your name is " + userName + ", right?");
            std::string text = "Your name is " + userName + ", right?";
            speak(text);
            return "";
        }
        if (userEmot.empty() && userCar.empty()) {
            typeEffect("Your name is " + userName + " and you like the color " + userColor + ", right?");
            std::string text = "Your name is " + userName + " and you like the color " + userColor + ", right?";
            speak(text);
            return "";
        }
        if (userColor.empty() && userCar.empty()) {
            typeEffect("Your name is " + userName + " and you feel " + userEmot + ", right?");
            std::string text = "Your name is " + userName + " and you feel " + userEmot + ", right?";
            speak(text);
            return "";
        }
        if (userEmot.empty() && userColor.empty()) {
            typeEffect("Your name is " + userName + " and you like to drive a " + userCar + ", right?");
            std::string text = "Your name is " + userName + " and you like to drive a " + userCar + ", right?";
            speak(text);
            return "";
        }
        if (userColor.empty()) {
            typeEffect("Your name is " + userName + " and you feel " + userEmot + ", and you like to drive a " + userCar + " right?");
            std::string text = "Your name is " + userName + " and you feel " + userEmot + ", and you like to drive a " + userCar + " right?";
            speak(text);
            return "";
        }
        if (userEmot.empty()) {
            typeEffect("Your name is " + userName + " and you like the color " + userColor + ", and you like to drive a " + userCar + " right?");
            std::string text = "Your name is " + userName + " and you like the color " + userColor + ", and you like to drive a " + userCar + " right?";
            speak(text);
            return "";
        }
        if (userCar.empty()) {
            typeEffect("Your name is " + userName + " and you like the color " + userColor + ", and you feel " + userEmot + " right?");
            std::string text = "Your name is " + userName + " and you like the color " + userColor + ", and you feel " + userEmot + " right?";
            speak(text);
            return "";
        }
        typeEffect(userName + " feels " + userEmot + " today and likes the color " + userColor + " and you drive a " + userCar + ", right?");
        std::string text = userName + " feels " + userEmot + " today and likes the color " + userColor + " and you drive a " + userCar + ", right ? ";
        speak(text);
        return "";
    }

    auto it = responses.find(lowercaseInput);
    if (it != responses.end()) {
        return it->second;
    }


    if (containsSubstring(lowercaseInput, "numbers game") || containsSubstring(lowercaseInput, "number game") || containsSubstring(lowercaseInput, "guess my number") || containsSubstring(lowercaseInput, "number") || containsSubstring(lowercaseInput, "numbers")) {
        std::cout << "Dumbbot: Numbers Game it is." << std::endl;
        numberGame();
    }
    else if (containsSubstring(lowercaseInput, "craps") || containsSubstring(lowercaseInput, "dice") || containsSubstring(lowercaseInput, "lets play craps")) {
        std::cout << "Dumbbot: Craps it is." << std::endl;
        crapsStart();
    }
    else if (containsSubstring(lowercaseInput, "guessing game") || containsSubstring(lowercaseInput, "guess") || containsSubstring(lowercaseInput, "game")) {
        std::cout << "Dumbbot: Guessing Game it is." << std::endl;
        playNumberGame();
    }
    else if (containsSubstring(lowercaseInput, "mad") || containsSubstring(lowercaseInput, "sad") || containsSubstring(lowercaseInput, "well") || containsSubstring(lowercaseInput, "good") || containsSubstring(lowercaseInput, "happy")) {
        std::cout << "Dumbbot: Great! But I really do not care " + userName + "." << std::endl;
    }
    else if (containsSubstring(lowercaseInput, "greg") || containsSubstring(lowercaseInput, "alex") || containsSubstring(lowercaseInput, "mike")) {
        std::cout << "Dumbbot: That is an odd name." << std::endl;
    }
    else {
        //std::cout << "I'm sorry, I didn't understand that " + userName + " try again." << std::endl;
        std::cout << "Nice! " + userName + ", but try again." << std::endl;
    }

    return ";)";
}


