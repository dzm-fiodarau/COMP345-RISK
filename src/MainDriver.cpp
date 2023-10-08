void testCards();
void testGameStates();
void testLoadMaps();
void testOrdersLists();
void testPlayers();
void testCommandProcessing(int, char** argv);

#include <iostream>

//  Regular runtime
#ifndef DEBUG
int main(int argc, char *argv[])
{
    testLoadMaps();
    testOrdersLists();
    testCards();
    testPlayers();
    testGameStates();
    testCommandProcessing();

    return 0;
}
#endif



#ifdef DEBUG
int main(int argc, char** argv)
{
#ifdef EXECUTE_CARDS
    testCards();
#endif

#ifdef EXECUTE_MAP
    testLoadMaps();
#endif

#ifdef EXECUTE_ORDERS
    testOrdersLists();
#endif

#ifdef EXECUTE_PLAYER
    testPlayers();
#endif

#ifdef EXECUTE_GAME_ENGINE
    testGameStates();
#endif

#ifdef EXECUTE_COMMAND_PROCESSING
    //  Calling the driver test function, taking in the program arguments
    testCommandProcessing(argc, argv);
#endif

    return 0;
}
#endif