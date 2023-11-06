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

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/gameengine/State.h"

#include "../../headers/macros/DebugMacros.h"

#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"

#include "../../headers/Map.h"
#include "../../headers/Player.h"

#define PRESS_ENTER_TO_CONTINUE(clearConsole)                       \
    std::string _IGNORE_STRING;                                     \
    std::cout << "Press Enter to Continue... ";                     \
    std::getline(std::cin, _IGNORE_STRING);                         \
                                                                    \
    if (clearConsole) {                                             \
        system("cls");                                              \
    }                                                               \




//----------------------------------------------------------------------------------------------------------------------
//  Static functions



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
//  "TransitionData" implementations

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
    : GameEngine({}, {}, nullptr) {

    //  Empty
    DEBUG_PRINT("Called [GameEngine, Default Constructor]")
}

GameEngine::GameEngine(std::vector<State> states, std::vector<TransitionData> transitionDatabase,
                       CommandProcessor* commandProcessor) : states(std::move(states)),
                       transitionDatabase(std::move(transitionDatabase)), currentStateIndex(0), isRunning(false),
                       map(nullptr) {

    //  In-case the passed command processor is a nullptr, set default to console command processor
    //  Otherwise, get a copy/clone of the passed command processor
    if (commandProcessor == nullptr) {
        this->commandProcessor = new ConsoleCommandProcessorAdapter(this->states, this->transitionDatabase);
    } else {
        this->commandProcessor = commandProcessor->clone();
    }

    DEBUG_PRINT("Called [GameEngine, Parameterized Constructor (std::vector<State>, std::vector<TransitionDatabase>, CommandProcessor*)]")
}

GameEngine::GameEngine(const GameEngine& other) : GameEngine(other.getStates(), other.getTransitionDatabase(), other.commandProcessor) {

    //  Empty
    DEBUG_PRINT("Called [GameEngine, Parameterized Constructor (const GameEngine&)]")
}


GameEngine::~GameEngine() {
    //  Delete/Deallocate all player objects
    for (auto* playerPtr : players) {
        delete playerPtr;
    }

    //  Deallocate other pointer objects
    delete commandProcessor;
    delete map;
    commandProcessor = nullptr;
    map = nullptr;
}


GameEngine& GameEngine::operator=(const GameEngine& otherGameEngine) {
    //  Check for self-assignment
    if (this != &otherGameEngine) {
        this->currentStateIndex = otherGameEngine.currentStateIndex;
        this->isRunning = otherGameEngine.isRunning;
        this->states = otherGameEngine.states;
        this->transitionDatabase = otherGameEngine.transitionDatabase;
        this->commandProcessor = otherGameEngine.commandProcessor->clone();
        this->players = otherGameEngine.getPlayers();

        //  Checks if map is not a nullptr. If not, creates a copy.
        this->map = (otherGameEngine.map == nullptr) ? nullptr : new Map(*otherGameEngine.map);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const GameEngine& gameEngine) {
    os << "STATES:\n";
    for (int i = 0; i < gameEngine.states.size(); i++) {
        os << "[" << i << "] " << gameEngine.states[i] << std::endl;
    }

    os << "\nTRANSITIONS:\n";
    for (int i = 0; i < gameEngine.transitionDatabase.size(); i++) {
        os << "[" << i << "] " << gameEngine.transitionDatabase[i] << std::endl;
    }

    os << "\nCURRENT STATE: " << gameEngine.states[gameEngine.currentStateIndex].getStateName();
    os << "\nIS RUNNING:    " << (gameEngine.isRunning ? "true" : "false");
    return os;
}

/** Implementation Details:
 *  - First, the method checks if all of the member variables have been instantiated and provide some info. There must
 *    at least one state and at least one TransitionData object. Furthermore, a CommandProcessor object must be given.
 *
 *  - Second the game loop then starts, starting when a member variable 'isRunning' is set to true, and ending when set
 *    to false.
 *    The client is prompted and then input is taken through the command processor object. Command is encapsulated in a
 *    'Command' object. The command is then verified. If valid, then it is executed by calling the respective transition
 *    function. If invalid, then an error message is printed.
 */
void GameEngine::execute() {
    //  Checks if all the required variables have been instantiated and checks for the presence of any data
    if (states.empty() || transitionDatabase.empty() || commandProcessor == nullptr) {
        std::cerr << "ERROR: GameEngine not fully instantiated. Dependencies missing" << std::endl;
        return;
    }

    //  Set the isRunning status variable to 'true'
    isRunning = true;

    while (isRunning) {
        //  Printing out prompt
        std::string currentStateName = states[currentStateIndex].getStateName();
        std::cout << "CURRENT STATE:\t[" << currentStateName << "]" << std::endl;
        std::cout << "Please enter a command:\n";

        //  Taking input
        auto command = std::make_unique<Command>(commandProcessor->getCommand(states[currentStateIndex]));

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
            std::cerr << "\033[1;31m" << "INVALID COMMAND. View the valid commands below:" << "\033[0m";
            std::cerr << getHelpStrings() << std::endl;
        }

        //  Press enter to continue.
        PRESS_ENTER_TO_CONTINUE(true)
    }
}

