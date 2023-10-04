#include <iostream>
#include "../headers/Orders.h"

//----------------------------------------------------------------------------------------------------------------------
//  ORDERS LIST
OrdersList::OrdersList()
{
}

OrdersList::OrdersList(OrdersList &ordersList)
{
    orders = ordersList.orders;
}

OrdersList &OrdersList::operator=(const OrdersList &ordersList)
{
    orders = ordersList.orders;
    return *this;
}

ostream &operator<<(ostream &outs, const OrdersList &ordersList)
{
    outs << "Orders List: {\n";
    list<Order *>::const_iterator it;
    for (it = ordersList.orders.begin(); it != ordersList.orders.end(); ++it)
    {
        outs << *it << ";" << endl;
    }
    outs << "}";
    return outs;
}

void OrdersList::remove(Order *order)
{
    orders.remove(order);
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
    list<Order *>::iterator it = orders.begin();
    advance(it, index);
    orders.insert(it, order);
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER
Order::Order(string type /*, Territory* target*/)
{
    Order::type = type;
    // Order::target = target;
}

Order::Order(Order &order)
{
    type = order.type;
    // target = order.target;
}

Order &Order::operator=(const Order &order)
{
    type = order.type;
    // target = order.target;
    return *this;
}

ostream &operator<<(ostream &outs, const Order &order)
{
    return outs << "Order: type = " << order.type << ", target = " /*<< order.target*/;
}

//----------------------------------------------------------------------------------------------------------------------
//  DEPLOY ORDER
DeployOrder::DeployOrder(/*Territory* target,*/ int armyUnits)
    : Order("deploy" /*, target*/)
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
    return outs << "DeployOrder: type = " << order.type << ", target = " /*<< order.target*/
                << ", army units = " << order.armyUnits;
}

//----------------------------------------------------------------------------------------------------------------------
//  ADVANCE ORDER
AdvanceOrder::AdvanceOrder(/*Territory* target,*/ int armyUnits /*, Territory* source*/)
    : Order("advance" /*, target*/)
{
    AdvanceOrder::armyUnits = armyUnits;
    // AdvanceOrder::source = source;
}

AdvanceOrder::AdvanceOrder(AdvanceOrder &order)
    : Order(order)
{
    AdvanceOrder::armyUnits = order.armyUnits;
    // AdvanceOrder::source = order.source;
}

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order)
{
    Order::operator=(order);
    armyUnits = order.armyUnits;
    // source = order.source;
    return *this;
}

ostream &operator<<(ostream &outs, const AdvanceOrder &order)
{
    return outs << "AdvanceOrder: type = " << order.type << ", target = " /*<< order.target*/
                << ", army units = " << order.armyUnits << ", source = " /*<<order.source*/;
}

//----------------------------------------------------------------------------------------------------------------------
//  BOMB ORDER
BombOrder::BombOrder(/*Territory* target*/)
    : Order("bomb" /*, target*/)
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
    return outs << "BombOrder: type = " << order.type << ", target = " /*<< order.target*/;
}

//----------------------------------------------------------------------------------------------------------------------
//  BLOCKADE ORDER
BlockadeOrder::BlockadeOrder(/*Territory* target*/)
    : Order("blockade" /*, target*/)
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
    return outs << "BlockadeOrder: type = " << order.type << ", target = " /*<< order.target*/;
}

//----------------------------------------------------------------------------------------------------------------------
//  AIRLIFT ORDER
AirliftOrder::AirliftOrder(/*Territory* target,*/ int armyUnits /*, Territory* source*/)
    : Order("airlift" /*, target*/)
{
    AirliftOrder::armyUnits = armyUnits;
    // AirliftOrder::source = source;
}

AirliftOrder::AirliftOrder(AirliftOrder &order)
    : Order(order)
{
    AirliftOrder::armyUnits = order.armyUnits;
    // AirliftOrder::source = order.source;
}

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order)
{
    Order::operator=(order);
    armyUnits = order.armyUnits;
    // source = order.source;
    return *this;
}

ostream &operator<<(ostream &outs, const AirliftOrder &order)
{
    return outs << "AirliftOrder: type = " << order.type << ", target = " /*<< order.target*/
                << ", army units = " << order.armyUnits << ", source = " /*<<order.source*/;
}

//----------------------------------------------------------------------------------------------------------------------
//  NEGOTIATE ORDER
NegotiateOrder::NegotiateOrder(/*Player* player*/)
    : Order("negotiate" /*, NULL*/)
{
    // NegotiateOrder::player = player;
}

NegotiateOrder::NegotiateOrder(NegotiateOrder &order)
    : Order(order)
{
    // NegotiateOrder::player = order.player;
}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order)
{
    Order::operator=(order);
    // player = order.player;
    return *this;
}

ostream &operator<<(ostream &outs, const NegotiateOrder &order)
{
    return outs << "NegotiateOrder: type = " << order.type << ", target player = " /*<<order.player*/;
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER VALIDATIONS
bool Order::validate()
{
    string allowedTypes[6] = {"deploy", "advance", "bomb", "blockade", "airlift", "negotiate"};

    // TO DO: add verification of target territory validity
    return find(begin(allowedTypes), end(allowedTypes), type) != end(allowedTypes);
}

bool DeployOrder::validate()
{
    // TO DO: add verification of number of army units specified validity (reinforcement pool)
    return Order::validate() && armyUnits > 0;
}

bool AdvanceOrder::validate()
{
    // TO DO: add verification of number of army units specified validity (<= source territory's)
    // TO DO: add verification of source territory validity
    return Order::validate() && armyUnits > 0;
}

bool BombOrder::validate()
{
    // TO DO: add verification of bomb card in hand
    return Order::validate();
}

bool BlockadeOrder::validate()
{
    // TO DO: add verification of blockade card in hand
    return Order::validate();
}

bool AirliftOrder::validate()
{
    // TO DO: add verification of number of army units specified validity (<= source territory's)
    // TO DO: add verification of source territory validity
    // TO DO: add verification of airlift card in hand
    return Order::validate() && armyUnits > 0;
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER EXECUTES
string Order::execute()
{
    cout << this << " has been executed.";
    return "Order has been executed.";
}

// TO DO: Implement DeployOrder's actions
string DeployOrder::execute()
{
    cout << this << " has been executed.";
    return "DeployOrder has been executed.";
}

// TO DO: Implement AdvanceOrder's actions
string AdvanceOrder::execute()
{
    cout << this << " has been executed.";
    return "AdvanceOrder has been executed.";
}

// TO DO: Implement BombOrder's actions
string BombOrder::execute()
{
    cout << this << " has been executed.";
    return "BombOrder has been executed.";
}

// TO DO: Implement BlockadeOrder's actions
string BlockadeOrder::execute()
{
    cout << this << " has been executed.";
    return "BlockadeOrder has been executed.";
}

// TO DO: Implement AirliftOrder's actions
string AirliftOrder::execute()
{
    cout << this << " has been executed.";
    return "AirliftOrder has been executed.";
}

// TO DO: Implement NegotiateOrder's actions
string NegotiateOrder::execute()
{
    cout << this << " has been executed.";
    return "NegotiateOrder has been executed.";
}