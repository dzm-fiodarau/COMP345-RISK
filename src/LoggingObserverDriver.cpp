#include <iostream>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "../headers/gameengine/GameEngine.h"
#include "../headers/gameengine/State.h"
#include "../headers/gameengine/TransitionFunctions.h"
#include "../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../headers/commandprocessing/FileCommandProcessorAdapter.h"
#include "../headers/LoggingObserver.h"
#include "../headers/Player.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

#define STATES_CONTENTS                                                                  \
    State("start"), State("map loaded"), State("map validated"), State("players added"), \
        State("assign reinforcement"), State("issueorder"), State("execute orders"), State("win"), State("END")

#define TRANSITIONS_CONTENTS                                                                      \
    TransitionData(0, 1, 1, "loadmap", "loadmap [--filepath]", &game_loadMap),                    \
        TransitionData(1, 1, 1, "loadmap", "loadmap [--filepath]", &game_loadMap),                \
        TransitionData(1, 2, 0, "validatemap", "validatemap", &game_validateMap),                 \
        TransitionData(2, 3, 1, "addplayer", "addplayer [--playername]", &game_addPlayer),        \
        TransitionData(3, 3, 1, "addplayer", "addplayer [--playername]", &game_addPlayer),        \
        TransitionData(3, 3, 0, "viewplayers", "viewplayers", &game_printPlayers),                \
        TransitionData(3, 4, 0, "gamestart", "gamestart", &game_gameStart),                       \
        TransitionData(4, 5, 0, "issueorder", "issueorder -SAMPLE ARGUMENTS-", &game_issueOrder), \
        TransitionData(5, 5, 0, "issueorder", "issueorder -SAMPLE ARGUMENTS-", &game_issueOrder), \
        TransitionData(5, 6, 0, "endissueorders", "endissueorders", &game_endIssueOrders),        \
        TransitionData(6, 6, 0, "execorder", "execorder -SAMPLE ARGUMENTS-", &game_executeOrder), \
        TransitionData(6, 4, 0, "endexecorders", "endexecorders", &game_endExecuteOrders),        \
        TransitionData(6, 7, 0, "win", "win", &game_winGame),                                     \
        TransitionData(7, 0, 0, "replay", "replay", &game_restart),                               \
        TransitionData(7, 8, 0, "quit", "quit", &game_quit)

void testLoggingObserver()
{
    Observer *observer = new LogObserver();

    //  Dependencies instantiation
    std::vector<State> states = {STATES_CONTENTS};
    std::vector<TransitionData> transitionDatabase = {TRANSITIONS_CONTENTS};
    auto commandProcessor = std::make_unique<FileCommandProcessorAdapter>(states, transitionDatabase, "../../commands.txt");

    //  Main objects
    //  Dependency Injection
    auto gameEngine = std::make_unique<GameEngine>();
    gameEngine->setStates(states);
    gameEngine->setTransitionData(transitionDatabase);
    gameEngine->setCommandProcessor(*commandProcessor);
    gameEngine->Attach(observer);

    //  Run
    gameEngine->execute();

    // Create cards for players
    Card *card1 = new Card(type::bomb);
    Card *card2 = new Card(type::blockade);

    // Create player objects
    vector<Territory *> territories1;
    vector<Card *> handCards1 = {card1};
    Player *player1 = new Player("Player1", territories1, handCards1);

    vector<Territory *> territories2;
    vector<Card *> handCards2 = {card2};
    Player *player2 = new Player("Player2", territories2, handCards2);

    // Creating continents
    Continent *continent1 = new Continent("Africa", 0);
    Continent *continent2 = new Continent("Europe", 0);

    // Create territories for players
    Territory *territory1 = new Territory("Territory1", 5, 3, continent1, player1, 0);
    Territory *territory2 = new Territory("Territory2", 4, 2, continent2, player2, 0);

    // Add territories the player's 'owned territories' list
    player1->territory.push_back(territory1);
    player2->territory.push_back(territory2);

    // Call toAttack() and toDefend() methods for player1 and player2
    cout << "Player 1 territories to attack: ";
    player1->toAttack();
    cout << "Player 1 territories to defend: ";
    player1->toDefend();

    cout << "Player 2 territories to attack: ";
    player2->toAttack();
    cout << "Player 2 territories to defend: ";
    player2->toDefend();

    // Issue orders for player1 and player2
    player1->issueOrder("deploy", territory1, 3, nullptr, nullptr);
    player1->issueOrder("advance", territory1, 2, territory2, nullptr);

    player2->issueOrder("deploy", territory2, 2, nullptr, nullptr);
    player2->issueOrder("advance", territory2, 1, territory1, nullptr);

    gameEngine->Detach(observer);
}