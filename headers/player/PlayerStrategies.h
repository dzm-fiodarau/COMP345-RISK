#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include <memory>
#include <vector>
using namespace std;

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Territory;
class Order;
class Player;
class GameEngine;

/** \brief  An interface for classes to customize/adapt the attack and defending patterns or behaviors of a player.
 *          This class is intended to be coupled with a corresponding <code>Player</code> object and it determines the
 *          <b>attacking/defending behavior/preferences of the <code>Player</code> object</b>.
 *  \remarks    <ul><li>Derived classes can take order data from different sources, hence makes it able to define pre-programmed
 *                      or even AI type behavior.
 *              </ul>
 */
class PlayerStrategy
{
public:
    /** \brief Initializes a <code>PlayerStrategy</code> object. Takes in a <code>Player</code> object.
     *  \param owner The player that 'owns' this <code>PlayerStrategy</code> object.
     */
    explicit PlayerStrategy(Player *owner);

    /** \brief Deconstructs a <code>PlayerStrategy</code> object. */
    virtual ~PlayerStrategy();

    /** \brief Entry point when a player is signaled that it is their turn to play. The player is to decide what
     *         actions they would like to perform.
     */
    virtual void play() = 0;

    /** \brief Player issues their 'issuing orders' phase.
     *  \param CommandProcessor A command processor object to get input from.
     */
    virtual void issueOrders(GameEngine *) = 0;

    virtual std::vector<Territory *> toAttack() = 0;

    virtual std::vector<Territory *> toDefend() = 0;

    virtual PlayerStrategy *clone() = 0;

protected:
    /** \brief The player associated with this <code>PlayerStrategy</code> object.
     */
    Player *player;
};

class HumanPlayerStrategy final : public PlayerStrategy
{
public:
    explicit HumanPlayerStrategy(Player *owner);
    ~HumanPlayerStrategy() override;

    void play() override;
    void issueOrders(GameEngine *) override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;
    PlayerStrategy *clone() override;
};

class AggressivePlayerStrategy final : public PlayerStrategy
{
public:
    explicit AggressivePlayerStrategy(Player *owner);
    ~AggressivePlayerStrategy() override;

    void play() override;
    void issueOrders(GameEngine *) override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;
    PlayerStrategy *clone() override;
};

class BenevolentPlayerStrategy final : public PlayerStrategy
{
public:
    explicit BenevolentPlayerStrategy(Player *owner);
    ~BenevolentPlayerStrategy() override;

    void play() override;
    void issueOrders(GameEngine *) override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;
    PlayerStrategy *clone() override;
};

class NeutralPlayerStrategy final : public PlayerStrategy
{
public:
    explicit NeutralPlayerStrategy(Player *owner);
    ~NeutralPlayerStrategy() override;

    void play() override;
    void issueOrders(GameEngine *) override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;
    PlayerStrategy *clone() override;
};

class CheaterPlayerStrategy final : public PlayerStrategy
{
public:
    explicit CheaterPlayerStrategy(Player *owner);
    ~CheaterPlayerStrategy() override;

    void play() override;
    void issueOrders(GameEngine *) override;
    std::vector<Territory *> toAttack() override;
    std::vector<Territory *> toDefend() override;
    PlayerStrategy *clone() override;
};

#endif //  PLAYER_STRATEGIES_H