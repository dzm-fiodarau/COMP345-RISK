//----------------------------------------------------------------------------------------------------------------------
//  Macros

//  System includes
#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>

//  Project includes
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

/**
 *
 * @param tokens
 * @return
 */
static std::string reduceStringVector(const std::vector<std::string>& tokens) {
    std::string stringBuilder;

    //  Concats each element with a whitespace in between
    for (const std::string& str : tokens) {
        stringBuilder += str + " ";
    }

    //  Trims the trailing whitespace of the string
    size_t endpos = stringBuilder.find_last_not_of(" \t\n\r");  // Finding the last non-whitespace character
    if( std::string::npos != endpos ) {
        stringBuilder = stringBuilder.substr(0, endpos+1);  // Trimming the trailing whitespaces
    }
    return stringBuilder;
}


//----------------------------------------------------------------------------------------------------------------------
//  TRANSITION FUNCTIONS



bool restart(const std::string& _, GameEngine& gameEngine) {
    //  Split the input string into tokens then asserts that no tokens were passed
    vector<std::string> tokens = getTokens(_);
    if (!tokens.empty()) {
        std::cout << "\033[1;31m" << "FAILED TO LOAD MAP, INVALID NUMBER OF ARGUMENTS, EXPECTED 0, GOT " << tokens.size()
                  << "\033[0m" << std::endl;
        return false;
    }

    //  TODO IMPLEMENT LAST
    return true;
}



/** \brief Loads a map. Sample command:
 *         |    loadmap FILEPATH
 *  \param input    String of all arguments passed.
 *  \return 'true' if the command executes successfully, 'false' otherwise */
bool loadMap(const std::string& input, GameEngine& gameEngine) {
    //  Split the string into tokens
    vector<std::string> tokens = getTokens(input);

    //  Check that only one argument was passed:
    if (tokens.size() != 1) {
        std::cout << "\033[1;31m" << "FAILED TO LOAD MAP, INVALID NUMBER OF ARGUMENTS, EXPECTED 1, GOT " << tokens.size()
                  << "\033[0m" << std::endl;
        return false;
    }

    //  Load Map
    MapLoader* mapLoader = new MapLoader(tokens.front());
    Map* map = mapLoader->load();

    //  Check if loading was successful (nullptr means fail)
    if (map == nullptr) {
        //  failed to load map
        std::cout << "\033[1;31m" << "FAILED TO LOAD MAP, PERHAPS INVALID FILE" << "\033[0m" << std::endl;
        return false;
    }
    gameEngine.setMap(map);

    //  Deallocate memory and return true
    delete mapLoader;
    return true;
}



/** \brief Validates the map. Sample command:
 *       |  validatemap
 *  \param _ String of all arguments passed. Meant to be empty, without tokens.
 *  \return 'true' if the command executes successfully, 'false' otherwise */
bool validateMap(const std::string& _, GameEngine& gameEngine) {
    //  Split the input string into tokens then asserts that no tokens were passed
    vector<std::string> tokens = getTokens(_);
    if (!tokens.empty()) {
        std::cout << "\033[1;31m" << "FAILED TO LOAD MAP, INVALID NUMBER OF ARGUMENTS, EXPECTED 0, GOT " << tokens.size()
                  << "\033[0m" << std::endl;
        return false;
    }

    //  Double check that the map pointer in the GameEngine is NOT null
    if (gameEngine.getMap() == nullptr) {
        std::cout << "\033[1;31m" << "FAILED TO VALIDATE MAP, NO MAP DEFINED IN GameObject\n" << "\033[0m";
        return false;
    }

    //  Get the map, validate it and store status
    Map* map = gameEngine.getMap();
    bool validationStatus = map->validate();

    if (validationStatus) {
        //  Map successfully validated
        std::cout << "\033[34m" << "MAP SUCCESSFULLY VALIDATED" << "\033[0m" << std::endl;
        return true;
    } else {
        //  Map validation unsuccessful
        std::cout << "\033[1;31m" << "FAILED TO VALIDATE MAP, ERROR WITH MAP CONFIGURATION\n" << "\033[0m";
        return false;
    }
}



