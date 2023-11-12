#ifndef STATE_H
#define STATE_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#endif

#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <unordered_map>

//  Forward declaration of required classes from other header files. (included in .cpp file)
class GameEngine;                //  GameEngine.h
class State;

/** \brief Typedef for a tuple containing all the data for a transition from one state to another state. These data are:
 *         <ul><li>(<code>std::string</code>) The transition name/command.
 *             <li>(<code>State*</code>) A pointer to the next <code>State</code> object.
 *             <li>(<code>int</code>) The number of arguments expected. A value of -1 implies a variable number of arguments.
 *             <li>(<code>std::string</code>) A 'help' string associated with the transition. Printed out to help the user.
 *             <li>(<code>bool (*)(const std::vector<std::string>&, GameEngine&)</code>) An associated transition function
 *                 to execute when transitioning to the next state.</ul>
 */
using TransitionData = std::tuple<std::string, State*, int, std::string, bool (*)(const std::vector<std::string>&, GameEngine&)>;

/** \brief Class to represent a state.
 */
class State {
public:
    //  Constructors/Deconstructor
    /** \brief Constructs a default state object. */
    State();

    /** \brief Constructs a state object with the specified state name. */
    explicit State(std::string  stateName);

    /** \brief Constructs a state object from another state object. Copy constructor. */
    State(const State& otherState);

    /** \brief Deallocates a State object. */
    ~State();


    //  Operator overrides
    State& operator=(const State& otherState);

    /** \brief Prints the name of the state into the output stream. Stream insertion operator.*/
    friend std::ostream& operator<<(std::ostream& os, const State& otherState);

    /** \brief Compares two state objects and returns true if they are equal. False otherwise. */
    bool operator==(const State& otherState) const;


    //  Getter/Accessor methods
    std::string getStateName() const noexcept;

    std::optional<State*> getNextState(const std::string& transitionName) const;

    /** \brief Attempts to return the number of arguments required for a transition, given a transition name. */
    std::optional<int> getRequiredArguments(const std::string& transitionName) const;

    /** \brief Attempts to return the help string of a transition, given a transition name. */
    std::optional<std::string> getHelpString(const std::string& transitionName) const;

    /** \brief Attempts to return the associated function of a transition (transition function), given a transition name. */
    std::optional<bool (*)(const std::vector<std::string>&, GameEngine&)> getTransitionFunction(const std::string& transitionName) const;


    //  Other methods
    /** \brief Adds a transition from this <code>State</code> object to another <code>State</code> object. */
    void addTransition(const std::string& transitionName, State* nextState, int numberOfArguments, const std::string& helpString,
                       bool (*transitionFunction)(const std::vector<std::string>&, GameEngine&));

    /** \brief Returns true if there exits a valid transition corresponding to the passed transition name. */
    bool isValidTransition(const std::string& transitionName) const;

    /** \brief Returns a deep copy of this state, as well as all other connected states from this state.
     *  \remarks Can be costly, call only whenever needed.
     */
    State* deepCopy() const;

    /** \brief Returns a list of help-strings formatted into a single string variable. */
    std::string getHelpStrings() const;
    std::vector<std::string> getHelpStringsAsVector() const;

private:
    //  The name of the state
    std::string stateName;

    //  A vector of tuples. Each tuple contains data about a valid transition from one state to another.
    std::vector<TransitionData> transitionData{};

    //  Helper method to help deep copy
    State* deepCopyHelper(std::unordered_map<const State*, State*>& stateMap) const;
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif  //  STATE_H