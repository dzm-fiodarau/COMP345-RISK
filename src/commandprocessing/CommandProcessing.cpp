//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "readability-use-anyofallof"
#pragma ide diagnostic ignored "modernize-use-equals-default"
#endif

#include <sstream>
#include <utility>

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/gameengine/State.h"
#include "../../headers/commandprocessing/CommandProcessing.h"
#include "../../headers/macros/DebugMacros.h"

//  STATIC FUNCTION
/** \brief Splits a string by spaces and returns a vector of string tokens. */
static std::vector<std::string> getTokens(std::string input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    while (iss >> input) {
        tokens.push_back(input);
    }
    return tokens;
}

//  STATIC FUNCTION
/** \brief Prints an error block in case an incorrect raw command was inputted. */
void CommandProcessor::printErrorMenu(std::vector<std::string> helpStrings) {
    //  ANSI escape codes
    std::string AnsiRed = "\033[31m";
    std::string AnsiClear = "\033[0m";

    std::cout << AnsiRed << "ERROR, List of valid commands:" << AnsiClear << std::endl;
    for (size_t i = 0; i < helpStrings.size(); i++) {
        std::cout << AnsiRed << "\t(" << (i + 1) << ") \"" << helpStrings[i] << "\"" << AnsiClear << std::endl;
    }
    std::cout << std::endl;
}




//----------------------------------------------------------------------------------------------------------------------
//  Class: 'Command' implementation

Command::Command() : Command("") {
    //  Empty
}

Command::Command(const std::string& rawCommand) : Command(rawCommand, "NOT EXECUTED", nullptr) {
    //  Empty
}

Command::Command(const std::string& rawCommand, std::string effect, const bool* isValidExecution)
        : rawCommand(rawCommand), effect(std::move(effect)), isValidExecution(new bool(isValidExecution)), tokens(getTokens(rawCommand)) {
    //  Empty
}

Command::Command(const Command& otherCommand)
        : Command(otherCommand.getRawCommand(), otherCommand.getEffect(), otherCommand.getIsValidExecution()) {
    //  Empty
}

Command::~Command() {
    //  Deallocate memory pointed to by pointers.
    delete isValidExecution;

    //  Set to nullptr, removes dangling pointers.
    isValidExecution = nullptr;
}

