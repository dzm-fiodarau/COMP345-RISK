#include <iostream>
#include "../headers/GameEngine.h"

bool something(const std::string& ignored, GameEngine& gameEngine) {
    return true;

}

void testGameStates()
{
    GameEngine* firstGameEngine = new GameEngine();
    //firstGameEngine->execute();

    std::cout << *firstGameEngine << std::endl;
}

int main() {
    testGameStates();
    return 0;
}