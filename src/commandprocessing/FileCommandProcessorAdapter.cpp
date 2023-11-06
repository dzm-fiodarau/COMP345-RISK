//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "modernize-use-equals-default"
#endif

#include <fstream>

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/gameengine/State.h"
#include "../../headers/commandprocessing/FileCommandProcessorAdapter.h"
#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../../headers/macros/DebugMacros.h"

FileCommandProcessorAdapter::FileCommandProcessorAdapter() : CommandProcessor(), commandQueue(), filePath() {

    //  Initialize the backup command processor
    backupCommandProcessor = std::make_unique<ConsoleCommandProcessorAdapter>(this->states, this->transitionDatabase);

    //  Debug mode print
    DEBUG_PRINT("Called [FileCommandProcessorAdapter, Default Constructor]")
}

/**
 * \brief   Instantiates a 'FileCommandProcessorAdapter' object given a vector of states and transition data.
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::vector<State> states,
                                                         std::vector<TransitionData> transitionDatabase,
                                                         std::string filePath)
        : CommandProcessor(std::move(states), std::move(transitionDatabase)), commandQueue(), filePath(std::move(filePath)) {

    //  Initialize the backup command processor
    backupCommandProcessor = std::make_unique<ConsoleCommandProcessorAdapter>(this->states, this->transitionDatabase);

    //  Load file contents
    loadFileContents();

    //  Debug mode print
    DEBUG_PRINT("Called [FileCommandProcessorAdapter, Parameterized Constructor (std::vector<State>, std::vector<TransitionData>, std::string)]")
}

/**
 * \brief   Instantiate a 'FileCommandProcessorAdapter' object using a given game object.
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const GameEngine& gameEngine, std::string filePath)
        : CommandProcessor(gameEngine.getStates(), gameEngine.getTransitionDatabase()), commandQueue(), filePath(std::move(filePath)) {

    //  Initialize the backup command processor
    backupCommandProcessor = std::make_unique<ConsoleCommandProcessorAdapter>(this->states, this->transitionDatabase);

    //  Load file contents
    loadFileContents();

    //  Debug mode print
    DEBUG_PRINT("Called [FileCommandProcessorAdapter, Parameterized Constructor (const GameEngine&, const string&)]")
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {

    //  Nothing to deallocate
    //  Debug mode print
    DEBUG_PRINT("Called [FileCommandProcessorAdapter, DECONSTRUCTOR]")
}

Command& FileCommandProcessorAdapter::getCommand(const State& currentState) {
    //  Counter to keep track of the number of attempts to get a valid command
    int count = 0;
    //  ANSI escape codes
    std::string AnsiRed = "\033[31m";
    std::string AnsiClear = "\033[0m";

    while (!commandQueue.empty()) {
        if (validate(commandQueue.front(), currentState)) {
            auto* commandCopy = new Command(commandQueue.front());     //  Copy command
            commandQueue.pop();                                        //  Dequeue
            return *commandCopy;                                       //  Return
        }

        //  The first time entering an invalid command, print the list of valid commands to input
        if (count == 0)
            printErrorMenu(getHelpStrings(currentState));

        //  Print invalid command state
        std::cout << AnsiRed << "[" << (count + 1) << "]\t" << "INVALID COMMAND: " << commandQueue.front() << AnsiClear
                  << std::endl;
        count++;
        commandQueue.pop();
    }

    //  Reaching here means there are no more commands to take from the file.
    //  Takes a command from the backup command processor.
    return backupCommandProcessor->getCommand(currentState);
}

void FileCommandProcessorAdapter::loadFileContents() {
    auto* file = new std::ifstream(filePath);

    //  Check if the specified file exists
    if (!*file) {
        DEBUG_PRINT("ERROR: FileCommandProcessorAdapter, unable to find file")
        throw std::runtime_error("ERROR: THE FILE COULD NOT BE FOUND");
    }

    //  Read from the file, encapsulate into 'Command' object, and enqueue
    std::string line;
    while (std::getline(*file, line)) {
        commandQueue.emplace(line);
    }

    //  Deallocate and close all values
    file->close();
    delete file;

    DEBUG_PRINT("SUCCESSFULLY LOADED FILE")
}

FileCommandProcessorAdapter* FileCommandProcessorAdapter::clone() const noexcept {
    return new FileCommandProcessorAdapter(states, transitionDatabase, filePath);
}