//  Compiler specific macros
//  Disables clang modernize suggestions
#ifdef __GNUC__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "UnusedParameter"
#endif

#include <iostream>
#include <ranges>
#include <algorithm>
#include <utility>

#include "../../headers/player/Player.h"
#include "../../headers/player/HumanPlayerStrategy.h"
#include "../../headers/Orders.h"
#include "../../headers/Map.h"
#include "../../headers/Cards.h"

using namespace std;

Player *Player::neutralPlayer = new Player("Neutral");

//----------------------------------------------------------------------------------------------------------------------
//  Constructors

//  Default constructor
//  Default name of "DEFAULT", human player, empty lists of territories and cards
Player::Player()
        : Player("DEFAULT", new HumanPlayerStrategy(this), {}, {})
{ }

//  Parameterized constructor
//  Passed name, human player, empty lists of territories and cards
Player::Player(const string &name)
        : Player(name, new HumanPlayerStrategy(this), {}, {})
{ }

//  Parameterized constructor
//  Passed name, human player, passed list of territories and cards
Player::Player(const string& name, vector<Territory*> territories, vector<Card*> cards)
        : Player(name, new HumanPlayerStrategy(this), std::move(territories), std::move(cards))
{ }

//  Parameterized constructor
//  Passed name, passed player strategy/behavior, passed list of territories and cards
Player::Player(const string& name, PlayerStrategy* playerStrategy, vector<Territory*> territories, vector<Card*> cards)
{
    this->name = name;
    this->reinforcementPool = 0;    //  Start off with 0 troops in the reinforcement pool, can be increased later
    this->drawCard = false;         //  Start off with 'false' for now,
    this->playerStrategy = playerStrategy;
    this->ordersList = new OrdersList(this);     //  Create an 'OrderList' object with this player as the owner
    this->territories = std::move(territories);
    this->cards = std::move(cards);
    this->playersInNegotiation = {};
}

//  Copy constructor
Player::Player(const Player& otherPlayer)
        : Player(otherPlayer.name, otherPlayer.playerStrategy->clone(), otherPlayer.territories, otherPlayer.cards)
{
    this->reinforcementPool = otherPlayer.reinforcementPool;
    this->drawCard = otherPlayer.drawCard;
    this->ordersList = new OrdersList(*otherPlayer.ordersList);    //  Copy the 'OrdersList'
    this->playersInNegotiation = otherPlayer.playersInNegotiation;
}

Player::~Player() {
    delete playerStrategy;
    delete ordersList;
}

//----------------------------------------------------------------------------------------------------------------------
//  Operator overrides

// Assignment Operator Overload
Player &Player::operator=(const Player& otherPlayer)
{
    if (this != &otherPlayer)
    {
        name = otherPlayer.name;
        reinforcementPool = otherPlayer.reinforcementPool;
        drawCard = otherPlayer.drawCard;
        playerStrategy = otherPlayer.playerStrategy->clone();
        ordersList = new OrdersList(*otherPlayer.ordersList);
        territories = otherPlayer.territories;
        cards = otherPlayer.cards;
        playersInNegotiation = otherPlayer.playersInNegotiation;
    }

    return *this;
}

// Stream Insertion Operator Overload
ostream &operator<<(ostream &out, const Player &player)
{
    out << "Player Name: " << player.name << endl;
    out << "Territories: ";
    for (const auto &terr : player.territories)
    {
        out << terr->getName() << " ";
    }
    out << endl;
    out << "Orders List: ";
    out << *(player.ordersList);
    out << endl;
    return out;
}

//----------------------------------------------------------------------------------------------------------------------
//  Other methods

void Player::issueOrder(const string &type, Territory *target, int armyUnits, Territory *source, Player *player)
{
    if (type == "deploy")
        issueOrder(Order::OrderType::Deploy, target, armyUnits, source, player);
    else if (type == "advance")
        issueOrder(Order::OrderType::Advance, target, armyUnits, source, player);
    else if (type == "bomb")
        issueOrder(Order::OrderType::Bomb, target, armyUnits, source, player);
    else if (type == "blockade")
        issueOrder(Order::OrderType::Blockade, target, armyUnits, source, player);
    else if (type == "airlift")
        issueOrder(Order::OrderType::Airlift, target, armyUnits, source, player);
    else if (type == "negotiate")
        issueOrder(Order::OrderType::Negotiate, target, armyUnits, source, player);
    else
        cout << "Specified order is not allowed. No order was added to the orders list." << endl;
}

