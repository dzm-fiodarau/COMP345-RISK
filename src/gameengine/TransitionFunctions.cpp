#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/macros/DebugMacros.h"


#include <string>

bool game_restart(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_RESTART")
    return true;
}

bool game_loadMap(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_LOAD_MAP")
    return true;
}

bool game_validateMap(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_VALIDATE_MAP")
    return true;
}

bool game_addPlayer(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_ADD_PLAYER")
    return true;
}

bool game_printPlayers(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_PRINT_PLAYERS")
    return true;
}

bool game_gameStart(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_GAME_START")
    return true;
}

bool game_issueOrder(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_ISSUE_ORDER")
    return true;
}

bool game_endIssueOrders(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_END_ISSUE_ORDERS")
    return true;
}

bool game_executeOrder(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_EXECUTE_ORDER")
    return true;
}

bool game_endExecuteOrders(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_END_EXECUTE_ORDERS")
    return true;
}

bool game_winGame(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_WIN_GAME")
    return true;
}

bool game_quit(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_END_PROGRAM")
    return true;
}