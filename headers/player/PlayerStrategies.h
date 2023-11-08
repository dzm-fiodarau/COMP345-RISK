#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include <memory>
#include <vector>

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Territory;
class Order;
class Player;

/** \brief  An interface for classes to customize/adapt the attack and defending patterns or behaviors of a player.
 *  \remarks    <ul><li>Derived classes can take order data from different sources, hence makes it able to define pre-programmed
 *                  or even AI type behavior.</ul>
 */
class PlayerStrategy {
public:
    /** \brief Initializes a <code>PlayerStrategy</code> object. Takes in a <code>Player</code> object.
     * @param player The player that 'owns' this <code>PlayerStrategy</code> object.
     */
    explicit PlayerStrategy(const Player* player);

    virtual ~PlayerStrategy();

    /** \brief  Entry point when a player is signaled that it is their turn to play. The player is to decide what
     *          actions they would like to perform.
     */
    virtual void play() = 0;

    virtual Order* issueOrder() = 0;

    virtual std::vector<std::shared_ptr<Territory>> toAttack() = 0;

    virtual std::vector<std::shared_ptr<Territory>> toDefend() = 0;

    virtual PlayerStrategy* clone() = 0;

protected:
    /** \brief The player associated with this <code>PlayerStrategy</code> object.
     */
    std::weak_ptr<Player> player;
};

#endif  //  PLAYER_STRATEGIES_H