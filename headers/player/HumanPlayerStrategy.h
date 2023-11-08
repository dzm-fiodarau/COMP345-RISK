#ifndef HUMAN_PLAYER_STRATEGY_H
#define HUMAN_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

class HumanPlayerStrategy final : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(const Player* player);
    ~HumanPlayerStrategy() override;

    void play() override;
    Order* issueOrder() override;
    std::vector<std::shared_ptr<Territory>> toAttack() override;
    std::vector<std::shared_ptr<Territory>> toDefend() override;
    HumanPlayerStrategy* clone() override;
};

#endif  //  HUMAN_PLAYER_STRATEGY_H