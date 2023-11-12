
#include "../../headers/player/AggressivePlayerStrategy.h"

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *owner) : PlayerStrategy(owner) {

}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {

}


void AggressivePlayerStrategy::play() {

}

void AggressivePlayerStrategy::issueOrders(CommandProcessor*) {

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

