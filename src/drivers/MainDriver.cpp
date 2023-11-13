void testCards();
void testGameStates(int, char**);
void testLoadMaps();
void testOrdersLists();
void testOrderExecution();
void testPlayers();
void testLoggingObserver();

int main(int argc, char *argv[])
{
    //  testLoadMaps();
    //  testOrdersLists();
    //  testOrderExecution();
    //  testCards();
    //  testPlayers();
    // testLoggingObserver();
    testGameStates(argc, argv);

    return 0;
}
