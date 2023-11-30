//----------------------------------------------------------------------------------------------------------------------
//  Macros

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "UnusedParameter"
#endif

#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>
#include <memory>
#include <algorithm>

#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/commandprocessing/ConsoleCommandProcessorAdapter.h"
#include "../../headers/Map.h"
#include "../../headers/player/Player.h"

#define PRESS_ENTER_TO_CONTINUE(clearConsole)   \
    std::string _IGNORE_STRING;                 \
    std::cout << "Press Enter to Continue... "; \
    std::getline(std::cin, _IGNORE_STRING);     \
                                                \
    if (clearConsole)                           \
    {                                           \
        system("cls");                          \
    }

//----------------------------------------------------------------------------------------------------------------------
//  Static functions

/** \brief Splits a string by spaces and returns a vector of string tokens. */
static std::vector<std::string> getTokens(std::string input)
{
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    while (iss >> input)
    {
        tokens.push_back(input);
    }
    return tokens;
}

/** \brief Joins strings contained in a vector by a single whitespace. */
static std::string reduceStringVector(const std::vector<std::string> &tokens)
{
    std::string stringBuilder;

    //  Concats each element with a whitespace in between
    for (const std::string &str : tokens)
    {
        stringBuilder += str + " ";
    }

    //  Trims the trailing whitespace of the string
    size_t endpos = stringBuilder.find_last_not_of(" \t\n\r"); // Finding the last non-whitespace character
    if (std::string::npos != endpos)
    {
        stringBuilder = stringBuilder.substr(0, endpos + 1); // Trimming the trailing whitespaces
    }
    return stringBuilder;
}

//----------------------------------------------------------------------------------------------------------------------
//  "GameEngine" implementations

GameEngine::GameEngine(const vector<State *> &states, CommandProcessor *commandProcessor)
{
    this->ownedStates = states;
    this->currentState = *states.begin(); //  The first state in the vector
    this->commandProcessor = commandProcessor;
    this->players = {};
    this->map = {};
    this->isRunning = false;
}

GameEngine::~GameEngine()
{
    for (const auto &state : ownedStates)
        delete state;

    for (const auto &player : players)
        delete player;

    delete commandProcessor;
    delete map;
}

GameEngine &GameEngine::operator=(const GameEngine &otherGameEngine)
{
    if (this != &otherGameEngine)
    {
        //  Delete all current data
        for (const auto &state : ownedStates)
            delete state;

        for (const auto &player : players)
            delete player;

        delete commandProcessor;
        delete map;

        //  Copy data from other game engine
        this->ownedStates = otherGameEngine.ownedStates;
        this->currentState = otherGameEngine.currentState;
        this->commandProcessor = otherGameEngine.commandProcessor->clone();
        this->players = otherGameEngine.players;
        this->map = otherGameEngine.map;
        this->isRunning = false;
    }

    return *this;
}

std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine)
{
    //  TODO: implement when finished refactoring
    return os;
}

/** Implementation Details:
 *  - First, the method checks if all of the member variables have been instantiated and provide some info. There must
 *    at least one state and at least one TransitionData object. Furthermore, a CommandProcessor object must be given.
 *
 *  - Second the game loop then starts, starting when a member variable 'isRunning' is set to true, and ending when set
 *    to false.
 *    The client is prompted and then input is taken through the command processor object. Command is encapsulated in a
 *    'Command' object. The command is then verified. If valid, then it is executed by calling the respective transition
 *    function. If invalid, then an error message is printed.
 */
void GameEngine::execute()
{
    //  Checks if all the required variables have been instantiated and checks for the presence of any data
    if (currentState == nullptr || commandProcessor == nullptr)
    {
        std::cerr << "ERROR: GameEngine not fully instantiated. Dependencies missing" << std::endl;
        return;
    }

    //  Set the isRunning status variable to 'true'
    isRunning = true;

    while (isRunning)
    {
        //  Printing out prompt
        std::cout << "CURRENT STATE:\t[" << currentState->getStateName() << "]" << std::endl;
        std::cout << "Please enter a command:\n";

        //  Taking input
        auto command = std::make_unique<Command>(commandProcessor->getCommand(*currentState));

        //  If input is empty, refresh
        if (command->getRawCommand().empty())
        {
            system("cls");
            continue;
        }

        //  Checking if the given transition is valid
        if (currentState->isValidTransition(command->getFirstToken()))
        {
            //  Transition is valid
            //  Isolate the arguments, reduce them into a string, then pass it to be further processed
            std::string argumentsRaw = reduceStringVector(command->getRemainingTokens());
            auto transitionFunction = *currentState->getTransitionFunction(command->getFirstToken());
            auto nextState = *currentState->getNextState(command->getFirstToken());

            processCommand(transitionFunction, argumentsRaw, nextState);
        }
        else
        {
            //  Transition is invalid, print series of error messages
            std::cerr << "\033[1;31m"
                      << "INVALID COMMAND. View the valid commands below:"
                      << "\033[0m";
            std::cerr << currentState->getHelpStrings() << std::endl;
        }

        //  Press enter to continue.
        PRESS_ENTER_TO_CONTINUE(true)
    }
}

