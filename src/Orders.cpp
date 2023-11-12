#include <iostream>
#include <algorithm>

#include "../headers/Orders.h"
#include "../headers/Map.h"
#include "../headers/player/Player.h"
#include "../headers/Cards.h"

//----------------------------------------------------------------------------------------------------------------------
//  ORDERS LIST
OrdersList::OrdersList(Player *owner)
{
    OrdersList::owner = owner;
}

OrdersList::OrdersList(OrdersList &ordersList)
{
    orders = ordersList.orders;
    owner = ordersList.owner;
}

Order *OrdersList::operator[](size_t index)
{
    if (index < orders.size())
    {
        auto iterator = orders.begin();
        std::advance(iterator, index);
        return *iterator;
    }

    return nullptr;
}

OrdersList &OrdersList::operator=(const OrdersList &ordersList)
{
    if (this == &ordersList)
    {
    }

    orders = ordersList.orders;
    owner = ordersList.owner;
    return *this;
}

ostream &operator<<(ostream &outs, const OrdersList &ordersList)
{
    outs << "{\n";
    list<Order *>::const_iterator itr;
    for (itr = ordersList.orders.begin(); itr != ordersList.orders.end(); ++itr)
    {
        outs << *(*itr) << ";" << endl;
    }
    outs << "}";
    return outs;
}

bool OrdersList::addOrder(Order *order)
{
    if (order != nullptr)
    {
        orders.push_back(order);
        notify(this);
        return true;
    }

    return false;
}

const Player *OrdersList::getOwner() const
{
    return this->owner;
}

Order *OrdersList::getNextOrder()
{
    auto *nextOrder = !orders.empty() ? orders.front() : nullptr;
    if (!orders.empty())
    {
        orders.pop_front();
    }
    return nextOrder;
}

void OrdersList::apply(void (*func)(Order *))
{
    for (auto *order : orders)
    {
        if (order != nullptr)
        {
            func(order);
        }
    }
}

void OrdersList::remove(Order *order)
{
    auto it = std::find(orders.begin(), orders.end(), order);
    if (it == orders.end())
    {
        cout << "(OrdersList::remove())Order not found in the list." << endl;
        return;
    }

    orders.remove(order);
    delete order;

    cout << "OrdersList::remove() was successful.\n";
}

bool OrdersList::move(Order *order, int index)
{
    if (index < 0 || index >= orders.size())
    {
        cout << "(OrdersList::move())Specified index is wrong." << endl;
        return false;
    }
    auto it = std::find(orders.begin(), orders.end(), order);
    if (it == orders.end())
    {
        cout << "(OrdersList::move())Order not found in the list." << endl;
        return false;
    }

    orders.remove(order);
    it = orders.begin();
    advance(it, index);
    orders.insert(it, order);
    cout << "OrdersList::move() was successful.\n";
    return true;
}

