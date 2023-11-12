#ifndef CONSOLE_COMMAND_PROCESSOR_ADAPTER_H
#define CONSOLE_COMMAND_PROCESSOR_ADAPTER_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

#include "CommandProcessing.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class State;                                //  gameengine/State.h
class GameEngine;                           //  gameengine/GameEngine.h

/** \class ConsoleCommandProcessorAdapter
 *  \brief A class that supplies commands from the standard cpp input stream. */
class ConsoleCommandProcessorAdapter final : public CommandProcessor {
public:
    //  Constructors/Deconstructor
    /** \brief Constructs a 'default' ConsoleCommandProcessorAdapter object. */
    ConsoleCommandProcessorAdapter();

    /** \brief Deconstructs a ConsoleCommandProcessorAdapter object. */
    ~ConsoleCommandProcessorAdapter() override = default;

    //  Overridden methods
    /** \brief Gets a valid command object given the valid state. */
    Command& getCommand(const State&) override;

    /** \brief Returns a raw command. */
    std::string getCommand() override;

    /** \brief Returns a deep copy of the object. */
    ConsoleCommandProcessorAdapter* clone() const noexcept override;
};

#endif  //  CONSOLE_COMMAND_PROCESSOR_ADAPTER_H