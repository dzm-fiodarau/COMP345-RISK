
#include "../../headers/player/CheaterPlayerStrategy.h"

CheaterPlayerStrategy::CheaterPlayerStrategy(Player *owner) : PlayerStrategy(owner) {

}

CheaterPlayerStrategy::~CheaterPlayerStrategy() {

}


void CheaterPlayerStrategy::play() {

}

void CheaterPlayerStrategy::issueOrders(CommandProcessor*) {

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
