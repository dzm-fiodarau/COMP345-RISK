void testCards();
void testGameStates();
void testLoadMaps();
void testOrdersLists();
void testOrderExecution();
void testPlayers();
void testCommandProcessing();

//  Regular runtime
#ifndef DEBUG
int main()
{
    // testLoadMaps();
    // testOrdersLists();
    testOrderExecution();
    // testCards();
    // testPlayers();
    testGameStates();
    testCommandProcessing();

    return 0;
}
#endif

#ifdef DEBUG
int main()
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
    testCommandProcessing();
#endif

    return 0;
}
#endif