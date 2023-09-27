#include "../include/GameEngine.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define COMP345_RISK_DEFAULT_ARRAY_INIT                             \
    this->arraySize = 5;                                            \
    this->numberOfElements = 0;                                     \
    this->transitionNames = new std::string[this->arraySize];       \
    this->states = new State*[this->arraySize];                     \

//----------------------------------------------------------------------------------------------------------------------
//  STATE

//  todo delete after implementing separate functions for each command at each state
void temporaryFunction(const std::string& input) {
    std::cout << "\033[34m" << "ARGS: " << input << "\033[0m" << std::endl;
}

State::State() {
    COMP345_RISK_DEFAULT_ARRAY_INIT
    this->functions = new cmdFuncPtr[this->arraySize];
    this->stateName = "DEFAULT-STATE-NAME";
}

State::State(const std::string& stateName) {
    COMP345_RISK_DEFAULT_ARRAY_INIT
    this->functions = new cmdFuncPtr[this->arraySize];
    this->stateName = stateName;
}

State::State(const State &otherState) {
    this->stateName = otherState.stateName;
    this->arraySize = otherState.arraySize * 2;
    this->numberOfElements = otherState.numberOfElements;
    this->transitionNames = new std::string[this->arraySize];
    this->states = new State*[this->arraySize];
    this->functions = new cmdFuncPtr[this->arraySize];

    //  Copying the values from the other State object
    for (int i = 0; i < this->numberOfElements; i++) {
        this->transitionNames[i] = otherState.transitionNames[i];
        this->states[i] = otherState.states[i];
        this->functions[i] = otherState.functions[i];
    }
}

State::~State() {
    delete[] transitionNames;
    delete[] states;    //  Deallocates the pointers, not the values pointed by the pointers.
    delete[] functions;
}

State& State::operator=(const State &otherState) {
    if (this != &otherState) {  //  Check for self-assignment
        this->stateName = otherState.stateName;
        this->arraySize = otherState.arraySize * 2;
        this->numberOfElements = otherState.numberOfElements;
        this->transitionNames = new std::string[this->arraySize];
        this->states = new State*[this->arraySize];
        this->functions = new cmdFuncPtr[this->arraySize];

        //  Copying the values from the other State object
        for (int i = 0; i < this->numberOfElements; i++) {
            this->transitionNames[i] = otherState.transitionNames[i];
            this->states[i] = otherState.states[i];
            this->functions[i] = otherState.functions[i];
        }
    }

    return *this;
}

//  todo maybe implement checking that function addresses are the same
bool State::operator==(const State &otherState) const {
    //  Checking if the state name and the number of elements are equal (buffer capacity does not matter)
    if ((this->stateName != otherState.stateName)
     || (this->numberOfElements != otherState.numberOfElements)) {
        return false;
    }

    for (int i = 0; i < this->numberOfElements; i++) {
        if (!otherState.containsTransition(this->transitionNames[i])) {
            return false;
        }

        State resultingState = otherState.getState(this->transitionNames[i]);
        State comparisonState = this->getState(this->transitionNames[i]);

        if (resultingState.stateName != comparisonState.stateName) {
            return false;
        }
    }

    return true;
}


void State::addTransition(const std::string& transitionName, State* newState) {
    if (this->numberOfElements >= this->arraySize) {
        doubleCapacity();
    }

    this->transitionNames[numberOfElements] = transitionName;
    this->states[numberOfElements] = newState;
    this->functions[numberOfElements] = &temporaryFunction;     //  todo remove when implemented
    numberOfElements++;
}

bool State::containsTransition(const std::string &transitionName) const {
    for (int i = 0; i < this->numberOfElements; i++) {
        if (this->transitionNames[i] == transitionName) {
            return true;
        }
    }

    return false;
}

State& State::getState(const std::string& transitionName) const {
    for (int i = 0; i < this->numberOfElements; i++) {
        if (this->transitionNames[i] == transitionName) {
            return *(this->states[i]);
        }
    }
}

cmdFuncPtr State::getFunction(const std::string &transitionName) const {
    for (int i = 0; i < this->numberOfElements; i++) {
        if (this->transitionNames[i] == transitionName) {
            return this->functions[i];
        }
    }
}

