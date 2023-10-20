//----------------------------------------------------------------------------------------------------------------------
//  Macros

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "UnusedParameter"
#endif


#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>
#include <memory>

#include "../headers/GameEngine.h"
#include "../headers/CommandProcessing.h"


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
//  Game Functions

bool game_restart(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_RESTART")
    return true;
}

bool game_loadMap(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_LOAD_MAP")
    return true;
}

bool game_validateMap(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_VALIDATE_MAP")
    return true;
}

bool game_addPlayer(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_ADD_PLAYER")
    return true;
}

bool game_printPlayers(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_PRINT_PLAYERS")
    return true;
}

bool game_gameStart(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_GAME_START")
    return true;
}

bool game_issueOrder(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_ISSUE_ORDER")
    return true;
}

bool game_endIssueOrders(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_END_ISSUE_ORDERS")
    return true;
}

bool game_executeOrder(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_EXECUTE_ORDER")
    return true;
}

bool game_endExecuteOrders(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_END_EXECUTE_ORDERS")
    return true;
}

bool game_winGame(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_WIN_GAME")
    return true;
}

bool game_quit(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_END_PROGRAM")
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

void State::setStateName(const std::string& newStateName) {
    *this->stateName = newStateName;
}


//----------------------------------------------------------------------------------------------------------------------
//  "State" implementations

TransitionData::TransitionData(size_t index1, size_t index2, size_t numberOfArguments, std::string transitionName,
                               std::string helpString, bool (*transitionFunction)(const std::vector<std::string>&, GameEngine&)) {
    this->index1 = new size_t(index1);
    this->index2 = new size_t(index2);
    this->numberOfArguments = new size_t(numberOfArguments);
    this->transitionName = new std::string(std::move(transitionName));
    this->helpString = new std::string(std::move(helpString));
    this->transitionFunction = transitionFunction;
}

TransitionData::TransitionData(const TransitionData& other) {
    this->index1 = new size_t(other.getIndex1());
    this->index2 = new size_t(other.getIndex2());
    this->numberOfArguments = new size_t(other.getNumberOfArguments());
    this->transitionName = new std::string(other.getTransitionName());
    this->helpString = new std::string(other.getHelpString());
    this->transitionFunction = other.transitionFunction;
}

TransitionData::~TransitionData() {
    delete index1;
    delete index2;
    delete numberOfArguments;
    delete transitionName;
    delete helpString;

    index1 = nullptr;
    index2 = nullptr;
    numberOfArguments = nullptr;
    transitionName = nullptr;
    helpString = nullptr;
    transitionFunction = nullptr;
}

TransitionData &TransitionData::operator=(const TransitionData& other) {
    //  Check for self-assignment
    if (this != &other) {
        this->index1 = new size_t(other.getIndex1());
        this->index2 = new size_t(other.getIndex2());
        this->numberOfArguments = new size_t(other.getNumberOfArguments());
        this->transitionName = new std::string(other.getTransitionName());
        this->helpString = new std::string(other.getHelpString());
        this->transitionFunction = other.transitionFunction;
    }

    return *this;
}

std::ostream &operator<<(std::ostream &os, const TransitionData& transitionData) {
    os << transitionData.getIndex1() << " - " << transitionData.getIndex2() << " - " << transitionData.numberOfArguments
       << " - " << transitionData.getTransitionName() << " - \"" << transitionData.getHelpString() << "\" - "
       << transitionData.transitionFunction;
    return os;
}

bool TransitionData::execute(const std::vector<std::string>& tokens, GameEngine& gameEngine) {
    if (transitionFunction == nullptr) {
        return false;
    }

    return transitionFunction(tokens, gameEngine);
}

//  Standard setter/mutator functions
void TransitionData::setIndex1(size_t newIndex1) { *this->index1 = newIndex1; }

void TransitionData::setIndex2(size_t newIndex2) { *this->index2 = newIndex2; }

void TransitionData::setNumberOfArguments(size_t newNumberOfArguments) { *this->numberOfArguments = newNumberOfArguments; }

void TransitionData::setTransitionName(const std::string& newTransitionName) { *this->transitionName = newTransitionName; }

void TransitionData::setHelpString(const std::string& newHelpString) { *this->helpString = newHelpString; }

void TransitionData::setTransitionFunction(bool (*newTransitionFunction)(const std::vector<std::string>&, GameEngine&))
{ this->transitionFunction = newTransitionFunction; }



//----------------------------------------------------------------------------------------------------------------------
//  "GameEngine" implementations


GameEngine::GameEngine()
    : currentStateIndex(new size_t(0)), statesSize(new size_t(9)), transitionDatabaseSize(new size_t(15)),
      isRunning(new bool(false)), commandProcessor(nullptr), states(new State[0]),
      transitionDatabase(new TransitionData[0]) {
    //  Empty
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
    }

    for (int i = 0; i < *this->transitionDatabaseSize; i++) {
        this->transitionDatabase[i] = otherGameEngine.transitionDatabase[i];
    }

    //  Initialize the command processor member variable (get a copy)
    this->commandProcessor = otherGameEngine.commandProcessor->clone();
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

    //  Initialize the command processor member variable (get a copy)
    this->commandProcessor = otherGameEngine.commandProcessor->clone();

    //  Deallocates the values of the other GameEngine object and sets the pointers to a null pointer
    delete[] otherGameEngine.states;
    delete[] otherGameEngine.transitionDatabase;
    delete otherGameEngine.currentStateIndex;
    delete otherGameEngine.statesSize;
    delete otherGameEngine.transitionDatabaseSize;
    delete otherGameEngine.isRunning;
    delete otherGameEngine.commandProcessor;

    otherGameEngine.states = nullptr;
    otherGameEngine.transitionDatabase = nullptr;
    otherGameEngine.currentStateIndex = nullptr;
    otherGameEngine.statesSize = nullptr;
    otherGameEngine.transitionDatabaseSize = nullptr;
    otherGameEngine.isRunning = nullptr;
    otherGameEngine.commandProcessor = nullptr;
}


