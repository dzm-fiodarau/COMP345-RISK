#ifndef AGGRESSIVE_PLAYER_STRATEGY_H
#define AGGRESSIVE_PLAYER_STRATEGY_H

#include "PlayerStrategies.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Territory;
class Order;
class Player;

class AggressivePlayerStrategy final : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(const Player* owner);
    ~AggressivePlayerStrategy() override;

    void play() override;
    Order* issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() override;
};

#endif  //  AGGRESSIVE_PLAYER_STRATEGY_H