void State::doubleCapacity() {
    const size_t newSize = this->arraySize * 2;

    std::string* bufferArrayString = new std::string[newSize];
    State** bufferArrayState = new State*[newSize];
    cmdFuncPtr* bufferArrayFunctions = new cmdFuncPtr[newSize];

    for (int i = 0; i < this->numberOfElements; i++) {
        bufferArrayString[i] = this->transitionNames[i];
        bufferArrayState[i] = this->states[i];
        bufferArrayFunctions[i] = this->functions[i];
    }

    this->transitionNames = bufferArrayString;
    this->states = bufferArrayState;
    this->functions = bufferArrayFunctions;
    this->arraySize = newSize;
}

void State::printContents() const {
    std::cout << "PRINTING:" << std::endl;
    std::cout << this->arraySize << std::endl;
    for (int i = 0; i < this->numberOfElements; i++) {
        std::cout << this->transitionNames[i] << "\tfrom: " << this->states[i]->stateName << std::endl;
    }
}



//----------------------------------------------------------------------------------------------------------------------
//  GAME_ENGINE

static std::vector<std::string> getTokens(std::string input) {
    //  todo kinda figure this out
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    while (iss >> input) {
        tokens.push_back(input);
    }

    return tokens;
}

GameEngine::GameEngine() {
    //  Defining the 'nodes'
    State* start = new State("start");
    State* mapLoaded = new State("map loaded");
    State* mapValidated = new State("map validated");
    State* playersAdded = new State("players added");
    State* assignReinforcement = new State("assign reinforcement");
    State* issueOrders = new State("issue orders");
    State* executeOrders = new State("execute orders");
    State* win = new State("win");
    State* end = new State("END");

    //  Defining the 'transitions' and 'commands'
    start->addTransition("loadmap", mapLoaded);
    mapLoaded->addTransition("loadmap", mapLoaded);
    mapLoaded->addTransition("validatemap", mapValidated);
    mapValidated->addTransition("addplayer", playersAdded);
    playersAdded->addTransition("addplayer", playersAdded);
    playersAdded->addTransition("assigncountries", assignReinforcement);
    assignReinforcement->addTransition("issueorder", issueOrders);
    issueOrders->addTransition("issueorder", issueOrders);
    issueOrders->addTransition("endissueorders", executeOrders);
    executeOrders->addTransition("endexecorders", assignReinforcement);
    executeOrders->addTransition("execorder", executeOrders);
    executeOrders->addTransition("win", win);
    win->addTransition("play", start);
    win->addTransition("end", end);

    //  Init member attributes
    //  this->size = 9;
    this->states = new State[] {*start, *mapLoaded, *mapValidated, *playersAdded, *assignReinforcement, *issueOrders,
                                *executeOrders, *win, *end};
    this->currentState = *start;
}

GameEngine::~GameEngine() {
    delete[] this->states;
}

void GameEngine::execute() {
    while (true) {
        std::cout << "CURRENT STATE:\t" << this->currentState.getStateName() << std::endl;
        std::cout << "Please enter a command:\n> ";

        std::string userInput;
        std::getline(std::cin, userInput);
        std::cout << std::endl;

        //  Execute the user input command, if valid.
        takeCommand(userInput);

        std::cout << "Press Enter to Continue...\n> ";
        std::getline(std::cin, userInput);
        system("cls");
    }
}

void GameEngine::takeCommand(const std::string& rawCommand) {
    std::vector<std::string> tokens = getTokens(rawCommand);
    std::string& transitionName = tokens.front();

    //  If the transition is INVALID for the current state.
    if (!this->currentState.containsTransition(transitionName)) {
        std::cout << "\033[1;31m" << "ERROR, COMMAND INVALID PLEASE TRY AGAIN" << "\033[0m" << std::endl;
        return;
    }

    //  If the transition is VALID, execute command and shift state
    cmdFuncPtr transitionFunction = this->currentState.getFunction(transitionName);
    transitionFunction(rawCommand);

    State& nextState = this->currentState.getState(transitionName);


    this->currentState = nextState;
    std::cout << "SWITCHED STATES TO: " << currentState.getStateName() << std::endl;
}
