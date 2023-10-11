#include <iostream>
#include "../headers/CommandProcessing.h"
#include "../headers/GameEngine.h"

void testCommandProcessing() {
    auto* gameEngine = new GameEngine();
    gameEngine->execute();
}