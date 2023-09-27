#ifndef COMP345_RISK_GAMEENGINE_H
#define COMP345_RISK_GAMEENGINE_H

#include <string>

/**
 * \brief   Function pointer for functions to be called when executing the commands when transitioning from one State
 *          to another State.
 */
typedef void (*cmdFuncPtr)(const std::string&);

class State {
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
    explicit State(const std::string& stateName);

    /**
     * \brief               Constructs a state object using the values from another State object. Avoids deep copying.
     * \param otherState    The other State object to copy member variables from.
     */
    State(const State& otherState);

    /**
     * \brief   Deconstructs the State object.
     */
    ~State();


    /**
     * \brief               Assigns values of the passed State object into the current state object.
     * \param otherState    The other State object to copy member variables from.
     * \return              Returns instance of current object with values changed.
     */
    State& operator=(const State& otherState);

    /**
     * \brief               Compares two State objects. Returns true if objects are equal; false otherwise.
     * \remarks             Two State objects are considered equal if they have the same state name, contain the same
     *                      number of transitions, same transition names with the same corresponding state name
     *                      (we do not directly check if the state objects are equal, do not want infinite recursive).
     * \param otherState    The other State object to compare.
     * \return              True if the two State objects are considered equal. False otherwise.
     */
    bool operator==(const State& otherState) const;


    /**
     * \brief                   Adds a transition to another State object.
     * \param transitionName    The name of the transition.
     * \param newState          The State object liked to by the transition.
     */
    void addTransition(const std::string& transitionName, State* newState);

    /**
     * \break                   Checks whether a transition exist, given the transition name.
     * @param transitionName    The name of the transition to check for.
     * @return                  True if the transition exists, false otherwise.
     */
    bool containsTransition(const std::string& transitionName) const;

    /**
     * \brief                   Returns the state given by the transition specified by the transition name.
     * \remarks                 Make sure to assure that transition exists with 'containsTransition' before attempting
     *                          to get a state. Not doing so may result in undefined behavior.
     * \param transitionName    The name of the transition.
     * \return                  The corresponding State object.
     */
    State& getState(const std::string& transitionName) const;

    /**
     * \brief                   Returns the function given by the transition specified by the transition name.
     * \remarks                 Make sure to assure that transition exists with 'containsTransition' before attempting.
     *                          to get a state. Not doing so may result in undefined behavior.
     * \param transitionName    The name of the transition.
     * \return                  The corresponding function.
     */
    cmdFuncPtr getFunction(const std::string& transitionName) const;

    /**
     * \brief                   Prints the contents of the State object.
     * todo find a way to override the stream extraction operator
     */
    void printContents() const;

    /**
     * /brief   Returns the state name.
     * /return  Returns the state name.
     */
    inline std::string getStateName() const {
        return stateName;
    }

private:
    //  Name of the State object
    std::string stateName;

    //  The following dynamic arrays are meant to be the same size, with each element corresponding to each other
    //  Dynamic array of transition names
    std::string* transitionNames;

    //  Dynamic array of State objects
    State** states;

    //  Dynamic array of function pointers, with the respective behavior for each command
    cmdFuncPtr* functions;

    //  Capacity of the dynamic arrays, all share the same capacity
    size_t arraySize;

    //  Number of elements currently contained in the dynamic arrays
    size_t numberOfElements;

    //  Doubles the capacity of the buffer arrays
    void doubleCapacity();
};



class GameEngine {
public:
    static GameEngine& getInstance()  {
        static GameEngine instance;
        return instance;
    }

    void execute();
    void takeCommand(const std::string& rawCommand);

private:
    State* states;
    State currentState;

    GameEngine();
    ~GameEngine();

    GameEngine(const GameEngine& other) = delete;
    GameEngine& operator=(const GameEngine& other) = delete;
    GameEngine& operator=(const GameEngine& other) const = delete;
};

#endif //COMP345_RISK_GAMEENGINE_H
