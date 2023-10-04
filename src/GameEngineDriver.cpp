#include <iostream>
#include "../headers/GameEngine.h"

void testGameStates()
{
    GameEngine *gameEngine = &GameEngine::getInstance();
    gameEngine->execute();
}