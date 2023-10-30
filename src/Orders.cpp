#include <iostream>
#include <algorithm>

#include "../headers/Orders.h"
#include "../headers/Map.h"
#include "../headers/Player.h"
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

OrdersList &OrdersList::operator=(const OrdersList &ordersList)
{
    orders = ordersList.orders;
    owner = ordersList.owner;
    return *this;
}

ostream &operator<<(ostream &outs, const OrdersList &ordersList)
{
    outs << "Orders List: {\n";
    list<Order *>::const_iterator itr;
    for (itr = ordersList.orders.begin(); itr != ordersList.orders.end(); ++itr)
    {
        outs << *(*itr) << ";" << endl;
    }
    outs << "}";
    return outs;
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

//----------------------------------------------------------------------------------------------------------------------
//  ORDER
Order::Order(OrdersList *ordersList, string type, Territory *target)
{
    Order::ordersList = ordersList;
    Order::type = type;
    Order::target = target;
}

Order::Order(Order &order)
{
    ordersList = order.ordersList;
    type = order.type;
    target = order.target;
}

Order &Order::operator=(const Order &order)
{
    ordersList = order.ordersList;
    type = order.type;
    target = order.target;
    return *this;
}

ostream &operator<<(ostream &outs, const Order &order)
{
    if (order.target)
    {
        return outs << "Order: type = " << order.type << ", target = " << order.target->getName();
    }
    else
    {
        return outs << "Order: type = " << order.type << ", target = NULL";
    }
}

//----------------------------------------------------------------------------------------------------------------------
//  DEPLOY ORDER
DeployOrder::DeployOrder(OrdersList *ordersList, Territory *target, int armyUnits)
    : Order(ordersList, "deploy", target)
{
    DeployOrder::armyUnits = armyUnits;
}

DeployOrder::DeployOrder(DeployOrder &order)
    : Order(order)
{
    DeployOrder::armyUnits = order.armyUnits;
}

DeployOrder &DeployOrder::operator=(const DeployOrder &order)
{
    Order::operator=(order);
    armyUnits = order.armyUnits;
    return *this;
}

ostream &operator<<(ostream &outs, const DeployOrder &order)
{
    return outs << "DeployOrder: type = " << order.type << ", target = " << order.target->getName()
                << ", army units = " << order.armyUnits;
}

//----------------------------------------------------------------------------------------------------------------------
//  ADVANCE ORDER
AdvanceOrder::AdvanceOrder(OrdersList *ordersList, Territory *target, int armyUnits, Territory *source)
    : Order(ordersList, "advance", target)
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

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order)
{
    Order::operator=(order);
    armyUnits = order.armyUnits;
    source = order.source;
    return *this;
}

ostream &operator<<(ostream &outs, const AdvanceOrder &order)
{
    return outs << "AdvanceOrder: type = " << order.type << ", target = " << order.target->getName()
                << ", army units = " << order.armyUnits << ", source = " << order.source->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  BOMB ORDER
BombOrder::BombOrder(OrdersList *ordersList, Territory *target)
    : Order(ordersList, "bomb", target)
{
}

BombOrder::BombOrder(BombOrder &order)
    : Order(order)
{
}

BombOrder &BombOrder::operator=(const BombOrder &order)
{
    Order::operator=(order);
    return *this;
}

ostream &operator<<(ostream &outs, const BombOrder &order)
{
    return outs << "BombOrder: type = " << order.type << ", target = " << order.target->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  BLOCKADE ORDER
BlockadeOrder::BlockadeOrder(OrdersList *ordersList, Territory *target)
    : Order(ordersList, "blockade", target)
{
}

BlockadeOrder::BlockadeOrder(BlockadeOrder &order)
    : Order(order)
{
}

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order)
{
    Order::operator=(order);
    return *this;
}

ostream &operator<<(ostream &outs, const BlockadeOrder &order)
{
    return outs << "BlockadeOrder: type = " << order.type << ", target = " << order.target->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  AIRLIFT ORDER
AirliftOrder::AirliftOrder(OrdersList *ordersList, Territory *target, int armyUnits, Territory *source)
    : Order(ordersList, "airlift", target)
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

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order)
{
    Order::operator=(order);
    armyUnits = order.armyUnits;
    source = order.source;
    return *this;
}

ostream &operator<<(ostream &outs, const AirliftOrder &order)
{
    return outs << "AirliftOrder: type = " << order.type << ", target = " << order.target->getName()
                << ", army units = " << order.armyUnits << ", source = " << order.source->getName();
}

//----------------------------------------------------------------------------------------------------------------------
//  NEGOTIATE ORDER
NegotiateOrder::NegotiateOrder(OrdersList *ordersList, Player *player)
    : Order(ordersList, "negotiate", NULL)
{
    NegotiateOrder::player = player;
}

NegotiateOrder::NegotiateOrder(NegotiateOrder &order)
    : Order(order)
{
    player = order.player;
}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order)
{
    Order::operator=(order);
    player = order.player;
    return *this;
}

