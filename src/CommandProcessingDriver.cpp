#include <iostream>
#include "../headers/CommandProcessing.h"

void testCommandProcessing() {
    auto* gameEngine = new GameEngine();

    //  FileCommandProcessorAdapter* fcpa = new FileCommandProcessorAdapter(*gameEngine, "../commands.txt");

    //  std::cout << fcpa->getCommand(State("start")) << std::endl;
    //  std::cout << fcpa->getCommand(State("map loaded")) << std::endl;
    //  std::cout << fcpa->getCommand(State("map validated")) << std::endl;
    //  std::cout << fcpa->getCommand(State("players added")) << std::endl;

    auto* ccpa = new ConsoleCommandProcessorAdapter(*gameEngine);
    std::cout << ccpa->getCommand(State("start")) << std::endl;
}