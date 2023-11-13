#include <iostream>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "../headers/gameengine/GameEngine.h"
#include "../headers/gameengine/State.h"
#include "../headers/gameengine/TransitionFunctions.h"
#include "../headers/commandprocessing/CommandProcessing.h"
#include "../headers/LoggingObserver.h"
#include "../headers/player/Player.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

void testLoggingObserver(CommandProcessor* commandProcessor)
{
    Observer *observer = new LogObserver();

    //  Instantiating the states
    auto *start = new State("start");
    auto *mapLoaded = new State("map loaded");
    auto *mapValidated = new State("map validated");
    auto *playersAdded = new State("players added");
    auto *gameLoop = new State("game loop");
    auto *win = new State("win");
    auto *end = new State("END");

    //  Inserting the transitions
    start->addTransition("loadmap", mapLoaded, 1, "loadmap [--filepath]", &game_loadMap);
    mapLoaded->addTransition("loadmap", mapLoaded, 1, "loadmap [--filepath]", &game_loadMap);
    mapLoaded->addTransition("validatemap", mapValidated, 0, "validatemap", &game_validateMap);
    mapValidated->addTransition("addplayer", playersAdded, 1, "addplayer [--playername]", &game_addPlayer);
    playersAdded->addTransition("addplayer", playersAdded, 1, "addplayer [--playername]", &game_addPlayer);
    playersAdded->addTransition("viewplayers", playersAdded, 0, "viewplayers", &game_printPlayers);
    playersAdded->addTransition("gamestart", gameLoop, 0, "gamestart", &game_gameStart);
    gameLoop->addTransition("win", win, 0, "win", &game_winGame);
    win->addTransition("replay", start, 0, "replay", &game_restart);
    win->addTransition("quit", end, 0, "quit", &game_quit);

    //  Initialize required objects
    std::vector<State *> states = {start, mapLoaded, mapValidated, playersAdded, gameLoop, win, end};
    auto *gameEngine = new GameEngine(states, commandProcessor);
    gameEngine->Attach(observer);

    gameEngine->execute();

    auto *player1 = new Player("player1");
    auto *player2 = new Player("player1");

    player1->addToReinforcementPool(3);
    player2->addToReinforcementPool(2);

    player1->addCard(*(new Card(type::bomb)));
    player1->addCard(*(new Card(type::bomb)));
    player1->addCard(*(new Card(type::airlift)));
    player1->addCard(*(new Card(type::diplomacy)));

    player2->addCard(*(new Card(type::blockade)));
    player2->addCard(*(new Card(type::bomb)));
    player2->addCard(*(new Card(type::airlift)));
    player2->addCard(*(new Card(type::diplomacy)));

    auto *europe = new Continent("Europe", 100);

    auto *france = new Territory("France", 5, 5, europe, player1, 5);
    auto *belgium = new Territory("Belgium", 6, 4, europe, player1, 5);
    auto *england = new Territory("England", 5, 3, europe, player2, 5);
    auto *greece = new Territory("Greece", 10, 12, europe, player1, 5);

    france->addAdjacentTerritory(belgium);
    france->addAdjacentTerritory(england);
    belgium->addAdjacentTerritory(france);
    belgium->addAdjacentTerritory(england);
    england->addAdjacentTerritory(france);
    england->addAdjacentTerritory(belgium);

    player1->issueOrder("deploy", france, 3, nullptr, nullptr);
    player1->issueOrder("advance", belgium, 4, france, nullptr);
    player1->issueOrder("bomb", england, 0, nullptr, nullptr);
    player1->issueOrder("blockade", belgium, 0, nullptr, nullptr);
    player1->issueOrder("airlift", greece, 3, france, nullptr);
    player1->issueOrder("negotiate", nullptr, 0, nullptr, player2);

    player2->issueOrder("deploy", france, 2, nullptr, nullptr);
    player2->issueOrder("advance", belgium, 4, france, nullptr);
    player2->issueOrder("bomb", greece, 0, nullptr, nullptr);
    player2->issueOrder("blockade", france, 0, nullptr, nullptr);
    player2->issueOrder("airlift", england, 2, france, nullptr);
    player2->issueOrder("negotiate", nullptr, 0, nullptr, player2);

    gameEngine->Detach(observer);
    delete gameEngine;
    delete europe;
    delete france;
    delete belgium;
    delete england;
    delete greece;

    delete player1;
    delete player2;
}