ostream &operator<<(ostream &outs, const NegotiateOrder &order)
{
    return outs << "NegotiateOrder: type = " << order.type << ", target player = " << order.player->getPlayerName();
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER VALIDATIONS
bool Order::validate()
{
    return (find(begin(allowedOrders), end(allowedOrders), type) != end(allowedOrders)) && target;
}

bool DeployOrder::validate()
{
    return Order::validate() && ((find(begin(ordersList->owner->territory), end(ordersList->owner->territory), target)) != end(ordersList->owner->territory));
}

bool AdvanceOrder::validate()
{
    return Order::validate() && source && armyUnits > 0 && armyUnits <= (*source).numberOfArmies && (find(begin((*source).adjacentTerritories), end((*source).adjacentTerritories), target) != end((*source).adjacentTerritories)) && ((find(begin(ordersList->owner->territory), end(ordersList->owner->territory), source)) != end(ordersList->owner->territory)) && source != target && ((find(begin(ordersList->owner->playersInNegotiation), end(ordersList->owner->playersInNegotiation), target->getOwner())) == end(ordersList->owner->playersInNegotiation));
}

bool validateEnemyAdjacent(Player *attacker, Territory *target)
{
    for (unsigned int i = 0; i < attacker->territory.size(); i++)
    {
        if ((find(begin(attacker->territory[i]->adjacentTerritories), end(attacker->territory[i]->adjacentTerritories), target)) != end(attacker->territory[i]->adjacentTerritories))
        {
            return true;
        }
    }
    return false;
}

bool BombOrder::validate()
{
    return Order::validate() &&
           ((find(begin(ordersList->owner->territory), end(ordersList->owner->territory), target)) == end(ordersList->owner->territory)) && validateEnemyAdjacent(ordersList->owner, target) && ((find(begin(ordersList->owner->playersInNegotiation), end(ordersList->owner->playersInNegotiation), target->getOwner())) == end(ordersList->owner->playersInNegotiation));
}

bool BlockadeOrder::validate()
{
    return Order::validate() && ((find(begin(ordersList->owner->territory), end(ordersList->owner->territory), target)) != end(ordersList->owner->territory));
}

bool AirliftOrder::validate()
{
    return Order::validate() && source && armyUnits > 0 && armyUnits <= (*source).numberOfArmies &&
           ((find(begin(ordersList->owner->territory), end(ordersList->owner->territory), source)) != end(ordersList->owner->territory)) &&
           ((find(begin(ordersList->owner->territory), end(ordersList->owner->territory), target)) != end(ordersList->owner->territory));
}

bool NegotiateOrder::validate()
{
    return (find(begin(allowedOrders), end(allowedOrders), type) != end(allowedOrders)) && player && ordersList->owner != player;
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER EXECUTES

string DeployOrder::execute()
{
    if (validate())
    {
        target->setNumberOfArmies(target->getNumberOfArmies() + armyUnits);
        cout << *this << " has been executed." << endl;
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
                target->getOwner()->territory.erase(remove(target->getOwner()->territory.begin(), target->getOwner()->territory.end(), target),
                                                    target->getOwner()->territory.end());
                target->setOwner(ordersList->owner);
                ordersList->owner->territory.push_back(target);
                target->setNumberOfArmies(armyUnits > attackerUnitsKilled ? armyUnits - attackerUnitsKilled : 0);
                ordersList->owner->setDrawCard(true);
                cout << *this << " has been executed." << endl;
                return ordersList->owner->getPlayerName() + " has captured " + target->getName() + ". It is now occupied by " + to_string(target->getNumberOfArmies()) + " units.";
            }
            else
            {
                target->setNumberOfArmies(target->getNumberOfArmies() - defenderUnitsKilled);
                source->setNumberOfArmies(armyUnits >= attackerUnitsKilled ? source->getNumberOfArmies() - attackerUnitsKilled : source->getNumberOfArmies() - armyUnits);
                cout << *this << " has been executed." << endl;
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
        target->getOwner()->territory.erase(remove(target->getOwner()->territory.begin(), target->getOwner()->territory.end(), target),
                                            target->getOwner()->territory.end());
        ordersList->owner->neutralPlayer->territory.push_back(target);
        target->setOwner(ordersList->owner->neutralPlayer);
        target->setNumberOfArmies(target->getNumberOfArmies() * 2);

        cout << *this << " has been executed." << endl;
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
        ordersList->owner->playersInNegotiation.push_back(player);
        player->playersInNegotiation.push_back(ordersList->owner);
        cout << *this << " has been executed." << endl;
        return ordersList->owner->getPlayerName() + " and " + player->getPlayerName() + " are now negotiating.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}