#ifndef ORDERS_H
#define ORDERS_H

#include <string>
#include <list>
#include <iostream>
using namespace std;

#include "Map.h"
#include "Player.h"
#include "Cards.h"

class Card;
class OrdersList;
class Player;
class Territory;
// Global string array containing all allowed orders
const string allowedOrders[6] = {"deploy", "advance", "bomb", "blockade", "airlift", "negotiate"};

/**
 * \class   OrdersList
 * \brief   Class that holds the list of Order objects of each player
 */
class Order
{
public:
    /**
     * \brief   Constructs an Order object
     * \param   type Type of the Order object created
     * \param   target Territory targeted by the order
     */
    Order(OrdersList *ordersList, string type, Territory *target);
    /**
     * \brief   Constructs an Order object using values from another Order object
     * \param   order Other Order object to copy member variables from
     */
    Order(Order &order);

    /**
     * \brief   Verifies if the order is valid, has to be implemented in child classes
     * \return  True if the order is valid, false otherwise
     */
    virtual bool validate();

    /**
     * \brief   Executes the order, has to be implemented in child classes
     * \return  String that describes the effects of the executed order
     */
    virtual string execute();

    /**
     * \brief   Assigns new values to member varaiables of the Order object
     * \param   order Order object from which new values are to be taken
     */
    Order &operator=(const Order &order);

    /**
     * \brief   Stream insertion override, prints order's type and target territory
     */
    friend ostream &operator<<(ostream &outs, const Order &order);

protected:
    // OrdersList object that holds order
    OrdersList *ordersList;
    //  Type of order
    string type;
    // Territory targeted by the order
    Territory *target;
};

/**
 * \class   OrdersList
 * \brief   Class that holds the list of Order objects of each player
 */
class OrdersList
{
public:
    // List of pointers to Order objects
    list<Order *> orders;
    // Owner of orders list
    Player *owner;

    /**
     * \brief   Constructs an OrdersList object with an empty list of orders
     */
    OrdersList(Player *owner);
    /**
     * \brief   Constructs an OrdersList object using values from another OrdersList object
     * \param   ordersList Other OrdersList object to copy member variables from
     */
    OrdersList(OrdersList &ordersList);

    /**
     * \brief   Removes the specified order from the list
     * \param   order Order to be removed from the list
     * \return  True if order removal is successful, false otherwise
     */
    void remove(Order *order);
    /**
     * \brief   Moves specified order to specified index in the list
     * \param   order Order to be moved
     * \param   index New position of the order in the list
     * \return  True if order moving is successful, false otherwise
     */
    bool move(Order *order, int index);
    /**
     * \brief   Assigns new values to member varaiables of the OrdersList object
     * \param   ordersList OrdersList object from which new values are to be taken
     */
    OrdersList &operator=(const OrdersList &ordersList);

    /**
     * \brief   Stream insertion override, prints all orders contained in the list
     */
    friend ostream &operator<<(ostream &outs, const OrdersList &ordersList);
};

/**
 * \class   DeployOrder
 * \brief   Class that represents the deploy action
 * \extends Order
 */
class DeployOrder : public Order
{
public:
    /**
     * \brief   Constructs a DeployOrder object
     * \param   type Type of the DeployOrder object created
     * \param   target Territory to which troups are to be deployed
     * \param   armyUnits Number of army units to be deployed
     */
    DeployOrder(OrdersList *ordersList, Territory *target, int armyUnits);
    /**
     * \brief   Constructs a DeployOrder object using values from another DeployOrder object
     * \param   order Other DeployOrder object to copy member variables from
     */
    DeployOrder(DeployOrder &order);

    /**
     * \brief   Verifies if the order is valid
     * \return  True if the order is valid, false otherwise
     */
    bool validate() override;

    /**
     * \brief   Executes the order
     * \return  String that describes the effects of the executed order
     */
    string execute() override;

    /**
     * \brief   Assigns new values to member varaiables of the DeployOrder object
     * \param   order DeployOrder object from which new values are to be taken
     */
    DeployOrder &operator=(const DeployOrder &order);

    /**
     * \brief   Stream insertion override, prints order's type, target territory and number of army units
     */
    friend ostream &operator<<(ostream &outs, const DeployOrder &order);

private:
    // Number of army units to deploy
    int armyUnits;
};

/**
 * \class   AdvanceOrder
 * \brief   Class that represents the advance action
 * \extends Order
 */
class AdvanceOrder : public Order
{
public:
    /**
     * \brief   Constructs an AdvanceOrder object
     * \param   type Type of the Order object created
     * \param   target Territory to which troups are to be moved
     * \param   armyUnits Number of army units to be moved
     * \param   source  Territory from which troups are to be moved
     */
    AdvanceOrder(OrdersList *ordersList, Territory *target, int armyUnits, Territory *source);
    /**
     * \brief   Constructs an AdvanceOrder object using values from another AdvanceOrder object
     * \param   order Other AdvanceOrder object to copy member variables from
     */
    AdvanceOrder(AdvanceOrder &order);

    /**
     * \brief   Verifies if the order is valid
     * \return  True if the order is valid, false otherwise
     */
    bool validate() override;
    /**
     * \brief   Executes the order
     * \return  String that describes the effects of the executed order
     */
    string execute() override;
    /**
     * \brief   Assigns new values to member varaiables of the AdvanceOrder object
     * \param   order AdvanceOrder object from which new values are to be taken
     */
    AdvanceOrder &operator=(const AdvanceOrder &order);

