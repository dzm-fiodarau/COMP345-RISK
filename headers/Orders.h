#ifndef ORDERS_H
#define ORDERS_H

#include <string>
#include <list>
#include <iostream>

//  Forward declaration of required classes from other header files. (included in .cpp file)
class Territory;    //  Map.h
class Player;       //  Player.h

using namespace std;

/**
 * \class   Order
 * \brief   A class that represents an order
 */
class Order
{
public:

    /** \brief An enum class that indicates the type of the order
     */
    enum class OrderType {
        Deploy, Advance, Bomb, Blockade, Airlift, Negotiate, Invalid
    };

    /** \brief Returns an <code>OrderType</code> enum corresponding to a string.
     * @param orderTypeAsString The string to be parsed.
     * @return Returns <code>OrderType</code> enum.
     */
    static Order::OrderType parseOrderType(const string& orderTypeAsString);

    /** \brief Returns a string representation of an <code>OrderType</code> enum.
     * @param orderType The <code>OrderType</code> enum.
     * @return Returns a string representation of the enum.
     */
    static string orderTypeToString(Order::OrderType orderType);

    /**
     * \brief   Constructs an Order object
     * \param   type Type of the Order object created
     * \param   target Territory targeted by the order
     */
    Order(Player *owner, const string& type, Territory *target);

    /**
     * \brief   Constructs an Order object.
     * @param orderType The type of the <code>Order</code> object created. A <code>OrderType</code> enum.
     * @param target Territory targeted by the order.
     */
    Order(Player *owner, Order::OrderType orderType, Territory *target);

    /**
     * \brief   Constructs an Order object using values from another Order object
     * \param   order Other Order object to copy member variables from
     */
    Order(Order &order);

    /**
     * \brief   Deallocates dynamically declared memory during runtime
     */
    virtual ~Order();

    /**
     * \brief   Verifies if the order is valid, has to be implemented in child classes
     * \return  True if the order is valid, false otherwise
     */
    virtual bool validate() = 0;

    /**
     * \brief   Executes the order, has to be implemented in child classes
     * \return  String that describes the effects of the executed order
     */
    virtual string execute() = 0;

    /**
     * \brief   Assigns new values to member variables of the Order object
     * \param   order Order object from which new values are to be taken
     */
    Order &operator=(const Order &order);

    /**
     * \brief   Stream insertion override, prints order's type and target territory
     */
    friend ostream &operator<<(ostream &outs, const Order &order);

    inline Order::OrderType getOrderType() const noexcept { return type; }

protected:
    //  Pointer to the player that owns the object
    Player *owner;
    //  Type of order
    OrderType type;
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
    /**
     * \brief   Constructs an OrdersList object with an empty list of orders
     */
    explicit OrdersList(Player *owner);

    /**
     * \brief   Constructs an OrdersList object using values from another OrdersList object
     * \param   ordersList Other OrdersList object to copy member variables from
     */
    OrdersList(OrdersList &ordersList);

    /**
     * \brief   Appends a pointer to the end of the orders list - if it is not null
     * \param order Pointer to an Order object
     * \return  Returns true if append was successful, false otherwise
     */
    bool addOrder(Order* order);

    /**
     * \brief   Pops the head pointer in the list
     * \return  Pointer to an Order object; head of the list
     */
    Order* getNextOrder();

    /**
     * \brief   Gets the owner
     * \return  Pointer to a player object
     */
    const Player* getOwner() const;

    /**
     * \brief   Takes in a function, then applies that function to each order in the list
     * \param func  Function to apply to each Order element in the list
     * \remarks Solution to external iteration. Now iteration can occur internally through passing of a function.
     *          Upholds encapsulation.
     */
    void apply(void (*func)(Order*));

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
     * \brief   Directly accesses the internal orders list
     * \param index Index of internal list
     * \return  Returns a Order pointer using the passed index. Returns nullptr if invalid index.
     */
    Order* operator[](std::size_t index);

    /**
     * \brief   Assigns new values to member variables of the OrdersList object
     * \param   ordersList OrdersList object from which new values are to be taken
     */
    OrdersList &operator=(const OrdersList &ordersList);

    /**
     * \brief   Stream insertion override, prints all orders contained in the list
     */
    friend ostream &operator<<(ostream &outs, const OrdersList &ordersList);

private:
    // List of pointers to Order objects
    list<Order *> orders;
    // Owner of orders list
    Player *owner;
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
     * \param   target Territory to which troops are to be deployed
     * \param   armyUnits Number of army units to be deployed
     */
    DeployOrder(Player *owner, Territory *target, int armyUnits);

    /**
     * \brief   Constructs a DeployOrder object using values from another DeployOrder object
     * \param   order Other DeployOrder object to copy member variables from
     */
    DeployOrder(DeployOrder &order);

    /**
     * \brief   Deconstructs a DeployOrder object
     */
    ~DeployOrder() override;

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
     * \brief   Assigns new values to member variables of the DeployOrder object
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
     * \param   target Territory to which troops are to be moved
     * \param   armyUnits Number of army units to be moved
     * \param   source  Territory from which troops are to be moved
     */
    AdvanceOrder(Player *owner, Territory *target, int armyUnits, Territory *source);

    /**
     * \brief   Constructs an AdvanceOrder object using values from another AdvanceOrder object
     * \param   order Other AdvanceOrder object to copy member variables from
     */
    AdvanceOrder(AdvanceOrder &order);

    /**
     * \brief   Deconstructs an AdvanceOrder object
     */
    ~AdvanceOrder() override;

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
     * \brief   Assigns new values to member variables of the AdvanceOrder object
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
    BombOrder(Player *owner, Territory *target);
    /**
     * \brief   Constructs a BombOrder object using values from another BombOrder object
     * \param   order Other BombOrder object to copy member variables from
     */
    BombOrder(BombOrder &order);

    /**
     * \brief   Deconstructs a BombOrder object.
     */
    ~BombOrder() override;

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
     * \brief   Assigns new values to member variables of the BombOrder object
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
    BlockadeOrder(Player *owner, Territory *target);
    /**
     * \brief   Constructs a BlockadeOrder object using values from another BlockadeOrder object
     * \param   order Other BlockadeOrder object to copy member variables from
     */
    BlockadeOrder(BlockadeOrder &order);

    /**
     * \brief   Deconstructs a BlockadeOrder object.
     */
    ~BlockadeOrder() override;

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
     * \brief   Assigns new values to member variables of the BlockadeOrder object
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
     * \param   target Territory to which troops are to be moved
     * \param   armyUnits Number of army units to be moved
     * \param   source  Territory from which troops are to be moved
     */
    AirliftOrder(Player *owner, Territory *target, int armyUnits, Territory *source);
    /**
     * \brief   Constructs an AirliftOrder object using values from another AirliftOrder object
     * \param   order Other AirliftOrder object to copy member variables from
     */
    AirliftOrder(AirliftOrder &order);

    /**
     * \brief   Deconstructs an AirliftOrder object.
     */
    ~AirliftOrder() override;

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
     * \brief   Assigns new values to member variables of the AirliftOrder object
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
    NegotiateOrder(Player *owner, Player *player);
    /**
     * \brief   Constructs a NegotiateOrder object using values from another NegotiateOrder object
     * \param   order Other NegotiateOrder object to copy member variables from
     */
    NegotiateOrder(NegotiateOrder &order);

    /**
     * \brief   Deconstructs a NegotiateOrder object.
     */
    ~NegotiateOrder() override;

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
     * \brief   Assigns new values to member variables of the NegotiateOrder object
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