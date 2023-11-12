#ifndef TRANSITION_FUNCTIONS_H
#define TRANSITION_FUNCTIONS_H

#include <string>
#include <vector>

//  Forward declaration of required classes from other header files. (included in .cpp file)
class GameEngine;               //  gameengine/GameEngine.h

/** \brief Restarts the game after concluding. */
bool game_restart(const std::vector<std::string>&, GameEngine&);

/** \brief Select and load a map from a list of .map files. */
bool game_loadMap(const std::vector<std::string>&, GameEngine&);

/** \brief Validates the map. */
bool game_validateMap(const std::vector<std::string>&, GameEngine&);

/** \brief Adds a player into the game.
 *  \remarks Only up to 6 players can be added into the game. */
bool game_addPlayer(const std::vector<std::string>&, GameEngine&);

/** \brief Prints a list of the players. */
bool game_printPlayers(const std::vector<std::string>&, GameEngine&);

/** \brief Starts the game.
 *  \remarks - Fairly distributes all the territories to the players.
 *           - Establish (randomly) the order of play between the players.
 *           - Give 50 initial army units to all players, placed in their respective reinforcement pools.
 *           - Give the players 2 drawn cards from the deck. */
bool game_gameStart(const std::vector<std::string>&, GameEngine&);

/** \brief Issue an order. */
bool game_issueOrder(const std::vector<std::string>&, GameEngine&);

/** \brief End the issuing orders phase. */
bool game_endIssueOrders(const std::vector<std::string>&, GameEngine&);

/** \brief Execute an order. */
bool game_executeOrder(const std::vector<std::string>&, GameEngine&);

/** \brief End the executing orders phase. */
bool game_endExecuteOrders(const std::vector<std::string>&, GameEngine&);

/** \brief Enter the game Win phase. */
bool game_winGame(const std::vector<std::string>&, GameEngine&);

/** \brief Quits the game phase. Stops the game engine. */
bool game_quit(const std::vector<std::string>&, GameEngine&);

/** \brief Attempts to enter a tournament mode. */
bool game_tournament(const std::vector<std::string>&, GameEngine&);

inline bool game_emptyFunction(const std::vector<std::string>&, GameEngine&) { return true; }

#endif //   TRANSITION_FUNCTIONS_H