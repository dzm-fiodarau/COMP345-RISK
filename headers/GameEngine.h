#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

//  Macro Includes
#include <string>

//  Project includes
#include "../headers/Map.h"
#include "../headers/Player.h"

//  Declare
class GameEngine;



/** \class State
 *  \brief Class to represent a state. */
class State {
public:
    /** \brief Constructs a default state object. */
    State();

    /** \brief Constructs a state object with the specified state name.
     *  \param stateName The name of the state. */
    explicit State(const std::string& stateName);

    /** \brief Constructs a state object from another state object. Copy constructor.
     *  \param otherState The other state object to copy. */
    State(const State& otherState);

    /** \brief Assigns the values of the passed state object into the current state object. Copy assignment operator.
     *  \param otherState The other state object to copy values from.
     *  \return The current state object with changed values. */
    State& operator=(const State& otherState);

    /** \brief Constructs a state object from another state object then sets the values of the other state object to
     *        unspecified. Move constructor.
     *  \param otherState The other state object to extract values from. */
    State(State&& otherState) noexcept;

    /** \brief Deallocates a State object. */
    ~State();

    /** \brief Assigns the values of the passed state object into the current state object then sets the values of the
     *        other state object to unspecified. Move assignment operator.
     *  \param otherState The other state object to extract values from.
     *  \return The current state object with changed values. */
    State& operator=(State&& otherState) noexcept;

    /** \brief Prints the name of the state into the output stream. Stream insertion operator.*/
    friend std::ostream& operator<<(std::ostream& os, const State& otherState);

    /** \brief Compares two state objects and returns true if they are equal. False otherwise. */
    bool operator==(const State& otherState) const;

    /** /brief   Gets the name of the state.
     *  /return  Returns the name of the state. */
    std::string getStateName() const;

private:
    //  The name of the state
    std::string* stateName;
};



/** \class TransitionData
 *  \brief A class that stores transition data for the GameObject class.
 *  States are stored in the GameObject class as elements in an array. States are to be linked by index pairs. This
 *  class stores index pairs indicating the direction of flow between states. The class contains addition information
 *  about the transition including the valid transition name, a help string, and a function pointer that points to the
 *  transitions corresponding function. */
class TransitionData {
public:
    /** \brief Constructs a default TransitionData object. */
    TransitionData() = default;

    /** \brief Constructs a TransitionData object given the passed data. */
    TransitionData(size_t, size_t, std::string, std::string, bool (*)(const std::string&, GameEngine&));

    /** \brief Constructs a TransitionData object from another TransitionData object. Copy constructor. */
    TransitionData(const TransitionData&);

    /** \brief Destructs a TransitionData object. */
    ~TransitionData();

    /** \brief Assigns the values of the passed TransitionData object to the current. Copy assignment operator. */
    TransitionData& operator=(const TransitionData&);

    /** \brief Prints a brief summary of the TransitionData object. Stream insertion operator. */
    friend std::ostream& operator<<(std::ostream& os, const TransitionData& obj);


    /** \brief Get the first index, which indicates the state BEFORE transition. */
    size_t getIndex1() const { return *index1; }

    /** \brief Get the second index, which indicates the state AFTER transition. */
    size_t getIndex2() const { return *index2; }

    /** \brief Get the name of the transition. */
    std::string getTransitionName() const { return *transitionName; }

    /** \brief Get the help string. */
    std::string getHelpString() const { return *helpString; }

    /** \brief Get the transition function. */
    bool (*getTransitionFunction())(const std::string&, GameEngine&) { return transitionFunction; }

private:
    //  The first index which indicates the index of the state BEFORE transition
    size_t* index1;

    //  The second index which indicates the index of the state AFTER transition
    size_t* index2;

    //  The transition name
    std::string* transitionName;

    //  The help string
    std::string* helpString;

    //  The transition function
    bool (*transitionFunction)(const std::string&, GameEngine&);
};

/** \class GameEngine
 *  \brief A class that controls the flow of the game through notations of state and transitions. */
class GameEngine {
public:
    /** \brief Constructs a default GameEngine object with the specification as shown in the instructions. */
    GameEngine();

    /** \brief Constructs a GameEngine object from another GameEngine object. Copy constructor. */
    GameEngine(const GameEngine&);

    /** \brief Constructs a GameEngine object from another then deallocates all variables and sets pointer to an
     *  unspecified state. Move constructor. */
    GameEngine(GameEngine&&) noexcept;

    /** \brief Deconstructs a GameEngine object. */
    ~GameEngine();

    /** \brief Assigns the values of the passed GameEngine object to the current. Copy assignment operator. */
    GameEngine& operator=(const GameEngine&);

    /** \brief Assigns the values of the passed GameEngine object to the current, then deallocates dynamic variables
     *  and sets values to an unspecified state. Move assignment operator. */
    GameEngine& operator=(GameEngine&&) noexcept;

    friend std::ostream& operator<<(std::ostream&, const GameEngine&);

    /** \brief Starts the execution of the game object. Main entry point. */
    void execute();

    //  Getters & Setters
    /** \brief Sets the map pointer. */
    void setMap(Map*);
    /** \brief Returns the map pointer. */
    Map* getMap() const;

    /** \brief Returns a copy of the players list */
    std::vector<Player> getPlayers() const;

    /** \brief Adds a player to the players list */
    void addPlayerToGame(const Player&) const;

    /** \brief Sets the game running flag to false */
    void stopRunning() { *isRunning = false; }

private:
    //  A list of states
    State* states;

    //  A list of TransitionData objects (contains the data about transitions)
    TransitionData* transitionDatabase;

    //  The index of the current state of the game engine
    size_t* currentStateIndex;

    //  The size of the list of states member array
    size_t* statesSize;

    //  The size of the list of the transition data member array
    size_t* transitionDatabaseSize;

    //  Whether the game running or not
    bool* isRunning;

    //  Map pointer
    Map* map;

    //  List of players
    std::vector<Player>* playersList;


    //  Returns the index of the appropriate transition struct given the current state and the given transition name
    //  Returns -1 if no matching transition is found
    int indexOfTransition(const std::string& transitionName) const;

    //  Returns the help strings of the possible transitions in the current state.
    std::string getHelpStrings() const;

    //  Further process a command. Calls the corresponding transition function.
    void processCommand(TransitionData, const std::string&);
};

#endif // GAME_ENGINE_H
