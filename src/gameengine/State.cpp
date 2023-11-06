#include <iostream>

#include "../../headers/gameengine/State.h"

/** \brief Returns a std::string pointer with the given string. Memory management is delegated to another entity. */
static std::string* stringToPointer(std::string str) {
    return new std::string(std::move(str));
}

State::State()
        : stateName(stringToPointer("DEFAULT")) { }

State::State(const std::string& stateName)
        : stateName(stringToPointer(stateName)) { }

State::State(const State& otherState)
        : stateName(stringToPointer(otherState.getStateName())) { }

State& State::operator=(const State& otherState) {
    //  Check for self-assignment
    if (this != &otherState) {
        std::string stateNameValue = otherState.getStateName();
        this->stateName = stringToPointer(stateNameValue);
    }
    return *this;
}

State::State(State&& otherState) noexcept
        : stateName(stringToPointer(otherState.getStateName())) {
    otherState.stateName->clear();   //  Clear the contents of the string
}

State &State::operator=(State&& otherState) noexcept {
    //  Check for self-assignment
    if (this != &otherState) {
        std::string stateNameValue = otherState.getStateName();
        this->stateName = stringToPointer(stateNameValue);

        otherState.stateName->clear();
    }
    return *this;
}
State::~State() {
    delete stateName;       //  Deallocate memory
    stateName = nullptr;    //  Set to null ptr
}

std::ostream &operator<<(std::ostream& os, const State& otherState) {
    os << "State Name:\t" << otherState.getStateName();
    return os;
}

bool State::operator==(const State& otherState) const {
    return this->getStateName() == otherState.getStateName();
}

std::string State::getStateName() const {
    return *(this->stateName);
}

void State::setStateName(const std::string& newStateName) {
    *this->stateName = newStateName;
}