/** \brief Adds a player to the list of players.
 *         |    addplayer PLAYERNAME
 * \param input String of all arguments passed. Only one parameter expected, which will be the player name.
 * \return True if the player addition was successful */
bool addPlayer(const std::string& input, GameEngine& gameEngine) {
    //  Split the string into tokens and checks that only one argument was passed
    vector<std::string> tokens = getTokens(input);
    if (tokens.size() != 1) {
        std::cout << "\033[1;31m" << "FAILED TO LOAD MAP, INVALID NUMBER OF ARGUMENTS, EXPECTED 1, GOT " << tokens.size()
                  << "\033[0m" << std::endl;
        return false;
    }

    Player newPlayer(tokens.front(), {}, {});   //  TODO IMPLEMENT LATER WHEN BETTER UNDERSTANDING
    gameEngine.addPlayerToGame(newPlayer);
    return true;
}

/** \brief Prints the names of all the players in the game.
 *  \param _ String of all arguments passed. Meant to be empty, without tokens.
 *  \return Returns true in all cases. */
bool printPlayers(const std::string& _, GameEngine& gameEngine) {
    //  Split the input string into tokens then asserts that no tokens were passed
    vector<std::string> tokens = getTokens(_);
    if (!tokens.empty()) { return false; }

    std::cout << "PLAYERS LIST:" << std::endl;
    for (const Player& player : gameEngine.getPlayers()) {
        std::cout << player.getPlayerName() << std::endl;
    }
    return true;    //  Always return true
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

/** \brief Ends the GameEngine execution. */
bool endProgram(const std::string& _, GameEngine& gameEngine) {
    gameEngine.stopRunning();
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
        TransitionData(0, 1, "loadmap", "loadmap FILEPATH", &loadMap),
        TransitionData(1, 1, "loadmap", "loadmap FILEPATH", &loadMap),
        TransitionData(1, 2, "validatemap", "validatemap", &validateMap),
        TransitionData(2, 3, "addplayer", "addplayer PLAYERNAME", &addPlayer),
        TransitionData(3, 3, "addplayer", "addplayer PLAYERNAME", &addPlayer),
        TransitionData(3, 3, "viewplayers", "viewplayers", &printPlayers),
        TransitionData(3, 4, "assigncountries", "assigncountries SAMPLE", &assignCountries),
        TransitionData(4, 5, "issueorder", "issueorder SAMPLE", &issueOrder),
        TransitionData(5, 5, "issueorder", "issueorder SAMPLE", &issueOrder),
        TransitionData(5, 6, "endissueorders", "endissueorders SAMPLE", &endIssueOrders),
        TransitionData(6, 6, "execorder", "execorder SAMPLE", &executeOrder),
        TransitionData(6, 4, "endexecorders", "endexecorders SAMPLE", &endExecuteOrders),
        TransitionData(6, 7, "win", "win SAMPLE", &winGame),
        TransitionData(7, 0, "play", "play", &restart),
        TransitionData(7, 8, "end", "end SAMPLE", &endProgram)
    };

    this->map = nullptr;
    this->playersList = new std::vector<Player>();
}


GameEngine::GameEngine(const GameEngine& otherGameEngine) {
    //  Initializing the dynamic arrays
    this->statesSize = new size_t(*otherGameEngine.statesSize);
    this->transitionDatabaseSize = new size_t(*otherGameEngine.transitionDatabaseSize);
    this->currentStateIndex = new size_t(*otherGameEngine.currentStateIndex);
    this->isRunning = new bool(*otherGameEngine.isRunning);
    this->map = new Map(*otherGameEngine.map);
    this->playersList = new std::vector<Player>(*otherGameEngine.playersList);

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
    this->map = new Map(*otherGameEngine.map);
    this->playersList = new std::vector<Player>(*otherGameEngine.playersList);

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
    delete otherGameEngine.map;
    delete otherGameEngine.playersList;

    otherGameEngine.states = nullptr;
    otherGameEngine.transitionDatabase = nullptr;
    otherGameEngine.currentStateIndex = nullptr;
    otherGameEngine.statesSize = nullptr;
    otherGameEngine.transitionDatabaseSize = nullptr;
    otherGameEngine.isRunning = nullptr;
    otherGameEngine.map = nullptr;
    otherGameEngine.playersList = nullptr;
}


