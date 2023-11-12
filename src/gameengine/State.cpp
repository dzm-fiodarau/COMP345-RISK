#include <iostream>
#include <utility>

#include "../../headers/gameengine/State.h"

State::State()
        : stateName("DEFAULT") { }

State::State(std::string stateName)
        : stateName(std::move(stateName)) { }

State::State(const State& otherState)
        : stateName(otherState.getStateName()) { }

State& State::operator=(const State& otherState) {
    //  Check for self-assignment
    if (this != &otherState) {
        std::string stateNameValue = otherState.getStateName();
        this->stateName = stateNameValue;
    }
    return *this;
}

State::~State() = default;

std::ostream &operator<<(std::ostream& os, const State& otherState) {
    os << "State Name:\t" << otherState.getStateName();
    return os;
}

bool State::operator==(const State& otherState) const {
    return this->getStateName() == otherState.getStateName();
}

std::string State::getStateName() const noexcept {
    return stateName;
}

void State::addTransition(const std::string& transitionName, State* nextState, int numberOfArguments, const std::string& helpString,
                          bool (*transitionFunction)(const std::vector<std::string> &, GameEngine &)) {
    transitionData.emplace_back(transitionName, nextState, numberOfArguments, helpString, transitionFunction);
}

/** \brief Returns an iterator to some position in the transition data vector given a transition name.
 */
static auto getIterator(const std::vector<TransitionData>& transitionData, const std::string& transitionName) {
    auto it = std::find_if(transitionData.begin(), transitionData.end(), [transitionName] (const TransitionData& data) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = data;
        return transitionName == _transName;
    });

    return it;
}

bool State::isValidTransition(const std::string& transitionName) const {
    return getIterator(transitionData, transitionName) != transitionData.end();
}

State *State::deepCopy() const {
    std::unordered_map<const State*, State*> stateMap;
    return deepCopyHelper(stateMap);
}

State *State::deepCopyHelper(std::unordered_map<const State*, State*>& stateMap) const {
    if (stateMap.find(this) != stateMap.end())
        return stateMap[this];

    auto* copiedState = new State(stateName);
    stateMap[this] = copiedState;

    for (const auto& data : transitionData) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = data;
        if (_nextState)
            copiedState->addTransition(_transName, _nextState->deepCopyHelper(stateMap), _noArgs, _helpStr, _transFunc);
        else
            copiedState->addTransition(_transName, _nextState, _noArgs, _helpStr, _transFunc);
    }

    return copiedState;
}

std::optional<State *> State::getNextState(const std::string &transitionName) const {
    auto it = getIterator(transitionData, transitionName);

    if (it != transitionData.end()) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = *it;
        return _nextState;
    } else {
        return {};  //  Return an empty std::optional
    }
}

std::optional<int> State::getRequiredArguments(const std::string &transitionName) const {
    auto it = getIterator(transitionData, transitionName);

    if (it != transitionData.end()) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = *it;
        return _noArgs;
    } else {
        return {};  //  Return an empty std::optional
    }
}

std::optional<std::string> State::getHelpString(const std::string &transitionName) const {
    auto it = getIterator(transitionData, transitionName);

    if (it != transitionData.end()) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = *it;
        return _helpStr;
    } else {
        return {};  //  Return an empty std::optional
    }
}

std::optional<bool(*)(const std::vector<std::string>&, GameEngine&)> State::getTransitionFunction(const std::string &transitionName) const {
    auto it = getIterator(transitionData, transitionName);

    if (it != transitionData.end()) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = *it;
        return _transFunc;
    } else {
        return {};  //  Return an empty std::optional
    }
}

std::string State::getHelpStrings() const {
    std::string stringBuilder;
    for (const auto& data : transitionData) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = data;
            stringBuilder += "\n\"" + _helpStr + "\"";
    }
    return stringBuilder;
}

std::vector<std::string> State::getHelpStringsAsVector() const {
    std::vector<std::string> helpStrings{};
    for (const auto& data : transitionData) {
        auto [_transName, _nextState, _noArgs, _helpStr, _transFunc] = data;
        helpStrings.push_back(_helpStr);
    }
    return helpStrings;
}
