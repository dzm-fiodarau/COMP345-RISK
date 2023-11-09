void testCards();
void testGameStates();
void testLoadMaps();
void testOrdersLists();
void testOrderExecution();
void testPlayers();
void testCommandProcessing();
void testLoggingObserver();

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
    testLoggingObserver();

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

#ifdef EXECUTE_LOGGING_OBSERVER
    testLoggingObserver();
#endif

    return 0;
}
#endif