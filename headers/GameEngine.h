#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <string>

//  Function pointer for functions to be called when executing the commands when transitioning from one State to another
//  State.
typedef void (*cmdFuncPtr)(const std::string &);

/**
 * \class   State
 * \brief   A class to represent a State object. Contains a state name and links or transitions to other State objects.
 */
class State
{
public:
    /**
     * \brief   Constructs a state object with a default name of "DEFAULT-STATE-NAME" and an initial capacity of five
     *          transitions.
     */
    State();

    /**
     * \brief               Constructs a state object with a specified name and an initial capacity of five transitions.
     * \param stateName     The name of the state.
     */
    explicit State(const std::string &stateName);

    /**
     * \brief               Constructs a state object using the values from another State object. Avoids deep copying.
     * \param otherState    The other State object to copy member variables from.
     */
    State(const State &otherState);

    /**
     * \brief   Deconstructs the State object.
     */
    ~State();

    /**
     * \brief               Assigns values of the passed State object into the current state object.
     * \param otherState    The other State object to copy member variables from.
     * \return              Returns instance of current object with values changed.
     */
    State &operator=(const State &otherState);

    /**
     * \brief               Compares two State objects. Returns true if objects are equal; false otherwise.
     * \remarks             Two State objects are considered equal if they have the same state name, contain the same
     *                      number of transitions, same transition names with the same corresponding state name
     *                      (we do not directly check if the state objects are equal, do not want infinite recursive).
     * \param otherState    The other State object to compare.
     * \return              True if the two State objects are considered equal. False otherwise.
     */
    bool operator==(const State &otherState) const;

    /**
     * /brief               Stream Insertion Override. Prints state name and number of transitions.
     */
    friend std::ostream &operator<<(std::ostream &os, const State &state);

    /**
     * \brief                   Adds a transition to another State object.
     * \param transitionName    The name of the transition.
     * \param newState          The State object liked to by the transition.
     */
    void addTransition(const std::string &transitionName, State *newState, const cmdFuncPtr &functionPtr);

    /**
     * \break                   Checks whether a transition exist, given the transition name.
     * @param transitionName    The name of the transition to check for.
     * @return                  True if the transition exists, false otherwise.
     */
    bool containsTransition(const std::string &transitionName) const;

    /**
     * \brief                   Returns the state given by the transition specified by the transition name.
     * \remarks                 Make sure to assure that transition exists with 'containsTransition' before attempting
     *                          to get a state. Not doing so may result in undefined behavior.
     * \param transitionName    The name of the transition.
     * \return                  The corresponding State object.
     */
    State &getState(const std::string &transitionName) const;

    /**
     * \brief                   Returns the function given by the transition specified by the transition name.
     * \remarks                 Make sure to assure that transition exists with 'containsTransition' before attempting.
     *                          to get a state. Not doing so may result in undefined behavior.
     * \param transitionName    The name of the transition.
     * \return                  The corresponding function.
     */
    cmdFuncPtr getFunction(const std::string &transitionName) const;

    /**
     * /brief   Returns the state name.
     * /return  Returns the state name.
     */
    inline std::string getStateName() const
    {
        return stateName;
    }

private:
    //  Name of the State object
    std::string stateName;

    //  The following dynamic arrays are meant to be the same size, with each element corresponding to each other
    //  Dynamic array of transition names
    std::string *transitionNames;

    //  Dynamic array of State objects
    State **states;

    //  Dynamic array of function pointers, with the respective behavior for each command
    cmdFuncPtr *functions;

    //  Capacity of the dynamic arrays, all share the same capacity
    size_t arraySize;

    //  Number of elements currently contained in the dynamic arrays
    size_t numberOfElements;

    //  Doubles the capacity of the buffer arrays
    void doubleCapacity();
};

/**
 * \class   GameEngine
 * \brief   A class that controls the flow of the game through States.
 *
 * The game engine constructs a set of linked states, with each state representing a specific stage of the game. The
 * current state dictates the available commands and executable actions.
 */
class GameEngine
{
public:
    /**
     * \brief   Gets the singleton instance of the GameEngine class;
     * \return  Returns an instance of GameEngine.
     */
    static GameEngine &getInstance();

    /**
     * \brief   Start the main program loop.
     */
    void execute();

    /**
     * \brief   Signals the GameEngine object to stop.
     */
    void stop();

    /**
     * \brief               Takes a command. Determines if the command is valid depending on the current state.
     *                      If command is valid, executes command and transitions into another state. Otherwise, an
     *                      error message is thrown.
     * \param rawCommand    The raw command as entered by the user.
     */
    void takeCommand(const std::string &rawCommand);

    /**
     * /brief               Stream Insertion Override. Prints name of the current state.
     */
    friend std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine);

private:
    //  A dynamic array of all states used. Deallocated when deconstructor called.
    State *states;

    //  The current state.
    State currentState;

    //  True if the game is running.
    bool isRunning;

    //  Default Constructor, constructs the set of State objects.
    GameEngine();

    //  Deconstructor, deallocates all the state objects initialized in the constructor.
    ~GameEngine();

    GameEngine(const GameEngine &other) = delete;
    GameEngine &operator=(const GameEngine &other) = delete;
    GameEngine &operator=(const GameEngine &other) const = delete;
};

#endif // GAME_ENGINE_H