/**
 * \brief Flags the game engine to stop running.
 */
void GameEngine::stopRunning()
{
    this->isRunning = false;
}

/**
 * \brief Further processes a command, calling its respective transition function.
 *
 * Implementation details:
 * Calls the respective transition function given a command. A 'true' status indicates a successful operation. 'False'
 * indicates an error with the provided arguments or an error somewhere down the line of execution. If 'true', then
 * we transition into the next state, updating the 'currentStateIndex' member variable.
 */
void GameEngine::processCommand(bool (*transitionFunction)(const std::vector<std::string> &, GameEngine &), const std::string &argumentsRaw, State *nextState)
{
    if (!transitionFunction(getTokens(argumentsRaw), *this))
    {
        //  PRINTING OF ERROR MESSAGES IS DELEGATED TO TRANSITION FUNCTIONS.
        return;
    }

    //  Transition into the next state
    currentState = nextState;

    //  Print that you have switched states
    std::string newCurrentStateName = currentState->getStateName();
    std::cout << "\033[34m"
              << "SWITCHED STATES TO:\t[" << newCurrentStateName << "]\033[0m" << std::endl;
}

//  Getter/Accessor methods
bool GameEngine::isGameRunning() const
{
    return isRunning;
}

std::vector<Player *> GameEngine::getPlayers() const
{
    return players;
}

Map *GameEngine::getMap() const
{
    return map;
}

//  Setter/Mutator methods
CommandProcessor *GameEngine::getCommandProcessor()
{
    return commandProcessor;
}

void GameEngine::setCommandProcessor(const CommandProcessor &newCommandProcessor)
{
    this->commandProcessor = newCommandProcessor.clone();
}

void GameEngine::setMap(Map *newMap)
{
    this->map = newMap;
}

std::vector<Player *> GameEngine::setPlayers(std::vector<Player *> newPlayers)
{
    auto tempVector = std::move(players);  //  Store old list of players
    this->players = std::move(newPlayers); //  Set the new list of players
    return tempVector;                     //  Return old list through temp variable
}

Player *GameEngine::getPlayerByName(const std::string &name) const
{
    for (const auto &player : players)
    {
        if (player->getName() == name)
        {
            return player;
        }
    }
    return nullptr;
}

void GameEngine::addPlayer(Player *playerPtr)
{
    players.push_back(playerPtr);
}

size_t GameEngine::numberOfPlayers() const
{
    return players.size();
}

void GameEngine::mainGameLoop()
{
    while (true)
    {
        for (Player *player : players)
        {
            player->setIssuingOrders(true);
        }
        // 1. Reinforcement phase
        reinforcementPhase();
        // 2. Issue orders phase
        issueOrdersPhase();
        // 3. Execute orders phase
        executeOrdersPhase();
        // Remove eliminated players
        removeDefeatedPlayers();

        // Check end game conditions
        if (players.size() == 1)
        {
            cout << "Player " << players[0]->getName() << " wins!" << endl;
            break;
        }
    }
}

void GameEngine::reinforcementPhase()
{
    for (auto &player : players)
    {
        int reinforcements = int(player->getTerritories().size() / 3);
        reinforcements = max(reinforcements, 3); // Minimum 3
        // Give continent bonus

        for (Continent *continent : map->getContinents())
            if (playerOwnsContinent(player, continent))
                reinforcements += continent->getBonus();

        player->addToReinforcementPool(reinforcements);
    }
}

// Check if player owns all territories in continent
bool GameEngine::playerOwnsContinent(Player *player, Continent *continent)
{
    std::vector<Territory *> territories = continent->getTerritories();
    return std::ranges::all_of(territories.begin(), territories.end(), [player](Territory *territory) -> bool
                               { return territory->getOwner() == player; });
}

void GameEngine::issueOrdersPhase()
{
    bool issuingOrders = true;
    do
    {
        issuingOrders = false;
        for (Player *player : players)
        {
            if (player->isIssuingOrders())
            {
                player->issueOrders(this);
            }
        }
        for (Player *player : players)
        {
            if (player->isIssuingOrders())
            {
                issuingOrders = true;
                break;
            }
        }
    } while (issuingOrders);
}

void GameEngine::executeOrdersPhase()
{
    for (Player *player : players)
    {
        player->setIssuingOrders(true);
    }
    bool issuingOrders = true;
    do
    {
        issuingOrders = false;
        for (Player *player : players)
        {
            if (player->isIssuingOrders())
            {
                OrdersList *ordersList = player->getOrdersList();
                auto *nextOrder = ordersList->getNextOrder();
                nextOrder->execute();
                delete nextOrder;
                if (player->getOrdersList()->size() == 0)
                {
                    player->setIssuingOrders(false);
                }
            }
        }
        for (Player *player : players)
        {
            if (player->isIssuingOrders())
            {
                issuingOrders = true;
                break;
            }
        }
    } while (issuingOrders);
}

void GameEngine::removeDefeatedPlayers()
{
    players.erase(std::remove_if(players.begin(), players.end(), [](Player *player) -> bool
                                 { return true; }),
                  players.end());
}

string GameEngine::stringToLog()
{
    return "GameEngine updated state: " + currentState->getStateName();
}

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif
