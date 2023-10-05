//----------------------------------------------------------------------------------------------------------------------
//  MACROS

#include "../headers/GameEngine.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define STATE_ARRAY_INIT(x, y)                                \
    this->arraySize = x;                                      \
    this->numberOfElements = y;                               \
    this->transitionNames = new std::string[this->arraySize]; \
    this->states = new State *[this->arraySize];              \
    this->functions = new cmdFuncPtr[this->arraySize];

#define PRESS_ENTER_TO_CONTINUE(clearConsole)   \
    std::string _IGNORE_STRING;                 \
    std::cout << "Press Enter to Continue... "; \
    std::getline(std::cin, _IGNORE_STRING);     \
                                                \
    if (clearConsole)                           \
    {                                           \
        system("cls");                          \
    }

//----------------------------------------------------------------------------------------------------------------------
//  TRANSITION FUNCTIONS
//  todo implement transition functions

void printInput(const std::string &input)
{
    std::cout << "\033[34m"
              << "ARGS:\t" << input << "\033[0m" << std::endl;
}

void loadMap(const std::string &input)
{
    std::cout << "\033[34m"
              << "loading map w/ args:\t" << input << "\033[0m" << std::endl;
}

void validateMap(const std::string &input)
{
    std::cout << "\033[34m"
              << "validating map w/ args:\t" << input << "\033[0m" << std::endl;
}

void addPlayer(const std::string &input)
{
    std::cout << "\033[34m"
              << "adding player w/ args:\t" << input << "\033[0m" << std::endl;
}

void assignCountries(const std::string &input)
{
    std::cout << "\033[34m"
              << "assigning countries w/ args:\t" << input << "\033[0m" << std::endl;
}

void issueOrder(const std::string &input)
{
    std::cout << "\033[34m"
              << "issuing order w/ args:\t" << input << "\033[0m" << std::endl;
}

void endIssueOrders(const std::string &input)
{
    std::cout << "\033[34m"
              << "FINISHED issuing orders w/ args:\t" << input << "\033[0m" << std::endl;
}

void executeOrder(const std::string &input)
{
    std::cout << "\033[34m"
              << "executing order w/ args:\t" << input << "\033[0m" << std::endl;
}

void endExecuteOrders(const std::string &input)
{
    std::cout << "\033[34m"
              << "FINISHED executing orders w/ args:\t" << input << "\033[0m" << std::endl;
}

void winGame(const std::string &input)
{
    std::cout << "\033[34m"
              << "win w/ args:\t" << input << "\033[0m" << std::endl;
}

void endProgram(const std::string &ignored)
{
    GameEngine &gameEngine = GameEngine::getInstance();
    gameEngine.stop();
}

//----------------------------------------------------------------------------------------------------------------------
//  STATE

State::State()
{
    STATE_ARRAY_INIT(5, 0)
    this->stateName = "DEFAULT-STATE-NAME";
}

State::State(const std::string &stateName)
{
    STATE_ARRAY_INIT(5, 0)
    this->stateName = stateName;
}

State::State(const State &otherState)
{
    STATE_ARRAY_INIT(otherState.arraySize * 2, otherState.numberOfElements)
    this->stateName = otherState.stateName;

    //  Copying the values from the other State object
    for (int i = 0; i < this->numberOfElements; i++)
    {
        this->transitionNames[i] = otherState.transitionNames[i];
        this->states[i] = otherState.states[i];
        this->functions[i] = otherState.functions[i];
    }
}

State::~State()
{
    delete[] transitionNames;
    delete[] states; //  Deallocates the pointers, not the values pointed by the pointers.
    delete[] functions;
}

State &State::operator=(const State &otherState)
{
    if (this != &otherState)
    { //  Check for self-assignment
        STATE_ARRAY_INIT(otherState.arraySize * 2, otherState.numberOfElements)
        this->stateName = otherState.stateName;

        //  Copying the values from the other State object
        for (int i = 0; i < this->numberOfElements; i++)
        {
            this->transitionNames[i] = otherState.transitionNames[i];
            this->states[i] = otherState.states[i];
            this->functions[i] = otherState.functions[i];
        }
    }

    return *this;
}

//  todo maybe implement checking that function addresses are the same
bool State::operator==(const State &otherState) const
{
    //  Checking if the state name and the number of elements are equal (buffer capacity does not matter)
    if ((this->stateName != otherState.stateName) || (this->numberOfElements != otherState.numberOfElements))
    {
        return false;
    }

    for (int i = 0; i < this->numberOfElements; i++)
    {
        if (!otherState.containsTransition(this->transitionNames[i]))
        {
            return false;
        }

        State resultingState = otherState.getState(this->transitionNames[i]);
        State comparisonState = this->getState(this->transitionNames[i]);

        if (resultingState.stateName != comparisonState.stateName)
        {
            return false;
        }
    }

    return true;
}

std::ostream &operator<<(std::ostream &os, const State &state)
{
    os << state.stateName << "\t" << state.numberOfElements;
    return os;
}

void State::addTransition(const std::string &transitionName, State *newState, const cmdFuncPtr &functionPtr)
{
    if (this->numberOfElements >= this->arraySize)
    {
        doubleCapacity();
    }

    this->transitionNames[numberOfElements] = transitionName;
    this->states[numberOfElements] = newState;
    this->functions[numberOfElements] = functionPtr;
    numberOfElements++;
}

bool State::containsTransition(const std::string &transitionName) const
{
    for (int i = 0; i < this->numberOfElements; i++)
    {
        if (this->transitionNames[i] == transitionName)
        {
            return true;
        }
    }

    return false;
}

State &State::getState(const std::string &transitionName) const
{
    for (int i = 0; i < this->numberOfElements; i++)
    {
        if (this->transitionNames[i] == transitionName)
        {
            return *(this->states[i]);
        }
    }

    //  throw an error.
    throw std::runtime_error("UNKNOWN TRANSITION NAME");
}

cmdFuncPtr State::getFunction(const std::string &transitionName) const
{
    for (int i = 0; i < this->numberOfElements; i++)
    {
        if (this->transitionNames[i] == transitionName)
        {
            return this->functions[i];
        }
    }

    //  throw an error.
    throw std::runtime_error("UNKNOWN TRANSITION NAME");
}

void State::doubleCapacity()
{
    const size_t newSize = this->arraySize * 2;

    std::string *bufferArrayString = new std::string[newSize];
    State **bufferArrayState = new State *[newSize];
    cmdFuncPtr *bufferArrayFunctions = new cmdFuncPtr[newSize];

    for (int i = 0; i < this->numberOfElements; i++)
    {
        bufferArrayString[i] = this->transitionNames[i];
        bufferArrayState[i] = this->states[i];
        bufferArrayFunctions[i] = this->functions[i];
    }

    this->transitionNames = bufferArrayString;
    this->states = bufferArrayState;
    this->functions = bufferArrayFunctions;
    this->arraySize = newSize;

    bufferArrayString = nullptr;
    bufferArrayState = nullptr;
    bufferArrayFunctions = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//  GAME_ENGINE

static std::vector<std::string> getTokens(std::string input)
{
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    while (iss >> input)
    {
        tokens.push_back(input);
    }

    return tokens;
}

GameEngine &GameEngine::getInstance()
{
    static GameEngine instance;
    return instance;
}

GameEngine::GameEngine()
{
    //  Defining the 'nodes'
    State *start = new State("start");
    State *mapLoaded = new State("map loaded");
    State *mapValidated = new State("map validated");
    State *playersAdded = new State("players added");
    State *assignReinforcement = new State("assign reinforcement");
    State *issueOrders = new State("issue orders");
    State *executeOrders = new State("execute orders");
    State *win = new State("win");
    State *end = new State("END");

    //  Defining the 'transitions' and 'commands'
    start->addTransition("loadmap", mapLoaded, &loadMap);
    mapLoaded->addTransition("loadmap", mapLoaded, &loadMap);
    mapLoaded->addTransition("validatemap", mapValidated, &validateMap);
    mapValidated->addTransition("addplayer", playersAdded, &addPlayer);
    playersAdded->addTransition("addplayer", playersAdded, &addPlayer);
    playersAdded->addTransition("assigncountries", assignReinforcement, &assignCountries);
    assignReinforcement->addTransition("issueorder", issueOrders, &issueOrder);
    issueOrders->addTransition("issueorder", issueOrders, &issueOrder);
    issueOrders->addTransition("endissueorders", executeOrders, &endIssueOrders);
    executeOrders->addTransition("endexecorders", assignReinforcement, &endExecuteOrders);
    executeOrders->addTransition("execorder", executeOrders, &executeOrder);
    executeOrders->addTransition("win", win, &winGame);
    win->addTransition("play", start, &printInput);
    win->addTransition("end", end, &endProgram);

    //  Init member attributes
    //  this->size = 9;
    this->states = new State[]{*start, *mapLoaded, *mapValidated, *playersAdded, *assignReinforcement, *issueOrders,
                               *executeOrders, *win, *end};
    this->currentState = *start;
    this->isRunning = true;
}

GameEngine::~GameEngine()
{
    delete[] this->states;
}

void GameEngine::execute()
{
    while (this->isRunning)
    {
        std::cout << "CURRENT STATE:\t[" << this->currentState.getStateName() << "]" << std::endl;
        std::cout << "Please enter a command:\n> ";

        std::string userInput;
        std::getline(std::cin, userInput);
        std::cout << std::endl;

        //  Execute the user input command, if valid.
        takeCommand(userInput);

        //  Press enter to continue.
        PRESS_ENTER_TO_CONTINUE(true)
    }
}

void GameEngine::stop()
{
    this->isRunning = false;
}

void GameEngine::takeCommand(const std::string &rawCommand)
{

    //  Get the tokens of the raw command
    std::vector<std::string> tokens = getTokens(rawCommand);
    std::string &transitionName = tokens.front();

    //  If the transition is INVALID for the current state.
    if (!this->currentState.containsTransition(transitionName))
    {
        std::cout << "\033[1;31m"
                  << "ERROR, COMMAND INVALID PLEASE TRY AGAIN"
                  << "\033[0m" << std::endl;
        return;
    }

    //  If the transition is VALID, execute command and shift state
    State &nextState = this->currentState.getState(transitionName);
    cmdFuncPtr transitionFunction = this->currentState.getFunction(transitionName);

    transitionFunction(rawCommand);

    this->currentState = nextState;
    std::cout << "\033[34m"
              << "SWITCHED STATES TO:\t[" << currentState.getStateName() << "]\033[0m" << std::endl;
}

std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine)
{
    os << gameEngine.currentState.getStateName();
    return os;
}