GameEngine::~GameEngine() {
    //  Deallocate the values/contents of the pointers
    delete[] states;
    delete[] transitionDatabase;
    delete currentStateIndex;
    delete statesSize;
    delete transitionDatabaseSize;
    delete isRunning;
    delete map;
    delete playersList;

    //  Set the member pointers to null pointer
    states = nullptr;
    transitionDatabase = nullptr;
    currentStateIndex = nullptr;
    statesSize = nullptr;
    transitionDatabaseSize = nullptr;
    isRunning = nullptr;
    map = nullptr;
    playersList = nullptr;
}

GameEngine& GameEngine::operator=(const GameEngine &otherGameEngine) {
    //  Check for self-assignment
    if (this != &otherGameEngine) {
        this->statesSize = new size_t(*otherGameEngine.statesSize);
        this->transitionDatabaseSize = new size_t(*otherGameEngine.transitionDatabaseSize);
        this->currentStateIndex = new size_t(*otherGameEngine.currentStateIndex);
        this->isRunning = new bool(*otherGameEngine.isRunning);
        this->map = new Map(*otherGameEngine.map);
        this->playersList = new std::vector<Player>(*otherGameEngine.playersList);

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
        this->map = new Map(*otherGameEngine.map);
        this->playersList = new std::vector<Player>(*otherGameEngine.playersList);

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
        delete otherGameEngine.map;
        delete otherGameEngine.playersList;

        otherGameEngine.states = nullptr;
        otherGameEngine.transitionDatabase = nullptr;
        otherGameEngine.currentStateIndex = nullptr;
        otherGameEngine.statesSize = nullptr;
        otherGameEngine.transitionDatabaseSize = nullptr;
        otherGameEngine.isRunning = nullptr;
        otherGameEngine.map = nullptr;
        otherGameEngine.playersList = nullptr;
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

        //  If input is empty, refresh
        if (userInput.empty()) {
            system("cls");
            continue;
        }

        //  Get the tokens of the raw command
        std::vector<std::string> tokens = getTokens(userInput);
        std::string& transitionName = tokens.front();

        //  Checking if the given transition is valid
        int transitionDataIndex = indexOfTransition(transitionName);
        if (transitionDataIndex != -1) {
            //  Transition is valid
            //  Isolate the arguments, reduce them into a string, then pass it to be further processed
            std::vector<std::string> remainingTokens(tokens.begin() + 1, tokens.end());
            std::string argumentsRaw = reduceStringVector(remainingTokens);
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
    //  Call the transition function and then return the status
    bool status = transitionData.getTransitionFunction()(argumentsRaw, *this);

    if (!status) {
        //  An error occurred, print error message, and then return
        std::cout << "\033[1;31m" << "INVALID COMMAND. View the valid commands below:" << "\033[0m";
        std::cout << getHelpStrings() << std::endl;
        return;
    }

    //  Transition into the next state
    *currentStateIndex = transitionData.getIndex2();

    //  Print that you have switched states
    std::string newCurrentStateName = states[*currentStateIndex].getStateName();
    std::cout << "\033[34m" << "SWITCHED STATES TO:\t[" << newCurrentStateName << "]\033[0m" << std::endl;
}

void GameEngine::setMap(Map* const map) { this->map = map; }
Map* GameEngine::getMap() const { return map; }

std::vector<Player> GameEngine::getPlayers() const {
    std::vector<Player> copy = *playersList;
    return copy;
}

void GameEngine::addPlayerToGame(const Player& player) const {
    playersList->push_back(player);
}
