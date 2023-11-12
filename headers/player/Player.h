#ifndef PLAYER_H
#define PLAYER_H

//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "PlayerStrategies.h"
#include "../Orders.h"

//  Forward declaration of required classes from other header files. (included in .cpp file)
class OrdersList;           //  Orders.h
class Card;                 //  Cards.h
class Territory;            //  Map.h

using namespace std;

/** \class   Player
 *  \brief   Represents a player in the game.
 */
class Player final
{
private:
    //  Name of the player
    string name;

    // Number of army units the player has to deploy
    int reinforcementPool;

    // Indication of whether the player has to draw a card at the end of his turn or not
    bool drawCard;

    //  'PlayerStrategy' object that defines behavior for 'toAttack()', 'toDefend()', and 'issueOrder(..)'
    //  Class has single/sole ownership
    PlayerStrategy* playerStrategy;

    //  'OrdersList' object that keeps track of the orders the player has issued
    //  Class has single/sole ownership
    OrdersList* ordersList;

    //  The list of territories the player currently owns
    //  Class has NO ownership of the objects in the vector
    vector<Territory*> territories;

    //  The list of cards that the player has drawn
    //  Class has NO ownership of the objects in the vector
    vector<Card*> cards;

    //  The list of 'Player' objects the player is currently in negotiation with
    //  Class has NO ownership of the objects in the vector
    vector<Player*> playersInNegotiation;

public:
    /** \brief A 'player' object that represents neutrality. Un-claimed and un-owned territories not belonging to any
     *         player in the game is owned by this placeholder 'player'.
     */
    static Player *neutralPlayer;

    /** \brief Initializes a default <code>Player</code> object.
     */
    Player();

    /** \brief Initializes a <code>Player</code> object, given a player/user name.
     *  \param playerName The name of the player
     */
    explicit Player(const string& name);

    /** \brief  Initializes a <code>Player</code> object, given a player/user name, and a vector of territories and
     *          cards.
     *  \param name          The name of the player.
     *  \param territories   A vector containing the territories to be owned/associated to the player.
     *  \param cards         A vector of cards owned by the player.
     */
    Player(const string& name, vector<Territory*> territories, vector<Card *> cards);

    /** \brief  Initializes a <code>Player</code> object, given a player/user name, and a vector of territories and
     *          cards, and a <code>PlayerStrategy</code> object that defines the behavior of the player.
     * @param name              The name of the player.
     * @param playerStrategy    A <code>PlayerStrategy</code> object that defines the behavior of the player.
     * @param territories       A vector containing the territories to be owned/associated to the player.
     * @param cards             A vector of cards owned by the player.
     */
    Player(const string& name, PlayerStrategy* playerStrategy, vector<Territory*> territories, vector<Card*> cards);

    /** \brief  Initializes <code>Player</code> object, given another <code>Player</code> object.
     *  \remarks <b>Note that only a SHALLOW copy will be produced.<b>
     *  \param otherPlayer The other player to copy from
     */
    Player(const Player &otherPlayer);

    /** \brief  Deconstructs the <code>Player</code> object.
     */
    ~Player();

    /** \brief  Entry point when a player is signaled that it is their turn to play. The player is to decide what
     *          actions they would like to perform.
     *  \remarks    <ul><li>This function is intended to encapsulate the logic underlying selecting what action will
     *                      be performed. If the player is controlled by a person, then their request is processed and
     *                      the corresponding action will be performed. With a programmed player (AI), their preferences
     *                      on what action to take will be encapsulated in this method.</ul>
     */
    void play() { playerStrategy->play(); };

    /** \brief  Returns a list of of which <b>neighboring</b> territories are to be attacked. Territories in the front
     *          have higher priorities.
     */
    vector<Territory*> toAttack();

    /** \brief  Returns a list of which <b>currently owned</b> territories that are to be defended. Territories in the
     *          front have higher priorities.
     */
    vector<Territory*> toDefend();

