#include <iostream>

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

Order *OrdersList::operator[](size_t index) {
    if (index < orders.size()) {
        auto iterator = orders.begin();
        std::advance(iterator, index);
        return *iterator;
    }

    return nullptr;
}

OrdersList &OrdersList::operator=(const OrdersList &ordersList)
{
    if (this == &ordersList) {

    }

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

bool OrdersList::addOrder(Order *order) {
    if (order != nullptr) {
        orders.push_back(order);
        return true;
    }

    return false;
}

const Player* OrdersList::getOwner() const {
    return this->owner;
}

Order *OrdersList::getNextOrder() {
    auto* nextOrder = orders.front();
    orders.pop_front();
    return nextOrder;
}

void OrdersList::apply(void (*func)(Order*)) {
    for (auto* order : orders) {
        if (order != nullptr) {
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

//----------------------------------------------------------------------------------------------------------------------
//  ORDER
Order::Order(Player *owner, const string& type, Territory *target) {
    this->owner = owner;
    this->type = type;
    this->target = target;
}

Order::Order(Order &order)
{
    owner = order.owner;
    type = order.type;
    target = order.target;
}

Order::~Order() = default;

Order &Order::operator=(const Order &order)
{
    if (this != &order) {
        owner = order.owner;
        type = order.type;
        target = order.target;
    }

    return *this;
}

ostream &operator<<(ostream &outs, const Order &order)
{
    return outs << "Order: type = " << order.type << ", target = " << order.target;
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
    if (this != &order) {
        Order::operator=(order);
        armyUnits = order.armyUnits;
    }

    return *this;
}

ostream &operator<<(ostream &outs, const DeployOrder &order)
{
    return outs << "DeployOrder: type = " << order.type << ", target = " << order.target
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
    if (this != &order) {
        Order::operator=(order);
        armyUnits = order.armyUnits;
        source = order.source;
    }

    return *this;
}

ostream &operator<<(ostream &outs, const AdvanceOrder &order)
{
    return outs << "AdvanceOrder: type = " << order.type << ", target = " << order.target
                << ", army units = " << order.armyUnits << ", source = " << order.source;
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
    if (this != &order) {
        Order::operator=(order);
    }

    return *this;
}

ostream &operator<<(ostream &outs, const BombOrder &order)
{
    return outs << "BombOrder: type = " << order.type << ", target = " << order.target;
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
    if (this != &order) {
        Order::operator=(order);
    }

    return *this;
}

ostream &operator<<(ostream &outs, const BlockadeOrder &order)
{
    return outs << "BlockadeOrder: type = " << order.type << ", target = " << order.target;
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
    if (this != &order) {
        Order::operator=(order);
        armyUnits = order.armyUnits;
        source = order.source;
    }

    return *this;
}

ostream &operator<<(ostream &outs, const AirliftOrder &order)
{
    return outs << "AirliftOrder: type = " << order.type << ", target = " << order.target
                << ", army units = " << order.armyUnits << ", source = " << order.source;
}

//----------------------------------------------------------------------------------------------------------------------
//  NEGOTIATE ORDER
NegotiateOrder::NegotiateOrder(Player *owner, Player *player)
    : Order(owner, "negotiate", nullptr)
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
    if (this != &order) {
        Order::operator=(order);
        player = order.player;
    }

    return *this;
}

ostream &operator<<(ostream &outs, const NegotiateOrder &order)
{
    return outs << "NegotiateOrder: type = " << order.type << ", target player = " << order.player;
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER VALIDATIONS
bool Order::validate()
{
    return (find(begin(allowedOrders), end(allowedOrders), type) != end(allowedOrders)) && target;
}

bool DeployOrder::validate()
{
    // TO DO: add verification of number of army units specified validity (reinforcement pool)
    return Order::validate() && armyUnits > 0 && ((find(begin(owner->territory), end(owner->territory), target)) != end(owner->territory));
}

bool AdvanceOrder::validate()
{
    return Order::validate() && source && armyUnits > 0 && armyUnits <= (*target).numberOfArmies && (find(begin((*source).adjacentTerritories), end((*source).adjacentTerritories), target) != end((*source).adjacentTerritories)) && ((find(begin(owner->territory), end(owner->territory), source)) != end(owner->territory));
}

bool BombOrder::validate()
{
    return Order::validate() && (find_if(begin(owner->handCard), end(owner->handCard), [](const Card *card) -> bool
                                         { return (*card).getCardType() == type::bomb; }) != end(owner->handCard)) &&
           ((find(begin(owner->territory), end(owner->territory), target)) == end(owner->territory));
}

bool BlockadeOrder::validate()
{
    return Order::validate() && ((find(begin(owner->territory), end(owner->territory), target)) != end(owner->territory)) && (find_if(begin(owner->handCard), end(owner->handCard), [](const Card *card) -> bool
                                                                                                                                                                          { return (*card).getCardType() == type::blockade; }) != end((owner->handCard)));
}

bool AirliftOrder::validate()
{
    return Order::validate() && source && armyUnits > 0 && armyUnits <= (*target).numberOfArmies && (find_if(begin(owner->handCard), end(owner->handCard), [](const Card *card) -> bool
                                                                                                             { return (*card).getCardType() == type::airlift; }) != end(owner->handCard)) &&
           ((find(begin(owner->territory), end(owner->territory), source)) != end(owner->territory));
}

bool NegotiateOrder::validate()
{
    return (find(begin(allowedOrders), end(allowedOrders), type) != end(allowedOrders)) && player && (find_if(begin(owner->handCard), end(owner->handCard), [](const Card *card) -> bool
                                                                                                              { return (*card).getCardType() == type::diplomacy; }) != end(owner->handCard));
}

//----------------------------------------------------------------------------------------------------------------------
//  ORDER EXECUTES
string Order::execute()
{
    if (validate())
    {
        cout << *this << " has been executed.\n" << endl;
        return "Order has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

// TO DO: Implement DeployOrder's actions
string DeployOrder::execute()
{
    if (validate())
    {
        cout << *this << " has been executed." << endl;
        return "DeployOrder has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

// TO DO: Implement AdvanceOrder's actions
string AdvanceOrder::execute()
{
    if (validate())
    {
        cout << *this << " has been executed." << endl;
        return "AdvanceOrder has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

// TO DO: Implement BombOrder's actions
string BombOrder::execute()
{
    if (validate())
    {
        cout << *this << " has been executed." << endl;
        return "BombOrder has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

// TO DO: Implement BlockadeOrder's actions
string BlockadeOrder::execute()
{
    if (validate())
    {
        cout << *this << " has been executed." << endl;
        return "BlockadeOrder has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

// TO DO: Implement AirliftOrder's actions
string AirliftOrder::execute()
{
    if (validate())
    {
        cout << *this << " has been executed." << endl;
        return "AirliftOrder has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}

// TO DO: Implement NegotiateOrder's actions
string NegotiateOrder::execute()
{
    if (validate())
    {
        cout << *this << " has been executed." << endl;
        return "NegotiateOrder has been executed.";
    }
    else
    {
        cout << *this << " is an invalid order. No action is executed" << endl;
        return "Invalid order.";
    }
}