    /**
     * \brief   Stream insertion override, prints order's type, source and target territories, and number of army units
     */
    friend ostream &operator<<(ostream &outs, const AdvanceOrder &order);

private:
    // Number of army units to move
    int armyUnits;
    // Territory from which the army units are moved
    Territory *source;
};

/**
 * \class   BombOrder
 * \brief   Class that represents the bomb action
 * \extends Order
 */
class BombOrder : public Order
{
public:
    /**
     * \brief   Constructs a BombOrder object
     * \param   type Type of the Order object created
     * \param   target Territory to be bombed
     */
    BombOrder(OrdersList *ordersList, Territory *target);
    /**
     * \brief   Constructs a BombOrder object using values from another BombOrder object
     * \param   order Other BombOrder object to copy member variables from
     */
    BombOrder(BombOrder &order);

    /**
     * \brief   Verifies if the order is valid
     * \return  True if the order is valid, false otherwise
     */
    bool validate() override;
    /**
     * \brief   Executes the order
     * \return  String that describes the effects of the executed order
     */
    string execute() override;
    /**
     * \brief   Assigns new values to member varaiables of the BombOrder object
     * \param   order BombOrder object from which new values are to be taken
     */
    BombOrder &operator=(const BombOrder &order);

    /**
     * \brief   Stream insertion override, prints order's type and target territory
     */
    friend ostream &operator<<(ostream &outs, const BombOrder &order);
};

/**
 * \class   BlockadeOrder
 * \brief   Class that represents the blockade action
 * \extends Order
 */
class BlockadeOrder : public Order
{
public:
    /**
     * \brief   Constructs a BlockadeOrder object
     * \param   type Type of the Order object created
     * \param   target Territory to be blockaded
     */
    BlockadeOrder(OrdersList *ordersList, Territory *target);
    /**
     * \brief   Constructs a BlockadeOrder object using values from another BlockadeOrder object
     * \param   order Other BlockadeOrder object to copy member variables from
     */
    BlockadeOrder(BlockadeOrder &order);

    /**
     * \brief   Verifies if the order is valid
     * \return  True if the order is valid, false otherwise
     */
    bool validate() override;
    /**
     * \brief   Executes the order
     * \return  String that describes the effects of the executed order
     */
    string execute() override;
    /**
     * \brief   Assigns new values to member varaiables of the BlockadeOrder object
     * \param   order BlockadeOrder object from which new values are to be taken
     */
    BlockadeOrder &operator=(const BlockadeOrder &order);

    /**
     * \brief   Stream insertion override, prints order's type and target territory
     */
    friend ostream &operator<<(ostream &outs, const BlockadeOrder &order);
};

/**
 * \class   AirliftOrder
 * \brief   Class that represents the airlift action
 * \extends Order
 */
class AirliftOrder : public Order
{
public:
    /**
     * \brief   Constructs an AirliftOrder object
     * \param   type Type of the Order object created
     * \param   target Territory to which troups are to be moved
     * \param   armyUnits Number of army units to be moved
     * \param   source  Territory from which troups are to be moved
     */
    AirliftOrder(OrdersList *ordersList, Territory *target, int armyUnits, Territory *source);
    /**
     * \brief   Constructs an AirliftOrder object using values from another AirliftOrder object
     * \param   order Other AirliftOrder object to copy member variables from
     */
    AirliftOrder(AirliftOrder &order);

    /**
     * \brief   Verifies if the order is valid
     * \return  True if the order is valid, false otherwise
     */
    bool validate() override;
    /**
     * \brief   Executes the order
     * \return  String that describes the effects of the executed order
     */
    string execute() override;
    /**
     * \brief   Assigns new values to member varaiables of the AirliftOrder object
     * \param   order AirliftOrder object from which new values are to be taken
     */
    AirliftOrder &operator=(const AirliftOrder &order);

    /**
     * \brief   Stream insertion override, prints order's type, source and target territories, and number of army units
     */
    friend ostream &operator<<(ostream &outs, const AirliftOrder &order);

private:
    // Number of army units advanced
    int armyUnits;
    // Territory from which the army units are advanced
    Territory *source;
};

/**
 * \class   NegotiateOrder
 * \brief   Class that represents the negotiation action
 * \extends Order
 */
class NegotiateOrder : public Order
{
public:
    /**
     * \brief   Constructs a NegotiateOrder object
     * \param   type Type of the Order object created
     * \param   player Player with whom negotiation happens
     */
    NegotiateOrder(OrdersList *ordersList, Player *player);
    /**
     * \brief   Constructs a NegotiateOrder object using values from another NegotiateOrder object
     * \param   order Other NegotiateOrder object to copy member variables from
     */
    NegotiateOrder(NegotiateOrder &order);

    /**
     * \brief   Verifies if the order is valid
     * \return  True if the order is valid, false otherwise
     */
    bool validate() override;
    /**
     * \brief   Executes the order
     * \return  String that describes the effects of the executed order
     */
    string execute() override;
    /**
     * \brief   Assigns new values to member varaiables of the NegotiateOrder object
     * \param   order NegotiateOrder object from which new values are to be taken
     */
    NegotiateOrder &operator=(const NegotiateOrder &order);

    /**
     * \brief   Stream insertion override, prints order's type and target player
     */
    friend ostream &operator<<(ostream &outs, const NegotiateOrder &order);

private:
    // Player with whom attacks are prevented during current turn
    Player *player;
};

#endif