string OrdersList::stringToLog()
{
    return "New Order added: " + Order::orderTypeToString(orders.back()->getOrderType());
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER

Order::OrderType Order::parseOrderType(const string& orderTypeAsString) {
    if (orderTypeAsString == "deploy")
        return Order::OrderType::Deploy;

    else if (orderTypeAsString == "advance")
        return Order::OrderType::Advance;

    else if (orderTypeAsString == "bomb")
        return Order::OrderType::Bomb;

    else if (orderTypeAsString == "blockade")
        return Order::OrderType::Blockade;

    else if (orderTypeAsString == "airlift")
        return Order::OrderType::Airlift;

    else if (orderTypeAsString == "negotiate")
        return Order::OrderType::Negotiate;

    else
        return Order::OrderType::Invalid;
}

string Order::orderTypeToString(Order::OrderType orderType) {
    switch (orderType) {
        case Order::OrderType::Deploy:
            return "deploy";

        case Order::OrderType::Advance:
            return "advance";

        case Order::OrderType::Bomb:
            return "bomb";

        case Order::OrderType::Blockade:
            return "blockade";

        case Order::OrderType::Airlift:
            return "airlift";

        case Order::OrderType::Negotiate:
            return "negotiate";

        default:
            return "invalid";
    }
}


Order::Order(Player *owner, const string& type, Territory *target)
{
    this->owner = owner;
    this->type = parseOrderType(type);
    this->target = target;
}

Order::Order(Player *owner, Order::OrderType orderType, Territory *target)
{
    this->owner = owner;
    this->type = orderType;
    this->target = target;
}

Order::Order(Order &order)
{
    owner = order.owner;
    type = order.type;
    target = order.target;
}

Order::~Order() = default;

Order::OrderType Order::getOrderType()
{
    return this->type;
}

Order &Order::operator=(const Order &order)
{
    if (this != &order)
    {
        owner = order.owner;
        type = order.type;
        target = order.target;
    }

    return *this;
}

ostream &operator<<(ostream &outs, const Order &order)
{
    if (order.target)
    {
        return outs << "Order: type = " << Order::orderTypeToString(order.type) << ", target = " << order.target->getName();
    }
    else
    {
        return outs << "Order: type = " << Order::orderTypeToString(order.type) << ", target = NULL";
    }
}

//----------------------------------------------------------------------------------------------------------------------
//  DEPLOY ORDER
DeployOrder::DeployOrder(Player *owner, Territory *target, int armyUnits)
    : Order(owner, "deploy", target)
{
    DeployOrder::armyUnits = armyUnits;
}

DeployOrder::DeployOrder(DeployOrder &order)
    : Order(order)
{
    DeployOrder::armyUnits = order.armyUnits;
}

DeployOrder::~DeployOrder() = default;

DeployOrder &DeployOrder::operator=(const DeployOrder &order)
{
    if (this != &order)
    {
        Order::operator=(order);
        armyUnits = order.armyUnits;
    }

    return *this;
}

string DeployOrder::stringToLog()
{
    return "Order: type = " + Order::orderTypeToString(this->type) + ", target = " + this->target->getName() + ", army units = " + std::to_string(this->armyUnits);
}

ostream &operator<<(ostream &outs, const DeployOrder &order)
{
    return outs << "DeployOrder: type = " << Order::orderTypeToString(order.type) << ", target = " << order.target->getName()
                << ", army units = " << order.armyUnits;
}

//----------------------------------------------------------------------------------------------------------------------
//  ADVANCE ORDER
AdvanceOrder::AdvanceOrder(Player *owner, Territory *target, int armyUnits, Territory *source)
    : Order(owner, "advance", target)
{
    AdvanceOrder::armyUnits = armyUnits;
    AdvanceOrder::source = source;
}

AdvanceOrder::AdvanceOrder(AdvanceOrder &order)
    : Order(order)
{
    AdvanceOrder::armyUnits = order.armyUnits;
    AdvanceOrder::source = order.source;
}

AdvanceOrder::~AdvanceOrder() = default;

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order)
{
    if (this != &order)
    {
        Order::operator=(order);
        armyUnits = order.armyUnits;
        source = order.source;
    }

    return *this;
}

string AdvanceOrder::stringToLog()
{
    return "Order: type = " + Order::orderTypeToString(this->type) + ", target = " + this->target->getName() + ", army units = " + std::to_string(this->armyUnits) + ", source = " + this->source->getName();
}