void Player::issueOrder(Order::OrderType orderType, Territory *target, int armyUnits, Territory *source, Player *player) {
    switch (orderType) {

        //  Issues a 'deploy' order. No card has to be present like other orders.
        case Order::OrderType::Deploy:
            if (armyUnits > 0 && armyUnits <= reinforcementPool)
            {
                ordersList->addOrder(new DeployOrder(this, target, armyUnits));
                reinforcementPool -= armyUnits;
                return;
            }
            cout << "Specified number of army units is invalid. No order was added to the orders list." << endl;
            return;

        //  Issue an 'advance' order. No card has to be present like other orders.
        case Order::OrderType::Advance:
        {
            ordersList->addOrder(new AdvanceOrder(this, target, armyUnits, source));
            return;
        }

        //  We check if the player has a 'bomb' card. If so, we 'consume' the card and issue an order
        //  If not, print an error message, nothing is issued.
        case Order::OrderType::Bomb:
            for (int i = 0; i < cards.size(); i++)
            {
                if (cards[i]->getCardType() != type::bomb)
                    continue;

                ordersList->addOrder(new BombOrder(this, target));
                Deck::getInstance().addCard(cards[i]);
                cards.erase(cards.begin() + i);
                return;
            }
            cout << "The card to issue Bomb order was not found in the player's hand. No order was added to the orders list." << endl;
            return;

        //  We check if the player has a 'blockage' card. If so, we 'consume' the card and issue an order
        //  If not, print an error message, nothing is issued.
        case Order::OrderType::Blockade:
        {
            for (int i = 0; i < cards.size(); i++)
            {
                if (cards[i]->getCardType() != type::blockade)
                    continue;

                ordersList->addOrder(new BlockadeOrder(this, target));
                Deck::getInstance().addCard(cards[i]);
                cards.erase(cards.begin() + i);
                return;

            }
            cout << "The card to issue Blockade order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }

        //  We check if the player has a 'airlift' card. If so, we 'consume' the card and issue an order
        //  If not, print an error message, nothing is issued.
        case Order::OrderType::Airlift:
        {
            for (int i = 0; i < cards.size(); i++)
            {
                if (cards[i]->getCardType() != type::airlift)
                    continue;

                ordersList->addOrder(new AirliftOrder(this, target, armyUnits, source));
                Deck::getInstance().addCard(cards[i]);
                cards.erase(cards.begin() + i);
                return;
            }
            cout << "The card to issue Airlift order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }

        //  We check if the player has a 'negotiate' card. If so, we 'consume' the card and issue an order
        //  If not, print an error message, nothing is issued.
        case Order::OrderType::Negotiate:
        {
            for (int i = 0; i < cards.size(); i++)
            {
                if (cards[i]->getCardType() != type::diplomacy)
                    continue;

                ordersList->addOrder(new NegotiateOrder(this, player));
                Deck::getInstance().addCard(cards[i]);
                cards.erase(cards.begin() + i);
                return;
            }
            cout << "The card to issue Negotiate order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }

        default:
            cout << "Specified order is not allowed. No order was added to the orders list." << endl;
            return;
    }
}

vector<Territory*> Player::toAttack() {
    return playerStrategy->toAttack();
}

vector<Territory*> Player::toDefend() {
    return playerStrategy->toDefend();
}

void Player::issueOrder() {
    ordersList->addOrder(playerStrategy->issueOrder());
}

//----------------------------------------------------------------------------------------------------------------------
//  Getter/Accessor methods

string Player::getName() const { return name; }

int Player::getUnits() const { return reinforcementPool; }

bool Player::drawsCard() const { return drawCard; }

vector<const Territory*> Player::getTerritories() const {
    vector<const Territory*> territoriesCopy(territories.size());
    std::transform(territories.begin(),
                   territories.end(),
                   territoriesCopy.begin(),
                   [] (Territory* ptr) -> const Territory* { return ptr; });

    return territoriesCopy;
}

vector<const Card*> Player::getCards() const {
    vector<const Card*> cardsCopy(cards.size());
    std::transform(cards.begin(),
                   cards.end(),
                   cardsCopy.begin(),
                   [] (Card* ptr) -> const Card* { return ptr; });

    return cardsCopy;
}

vector<const Player*> Player::getPlayersInNegotiation() const {
    vector<const Player*> playersInNegotiationCopy(playersInNegotiation.size());
    std::transform(playersInNegotiation.begin(),
                   playersInNegotiation.end(),
                   playersInNegotiationCopy.begin(),
                   [] (Player* ptr) -> const Player* { return ptr; });

    return playersInNegotiationCopy;
}


//----------------------------------------------------------------------------------------------------------------------
//  Setter/Mutator methods

void Player::setPlayerName(const string& newName) {
    name = newName;
}

void Player::addToReinforcementPool(int units)
{
    reinforcementPool += units;
}

void Player::setDrawCard(bool doesDrawCard)
{
    this->drawCard = doesDrawCard;
}

void Player::addTerritory(Territory& newTerritory)
{
    territories.push_back(&newTerritory);
}

void Player::addCard(Card& newCard)
{
    cards.push_back(&newCard);
}

void Player::negotiateWith(Player& player)
{
    if (!isPlayerInNegotiations(player))
        playersInNegotiation.push_back(&player);
}

void Player::removeTerritory(Territory& target)
{
    //  TODO: Implement soon after idk
}

bool Player::ownsTerritory(Territory& territory) const
{
    auto it = std::find_if(territories.begin(), territories.end(),
                           [&territory] (Territory* ptr) { return &territory == ptr;
    });

    return it != territories.end();
}

bool Player::isTerritoryAdjacent(Territory& target) const
{
    Territory* targetAsPtr = &target;
    return std::ranges::any_of(territories, [targetAsPtr] (Territory* ptr) {
        return ptr->isTerritoryAdjacent(targetAsPtr);
    });
}

bool Player::isPlayerInNegotiations(Player& player) const
{
    auto it = std::find_if(playersInNegotiation.begin(), playersInNegotiation.end(),
                           [&player] (Player* ptr) { return &player == ptr; });

    return it != playersInNegotiation.end();
}

OrdersList *Player::getOrdersList() const {
    return ordersList;
}

//  ->-------------------------------------------------------------------------------------------------------------------



#ifdef __GNUC__
#pragma clang diagnostic pop
#endif