
#include "../../headers/player/NeutralPlayerStrategy.h"

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *owner) : PlayerStrategy(owner) {

}


NeutralPlayerStrategy::~NeutralPlayerStrategy() {

}

void NeutralPlayerStrategy::play() {

}

void NeutralPlayerStrategy::issueOrders(CommandProcessor*) {

}

std::vector<Territory *> NeutralPlayerStrategy::toAttack() {
    return std::vector<Territory *>();
}

std::vector<Territory *> NeutralPlayerStrategy::toDefend() {
    return std::vector<Territory *>();
}

PlayerStrategy *NeutralPlayerStrategy::clone() {
    return nullptr;
}