ostream &operator<<(ostream &outs, const AdvanceOrder &order)
{
    return outs << "AdvanceOrder: type = " << Order::orderTypeToString(order.type) << ", target = " << order.target->getName()
                << ", army units = " << order.armyUnits << ", source = " << order.source->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  BOMB ORDER
BombOrder::BombOrder(Player *owner, Territory *target)
    : Order(owner, "bomb", target)
{
    //  Empty
}

BombOrder::BombOrder(BombOrder &order)
    : Order(order)
{
    //  Empty
}

BombOrder::~BombOrder() = default;

BombOrder &BombOrder::operator=(const BombOrder &order)
{
    if (this != &order)
    {
        Order::operator=(order);
    }

    return *this;
}

string BombOrder::stringToLog()
{
    return "Order: type = " + Order::orderTypeToString(this->type) + ", target = " + this->target->getName();
}

ostream &operator<<(ostream &outs, const BombOrder &order)
{
    return outs << "BombOrder: type = " << Order::orderTypeToString(order.type) << ", target = " << order.target->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  BLOCKADE ORDER
BlockadeOrder::BlockadeOrder(Player *owner, Territory *target)
    : Order(owner, "blockade", target)
{
    //  Empty
}

BlockadeOrder::BlockadeOrder(BlockadeOrder &order)
    : Order(order)
{
    //  Empty
}

BlockadeOrder::~BlockadeOrder() = default;

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order)
{
    if (this != &order)
    {
        Order::operator=(order);
    }

    return *this;
}

string BlockadeOrder::stringToLog()
{
    return "Order: type = " + Order::orderTypeToString(this->type) + ", target = " + this->target->getName();
}

ostream &operator<<(ostream &outs, const BlockadeOrder &order)
{
    return outs << "BlockadeOrder: type = " << Order::orderTypeToString(order.type) << ", target = " << order.target->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  AIRLIFT ORDER
AirliftOrder::AirliftOrder(Player *owner, Territory *target, int armyUnits, Territory *source)
    : Order(owner, "airlift", target)
{
    AirliftOrder::armyUnits = armyUnits;
    AirliftOrder::source = source;
}

AirliftOrder::AirliftOrder(AirliftOrder &order)
    : Order(order)
{
    AirliftOrder::armyUnits = order.armyUnits;
    AirliftOrder::source = order.source;
}

AirliftOrder::~AirliftOrder() = default;

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order)
{
    if (this != &order)
    {
        Order::operator=(order);
        armyUnits = order.armyUnits;
        source = order.source;
    }

    return *this;
}

string AirliftOrder::stringToLog()
{
    return "Order: type = " + Order::orderTypeToString(this->type) + ", target = " + this->target->getName()
                            + ", army units = " + std::to_string(this->armyUnits) + ", source = " + this->source->getName();
}

ostream &operator<<(ostream &outs, const AirliftOrder &order)
{
    return outs << "AirliftOrder: type = " << Order::orderTypeToString(order.type) << ", target = " << order.target->getName()
                << ", army units = " << order.armyUnits << ", source = " << order.source->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  NEGOTIATE ORDER
NegotiateOrder::NegotiateOrder(Player *owner, Player *player)
    : Order(owner, Order::OrderType::Negotiate, nullptr)
{
    NegotiateOrder::player = player;
}

NegotiateOrder::NegotiateOrder(NegotiateOrder &order)
    : Order(order)
{
    player = order.player;
}

NegotiateOrder::~NegotiateOrder() = default;

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order)
{
    if (this != &order)
    {
        Order::operator=(order);
        player = order.player;
    }

    return *this;
}

string NegotiateOrder::stringToLog()
{
    return "Order: type = " + Order::orderTypeToString(this->type) + ", target player = " + this->player->getPlayerName();
}

ostream &operator<<(ostream &outs, const NegotiateOrder &order)
{
    return outs << "NegotiateOrder: type = " << Order::orderTypeToString(order.type) << ", target player = " << order.player->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER VALIDATIONS

/** \remarks Generally, a valid order is one that is:
 *           <ul><li> <b>NOT</b> invalid in type.
 *               <li> A valid target is specified.
 *           </ul>
 *           <br>
 *           Note that <code>Order</code> subclasses may have extra conditions to be valid.
 */
bool Order::validate()
{
    return type != Order::OrderType::Invalid
        && (target != nullptr);
}

/** \remarks A <code>DeployOrder</code> is valid when:
 *           <ul><li> A valid target is specified. Additionally, the target territory must be owned by he player
 *                    owning the order.
 *           </ul>
 */
bool DeployOrder::validate()
{
    return (target != nullptr)
        && owner->ownsTerritory(*target);
}

/** \remarks A <code>AdvanceOrder</code> is valid when:
 *           <ul><li> A valid source and target territories are specified. This source territory does not need to be
 *                    owned by the player.
 *               <li> A valid number of army units are specified.
 *               <li> The owner owns the source territory.
 *               <li> The target territory is adjacent to the source territory.
 *               <li> The owner of the target territory must <b>NOT</b> be in negotiations with the current player.
 *           </ul>
 */
bool AdvanceOrder::validate()
{
    return (source != nullptr && target != nullptr)
        && (armyUnits > 0 && armyUnits <= source->numberOfArmies)   //  TODO: probably a logical bug here
        && (owner->ownsTerritory(*source))
        && (source->isTerritoryAdjacent(target))
        && !(owner->isPlayerInNegotiations(*target->getOwner()));
}

/** \remarks A <code>BombOrder</code> is valid when:
 *           <ul><li> A valid target territory is specified. A player <b>CANNOT</b> bomb their own territory.
 *               <li> The target territory must be adjacent to the player's territories.
 *               <li> The owner of the target territory must <b>NOT</b> be in negotiations with the current player.
 *           </ul>
 */
bool BombOrder::validate()
{
    return (target != nullptr)
        && !(owner->ownsTerritory(*target))
        && (owner->isTerritoryAdjacent(*target))
        && !(owner->isPlayerInNegotiations(*target->getOwner()));
}

/** \remarks A <code>BlockadeOrder</code> is valid when:
 *           <ul><li> A valid target territory is specified. A player <b>CAN ONLY</b> blockade their own territory.
 *           </ul>
 */
bool BlockadeOrder::validate()
{
    return (target != nullptr)
        && (owner->ownsTerritory(*target));
}

/** \remarks A <code>AirliftOrder</code> is valid when:
 *           <ul><li> Valid source and target territories are specified. The player <b>MUST OWN</b> these territories.
 *               <li> A valid number of army units are specified.
 *           </ul>
 */
bool AirliftOrder::validate()
{
    return (source != nullptr && target != nullptr)
        && (source && armyUnits > 0 && armyUnits <= source->numberOfArmies)
        && (owner->ownsTerritory(*source))
        && (owner->ownsTerritory(*target));
}

/** \remarks A <code>NegotiateOrder</code> is valid when:
 *           <ul><li> A valid player is specified. A player cannot negotiate with themselves.
 *           </ul>
 */
bool NegotiateOrder::validate()
{
    return (player != nullptr)
        && (player != owner);
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER EXECUTES
string DeployOrder::execute()
{
    if (validate())
    {
        target->setNumberOfArmies(target->getNumberOfArmies() + armyUnits);
        cout << *this << " has been executed." << endl;
        notify(this);
        return to_string(armyUnits) + " units were added to " + target->getName() + ". It now has " + to_string(target->getNumberOfArmies()) + " units.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

string AdvanceOrder::execute()
{
    if (validate())
    {
        if (source->getOwner() == target->getOwner())
        {
            source->setNumberOfArmies(source->getNumberOfArmies() - armyUnits);
            target->setNumberOfArmies(target->getNumberOfArmies() + armyUnits);
            cout << *this << " has been executed." << endl;
            notify(this);
            return to_string(armyUnits) + " units were moved from " + source->getName() + " to " + target->getName() + ".";
        }
        else
        {
            int defenderUnitsKilled = 0;
            int attackerUnitsKilled = 0;
            for (int i = 0; i < armyUnits; i++)
            {
                float random = (float)rand() / RAND_MAX;
                if (random <= 0.6f)
                {
                    defenderUnitsKilled++;
                }
            }
            for (int i = 0; i < target->getNumberOfArmies(); i++)
            {
                float random = (float)rand() / RAND_MAX;
                if (random <= 0.7f)
                {
                    attackerUnitsKilled++;
                }
            }
            if (defenderUnitsKilled >= target->getNumberOfArmies())
            {
                target->getOwner()->removeTerritory(*target);    //  The owner of the target no longer owns the target territory
                target->setOwner(owner);                        //  Set the owner to this player (player that owns the order)
                owner->addTerritory(*target);
                source->setNumberOfArmies(source->getNumberOfArmies() - armyUnits);
                target->setNumberOfArmies(armyUnits > attackerUnitsKilled ? armyUnits - attackerUnitsKilled : 0);
                owner->setDrawCard(true);
                cout << *this << " has been executed." << endl;
                notify(this);
                return owner->getName() + " has captured " + target->getName() + ". It is now occupied by " + to_string(target->getNumberOfArmies()) + " units.";
            }
            else
            {
                target->setNumberOfArmies(target->getNumberOfArmies() - defenderUnitsKilled);
                source->setNumberOfArmies(armyUnits >= attackerUnitsKilled ? source->getNumberOfArmies() - attackerUnitsKilled : source->getNumberOfArmies() - armyUnits);
                cout << *this << " has been executed." << endl;
                notify(this);
                return "The attack resulted in " + source->getName() + " having " + to_string(source->getNumberOfArmies()) + "units and " + target->getName() + " having " + to_string(target->getNumberOfArmies()) + " units left.";
            }
        }
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

string BombOrder::execute()
{
    if (validate())
    {
        target->setNumberOfArmies(target->getNumberOfArmies() / 2);
        cout << *this << " has been executed." << endl;
        notify(this);
        return target->getName() + " was bombed. It has" + to_string(target->getNumberOfArmies()) + " units left.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

string BlockadeOrder::execute()
{
    if (validate())
    {
        target->getOwner()->removeTerritory(*target);        //  The owner of the target no longer owns the target territory
        Player::neutralPlayer->addTerritory(*target);
        target->setOwner(Player::neutralPlayer);
        target->setNumberOfArmies(target->getNumberOfArmies() * 2);

        cout << *this << " has been executed." << endl;
        notify(this);
        return "Neutral player now owns " + target->getName() + " with " + to_string(target->getNumberOfArmies()) + " units on it.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

string AirliftOrder::execute()
{
    if (validate())
    {
        source->setNumberOfArmies(source->getNumberOfArmies() - armyUnits);
        target->setNumberOfArmies(target->getNumberOfArmies() + armyUnits);
        cout << *this << " has been executed." << endl;
        notify(this);
        return to_string(armyUnits) + " units have been moved from " + source->getName() + " to " + target->getName() + ".";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

string NegotiateOrder::execute()
{
    if (validate())
    {
        owner->negotiateWith(*player);
        player->negotiateWith(*owner);
        cout << *this << " has been executed." << endl;
        notify(this);
        return owner->getName() + " and " + player->getName() + " are now negotiating.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}