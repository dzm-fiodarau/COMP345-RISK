#ifndef BENEVOLENT_PLAYER_STRATEGY_H
#define BENEVOLENT_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

class BenevolentPlayerStrategy final : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* owner);
    ~BenevolentPlayerStrategy() override;

    void play() override;
    void issueOrders(CommandProcessor*) override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() override;
};

#endif  //  BENEVOLENT_PLAYER_STRATEGY_H