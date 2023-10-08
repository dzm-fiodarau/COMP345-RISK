#include <iostream>
#include "../headers/CommandProcessing.h"

//  Forward declaration, functions
CommandProcessor& getCommandProcessing(int, char**, const GameEngine&);


void testCommandProcessing(int argc, char** argv) {
    auto* gameEngine = new GameEngine();

    getCommandProcessing(argc, argv, *gameEngine);

    //  FileCommandProcessorAdapter* fcpa = new FileCommandProcessorAdapter(*gameEngine, "../commands.txt");

    //  std::cout << fcpa->getCommand(State("start")) << std::endl;
    //  std::cout << fcpa->getCommand(State("map loaded")) << std::endl;
    //  std::cout << fcpa->getCommand(State("map validated")) << std::endl;
    //  std::cout << fcpa->getCommand(State("players added")) << std::endl;

    delete gameEngine;
}


CommandProcessor& getCommandProcessing(int argc, char** argv, const GameEngine& gameEngine) {
    //  Isolate arguments into a string array of tokens, makes it easier to manipulate
    size_t size = argc - 1;
    auto* arguments = new std::string[size];
    for (size_t i = 0; i < size; i++) {
        arguments[i] = (argv[i + 1]);
    }

    //  Looking for the '-file' identifier
    for (size_t i = 0; i < size - 1; i++) {
        if ((arguments[i] == "-file")) {
            std::string filePath = arguments[i + 1];

            try {
                auto* fcpa = new FileCommandProcessorAdapter(gameEngine, filePath);
                return *fcpa;
            } catch (const std::runtime_error& re) {
                std::cout << "ERROR:" << re.what() << std::endl;

                //  Return a console command processor (default) instead
                auto* ccpa = new ConsoleCommandProcessorAdapter(gameEngine);
                return *ccpa;
            }
        }
    }
    if (arguments[size - 1] == "-file") {
        std::cout << "NO FILE PATH PROVIDED AFTER '-file'" << std::endl;
    }


    //  Return console command processor as default
    auto* ccpa = new ConsoleCommandProcessorAdapter(gameEngine);
    return *ccpa;
}