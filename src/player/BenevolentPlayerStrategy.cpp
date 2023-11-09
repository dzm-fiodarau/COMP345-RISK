
#include "../../headers/player/BenevolentPlayerStrategy.h"

BenevolentPlayerStrategy::BenevolentPlayerStrategy(const Player *owner) : PlayerStrategy(owner) {

}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {

}


void BenevolentPlayerStrategy::play() {

}

Order *BenevolentPlayerStrategy::issueOrder() {
    return nullptr;
}

std::vector<Territory *> BenevolentPlayerStrategy::toAttack() {
    return std::vector<Territory *>();
}

std::vector<Territory *> BenevolentPlayerStrategy::toDefend() {
    return std::vector<Territory *>();
}

PlayerStrategy *BenevolentPlayerStrategy::clone() {
    return nullptr;
}

