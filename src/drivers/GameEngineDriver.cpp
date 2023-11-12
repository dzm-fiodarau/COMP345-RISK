#include <vector>
#include <iostream>
#include <cstring>

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/gameengine/TransitionFunctions.h"
#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../../headers/commandprocessing/FileCommandProcessorAdapter.h"

void testGameStates(int argc, char* argv[])
{
    CommandProcessor* commandProcessor = new ConsoleCommandProcessorAdapter();
    //  If there are arguments
    if (argc > 1) {
        if (strcmp(*(argv + 1), "-console") == 0 && argc == 2) {
            commandProcessor = new ConsoleCommandProcessorAdapter();
        } else if (strcmp(*(argv + 1), "-file") == 0 && argc == 3) {
            std::cout << *(argv + 2) << std::endl;
            commandProcessor = new FileCommandProcessorAdapter(*(argv + 2));
        } else {
            std::cerr << "ERROR: Incorrect arguments provided" << std::endl;
            return;
        }
    }



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

    auto* tournament = new State("tournament");

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

    start->addTransition("tournament", tournament, -1, "tournament -M [listofmapfiles] -P [listofplayerstrategies] -G [numberofgames] -D [maxnumberofturns]", &game_tournament);
    tournament->addTransition("replay", start, 0, "replay", &game_emptyFunction);

    //  Initialize required objects
    std::vector<State*> states = { start, mapLoaded, mapValidated, playersAdded, assignReinforcement, issueOrders, executeOrders, win, end };
    auto* gameEngine = new GameEngine(states, commandProcessor);

    gameEngine->execute();

    delete gameEngine;
}