GameEngine::~GameEngine() {
    //  Deallocate the values/contents of the pointers
    delete[] states;
    delete[] transitionDatabase;
    delete currentStateIndex;
    delete statesSize;
    delete transitionDatabaseSize;
    delete isRunning;
    delete commandProcessor;

    //  Set the member pointers to null pointer
    states = nullptr;
    transitionDatabase = nullptr;
    currentStateIndex = nullptr;
    statesSize = nullptr;
    transitionDatabaseSize = nullptr;
    isRunning = nullptr;
    commandProcessor = nullptr;
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

        //  Initialize the command processor member variable (get a copy)
        this->commandProcessor = otherGameEngine.commandProcessor->clone();
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

        //  Initialize the command processor member variable (get a copy)
        this->commandProcessor = otherGameEngine.commandProcessor->clone();

        //  Deallocates the values of the other GameEngine object and sets the pointers to a null pointer
        delete[] otherGameEngine.states;
        delete[] otherGameEngine.transitionDatabase;
        delete otherGameEngine.currentStateIndex;
        delete otherGameEngine.statesSize;
        delete otherGameEngine.transitionDatabaseSize;
        delete otherGameEngine.isRunning;
        delete otherGameEngine.commandProcessor;

        otherGameEngine.states = nullptr;
        otherGameEngine.transitionDatabase = nullptr;
        otherGameEngine.currentStateIndex = nullptr;
        otherGameEngine.statesSize = nullptr;
        otherGameEngine.transitionDatabaseSize = nullptr;
        otherGameEngine.isRunning = nullptr;
        otherGameEngine.commandProcessor = nullptr;
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

    while (*isRunning) {
        //  Printing out prompt
        std::string currentStateName = states[*currentStateIndex].getStateName();
        std::cout << "CURRENT STATE:\t[" << currentStateName << "]" << std::endl;
        std::cout << "Please enter a command:\n";

        //  Taking input
        auto command = std::make_unique<Command>(commandProcessor->getCommand(states[*currentStateIndex]));

        //  If input is empty, refresh
        if (command->getRawCommand().empty()) {
            system("cls");
            continue;
        }

        //  Checking if the given transition is valid
        int transitionDataIndex = indexOfTransition(command->getFirstToken());
        if (transitionDataIndex != -1) {
            //  Transition is valid
            //  Isolate the arguments, reduce them into a string, then pass it to be further processed
            std::string argumentsRaw = reduceStringVector(command->getRemainingTokens());
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
    bool status = transitionData.execute(getTokens(argumentsRaw), *this);

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

const State* GameEngine::getStates() const {
    auto* statesCopy = new State[*this->statesSize];

    for (size_t i = 0; i < *this->statesSize; i++)
        statesCopy[i] = State(this->states[i]);

    return statesCopy;
}

const TransitionData* GameEngine::getTransitionsDatabase() const {
    auto* transitionDatabaseCopy = new TransitionData[*this->transitionDatabaseSize];

    for (size_t i = 0; i < *this->transitionDatabaseSize; i++)
        transitionDatabaseCopy[i] = TransitionData(this->transitionDatabase[i]);

    return transitionDatabaseCopy;
}

size_t GameEngine::getStatesSize() const { return *statesSize; }

size_t GameEngine::getTransitionDatabaseSize() const { return *transitionDatabaseSize; }

void GameEngine::setStates(State* newStates, size_t size) {
    //  Deallocate previous values
    delete[] states;
    delete statesSize;

    this->statesSize = new size_t(size);

    //  Deep copying values of passed dynamic array
    this->states = new State[size];
    for (int i = 0; i < *this->statesSize; i++) {
        this->states[i] = State(newStates[i]);
    }
}

void GameEngine::setStates(std::vector<State> newStates) {
    setStates(newStates.data(), newStates.size());
}

void GameEngine::setTransitionData(TransitionData* newTransitionDatabase, size_t size) {
    //  Deallocate previous values
    delete[] transitionDatabase;
    delete transitionDatabaseSize;

    this->transitionDatabaseSize = new size_t(size);

    //  Deep copying values of passed dynamic array
    this->transitionDatabase = new TransitionData[size];
    for (int i = 0; i < *this->transitionDatabaseSize; i++) {
        this->transitionDatabase[i] = TransitionData(newTransitionDatabase[i]);
    }
}

void GameEngine::setTransitionData(std::vector<TransitionData> newTransitionDatabase) {
    setTransitionData(newTransitionDatabase.data(), newTransitionDatabase.size());
}

void GameEngine::setCommandProcessor(const CommandProcessor& newCommandProcessor) {
    this->commandProcessor = newCommandProcessor.clone();
}

void GameEngine::initializeCommandProcessor() {
    //  TODO find a way to make the command processor dependent on the command line arguments
    commandProcessor = new ConsoleCommandProcessorAdapter(states, transitionDatabase, *statesSize,
                                                          *transitionDatabaseSize);

    //  commandProcessor = new FileCommandProcessorAdapter(states, transitionDatabase, *statesSize, *transitionDatabaseSize,
    //                                                     "../commands.txt");
}



#ifdef __GNUC__
#pragma clang diagnostic pop
#endif
