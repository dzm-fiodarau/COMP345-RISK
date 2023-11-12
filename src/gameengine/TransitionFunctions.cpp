//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
#endif

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/macros/DebugMacros.h"
#include "../../headers/Map.h"
#include "../../headers/player/Player.h"
#include "../../headers/Cards.h"

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <chrono>


#define PRESS_ENTER_TO_CONTINUE(clearConsole)                       \
	std::string _IGNORE_STRING;                                     \
	std::cout << "Press Enter to Continue... ";                     \
	std::getline(std::cin, _IGNORE_STRING);                         \
																	\
	if (clearConsole) {                                             \
		system("cls");                                              \
	}                                                               \


//----------------------------------------------------------------------------------------------------------------------
//  Static 'helper' functions

/**
 * \brief   'Fairly' distributes territories to players.
 * \param players       List of players to evenly distribute territories to
 * \param territories   List of territories to be distributed
 */
static void distributeTerritories(const std::vector<Player*>& players, const std::vector<Territory*>& territories) {
    size_t playersVectorIndex = 0;
    for (auto territory : territories) {
        if (playersVectorIndex >= players.size())
            playersVectorIndex = 0;

        players[playersVectorIndex++]->addTerritory(*territory);
    }
}

/**
 * \brief   Prints out details about the players given territories, drawn cards, and units just before the game starts
 *          playing.
 * \remarks Does not print out ALL details about the player, just the relevant information initialized during game start
 * \param players   The list/vector of players
 */
static void printPlayerInfo(const std::vector<Player*>& players) {
    for (size_t i = 0; i < players.size(); i++) {
        auto* currentPlayer = players[i];

        //  Print out player name + their position in the order of play
        std::cout << "[" << (i + 1) << "].    " << currentPlayer->getName() << std::endl;

        //  Print out owned territories
        std::cout << "  Territories:" << std::endl;
        std::cout << "  FORMAT: NAME(x, y); CONTINENT" << std::endl;
        for (const auto& territory : currentPlayer->getTerritories()) {
            std::cout << "      - " << territory->getName() << "(" << territory->getX() << ", " << territory->getY()
                      << "); " << territory->getContinent()->getName() << std::endl;
        }

        //  Print out drawn cards
        std::cout << "  Cards:" << std::endl;
        for (const auto& card : currentPlayer->getCards()) {
            std::cout << "      - " << *card << std::endl;
        }

        //  Prints out units in the reinforcement pool
        std::cout << "  Units: (in reinforcement pool)" << std::endl;
        std::cout << "      - " << currentPlayer->getUnits() << std::endl;

        //  Add extra space for good formatting
        std::cout << std::endl;
    }
}

//----------------------------------------------------------------------------------------------------------------------

bool game_restart(const std::vector<std::string>& values, GameEngine& gameEngine) {
    DEBUG_PRINT("GAME_RESTART")
    return true;
}

/**
 * \brief   Loads a .map file from the working directory.
 * \remarks 1.  The function attempts to load the map using MapLoader's 'loadMap(...)' method.
 *              A 'nullptr' indicates file loading unsuccessful.
 *          2.  Load the map into the passed GameEngine object.
 *
 * \param values        Passed arguments. Note that these arguments are validated to be a certain of a certain number
 *                      before calling method. EXPECT 1 ARGUMENT: (std::string) filepath.
 * \param gameEngine    Game object to change/obtain values.
 * \return  True if loading was successful, false otherwise.
 */
bool game_loadMap(const std::vector<std::string>& values, GameEngine& gameEngine) {
    //  Attempt to load map
    auto mapLoader = std::make_unique<MapLoader>(values.front());
    auto* loadedMap = mapLoader->load();

    //  A 'nullptr' implies that the loading was unsuccessful
    if (loadedMap == nullptr) {
        std::cerr << "ERROR: An error has occurred when trying to load the map!" << std::endl;
        return false;
    }

    //  Extra newline character for presentation
    std::cout << std::endl;

    //  Transfer ownership of loaded map object to the game engine
    gameEngine.setMap(loadedMap);
    return true;
}

/**
 * \brief   Validates the currently loaded map.
 * \param _ignored_     Passed arguments. Note that these arguments are validated to be a certain of a certain number
 *                      before calling method. EXPECT 0 ARGUMENTS.
 * \param gameEngine    Game object to change/obtain values.
 * \return True if validating was successful, false otherwise.
 */
