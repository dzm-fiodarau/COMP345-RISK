#include <iostream>
#include "../../headers/commandprocessing/CommandProcessing.h"
#include "../../headers/gameengine/GameEngine.h"

void testCommandProcessing() {
    auto* gameEngine = new GameEngine();
    gameEngine->execute();
}