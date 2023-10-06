void testCards();
void testGameStates();
void testLoadMaps();
void testOrdersLists();
void testPlayers();

//  Regular runtime
#ifndef DEBUG
int main()
{
    testOrdersLists();
    testCards();
    testPlayers();
    testGameStates();
    testLoadMaps();

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

    return 0;
}
#endif