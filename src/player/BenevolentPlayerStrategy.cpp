
#include "../../headers/player/BenevolentPlayerStrategy.h"

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *owner) : PlayerStrategy(owner) {

}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {

}


void BenevolentPlayerStrategy::play() {

}

void BenevolentPlayerStrategy::issueOrders(CommandProcessor*) {

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