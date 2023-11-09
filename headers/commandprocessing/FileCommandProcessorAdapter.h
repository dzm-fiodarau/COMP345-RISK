#ifndef FILE_COMMAND_PROCESSOR_ADAPTER_H
#define FILE_COMMAND_PROCESSOR_ADAPTER_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

#include <string>
#include <vector>
#include <queue>

#include "CommandProcessing.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class State;                                //  gameengine/State.h
class GameEngine;                           //  gameengine/GameEngine.h
class ConsoleCommandProcessorAdapter;       //  commandprocessing/ConsoleCommandProcessorAdapter.h

/** \class FileCommandProcessorAdapter
 *  \brief A class that supplies commands from a specified file. */
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    //  Constructors/Deconstructors
    /** \brief Constructs a 'default' FileCommandProcessorAdapter object. */
    FileCommandProcessorAdapter();

    /** \brief Constructs a FileCommandProcessorAdapter object, reading from a specified file. */
    explicit FileCommandProcessorAdapter(std::string);

    /** \brief Deconstructs a FileCommandProcessorAdapter object. */
    ~FileCommandProcessorAdapter() override;

    //  Overridden methods
    /** \brief Gets a valid command object given the valid state. */
    Command& getCommand(const State&) override;
    /** \brief Returns a deep copy of the object. */
    FileCommandProcessorAdapter* clone() const noexcept override;

private:
    //  A queue of commands.
    std::queue<Command> commandQueue;

    //  The path of the file to read from.
    std::string filePath;

    //  A backup command processor. Used to take commands from in the case that the commands from the specified file
    //  have been exhausted / run out.
    std::unique_ptr<ConsoleCommandProcessorAdapter> backupCommandProcessor{};

    //  Loads the contents from the specified file to the command queue.
    //  THROWS an exception if the specified file could not be found.
    void loadFileContents();
};

#endif  //  FILE_COMMAND_PROCESSOR_ADAPTER_H