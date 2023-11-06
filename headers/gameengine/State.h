#ifndef STATE_H
#define STATE_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#endif

#include <string>

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

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif  //  STATE_H