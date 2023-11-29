#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

#include <memory>
#include <string>
#include <vector>

#include "State.h"
#include "../LoggingObserver.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Player;    //  Player.h
class Map;       //  Map.h
class Continent; //  Map.h
class Order;     //  Orders.h
class State;     //  gameengine/State.h

//  Forward declaration of classes
class GameEngine;
class CommandProcessor;

/** \class GameEngine
 *  \brief A class that controls the flow of the game through notations of state and transitions. */
class GameEngine : public ILoggable, public Subject
{
public:
    const static int MAX_PLAYERS = 6;

    //  Constructors/Deconstructor
    /** \brief Constructs a default <code>GameEngine</code> object.
     */
    GameEngine() = default;

    /** \brief Constructs a <code>GameEngine</code> object with a given state configuration and a command processor.
     */
    GameEngine(const std::vector<State *> &, CommandProcessor *);

    /** \brief Deconstructs a <code>GameEngine</code> object.
     */
    ~GameEngine() override;

    //  Operator overrides
    /** \brief Assigns the values of the passed GameEngine object to the current. Copy assignment operator.
     */
    GameEngine &operator=(const GameEngine &);

    /** \brief Prints a brief description of the current internal configurations of the instance.
     */
    friend std::ostream &operator<<(std::ostream &, const GameEngine &);

    //  Other Methods
    /** \brief Starts the execution of the game object. Main entry point.
     */

    void execute();
    /** \brief Sets the game running flag to false
     */
    void stopRunning();

    //  Getter/Accessor methods
    /** \brief Is the game running
     */
    bool isGameRunning() const;

    /** \brief Returns the list of players.
     */
    std::vector<Player *> getPlayers() const;

    /** \brief Returns the currently loaded map.
     */
    Map *getMap() const;

    //  Setter/Mutator methods
    /** \brief Sets the command processor for the object.
     */
    void setCommandProcessor(const CommandProcessor &);
    /** \brief Gets the command processor for the object.
     */
    CommandProcessor *getCommandProcessor();
    /** \brief Sets the players. Returns a vector of the previous players.
     */
    std::vector<Player *> setPlayers(std::vector<Player *>);
    /** \brief Sets the currently loaded map.
     */
    void setMap(Map *);
    /** \brief Gets pointer to Player from given name
     */
    Player *getPlayerByName(const std::string &name) const;

    //  Additional behavior for Setter/Mutators
    /** \brief Appends a player instance to the end of the player vector. Moves data.
     */
    void addPlayer(Player *);
    /** \brief Returns the number of currently registered players.
     */
    size_t numberOfPlayers() const;

    /**
     * \brief   Creates the string to log
     */
    string stringToLog() override;

    //  Gameplay methods
    /**  \brief Enters the main gameplay loop.
     */
    void mainGameLoop();

    /** \brief Each player gets a certain number of reinforcements this phase.
     */
    void reinforcementPhase();

    /** \brief Each player gets the opportunity to issue orders.
     */
    void issueOrdersPhase();

    /** \brief In the player order determined at game start, executes the orders each player has issued.
     */
    void executeOrdersPhase();

    /** \brief Removes all players who do not have any territories anymore.
     */
    void removeDefeatedPlayers();

private:
    //  A list of states that is 'owned' by the game engine.
    //  The game engine is responsible for deallocating each state in the list.
    std::vector<State *> ownedStates = {};

    //  The current internal state in the game engine.
    State *currentState = nullptr;

    //  The command processor to take commands from.
    //  The game engine is responsible for deallocating.
    CommandProcessor *commandProcessor = nullptr;

    //  A vector containing the list of currently added players
    //  The game engine is responsible for deallocating each state in the list.
    std::vector<Player *> players = {};

    //  The playing map
    Map *map = nullptr;

    bool isRunning = false;

    //  Checks if a player owns all the territories in a continent
    static bool playerOwnsContinent(Player *player, Continent *continent);

    //  Further process a command. Calls the corresponding transition function.
    void processCommand(bool (*)(const std::vector<std::string> &, GameEngine &), const std::string &, State *);
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif // GAME_ENGINE_H