Command &Command::operator=(const Command& otherCommand) {
    //  Check for self-assignment
    if (this != &otherCommand) {
        this->rawCommand = otherCommand.getRawCommand();
        this->effect = otherCommand.getEffect();
        this->isValidExecution = new bool(otherCommand.getIsValidExecution());
        this->tokens = getTokens(this->rawCommand);     //  Works since member variable is already updated
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Command& command) {
    if (command.isValidExecution == nullptr) {
        os << "[1/3]  "<< "\"" << command.rawCommand << "\"";
    } else {
        os << "[3/3]  "
           << "\"" << command.rawCommand << "\" | "
           << "\"" << command.effect << "\" | "
           << (*command.isValidExecution == 0 ? "false" : "true");
    }
    return os;
}

//  GETTER/ACCESSOR METHODS
/** \brief Gets the raw command. */
std::string Command::getRawCommand() const { return rawCommand; }

/** \brief Gets the effect of the command. */
std::string Command::getEffect() const { return effect; }

/** \brief Gets the execution status of the command.
 * \return Bool pointer: 'nullptr' for an un-executed command, false for invalid execution, true otherwise.*/
const bool* Command::getIsValidExecution() const { return isValidExecution; }

//  SETTER/MUTATOR METHODS
/** \brief Sets the value of the raw command member variable. */
void Command::setRawCommand(const std::string& newRawCommand) { rawCommand = newRawCommand; }

/** \brief Sets the value of the effect member variable. */
void Command::setEffect(const std::string& newEffect) { effect = newEffect; }

/** \brief  Sets the execution status of the command
 *  \param newStatus The new status. False for unsuccessful execution, true otherwise.
 *  \remarks Member variable originally set to a 'nullptr', which indicates that the command has not been executed yet.
 */
void Command::setExecutionStatus(bool newStatus) {
    if (isValidExecution == nullptr) {
        isValidExecution = new bool(newStatus);
        return;
    }

    *isValidExecution = newStatus;
}

std::string Command::getFirstToken() const {
    return tokens.front();
}

std::vector<std::string> Command::getRemainingTokens() const {
    std::vector<std::string> remainingTokens(tokens.begin() + 1, tokens.end());     //  Get a subset of the vector
    return remainingTokens;
}

size_t Command::getNumberOfArguments() const {
    return getRemainingTokens().size();
}



//----------------------------------------------------------------------------------------------------------------------
//  Class: 'CommandProcessor' implementation

CommandProcessor::CommandProcessor() : CommandProcessor({}, {}) {
    //  Empty
    DEBUG_PRINT("Called [CommandProcessor, Default Constructor]")
}

CommandProcessor::CommandProcessor(std::vector<State> states, std::vector<TransitionData> transitionDatabase)
    : states(std::move(states)), transitionDatabase(std::move(transitionDatabase)) {
    //  Empty
    DEBUG_PRINT("Called [CommandProcessor, Parameterized Constructor (std::vector<State>, std::vector<TransitionData>)]")
}

CommandProcessor::CommandProcessor(const GameEngine& gameEngine)
    : states(gameEngine.getStates()), transitionDatabase(gameEngine.getTransitionDatabase()) {
    //  Empty
    DEBUG_PRINT("Called [CommandProcessor, Parameterized Constructor (const GameEngine&)]")
}

CommandProcessor::~CommandProcessor() {
    //  Empty
    //  Nothing to deallocate
    DEBUG_PRINT("Called [CommandProcessor, DECONSTRUCTOR]")
}


/**
 * \brief Checks if the given command is valid given a current state.
 * \param command       The command object to validate
 * \param currentState  The current state in the
 * \return True if command is valid, false otherwise
 *
 * TODO CHECK IF VALID, RE WRITTEN
 */
bool CommandProcessor::validate(const Command& command, const State& currentState) {
    //  Get the id of the current state on the states list
    int stateIndex = -1;
    for (size_t i = 0; i < states.size(); i++) {
        if (states[i] == currentState) {
            stateIndex = static_cast<int>(i);
        }
    }
    if (stateIndex == -1) { // State not found in states list
        return false;
    }

    //  A function that takes in a transition data object and returns true if its index1 equals the variable 'stateIndex'
    auto equalState = [stateIndex] (const TransitionData& transitionData) -> bool { return transitionData.getIndex1() == stateIndex; };
    //  Copy of the transition database
    auto databaseCopy = std::vector<TransitionData>(transitionDatabase);
    //  Erases all entries whose 'index1' value does not equal the value of the local variable 'stateIndex'
    databaseCopy.erase(std::remove_if(databaseCopy.begin(), databaseCopy.end(), equalState), databaseCopy.end());

    //  Iterates through the array and searches whether the data in the command object matches one of the transition data objects
    for (const auto& transitionData : transitionDatabase) {
        if (transitionData.getTransitionName() == command.getFirstToken() &&
            transitionData.getNumberOfArguments() == command.getNumberOfArguments()) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> CommandProcessor::getHelpStrings(const State& currentState) const {
    //  Get the index of the passed states in the states list
    int currentStateIndex = -1;
    for (size_t i = 0; i < states.size(); i++) {
        if (states[i] == currentState) {
            currentStateIndex = static_cast<int>(i);
            break;
        }
    }

    //  Iterate through the transition database, adding any help strings to the collection
    std::vector<std::string> helpStrings;
    for (const auto& transitionData : transitionDatabase) {
        if (transitionData.getIndex1() == currentStateIndex) {
            helpStrings.push_back(transitionData.getHelpString());
        }
    }

    return helpStrings;
}

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif