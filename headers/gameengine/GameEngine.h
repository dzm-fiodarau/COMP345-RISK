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
#include "LoggingObserver.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Player;               //  Player.h
class Map;                  //  Map.h
class State;                //  gameengine/State.h

//  Forward declaration of classes
class GameEngine;
class CommandProcessor;


/** \class TransitionData
 *  \brief A class that stores transition data for the GameObject class.
 *  States are stored in the GameObject class as elements in an array. States are to be linked by index pairs. This
 *  class stores index pairs indicating the direction of flow between states. The class contains addition information
 *  about the transition including the valid transition name, a help string, and a function pointer that points to the
 *  transitions corresponding function. */
class TransitionData {
public:
    //  Constructors/Deconstructor
    /** \brief Constructs a default TransitionData object. */
    TransitionData() = default;
    /** \brief Constructs a TransitionData object given the passed data. */
    TransitionData(size_t, size_t, size_t, std::string, std::string, bool (*)(const std::vector<std::string>&, GameEngine&));
    /** \brief Constructs a TransitionData object from another TransitionData object. Copy constructor. */
    TransitionData(const TransitionData&);
    /** \brief Destructs a TransitionData object. */
    ~TransitionData();

    //  Operator overrides
    /** \brief Assigns the values of the passed TransitionData object to the current. Copy assignment operator. */
    TransitionData& operator=(const TransitionData&);
    /** \brief Prints a brief summary of the TransitionData object. Stream insertion operator. */
    friend std::ostream& operator<<(std::ostream& os, const TransitionData& obj);

    //  Other methods
    /** \brief Get the transition function. */
    bool execute(const std::vector<std::string>&, GameEngine&);

    //  Getter/Accessor methods
    /** \brief Get the first index, which indicates the state BEFORE transition. */
    size_t getIndex1() const { return *index1; }
    /** \brief Get the second index, which indicates the state AFTER transition. */
    size_t getIndex2() const { return *index2; }
    /** \brief Gets the number of expected argument tokens after the command token. */
    size_t getNumberOfArguments() const { return *numberOfArguments; }
    /** \brief Get the name of the transition. */
    std::string getTransitionName() const { return *transitionName; }
    /** \brief Get the help string. */
    std::string getHelpString() const { return *helpString; }

    //  Setter/Mutator methods
    /** \brief Sets the first index. */
    void setIndex1(size_t);
    /** \brief Sets the second index. */
    void setIndex2(size_t);
    /** \brief Sets the number of arguments required for the command. */
    void setNumberOfArguments(size_t);
    /** \brief Sets the transition name between the two states. */
    void setTransitionName(const std::string&);
    /** \brief Sets the help string for the two states. */
    void setHelpString(const std::string&);
    /** \brief Sets the transition function. */
    void setTransitionFunction(bool (*)(const std::vector<std::string>&, GameEngine&));

private:
    //  The first index which indicates the index of the state BEFORE transition
    size_t* index1;

    //  The second index which indicates the index of the state AFTER transition
    size_t* index2;

    //  The number of arguments expected after the command token
    size_t* numberOfArguments;

    //  The transition name
    std::string* transitionName;

    //  The help string
    std::string* helpString;

    //  The transition function
    bool (*transitionFunction)(const std::vector<std::string>&, GameEngine&);
};



/** \class GameEngine
 *  \brief A class that controls the flow of the game through notations of state and transitions. */
class GameEngine : public ILoggable, public Subject {
public:
    const static int MAX_PLAYERS = 6;

    //  Constructors/Deconstructor
    /** \brief Constructs a default GameEngine object with the specification as shown in the instructions. */
    GameEngine();
    /** \brief Constructs a GameEngine object. */
    GameEngine(std::vector<State>, std::vector<TransitionData>, CommandProcessor*);
    /** \brief Constructs a GameEngine object from another GameEngine object. Copy constructor. */
    GameEngine(const GameEngine&);
    /** \brief Deconstructs a GameEngine object. */
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
    /** \brief Returns the state configuration of the game engine. */
    std::vector<State> getStates() const;
    /** \brief Returns the transition data configuration of the game engine. */
    std::vector<TransitionData> getTransitionDatabase() const;
    /** \brief Returns whether the game is running. */
    bool isGameRunning() const;
    /** \brief Returns a vector of currently added players. */
    std::vector<Player*> getPlayers() const;
    /** \brief Returns the currently loaded map. */
    Map* getMap() const;

    //  Setter/Mutator methods
    /** \brief Sets the states configuration of the object. */
    void setStates(std::vector<State>);
    /** \brief Sets the transition database configuration of the object. */
    void setTransitionData(std::vector<TransitionData>);
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

    /**
     * \brief   Creates the string to log
     */
    string stringToLog();
private:

    //  The index of the current state of the game engine
    size_t currentStateIndex = 0;

    //  Whether the game running or not
    bool isRunning;

    //  A vector of states
    std::vector<State> states;

    //  A vector of TransitionData objects 'connecting' the states together
    std::vector<TransitionData> transitionDatabase;

    //  The command processor to take commands from
    CommandProcessor* commandProcessor;

    //  A vector containing the list of currently added players
    std::vector<Player*> players;

    //  The playing map
    Map* map;
    


    //  Returns the index of the appropriate transition struct given the current state and the given transition name
    //  Returns -1 if no matching transition is found
    int indexOfTransition(const std::string& transitionName) const;

    //  Returns the help strings of the possible transitions in the current state.
    std::string getHelpStrings() const;

    //  Further process a command. Calls the corresponding transition function.
    void processCommand(TransitionData, const std::string&);
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif // GAME_ENGINE_H
