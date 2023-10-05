//----------------------------------------------------------------------------------------------------------------------
//  Macros

#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>
#include "../headers/GameEngine.h"


#define PRESS_ENTER_TO_CONTINUE(clearConsole)                       \
    std::string _IGNORE_STRING;                                     \
    std::cout << "Press Enter to Continue... ";                     \
    std::getline(std::cin, _IGNORE_STRING);                         \
                                                                    \
    if (clearConsole) {                                             \
        system("cls");                                              \
    }                                                               \


#ifdef RED_DEBUG_OUTPUT
#define ANSI_RED_INSERT "\033[31m" <<
#define ANSI_RESET_INSERT "\033[0m" <<
#else
#define ANSI_RED_INSERT
#define ANSI_RESET_INSERT
#endif


#ifdef DEBUG
#define DEBUG_PRINT(...) print(__VA_ARGS__);
#else
#define DEBUG_PRINT(...)
#endif


//----------------------------------------------------------------------------------------------------------------------
//  Static functions


/** \brief Returns a std::string pointer with the given string. Memory management is delegated to another entity. */
static std::string* stringToPointer(std::string str) {
    return new std::string(std::move(str));
}

/** \brief Base case for recursively printing function 'print(...)' */
static void print() {
    std::cout << std::endl;
}

/** \brief Variadic template function to print multiple passed arguments. */
template<typename T, typename... Args>
static void print(T first, Args... rest) {
    std::cout << ANSI_RED_INSERT first << ANSI_RESET_INSERT "";
    print(rest...);  // Recursive call with one less argument
}

/** \brief Splits a string by spaces and returns a vector of string tokens. */
static std::vector<std::string> getTokens(std::string input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    while (iss >> input) {
        tokens.push_back(input);
    }
    return tokens;
}


//----------------------------------------------------------------------------------------------------------------------
//  TRANSITION FUNCTIONS
//  todo implement transition functions

