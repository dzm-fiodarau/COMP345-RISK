#ifndef CHEATER_PLAYER_STRATEGY_H
#define CHEATER_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

class CheaterPlayerStrategy final : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(const Player* owner);
    ~CheaterPlayerStrategy() override;

    void play() override;
    Order* issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() override;
};

#endif  //  CHEATER_PLAYER_STRATEGY_H