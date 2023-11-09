#include "../../headers/player/HumanPlayerStrategy.h"
#include "../../headers/player/Player.h"

HumanPlayerStrategy::HumanPlayerStrategy(const Player *owner) : PlayerStrategy(owner) {

}

HumanPlayerStrategy::~HumanPlayerStrategy() {

}

void HumanPlayerStrategy::play() {

}

Order *HumanPlayerStrategy::issueOrder() {
    return nullptr;
}

std::vector<Territory *> HumanPlayerStrategy::toAttack() {
    return std::vector<Territory *>();
}

std::vector<Territory *> HumanPlayerStrategy::toDefend() {
    return std::vector<Territory *>();
}

PlayerStrategy *HumanPlayerStrategy::clone() {
    return nullptr;
}