bool printInput(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "ARGS:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool loadMap(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "loading map w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool validateMap(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "validating map w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool addPlayer(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "adding player w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool assignCountries(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "assigning countries w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool issueOrder(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "issuing order w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool endIssueOrders(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "FINISHED issuing orders w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool executeOrder(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "executing order w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool endExecuteOrders(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "FINISHED executing orders w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool winGame(const std::string& input, GameEngine& gameEngine) {
    std::cout << "\033[34m" << "win w/ args:\t" << input << "\033[0m" << std::endl;
    return true;
}

bool endProgram(const std::string& ignored, GameEngine& gameEngine) {
    return true;

}


//----------------------------------------------------------------------------------------------------------------------
//  "State" implementations


State::State()
    : stateName(stringToPointer("DEFAULT")) { }

State::State(const std::string& stateName)
    : stateName(stringToPointer(stateName)) { }

State::State(const State& otherState)
    : stateName(stringToPointer(otherState.getStateName())) { }

State& State::operator=(const State& otherState) {
    //  Check for self-assignment
    if (this != &otherState) {
        std::string stateNameValue = otherState.getStateName();
        this->stateName = stringToPointer(stateNameValue);
    }
    return *this;
}

State::State(State&& otherState) noexcept
    : stateName(stringToPointer(otherState.getStateName())) {
    otherState.stateName->clear();   //  Clear the contents of the string
}

State &State::operator=(State&& otherState) noexcept {
    //  Check for self-assignment
    if (this != &otherState) {
        std::string stateNameValue = otherState.getStateName();
        this->stateName = stringToPointer(stateNameValue);

        otherState.stateName->clear();
    }
    return *this;
}
State::~State() {
    delete stateName;       //  Deallocate memory
    stateName = nullptr;    //  Set to null ptr
}

std::ostream &operator<<(std::ostream& os, const State& otherState) {
    os <<  "State Name:\t" << otherState.getStateName();
    return os;
}

bool State::operator==(const State& otherState) const {
    return this->getStateName() == otherState.getStateName();
}

std::string State::getStateName() const {
    return *(this->stateName);
}


//----------------------------------------------------------------------------------------------------------------------
//  "State" implementations

TransitionData::TransitionData(size_t index1, size_t index2, std::string transitionName, std::string helpString,
                               bool (*transitionFunction)(const std::string &, GameEngine &)) {
    this->index1 = new size_t(index1);
    this->index2 = new size_t(index2);
    this->transitionName = new std::string(std::move(transitionName));
    this->helpString = new std::string(std::move(helpString));
    this->transitionFunction = transitionFunction;
}

TransitionData::TransitionData(const TransitionData& other) {
    this->index1 = new size_t(other.getIndex1());
    this->index2 = new size_t(other.getIndex2());
    this->transitionName = new std::string(other.getTransitionName());
    this->helpString = new std::string(other.getHelpString());
    this->transitionFunction = other.transitionFunction;
}

TransitionData::~TransitionData() {
    delete index1;
    delete index2;
    delete transitionName;
    delete helpString;

    index1 = nullptr;
    index2 = nullptr;
    transitionName = nullptr;
    helpString = nullptr;
    transitionFunction = nullptr;
}

TransitionData &TransitionData::operator=(const TransitionData& other) {
    //  Check for self-assignment
    if (this != &other) {
        this->index1 = new size_t(other.getIndex1());
        this->index2 = new size_t(other.getIndex2());
        this->transitionName = new std::string(other.getTransitionName());
        this->helpString = new std::string(other.getHelpString());
        this->transitionFunction = other.transitionFunction;
    }

    return *this;
}

std::ostream &operator<<(std::ostream &os, const TransitionData& transitionData) {
    os << transitionData.getIndex1() << " - " << transitionData.getIndex2() << " - "
       << transitionData.getTransitionName() << " - \"" << transitionData.getHelpString() << "\" - "
       << transitionData.transitionFunction;
    return os;
}


//----------------------------------------------------------------------------------------------------------------------
//  "GameEngine" implementations


GameEngine::GameEngine()
    : currentStateIndex(new size_t(0)), statesSize(new size_t(9)), transitionDatabaseSize(new size_t(14)),
      isRunning(new bool(false)) {
    states = new State[] {
        State("start"),
        State("map loaded"),
        State("map validated"),
        State("players added"),
        State("assign reinforcement"),
        State("issueorder"),
        State("execute orders"),
        State("win"),
        State("END")
    };

    transitionDatabase = new TransitionData[] {
        TransitionData(0, 1, "loadmap", "loadmap SAMPLE", &loadMap),
        TransitionData(1, 1, "loadmap", "loadmap SAMPLE", &loadMap),
        TransitionData(1, 2, "validatemap", "validatemap SAMPLE", &validateMap),
        TransitionData(2, 3, "addplayer", "addplayer SAMPLE", &addPlayer),
        TransitionData(3, 3, "addplayer", "addplayer SAMPLE", &addPlayer),
        TransitionData(3, 4, "assigncountries", "assigncountries SAMPLE", &assignCountries),
        TransitionData(4, 5, "issueorder", "issueorder SAMPLE", &issueOrder),
        TransitionData(5, 5, "issueorder", "issueorder SAMPLE", &issueOrder),
        TransitionData(5, 6, "endissueorders", "endissueorders SAMPLE", &endIssueOrders),
        TransitionData(6, 6, "execorder", "execorder SAMPLE", &executeOrder),
        TransitionData(6, 4, "endexecorders", "endexecorders SAMPLE", &endExecuteOrders),
        TransitionData(6, 7, "win", "win SAMPLE", &winGame),
        TransitionData(7, 0, "play", "play SAMPLE", &printInput), // temporary function
        TransitionData(7, 8, "end", "end SAMPLE", &endProgram)
    };
}


GameEngine::GameEngine(const GameEngine& otherGameEngine) {
    //  Initializing the dynamic arrays
    this->statesSize = new size_t(*otherGameEngine.statesSize);
    this->transitionDatabaseSize = new size_t(*otherGameEngine.transitionDatabaseSize);
    this->currentStateIndex = new size_t(*otherGameEngine.currentStateIndex);
    this->isRunning = new bool(*otherGameEngine.isRunning);

    this->states = new State[*this->statesSize];
    this->transitionDatabase = new TransitionData[*this->transitionDatabaseSize];

    for (int i = 0; i < *this->statesSize; i++) {
        this->states[i] = otherGameEngine.states[i];

        //  Assuring that the addresses are different (deep copy)
        DEBUG_PRINT("\nSTATES\nthis\t", &this->states[i], "\nother\t", &otherGameEngine.states[i])
    }

    for (int i = 0; i < *this->transitionDatabaseSize; i++) {
        this->transitionDatabase[i] = otherGameEngine.transitionDatabase[i];

        //  Assuring that the addresses are different (deep copy)
        DEBUG_PRINT("\nTRANSITION_STRUCT\nthis\t", &this->transitionDatabase[i],
                    "\nother\t", &otherGameEngine.transitionDatabase[i])
    }
}


GameEngine::GameEngine(GameEngine &&otherGameEngine) noexcept {
    //  Copy the values from the other object
    this->isRunning = new bool(*otherGameEngine.isRunning);
    this->statesSize = new size_t(*otherGameEngine.statesSize);
    this->transitionDatabaseSize = new size_t(*otherGameEngine.transitionDatabaseSize);
    this->currentStateIndex = new size_t(*otherGameEngine.currentStateIndex);
    this->states = new State[*this->statesSize];
    this->transitionDatabase = new TransitionData[*this->transitionDatabaseSize];

    for (int i = 0; i < *this->statesSize; i++) {
        this->states[i] = otherGameEngine.states[i];
    }

    for (int i = 0; i < *this->transitionDatabaseSize; i++) {
        this->transitionDatabase[i] = otherGameEngine.transitionDatabase[i];
    }

    //  Deallocates the values of the other GameEngine object and sets the pointers to a null pointer
    delete[] otherGameEngine.states;
    delete[] otherGameEngine.transitionDatabase;
    delete otherGameEngine.currentStateIndex;
    delete otherGameEngine.statesSize;
    delete otherGameEngine.transitionDatabaseSize;
    delete otherGameEngine.isRunning;

    otherGameEngine.states = nullptr;
    otherGameEngine.transitionDatabase = nullptr;
    otherGameEngine.currentStateIndex = nullptr;
    otherGameEngine.statesSize = nullptr;
    otherGameEngine.transitionDatabaseSize = nullptr;
    otherGameEngine.isRunning = nullptr;
}


GameEngine::~GameEngine() {
    //  Deallocate the values/contents of the pointers
    delete[] states;
    delete[] transitionDatabase;
    delete currentStateIndex;
    delete statesSize;
    delete transitionDatabaseSize;
    delete isRunning;

    //  Set the member pointers to null pointer
    states = nullptr;
    transitionDatabase = nullptr;
    currentStateIndex = nullptr;
    statesSize = nullptr;
    transitionDatabaseSize = nullptr;
    isRunning = nullptr;
}

GameEngine& GameEngine::operator=(const GameEngine &otherGameEngine) {
    //  Check for self-assignment
    if (this != &otherGameEngine) {
        this->statesSize = new size_t(*otherGameEngine.statesSize);
        this->transitionDatabaseSize = new size_t(*otherGameEngine.transitionDatabaseSize);
        this->currentStateIndex = new size_t(*otherGameEngine.currentStateIndex);
        this->isRunning = new bool(*otherGameEngine.isRunning);

        this->states = new State[*this->statesSize];
        this->transitionDatabase = new TransitionData[*this->transitionDatabaseSize];

        for (int i = 0; i < *this->statesSize; i++) {
            this->states[i] = otherGameEngine.states[i];
        }

        for (int i = 0; i < *this->transitionDatabaseSize; i++) {
            this->transitionDatabase[i] = otherGameEngine.transitionDatabase[i];
        }
    }

    return *this;
}

GameEngine& GameEngine::operator=(GameEngine &&otherGameEngine) noexcept {
    //  Check for self-assignment
    if (this != &otherGameEngine) {
        //  Copy the values from the other object
        this->isRunning = new bool(*otherGameEngine.isRunning);
        this->statesSize = new size_t(*otherGameEngine.statesSize);
        this->transitionDatabaseSize = new size_t(*otherGameEngine.transitionDatabaseSize);
        this->currentStateIndex = new size_t(*otherGameEngine.currentStateIndex);
        this->states = new State[*this->statesSize];
        this->transitionDatabase = new TransitionData[*this->transitionDatabaseSize];

        for (int i = 0; i < *this->statesSize; i++) {
            this->states[i] = otherGameEngine.states[i];
        }

        for (int i = 0; i < *this->transitionDatabaseSize; i++) {
            this->transitionDatabase[i] = otherGameEngine.transitionDatabase[i];
        }

        //  Deallocates the values of the other GameEngine object and sets the pointers to a null pointer
        delete[] otherGameEngine.states;
        delete[] otherGameEngine.transitionDatabase;
        delete otherGameEngine.currentStateIndex;
        delete otherGameEngine.statesSize;
        delete otherGameEngine.transitionDatabaseSize;
        delete otherGameEngine.isRunning;

        otherGameEngine.states = nullptr;
        otherGameEngine.transitionDatabase = nullptr;
        otherGameEngine.currentStateIndex = nullptr;
        otherGameEngine.statesSize = nullptr;
        otherGameEngine.transitionDatabaseSize = nullptr;
        otherGameEngine.isRunning = nullptr;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const GameEngine& gameEngine) {
    os << "STATES:\n";
    for (int i = 0; i < *gameEngine.statesSize; i++) {
        os << "[" << i << "] " << gameEngine.states[i] << std::endl;
    }

    os << "\nTRANSITIONS:\n";
    for (int i = 0; i < *gameEngine.transitionDatabaseSize; i++) {
        os << "[" << i << "] " << gameEngine.transitionDatabase[i] << std::endl;
    }

    os << "\nCURRENT STATE: " << gameEngine.states[*gameEngine.currentStateIndex].getStateName();
    os << "\nIS RUNNING:    " << (*gameEngine.isRunning ? "true" : "false");
    return os;
}


void GameEngine::execute() {
    //  Set the isRunning status variable to 'true'
    *isRunning = true;

    while (this->isRunning) {
        //  Printing out prompt
        std::string currentStateName = states[*currentStateIndex].getStateName();
        std::cout << "CURRENT STATE:\t[" << currentStateName << "]" << std::endl;
        std::cout << "Please enter a command:\n> ";

        //  Taking user input
        std::string userInput;
        std::getline(std::cin, userInput);
        std::cout << std::endl;

        //  Get the tokens of the raw command
        std::vector<std::string> tokens = getTokens(userInput);
        std::string& transitionName = tokens.front();

        //  Checking if the given transition is valid
        int transitionDataIndex = indexOfTransition(transitionName);
        if (transitionDataIndex != -1) {
            //  Transition is valid
            //  Isolate the arguments, reduce them into a string, then pass it to be further processed
            std::vector<std::string> remainingTokens(tokens.begin() + 1, tokens.end());
            std::string argumentsRaw = std::accumulate(remainingTokens.begin(), remainingTokens.end(), std::string(" "));
            processCommand(transitionDatabase[transitionDataIndex], argumentsRaw);
        } else {
            //  Transition is invalid, print series of error messages
            std::cout << "\033[1;31m" << "INVALID COMMAND. View the valid commands below:" << "\033[0m";
            std::cout << getHelpStrings() << std::endl;
        }

        //  Press enter to continue.
        PRESS_ENTER_TO_CONTINUE(true)
    }
}

int GameEngine::indexOfTransition(const std::string& givenTransitionName) const {
    for (int i = 0 ; i < *transitionDatabaseSize; i++) {
        TransitionData transitionData = transitionDatabase[i];
        if ((transitionData.getIndex1() == *currentStateIndex)
            && (transitionData.getTransitionName() == givenTransitionName)) {
            return i;
        }
    }

    return -1;
}

std::string GameEngine::getHelpStrings() const {
    std::string stringBuilder;

    for (int i = 0 ; i < *transitionDatabaseSize; i++) {
        TransitionData transitionData = transitionDatabase[i];
        if (transitionData.getIndex1() == *currentStateIndex) {
            stringBuilder += "\n\"" + transitionData.getHelpString() + "\"";
        }
    }

    return stringBuilder;
}

void GameEngine::processCommand(TransitionData transitionData, const std::string& argumentsRaw) {
    //  Get transition data
    transitionData.getTransitionFunction()(argumentsRaw, *this);     //  Call the corresponding transition function

    //  Transition into the next state
    *currentStateIndex = transitionData.getIndex2();

    //  Print that you have switched states
    std::string newCurrentStateName = states[*currentStateIndex].getStateName();
    std::cout << "\033[34m" << "SWITCHED STATES TO:\t[" << newCurrentStateName << "]\033[0m" << std::endl;
}
