#include <cstring>
#include <iostream>


#include "../../headers/gameengine/GameEngine.h"        //  TODO: Command processor inheritance only works w/ this include directive. Figure out why
#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../../headers/commandprocessing/FileCommandProcessorAdapter.h"

void testCards();
void testGameStates(CommandProcessor*);
void testLoadMaps();
void testOrdersLists();
void testOrderExecution();
void testPlayers();
void testLoggingObserver(CommandProcessor*);

int main(int argc, char *argv[])
{
    //  Determining what command processor to use
    CommandProcessor* commandProcessor = new ConsoleCommandProcessorAdapter();
    //  If there are arguments
    if (argc > 1) {
        if (std::strcmp(*(argv + 1), "-console") == 0 && argc == 2) {
            commandProcessor = new ConsoleCommandProcessorAdapter();
        } else if (std::strcmp(*(argv + 1), "-file") == 0 && argc == 3) {
            commandProcessor = new FileCommandProcessorAdapter(*(argv + 2));
        } else {
            std::cerr << "ERROR: Incorrect arguments provided" << std::endl;
            return 0;
        }
    }

    //  testLoadMaps();
    //  testOrdersLists();
    //  testOrderExecution();
    //  testCards();
    //  testPlayers();
    //  testLoggingObserver(commandProcessor);
    testGameStates(commandProcessor);

    return 0;
}
