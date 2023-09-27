#include <iostream>
#include <string>
#include <vector>
#include "../include/GameEngine.h"

int main() {

    GameEngine& gameEngine = GameEngine::getInstance();
    gameEngine.execute();

    return 0;
}