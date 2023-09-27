#include <iostream>
#include "../include/GameEngine.h"

void testGameStates();

int main() {
    testGameStates();
    return 0;
}

void testGameStates() {
    GameEngine* gameEngine = &GameEngine::getInstance();
    gameEngine->execute();
}