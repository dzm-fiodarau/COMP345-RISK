#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/gameengine/State.h"
#include "../../headers/gameengine/TransitionFunctions.h"

#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../../headers/commandprocessing/FileCommandProcessorAdapter.h"




void testGameStates()
{
    /*
    //  Dependencies instantiation
    std::vector<State> states = { STATES_CONTENTS };
    std::vector<TransitionData> transitionDatabase = { TRANSITIONS_CONTENTS };
    auto commandProcessor = std::make_unique<FileCommandProcessorAdapter>(states, transitionDatabase, "../../commands.txt");


    //  Main objects
    //  Dependency Injection
    auto gameEngine = std::make_unique<GameEngine>();
    gameEngine->setStates(states);
    gameEngine->setTransitionData(transitionDatabase);
    gameEngine->setCommandProcessor(*commandProcessor);

    //  Run
    gameEngine->execute();
     */

    //  Instantiating the states
    auto* start = new State("start");
    auto* mapLoaded = new State("map loaded");
    auto* mapValidated = new State("map validated");
    auto* playersAdded = new State("players added");
    auto* assignReinforcement = new State("assign reinforcement");
    auto* issueOrders = new State("issue orders");
    auto* executeOrders = new State("execute orders");
    auto* win = new State("win");
    auto* end = new State("END");

    //  Inserting the transitions
    start->addTransition("loadmap", mapLoaded, 1, "loadmap [--filepath]", &game_loadMap);
    mapLoaded->addTransition("loadmap", mapLoaded, 1, "loadmap [--filepath]", &game_loadMap);
    mapLoaded->addTransition("validatemap", mapValidated, 0, "validatemap", &game_validateMap);
    mapValidated->addTransition("addplayer", playersAdded, 1, "addplayer [--playername]", &game_addPlayer);
    playersAdded->addTransition("addplayer", playersAdded, 1, "addplayer [--playername]", &game_addPlayer);
    playersAdded->addTransition("viewplayers", playersAdded, 0, "viewplayers", &game_printPlayers);
    playersAdded->addTransition("gamestart", assignReinforcement, 0, "gamestart", &game_gameStart);
    assignReinforcement->addTransition("issueorder", issueOrders, 0, "issueorder -SAMPLE ARGUMENTS-", &game_issueOrder);
    issueOrders->addTransition("issueorder", issueOrders, 0, "issueorder -SAMPLE ARGUMENTS-", &game_issueOrder);
    issueOrders->addTransition("endissueorders", executeOrders, 0, "endissueorders", &game_endIssueOrders);
    executeOrders->addTransition("execorder", executeOrders, 0, "execorder -SAMPLE ARGUMENTS-", &game_executeOrder);
    executeOrders->addTransition("endexecorders", assignReinforcement, 0, "endexecorders", &game_endExecuteOrders);
    executeOrders->addTransition("win", win, 0, "win", &game_winGame);
    win->addTransition("replay", start, 0, "replay", &game_restart);
    win->addTransition("quit", end, 0, "quit", &game_quit);

    //  Initialize required objects
    std::vector<State*> states = { start, mapLoaded, mapValidated, playersAdded, assignReinforcement, issueOrders, executeOrders, win, end };
    CommandProcessor* commandProcessor = new FileCommandProcessorAdapter("../../commands.txt");
    auto* gameEngine = new GameEngine(states, commandProcessor);

    gameEngine->execute();

    delete gameEngine;
}