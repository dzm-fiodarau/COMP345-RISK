#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

#include <string>
#include <vector>



//  Forward declaration of classes
class GameEngine;
class CommandProcessor;



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



/** \class State
 *  \brief Class to represent a state. */
class State {
public:
    //  Constructors/Deconstructor
    /** \brief Constructs a default state object. */
    State();
    /** \brief Constructs a state object with the specified state name.
     *  \param stateName The name of the state. */
    explicit State(const std::string& stateName);
    /** \brief Constructs a state object from another state object. Copy constructor.
     *  \param otherState The other state object to copy. */
    State(const State& otherState);
    /** \brief Constructs a state object from another state object then sets the values of the other state object to
     *        unspecified. Move constructor.
     *  \param otherState The other state object to extract values from. */
    State(State&& otherState) noexcept;
    /** \brief Deallocates a State object. */
    ~State();

    //  Operator overrides
    /** \brief Assigns the values of the passed state object into the current state object. Copy assignment operator.
     *  \param otherState The other state object to copy values from.
     *  \return The current state object with changed values. */
    State& operator=(const State& otherState);
    /** \brief Assigns the values of the passed state object into the current state object then sets the values of the
     *        other state object to unspecified. Move assignment operator.
     *  \param otherState The other state object to extract values from.
     *  \return The current state object with changed values. */
    State& operator=(State&& otherState) noexcept;
    /** \brief Prints the name of the state into the output stream. Stream insertion operator.*/
    friend std::ostream& operator<<(std::ostream& os, const State& otherState);
    /** \brief Compares two state objects and returns true if they are equal. False otherwise. */
    bool operator==(const State& otherState) const;

    //  Getter/Accessor methods
    /** /brief   Gets the name of the state. */
    std::string getStateName() const;

    //  Setter/Mutator methods
    /** \brief   Changes the name of the state. */
    void setStateName(const std::string&);

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
class GameEngine {
public:
    //  Constructors/Deconstructor
    /** \brief Constructs a default GameEngine object with the specification as shown in the instructions. */
    GameEngine();
    /** \brief Constructs a GameEngine object from another GameEngine object. Copy constructor. */
    GameEngine(const GameEngine&);
    /** \brief Constructs a GameEngine object from another then deallocates all variables and sets pointer to an
     *  unspecified state. Move constructor. */
    GameEngine(GameEngine&&) noexcept;
    /** \brief Deconstructs a GameEngine object. */
    ~GameEngine();

    //  Operator overrides
    /** \brief Assigns the values of the passed GameEngine object to the current. Copy assignment operator. */
    GameEngine& operator=(const GameEngine&);
    /** \brief Assigns the values of the passed GameEngine object to the current, then deallocates dynamic variables
     *  and sets values to an unspecified state. Move assignment operator. */
    GameEngine& operator=(GameEngine&&) noexcept;
    /** \brief Prints a brief description of the current internal configurations of the instance. */
    friend std::ostream& operator<<(std::ostream&, const GameEngine&);

    //  Other Methods
    /** \brief Starts the execution of the game object. Main entry point. */
    void execute();
    /** \brief Sets the game running flag to false */
    void stopRunning() { *isRunning = false; }

    //  Getter/Accessor methods
    /** \brief Gets the list of states of the game engine. */
    const State* getStates() const;
    /** \brief Gets a list of 'TransitionData' objects that contain data about transitions from state to state. */
    const TransitionData* getTransitionsDatabase() const;
    /** \brief Gets the size of the states list. */
    size_t getStatesSize() const;
    /** \brief Gets the size of the transitions database list. */
    size_t getTransitionDatabaseSize() const;

    //  Setter/Mutator methods
    /** \brief Sets the states configuration of the object. Takes a raw array. */
    void setStates(State*, size_t);
    /** \brief Sets the states configuration of the object. Takes a std::vector<State> object. */
    void setStates(std::vector<State>);
    /** \brief Sets the transition database configuration of the object. Takes a raw array. */
    void setTransitionData(TransitionData*, size_t);
    /** \brief Sets the transition database configuration of the object. Takes in a std::vector<TransitionData> object. */
    void setTransitionData(std::vector<TransitionData>);
    /** \brief Sets the command processor for the object. */
    void setCommandProcessor(const CommandProcessor&);

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

    //  The command processor to take commands from
    CommandProcessor* commandProcessor;


    //  Returns the index of the appropriate transition struct given the current state and the given transition name
    //  Returns -1 if no matching transition is found
    int indexOfTransition(const std::string& transitionName) const;

    //  Returns the help strings of the possible transitions in the current state.
    std::string getHelpStrings() const;

    //  Further process a command. Calls the corresponding transition function.
    void processCommand(TransitionData, const std::string&);

    //  Assigns the 'commandProcessor' member variable on initialization. Meant to be called in the constructor.
    void initializeCommandProcessor();
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif // GAME_ENGINE_H
