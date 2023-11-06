#include <memory>

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/gameengine/State.h"
#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../../headers/macros/DebugMacros.h"

ConsoleCommandProcessorAdapter::ConsoleCommandProcessorAdapter() : CommandProcessor() {
    //  Empty
    DEBUG_PRINT("Called [ConsoleCommandProcessorAdapter, Default Constructor]")
}

ConsoleCommandProcessorAdapter::ConsoleCommandProcessorAdapter(std::vector<State> states, std::vector<TransitionData> transitionDatabase)
        : CommandProcessor(std::move(states), std::move(transitionDatabase)) {
    //  Empty
    DEBUG_PRINT("Called [ConsoleCommandProcessorAdapter, Parameterized Constructor (std::vector<State>, st::vector<TransitionData>)]")
}

ConsoleCommandProcessorAdapter::ConsoleCommandProcessorAdapter(const GameEngine& gameEngine)
        : CommandProcessor(gameEngine) {
    //  Empty
    DEBUG_PRINT("Called [ConsoleCommandProcessorAdapter, Parameterized Constructor (const GameEngine&)]")
}

Command& ConsoleCommandProcessorAdapter::getCommand(const State& currentState) {
    //  Counter to keep track of the number of attempts to get a valid command
    int count = 0;
    //  ANSI escape codes
    std::string AnsiRed = "\033[31m";
    std::string AnsiClear = "\033[0m";

    while (true) {
        //  Gets a raw command from the std input stream
        std::string userInput;
        std::getline(std::cin, userInput);

        //  If input string is empty, just continue
        if (userInput.empty()) { continue; }

        //  Encapsulate in a Command object, and check if the command is valid given the current state
        auto command = std::make_unique<Command>(userInput);
        if (validate(*command, currentState)) {
            auto* commandPtr = new Command(*command);
            return *commandPtr;
        }

        //  INVALID COMMAND CODE
        //  The first time entering an invalid command, print the list of valid commands to input
        if (count == 0)
            printErrorMenu(getHelpStrings(currentState));

        //  Print invalid command state
        std::cout << AnsiRed << "[" << (count + 1) << "]\t" << "INVALID COMMAND: " << *command << AnsiClear << std::endl;
        count++;
    }
}

ConsoleCommandProcessorAdapter* ConsoleCommandProcessorAdapter::clone() const noexcept {
    return new ConsoleCommandProcessorAdapter(states, transitionDatabase);
}