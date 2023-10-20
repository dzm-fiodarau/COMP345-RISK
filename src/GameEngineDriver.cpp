#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "../headers/GameEngine.h"
#include "../headers/CommandProcessing.h"



#define STATES_CONTENTS                                                                                             \
            State("start"), State("map loaded"), State("map validated"), State("players added"),                    \
            State("assign reinforcement"), State("issueorder"), State("execute orders"), State("win"), State("END") \

#define TRANSITIONS_CONTENTS                                                                                        \
            TransitionData(0, 1, 1, "loadmap", "loadmap [--filepath]", &game_loadMap),                              \
            TransitionData(1, 1, 1, "loadmap", "loadmap [--filepath]", &game_loadMap),                              \
            TransitionData(1, 2, 0, "validatemap", "validatemap", &game_validateMap),                               \
            TransitionData(2, 3, 1, "addplayer", "addplayer [--playername]", &game_addPlayer),                      \
            TransitionData(3, 3, 1, "addplayer", "addplayer [--playername]", &game_addPlayer),                      \
            TransitionData(3, 3, 0, "viewplayers", "viewplayers", &game_printPlayers),                              \
            TransitionData(3, 4, 0, "gamestart", "gamestart", &game_gameStart),                                     \
            TransitionData(4, 5, 0, "issueorder", "issueorder -SAMPLE ARGUMENTS-", &game_issueOrder),               \
            TransitionData(5, 5, 0, "issueorder", "issueorder -SAMPLE ARGUMENTS-", &game_issueOrder),               \
            TransitionData(5, 6, 0, "endissueorders", "endissueorders", &game_endIssueOrders),                      \
            TransitionData(6, 6, 0, "execorder", "execorder -SAMPLE ARGUMENTS-", &game_executeOrder),               \
            TransitionData(6, 4, 0, "endexecorders", "endexecorders", &game_endExecuteOrders),                      \
            TransitionData(6, 7, 0, "win", "win", &game_winGame),                                                   \
            TransitionData(7, 0, 0, "replay", "replay", &game_restart),                                             \
            TransitionData(7, 8, 0, "quit", "quit", &game_quit)                                               \



void testGameStates()
{
    //  Dependencies instantiation
    std::vector<State> states = { STATES_CONTENTS };
    std::vector<TransitionData> transitionDatabase = { TRANSITIONS_CONTENTS };
    auto commandProcessor = std::make_unique<ConsoleCommandProcessorAdapter>(states, transitionDatabase);


    //  Main objects
    //  Dependency Injection
    auto gameEngine = std::make_unique<GameEngine>();
    gameEngine->setStates(states);
    gameEngine->setTransitionData(transitionDatabase);
    gameEngine->setCommandProcessor(*commandProcessor);

    //  Run
    gameEngine->execute();
}