bool game_validateMap(const std::vector<std::string>& _ignored_, GameEngine& gameEngine) {
    auto* mapPtr = gameEngine.getMap();

    //  If the current map object in the game engine is a 'nullptr', then it means it hasn't been loaded yet
    if (mapPtr == nullptr) {
        std::cerr << "ERROR: No map has been loaded yet!" << std::endl;
        return false;
    }

    //  Check if valid, print a corresponding error/success message
    if (mapPtr->getIsValid()) {
        std::cout << "SUCCESS: Map has been successfully validated!" << std::endl;
        return true;
    } else {
        std::cerr << "ERROR: The current map is NOT valid!" << std::endl;
        return false;
    }
}

/**
 * \brief   Adds a player to the current list of players. Note that there exists a maximum player cap, and if the cap is
 *          reached, then no more players can be added - and an error message will be printed.
 * \param values        Passed arguments. Note that these arguments are validated to be a certain of a certain number
 *                      before calling method. EXPECT 1 ARGUMENT: (std::string) playername.
 * \param gameEngine    Game object to change/obtain values.
 * \return True if addition of player was successful, false otherwise.
 */
bool game_addPlayer(const std::vector<std::string>& values, GameEngine& gameEngine) {
    //  If number of player cap is met, unable to add any more players
    if (gameEngine.numberOfPlayers() >= GameEngine::MAX_PLAYERS) {
        std::cerr << "ERROR: Cannot add any more players!" << std::endl;
        std::cerr << "       Max no. of players (" << GameEngine::MAX_PLAYERS << " player(s)) reached!" << std::endl;
        return false;
    }

    //  Instantiate a new player and add it to the game engine
    const std::string& playerName = values.front();
    auto* newPlayer = new Player(playerName);
    gameEngine.addPlayer(newPlayer);

    std::cout << "Successfully added player!" << std::endl;
    return true;
}

/**
 * \brief   Prints the list of currently added players. Used before game start.
 * \param _ignored_     Passed arguments. Note that these arguments are validated to be a certain of a certain number
 *                      before calling method. EXPECT 0 ARGUMENTS.
 * \param gameEngine    Game object to change/obtain values.
 * \return  Returns true.
 */
bool game_printPlayers(const std::vector<std::string>& _ignored_, GameEngine& gameEngine) {
    std::vector<Player*> players = gameEngine.getPlayers();
    size_t playerCount = 1;     //   Counter variable to numerate the players when printing

    std::cout << "  CURRENT PLAYERS" << std::endl;
    for (auto* player : players) {
        std::cout << "  " << (playerCount++) << ". " << player->getName() << std::endl;
    }

    return true;
}

/**
 * \brief   Instantiates variables/attributes and the environment, indicating the end of the setup/startup phase and the
 *          transition into the play phase.
 * \remarks The method does 4 main things:
 *              1. Distribute the territories 'fairly' among the players.
 *                 Note that the number of players must be less than or equal to the number of territories - we can't
 *                 have a game where a player does not have a territory.
 *              2. Determine the order of play randomly
 *              3. Give 50 units to each player, placed in their reinforcement pools
 *              4. Let each player draw 2 cards from the deck
 * \param values        Passed arguments. Note that these arguments are validated to be a certain of a certain number
 *                      before calling method. EXPECT 0 ARGUMENTS.
 * \param gameEngine    Game object to change/obtain values.
 * \return  True if the initialization was successful, false otherwise.
 */
bool game_gameStart(const std::vector<std::string>& values, GameEngine& gameEngine) {

    std::vector<Territory*> territories = gameEngine.getMap()->getTerritories();
    std::vector<Player*> players = gameEngine.getPlayers();

    //  If there are more players than there are territories -> territories cannot be properly distributed
    if (players.size() > territories.size()) {
        std::cerr << "ERROR:    There are not enough territories for the number of players!" << std::endl;
        return false;
    }

    //  1.  'Fairly' distributes all the territories to the player
    distributeTerritories(players, territories);

    //  2.  Determine randomly the order of play of the players in the game
    //  Does so by scrambling the players vector. Order goes from start of the vector, to the end.
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::shuffle(players.begin(), players.end(), engine);
    (void)gameEngine.setPlayers(players);

    //  3.  Give 50 initial army units to the players, which are placed in their respective reinforcement pool
    for (Player* player : players)
        player->addToReinforcementPool(50);

    //  4.  Let each player draw 2 initial cards from the deck using the deck's 'draw()' method
    Deck& deck = Deck::getInstance();
    for (Player* player : players) {
        //  Draw two cards from the deck
        player->addCard(*deck.draw());
        player->addCard(*deck.draw());
    }

    //  Print current config.
    printPlayerInfo(players);

    PRESS_ENTER_TO_CONTINUE(true)

    gameEngine.mainGameLoop();
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

bool game_tournament(const std::vector<std::string>&, GameEngine&) {
    DEBUG_PRINT("GAME_TOURNAMENT")
    return true;
}

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif