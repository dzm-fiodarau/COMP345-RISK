//----------------------------------------------------------------------------------------------------------------------
//  Macros

#include "../headers/CommandProcessing.h"

//  Include directives
#include <algorithm>
#include <sstream>
#include <fstream>
#include <stdexcept>

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
//  Class: 'Command' implementation

Command::Command()
: rawCommand(new std::string("")), effect(new std::string("NOT EXECUTED")), isValidExecution(nullptr) {

    std::vector<std::string> extractedTokens = getTokens(*rawCommand);
    tokens = new std::vector<std::string>(extractedTokens);
}

Command::Command(const std::string& rawCommand)
: rawCommand(new std::string(rawCommand)), effect(new std::string("NOT EXECUTED")), isValidExecution(nullptr) {

    std::vector<std::string> extractedTokens = getTokens(*(this->rawCommand));
    tokens = new std::vector<std::string>(extractedTokens);
}

Command::Command(const Command& otherCommand) {
    //  Ensure that the value is not a null pointer first.
    this->rawCommand = (otherCommand.rawCommand == nullptr) ? nullptr : new std::string(*otherCommand.rawCommand);
    this->effect = (otherCommand.effect == nullptr) ? nullptr : new std::string(*otherCommand.effect);
    this->isValidExecution = (otherCommand.isValidExecution == nullptr) ? nullptr : new bool(*otherCommand.isValidExecution);
    this->tokens = (otherCommand.tokens == nullptr) ? nullptr : new std::vector<std::string>(*otherCommand.tokens);
}

Command::~Command() {
    //  Deallocate memory pointed to by pointers.
    delete rawCommand;
    delete effect;
    delete isValidExecution;
    delete tokens;

    //  Set to nullptr, removes dangling pointers.
    rawCommand = nullptr;
    effect = nullptr;
    isValidExecution = nullptr;
    tokens = nullptr;
}

