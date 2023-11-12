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

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Player;               //  Player.h
class Map;                  //  Map.h
class State;                //  gameengine/State.h

//  Forward declaration of classes
class GameEngine;
class CommandProcessor;

/** \class GameEngine
 *  \brief A class that controls the flow of the game through notations of state and transitions. */
class GameEngine {
public:
    const static int MAX_PLAYERS = 6;

    //  Constructors/Deconstructor
    /** \brief Constructs a default <code>GameEngine</code> object. */
    GameEngine() = default;

    /** \brief Constructs a <code>GameEngine</code> object with a given state configuration and a command processor. */
    GameEngine(const std::vector<State*>&, CommandProcessor*);

    /** \brief Deconstructs a <code>GameEngine</code> object. */
    ~GameEngine();

    //  Operator overrides
    /** \brief Assigns the values of the passed GameEngine object to the current. Copy assignment operator. */
    GameEngine& operator=(const GameEngine&);

    /** \brief Prints a brief description of the current internal configurations of the instance. */
    friend std::ostream& operator<<(std::ostream&, const GameEngine&);

    //  Other Methods
    /** \brief Starts the execution of the game object. Main entry point. */
    void execute();
    /** \brief Sets the game running flag to false */
    void stopRunning();

    //  Getter/Accessor methods

    /** \brief Returns whether the game is running. */
    bool isGameRunning() const;
    /** \brief Returns a vector of currently added players. */
    std::vector<Player*> getPlayers() const;
    /** \brief Returns the currently loaded map. */
    Map* getMap() const;

    //  Setter/Mutator methods

    /** \brief Sets the command processor for the object. */
    void setCommandProcessor(const CommandProcessor&);
    /** \brief Sets the players. Returns a vector of the previous players. */
    std::vector<Player*> setPlayers(std::vector<Player*>);
    /** \brief Sets the currently loaded map. */
    void setMap(Map*);

    //  Additional behavior for Setter/Mutators
    /** \brief Appends a player instance to the end of the player vector. Moves data. */
    void addPlayer(Player*);
    /** \brief Returns the number of currently registered players. */
    size_t numberOfPlayers() const;

private:
    //  A list of states that is 'owned' by the game engine.
    //  The game engine is responsible for deallocating each state in the list.
    std::vector<State*> ownedStates = {};

    //  The current internal state in the game engine.
    State* currentState = nullptr;

    //  The command processor to take commands from.
    //  The game engine is responsible for deallocating.
    CommandProcessor* commandProcessor = nullptr;

    //  A vector containing the list of currently added players
    //  The game engine is responsible for deallocating each state in the list.
    std::vector<Player*> players = {};

    //  The playing map
    //  The game engine is responsible for deallocating.
    Map* map = nullptr;

    bool isRunning = false;

    //  Further process a command. Calls the corresponding transition function.
    void processCommand(bool (*)(const std::vector<std::string>&, GameEngine&), const std::string&, State*);
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif // GAME_ENGINE_H
