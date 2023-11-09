#ifndef NEUTRAL_PLAYER_STRATEGY_H
#define NEUTRAL_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

class NeutralPlayerStrategy final : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(const Player* owner);
    ~NeutralPlayerStrategy() override;

    void play() override;
    Order* issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() override;
};

#endif  //  NEUTRAL_PLAYER_STRATEGY_H