    /** \brief  Issues an order and adds it to the list of issued orders.
     *  \remarks Order creation is delegated to the <code>PlayerStrategies</code> object.
     */
    void issueOrders(CommandProcessor*);

    /** \brief  Creates an order and appends it to the current list of orders based on the creation parameters given.
     *  \param type          The type of the order.
     *  \param target        The territory wo which troops are to be moved.
     *  \param armyUnits     The number of army units to be moved.
     *  \param source        Source of territory.
     *  \param player        The player object issuing the order.
     */
    void issueOrder(const string &type, Territory *target, int armyUnits, Territory *source, Player *player);

    /** \brief  Creates an order and appends it to the current list of orders based on the creation parameters given.
     *  \param orderType     The type of the order, uses a <code>OrderType</code> enum.
     *  \param target        The territory wo which troops are to be moved.
     *  \param armyUnits     The number of army units to be moved.
     *  \param source        Source of territory.
     *  \param player        The player object issuing the order.
     */
    void issueOrder(Order::OrderType orderType, Territory *target, int armyUnits, Territory *source, Player *player);


    // Operator Overloads
    Player &operator=(const Player &other);

    friend ostream &operator<<(ostream &out, const Player &player);


    //  Getter/Accessor methods
    /** \brief Returns the name of the player.
     */
    string getName() const;

    /** \brief Returns the number of army units the player is able to deploy.
     */
    int getUnits() const;

    /** \brief Returns the number of army units the player is able to deploy.
     *  \remarks Alternative to <code>int getUnits() const</code> for 'legacy' compatibility or alternative call style.
     */
    inline int getReinforcementPool() const { return getUnits(); }

    /** \brief Returns whether or not the player is to draw a card at the end of their turn. */
    bool drawsCard() const;

    /** \brief Returns a vector of territories that is associated to the player.
     *  \remarks The vector has territories that cannot be changed. Meant primarily for readonly operations.
     */
    vector<const Territory*> getTerritories() const;

    /** \brief Returns a list of cards that the player has drawn.
     *  \remarks The vector has cards that cannot be changed. Meant primarily for readonly operations.
     */
    vector<const Card*> getCards() const;

    /** \brief Returns a list of the players that are currently in negotiation with the player
     *  \remarks The vector has players that cannot be changed. Meant primarily for readonly operations.
     */
    vector<const Player*> getPlayersInNegotiation() const;

    /** \brief Gets the <code>OrdersList</code> object of the class. */
    OrdersList* getOrdersList() const;


    //  Setter/Mutator methods
    void setPlayerName(const std::string& newName);

    /** \brief Adds units to the current reinforcement pool of the player.
     */
    void addToReinforcementPool(int units);

    /** \brief Sets the number of units in the reinforcement pool
     */
    inline void setReinforcements(size_t units) { reinforcementPool = static_cast<int>(units); }

    /** \brief Sets whether or not the player should draw a card at the end of their turn. */
    void setDrawCard(bool drawCard);

    /** \brief Adds a <code>Territory</code> object to the list of owned territories. */
    void addTerritory(Territory& newTerritory);

    /** \brief Adds a <code>Card</code> object to the list of cards. */
    void addCard(Card& newCard);

    /** \brief Adds a player to the list of players currently under negotiation. */
    void negotiateWith(Player& player);

    /** \brief Removes a territory from the player's owned territory list. */
    void removeTerritory(Territory& target);

    /** \brief Checks whether a player owns a specified territory. */
    bool ownsTerritory(Territory& territory) const;

    /** \brief Checks whether a specified territory is adjacent to any of the player's owned territories.
     *  \remarks <ul><li>In the case that the player owns the target territory, returns true.</ul>
     */
    bool isTerritoryAdjacent(Territory& target) const;

    /** \brief Checks whether another player is currently under negotiations. */
    bool isPlayerInNegotiations(Player& player) const;
};

#ifdef __GNUC__
#pragma clang diagnostic pop
#endif

#endif  //  PLAYER_H