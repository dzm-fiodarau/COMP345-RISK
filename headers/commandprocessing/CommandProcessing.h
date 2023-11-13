#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include "../LoggingObserver.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class State;                //  GameEngine.h
class GameEngine;           //  GameEngine.h

/** \class Command
 *  \brief Class that encapsulates a command, storing additional information and providing helpful methods. */
class Command : public ILoggable, public Subject {
public:
    //  Constructors/Destructor
    /** \brief Constructs a Command object with default values. */
    Command();
    /** \brief Constructs a Command object with a given raw command string. */
    explicit Command(const std::string&);
    /** \brief Constructs a Command object with the given values. */
    Command(const std::string&, std::string , const bool*);
    /** \brief Constructs a Command object from another Command object. */
    Command(const Command&);
    /** \brief Deconstructs a Command object. */
    ~Command() override;


    //  Operator overrides
    /** \brief Assigns values from another Command object to the current object. */
    Command& operator=(const Command&);
    /** \brief Stream insertion operator override, outputs non-null values. */
    friend std::ostream& operator<<(std::ostream&, const Command&);


    //  Getter/Accessor methods
    /** \brief Gets the raw command. */
    std::string getRawCommand() const;
    /** \brief Gets the effect of command execution, if executed. */
    std::string getEffect() const;
    /** \brief Returns a pointer to a boolean variable indicating whether or not execution was successful or not.
     *  \remarks A 'nullptr' indicates that the command has not been executed yet. */
    const bool* getIsValidExecution() const;


    //  Setter/Mutator methods
    /** \brief Updates the raw command. */
    void setRawCommand(const std::string&);
    /** \brief Updates the effect. */
    void setEffect(const std::string&);
    /** \brief Sets the execution status of the command. */
    void setExecutionStatus(bool);


    //  Other methods
    /** \brief Returns the first token in the raw command string. */
    std::string getFirstToken() const;
    /** \brief Returns a vector containing the remaining tokens from the raw command string.
     * Returns a string list (//vector) containing every token but the first. */
    std::vector<std::string> getRemainingTokens() const;
    /** \brief Returns the number of arguments. */
    size_t getNumberOfArguments() const;

    /** \brief Creates the string to log */
    string stringToLog() override;

private:
    //  The raw command, as read from an input stream.
    std::string rawCommand;

    //  The effect message, or execution message. Could be an error or success message.
    std::string effect;

    //  Boolean on whether the command executed properly.
    bool* isValidExecution;

    //  A vector of tokens.
    std::vector<std::string> tokens;
};



/** \class CommandProcessor
 *  \brief Abstract class representing a source of pre-processed commands.
 *  \remarks Note that commands returned are syntactically valid, however, they may still be erroneous in a specific
 *  context (ex. command to attack a player that does not exist). Error handling for those types of exceptions are
 *  delegated to the caller or the client. */
class CommandProcessor : public ILoggable, public Subject {
public:
    //  Constructors/Deconstructor
    /** \brief Constructs a default CommandProcessor object. */
    CommandProcessor();

    /** \brief Deconstructs a CommandProcessor object. */
    ~CommandProcessor() override;

    //  Virtual methods
    /** \brief Returns a <code>Command</code> object from some source. */
    virtual Command& getCommand(const State&) = 0;

    /** \brief Returns a raw command from some source.
     *  \remarks <p>Use only when you need to obtain commands from objects other than the <code>GameEngine</code></p>
     */
    virtual std::string getCommand() = 0;

    /** \brief Returns a deep copy of the object. */
    virtual CommandProcessor* clone() const noexcept = 0;

    //  Other methods
    /** \brief Determines if a command is valid given the current state.
     *  \remarks Only tests if the command if it is syntactically valid, which does not guarantee that the command will
     *  be correctly executed. */
    bool validate(const Command&, const State&);

    /**
     * \brief   Creates the string to log
     */
    string stringToLog() override;

protected:

    /** \brief Prints an error block in case an incorrect raw command was inputted. */
    static void printErrorMenu(std::vector<std::string> helpStrings);
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif  //  COMMAND_PROCESSING_H