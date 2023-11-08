#include "../../headers/player/HumanPlayerStrategy.h"
#include "../../headers/player/Player.h"

HumanPlayerStrategy::HumanPlayerStrategy(const Player *player) : PlayerStrategy(player) {

}

HumanPlayerStrategy::~HumanPlayerStrategy() {

}

void HumanPlayerStrategy::play() {

}

Order *HumanPlayerStrategy::issueOrder() {
    return nullptr;
}

std::vector<std::shared_ptr<Territory>> HumanPlayerStrategy::toAttack() {
    return std::vector<std::shared_ptr<Territory>>();
}

std::vector<std::shared_ptr<Territory>> HumanPlayerStrategy::toDefend() {
    return std::vector<std::shared_ptr<Territory>>();
}

HumanPlayerStrategy* HumanPlayerStrategy::clone() {
    return new HumanPlayerStrategy(new Player());
}
