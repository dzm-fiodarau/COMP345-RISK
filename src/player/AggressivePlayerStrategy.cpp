
#include "../../headers/player/AggressivePlayerStrategy.h"

AggressivePlayerStrategy::AggressivePlayerStrategy(const Player *owner) : PlayerStrategy(owner) {
    //  Empty
}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {

}


void AggressivePlayerStrategy::play() {

}

Order *AggressivePlayerStrategy::issueOrder() {
    return nullptr;
}

std::vector<Territory *> AggressivePlayerStrategy::toAttack() {
    return std::vector<Territory *>();
}

std::vector<Territory *> AggressivePlayerStrategy::toDefend() {
    return std::vector<Territory *>();
}

PlayerStrategy *AggressivePlayerStrategy::clone() {
    return nullptr;
}

