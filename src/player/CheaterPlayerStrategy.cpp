
#include "../../headers/player/CheaterPlayerStrategy.h"

CheaterPlayerStrategy::CheaterPlayerStrategy(const Player *owner) : PlayerStrategy(owner) {

}

CheaterPlayerStrategy::~CheaterPlayerStrategy() {

}


void CheaterPlayerStrategy::play() {

}

Order *CheaterPlayerStrategy::issueOrder() {
    return nullptr;
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack() {
    return std::vector<Territory *>();
}

std::vector<Territory *> CheaterPlayerStrategy::toDefend() {
    return std::vector<Territory *>();
}

PlayerStrategy *CheaterPlayerStrategy::clone() {
    return nullptr;
}