/**
 * \brief Flags the game engine to stop running.
 */
void GameEngine::stopRunning() {
    this->isRunning = false;
}

/**
 * \brief Returns the corresponding TransitionData object using the currentStateIndex member variable and the given
 *        transition name.
 * \return  Returns the index of the corresponding TransitionData object in the transitionDatabase vector
 *
 * Implementation details:
 * Iterates through the transition database vector, comparing transition.index1 and transition.transitionName to
 * 'currentStateIndex' and 'givenTransitionName' respectively for equality. Returns the index of the first match, -1
 * otherwise.
 */
int GameEngine::indexOfTransition(const std::string& givenTransitionName) const {
    for (int i = 0 ; i < transitionDatabase.size(); i++) {
        TransitionData transitionData = transitionDatabase[i];
        if ((transitionData.getIndex1() == currentStateIndex)
            && (transitionData.getTransitionName() == givenTransitionName)) {
            return i;
        }
    }

    return -1;
}

/**
 * \brief Returns a string of all the help strings associated with each transition in the current state.
 *
 * Implementation details:
 * Iterates through the transition database vector, comparing the 'transition.index1' to 'currentStateIndex'. Equality
 * indicates a possible transition from the given state to another. For each transition data, take the help string and
 * append to the return string.
 */
std::string GameEngine::getHelpStrings() const {
    std::string stringBuilder;
    for (const auto& transitionData : transitionDatabase) {
        if (transitionData.getIndex1() == currentStateIndex) {
            stringBuilder += "\n\"" + transitionData.getHelpString() + "\"";
        }
    }
    return stringBuilder;
}

/**
 * \brief Further processes a command, calling its respective transition function.
 *
 * Implementation details:
 * Calls the respective transition function given a command. A 'true' status indicates a successful operation. 'False'
 * indicates an error with the provided arguments or an error somewhere down the line of execution. If 'true', then
 * we transition into the next state, updating the 'currentStateIndex' member variable.
 */
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
    currentStateIndex = transitionData.getIndex2();

    //  Print that you have switched states
    std::string newCurrentStateName = states[currentStateIndex].getStateName();
    std::cout << "\033[34m" << "SWITCHED STATES TO:\t[" << newCurrentStateName << "]\033[0m" << std::endl;
}

//  Getter/Accessor methods
std::vector<State> GameEngine::getStates() const { return states; }
std::vector<TransitionData> GameEngine::getTransitionDatabase() const { return transitionDatabase; }
bool GameEngine::isGameRunning() const { return isRunning; }
std::vector<Player*> GameEngine::getPlayers() const { return players; }
Map* GameEngine::getMap() const { return map; }

//  Setter/Mutator methods
void GameEngine::setStates(std::vector<State> newStates) { this->states = std::move(newStates); }
void GameEngine::setTransitionData(std::vector<TransitionData> newTransitionDatabase) { this->transitionDatabase = std::move(newTransitionDatabase); }
void GameEngine::setCommandProcessor(const CommandProcessor& newCommandProcessor) { this->commandProcessor = newCommandProcessor.clone(); }
void GameEngine::setMap(Map* newMap) { this->map = newMap; }

void GameEngine::setPlayers(std::vector<Player*> newPlayers) {
    for (auto* playerPtr : this->players) {
        delete playerPtr;
    }

    this->players = std::move(newPlayers);
}

void GameEngine::addPlayer(Player&& playerData) {
    auto* newPlayer = new Player();
    newPlayer->setPlayerName(playerData.getPlayerName());
    newPlayer->setOrdersList(playerData.getOrdersList());
    newPlayer->setHandCards(playerData.getHandCards());
    newPlayer->setTerritories(playerData.getTerritories());

    playerData.setPlayerName("");
    playerData.setOrdersList(nullptr);
    playerData.setHandCards({});
    playerData.setTerritories({});

    players.push_back(newPlayer);
}


#ifdef __GNUC__
#pragma clang diagnostic pop
#endif
