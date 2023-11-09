#ifndef HUMAN_PLAYER_STRATEGY_H
#define HUMAN_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

class HumanPlayerStrategy final : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(const Player* owner);
    ~HumanPlayerStrategy() override;

    void play() override;
    Order* issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() override;
};

#endif  //  HUMAN_PLAYER_STRATEGY_H