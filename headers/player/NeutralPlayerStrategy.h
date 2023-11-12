#ifndef NEUTRAL_PLAYER_STRATEGY_H
#define NEUTRAL_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

class NeutralPlayerStrategy final : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* owner);
    ~NeutralPlayerStrategy() override;

    void play() override;
    void issueOrders(CommandProcessor*) override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() override;
};

#endif  //  NEUTRAL_PLAYER_STRATEGY_H