Command &Command::operator=(const Command& otherCommand) {
    //  Check for self-assignment
    if (this != &otherCommand) {
        this->rawCommand = (otherCommand.rawCommand == nullptr) ? nullptr : new std::string(*otherCommand.rawCommand);
        this->effect = (otherCommand.effect == nullptr) ? nullptr : new std::string(*otherCommand.effect);
        this->isValidExecution = (otherCommand.isValidExecution == nullptr) ? nullptr : new bool(*otherCommand.isValidExecution);
        this->tokens = (otherCommand.tokens == nullptr) ? nullptr : new std::vector<std::string>(*otherCommand.tokens);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Command& command) {
    unsigned int numberOfInitializedValues = 0;
    std::ostringstream oss;

    if (command.rawCommand != nullptr) {
        numberOfInitializedValues++;
        oss << *command.rawCommand;
    }

    if (command.effect != nullptr) {
        numberOfInitializedValues++;
        oss << " | " << *command.effect;
    }

    if (command.isValidExecution != nullptr) {
        numberOfInitializedValues++;
        oss << " | " << (*command.isValidExecution == 0 ? "false" : "true");
    }

    os << "[" << numberOfInitializedValues << "/3]  "<< oss.str();
    return os;
}

//  GETTER/ACCESSOR METHODS
std::string &Command::getRawCommand() const { return *rawCommand; }

std::string &Command::getEffect() const { return *effect; }

const bool* Command::getIsValidExecution() const { return isValidExecution; }

//  SETTER/MUTATOR METHODS
void Command::setRawCommand(const std::string& newRawCommand) { this->rawCommand = new std::string(newRawCommand); }

void Command::setEffect(const std::string& newEffect) { this->effect = new std::string(newEffect); }

void Command::setExecutionStatus(bool newStatus) {
    if (isValidExecution == nullptr) {
        isValidExecution = new bool(newStatus);
        return;
    }

    *isValidExecution = newStatus;
}

std::string &Command::getFirstToken() const {
    return tokens->front();
}

std::vector<std::string> Command::getRemainingTokens() const {
    std::vector<std::string> remainingTokens(tokens->begin() + 1, tokens->end());
    return remainingTokens;
}

size_t Command::getNumberOfArguments() const {
    return getRemainingTokens().size();
}



//----------------------------------------------------------------------------------------------------------------------
//  Class: 'CommandProcessor' implementation

CommandProcessor::CommandProcessor()
: states(new State[0]), transitionDatabase(new TransitionData[0]), sizeOfStates(new size_t(0)),
  sizeOfTransitionDatabase(new size_t(0)) {

    //  Empty
    DEBUG_PRINT("Called [CommandProcessor, Default Constructor]")
}

CommandProcessor::CommandProcessor(State* states, TransitionData* transitionDatabase, size_t sizeOfStates,
                                   size_t sizeOfTransitionDatabase)
: states(new State[sizeOfStates]), transitionDatabase(new TransitionData[sizeOfTransitionDatabase]),
sizeOfStates(new size_t(sizeOfStates)), sizeOfTransitionDatabase(new size_t(sizeOfTransitionDatabase)) {

    //  Deep copying dynamic array values
    for (size_t i = 0; i < sizeOfStates; i++)
        this->states[i] = State(states[i]);

    for (size_t i = 0; i < sizeOfTransitionDatabase; i++)
        this->transitionDatabase[i] = TransitionData(transitionDatabase[i]);

    //  std::copy(states, states + sizeOfStates, this->states);
    //  std::copy(transitionDatabase, transitionDatabase + sizeOfTransitionDatabase, this->transitionDatabase);
    DEBUG_PRINT("Called [CommandProcessor, Parameterized Constructor (State*, TransitionData*, size_t, size_t)]")
}

CommandProcessor::CommandProcessor(const GameEngine& gameEngine)
: states(gameEngine.getStates()),
  transitionDatabase(gameEngine.getTransitionsDatabase()),
  sizeOfStates(new size_t(gameEngine.getStatesSize())),
  sizeOfTransitionDatabase(new size_t(gameEngine.getTransitionDatabaseSize())) {

    //  Empty
    DEBUG_PRINT("Called [CommandProcessor, Parameterized Constructor (const GameEngine&)]")
}

CommandProcessor::~CommandProcessor() {
    delete[] states;
    delete[] transitionDatabase;
    delete sizeOfStates;
    delete sizeOfTransitionDatabase;

    states = nullptr;
    transitionDatabase = nullptr;
    sizeOfStates = nullptr;
    sizeOfTransitionDatabase = nullptr;

    DEBUG_PRINT("Called [CommandProcessor, DECONSTRUCTOR]")
}


bool CommandProcessor::validate(const Command& command, const State& currentState) {
    //  Get the id of the current state on the states list
    int stateIndex = -1;
    for (size_t i = 0; i < *sizeOfStates; i++) {
        if (states[i] == currentState) {
            stateIndex = static_cast<int>(i);
        }
    }
    if (stateIndex == -1) { // State not found in states list
        return false;
    }

    //  Search for a transition that matches the command transition name given the current state
    for (size_t i = 0; i < *sizeOfTransitionDatabase; i++) {
        if ((transitionDatabase[i].getIndex1() == stateIndex)    // Ensuring states are equal
            && (transitionDatabase[i].getTransitionName() == command.getFirstToken())   // Ensuring transition names eq.
            && (transitionDatabase[i].getNumberOfArguments() == command.getNumberOfArguments())) // Ensuring no of args.
        {
            return true;
        }
    }
    return false;
}

void CommandProcessor::setStates(const State* states, size_t sizeOfStates) {
    this->states = new State[sizeOfStates];
    std::copy(states, states + sizeOfStates, this->states);

    this->sizeOfStates = new size_t(sizeOfStates);
}

void CommandProcessor::setTransitionDatabase(const TransitionData* transitionDatabase, size_t sizeOfTransitionDatabase) {
    this->transitionDatabase = new TransitionData[sizeOfTransitionDatabase];
    std::copy(transitionDatabase, transitionDatabase + sizeOfTransitionDatabase, this->transitionDatabase);

    this->sizeOfTransitionDatabase = new size_t(sizeOfTransitionDatabase);
}



//----------------------------------------------------------------------------------------------------------------------
//  Class: 'FileCommandProcessorAdapter' implementation

FileCommandProcessorAdapter::FileCommandProcessorAdapter()
: CommandProcessor(), filePath(new std::string("")), commandQueue(new std::queue<Command>()) {
    //  Empty
    DEBUG_PRINT("Called [FileCommandProcessorAdapter, Default Constructor]")
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(State* states, TransitionData* transitionDatabase,
                                                         size_t sizeOfStates, size_t sizeOfTransitionDatabase,
                                                         const string& filePath)
: CommandProcessor(states, transitionDatabase, sizeOfStates, sizeOfTransitionDatabase),
  filePath(new std::string(filePath)), commandQueue(new std::queue<Command>()) {

    DEBUG_PRINT("Called [FileCommandProcessorAdapter, Parameterized Constructor (State*, TransitionData*, size_t, size_t, const string&)]")

    //  Load file contents
    loadFileContents();
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const GameEngine& gameEngine, const string& filePath)
: CommandProcessor(gameEngine), filePath(new std::string(filePath)), commandQueue(new std::queue<Command>()) {

    DEBUG_PRINT("Called [FileCommandProcessorAdapter, Parameterized Constructor (const GameEngine&, const string&)]")

    //  Load file contents
    loadFileContents();
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete commandQueue;
    delete filePath;

    commandQueue = nullptr;
    filePath = nullptr;

    DEBUG_PRINT("Called [FileCommandProcessorAdapter, DECONSTRUCTOR]")
}

Command& FileCommandProcessorAdapter::getCommand(const State& currentState) {
    while (!commandQueue->empty()) {
        if (validate(commandQueue->front(), currentState)) {
            auto* commandCopy = new Command(commandQueue->front());     //  Copy command
            commandQueue->pop();                                        //  Dequeue
            return *commandCopy;                                        //  Return
        }

        DEBUG_PRINT("INVALID COMMAND: ", commandQueue->front())
        commandQueue->pop();
    }

    //  TODO, reaching here means there are no more values in the queue, find a way to start reading from console now.
    auto* defaultCommand = new Command();
    return *defaultCommand;
}

void FileCommandProcessorAdapter::loadFileContents() {
    std::ifstream* file = new std::ifstream(*filePath);

    //  Check if the specified file exists
    if (!*file) {
        DEBUG_PRINT("ERROR: FileCommandProcessorAdapter, unable to find file")
        throw std::runtime_error("ERROR: THE FILE COULD NOT BE FOUND");
    }

    //  Read from the file, encapsulate into 'Command' object, and enqueue
    std::string line;
    while (std::getline(*file, line)) {
        commandQueue->push(Command(line));
    }

    //  Deallocate and close all values
    file->close();
    delete file;

    DEBUG_PRINT("SUCCESSFULLY LOADED FILE")
}



//----------------------------------------------------------------------------------------------------------------------
//  Class: 'ConsoleCommandProcessorAdapter' implementation

ConsoleCommandProcessorAdapter::ConsoleCommandProcessorAdapter() : CommandProcessor() {

    //  Empty
    DEBUG_PRINT("Called [ConsoleCommandProcessorAdapter, Default Constructor]")
}

ConsoleCommandProcessorAdapter::ConsoleCommandProcessorAdapter(State* states, TransitionData* transitionDatabase,
                                                               size_t sizeOfStates, size_t sizeOfTransitionDatabase)
: CommandProcessor(states, transitionDatabase, sizeOfStates, sizeOfTransitionDatabase) {

    //  Empty
    DEBUG_PRINT("Called [ConsoleCommandProcessorAdapter, Parameterized Constructor (State*, TransitionData*, size_t, size_t)]")
}

ConsoleCommandProcessorAdapter::ConsoleCommandProcessorAdapter(const GameEngine& gameEngine)
: CommandProcessor(gameEngine) {

    //  Empty
    DEBUG_PRINT("Called [ConsoleCommandProcessorAdapter, Parameterized Constructor (const GameEngine&)]")
}

Command& ConsoleCommandProcessorAdapter::getCommand(const State& currentState) {
    while (true) {
        //  Gets a raw command from the std input stream
        std::string userInput;
        std::getline(std::cin, userInput);

        //  If input string is empty, just continue
        if (userInput.empty()) { continue; }

        //  Encapsulate in a Command object, and check if the command is valid given the current state
        Command command(userInput);
        if (validate(command, currentState)) {
            auto* commandPtr = new Command(command);
            return *commandPtr;
        }

        DEBUG_PRINT("INVALID COMMAND: ", command)
    }
}
