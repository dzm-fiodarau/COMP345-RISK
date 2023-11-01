#include <utility>

#include "../headers/Player.h"
#include "../headers/Orders.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

Player *Player::neutralPlayer = new Player("Neutral");

// Default constructor
Player::Player()
{
    playerName = "";
    ordersList = new OrdersList(this);
    reinforcementPool = 0;
    drawCard = false;
}

// Name constructor
Player::Player(string pn)
{
    playerName = pn;
    ordersList = new OrdersList(this);
    reinforcementPool = 0;
    drawCard = false;
}

// Constructor
Player::Player(string pn, vector<Territory *> t, vector<Card *> hd)
{
    playerName = std::move(pn);
    territory = std::move(t);
    handCard = std::move(hd);
    ordersList = new OrdersList(this);
    reinforcementPool = 0;
    drawCard = false;
}

// Copy constructor
Player::Player(const Player &p)
{
    playerName = p.playerName;
    territory = p.territory;
    handCard = p.handCard;
    ordersList = p.ordersList;
    reinforcementPool = p.getReinforcementPool();
    drawCard = p.getDrawCard();
    playersInNegotiation = p.playersInNegotiation;
}

// Destructor
Player::~Player()
{
    playerName.clear();
    delete ordersList;
}

// Assignment Operator Overload
Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        playerName = other.playerName;
        territory = other.territory;
        handCard = other.handCard;
        delete ordersList;                              // Free the existing memory
        ordersList = new OrdersList(*other.ordersList); // Creating a new OrdersList with copied contents
        reinforcementPool = other.getReinforcementPool();
        drawCard = other.getDrawCard();
        playersInNegotiation = playersInNegotiation;
    }
    return *this;
}

// Stream Insertion Operator Overload
ostream &operator<<(ostream &out, const Player &player)
{
    out << "Player Name: " << player.playerName << endl;
    out << "Territories: ";
    for (const auto &terr : player.territory)
    {
        out << terr->getName() << " ";
    }
    out << endl;
    out << "Orders List: ";
    out << *(player.ordersList);
    out << endl;
    return out;
}

// Method the get player name
string Player::getPlayerName() const
{
    return playerName;
}

// Method toAttack of Player
void Player::toAttack()
{
    cout << playerName << " is selecting territories to attack:" << endl;

    for (Territory *terr : territory)
    {
        cout << "Attacking " << terr->getName() << endl;
    }
}

// Method toDefend of Player
void Player::toDefend()
{
    cout << playerName << " is selecting territories to defend:" << endl;

    for (Territory *terr : territory)
    {
        cout << "Defending " << terr->getName() << endl;
    }
}

void Player::issueOrder(const string &type, Territory *target, int armyUnits, Territory *source, Player *player)
{
    Order *order;
    bool cardPresent = false;
    if (type == "deploy")
    {
        if (armyUnits > 0 && armyUnits <= reinforcementPool)
        {
            order = new DeployOrder(this, target, armyUnits);
            reinforcementPool -= armyUnits;
        }
        else
        {
            cout << "Specified number of army units is invalid. No order was added to the orders list." << endl;
            return;
        }
    }
    else if (type == "advance")
    {
        order = new AdvanceOrder(this, target, armyUnits, source);
    }
    else if (type == "bomb")
    {
        for (int i = 0; i < handCard.size(); i++)
        {
            if (handCard[i]->getCardType() == type::bomb)
            {
                cardPresent = true;
                Deck::getInstance().addCard(handCard[i]);
                handCard.erase(handCard.begin() + i);
                break;
            }
        }
        if (cardPresent)
        {
            order = new BombOrder(this, target);
        }
        else
        {
            cout << "The card to issue Bomb order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }
    }
    else if (type == "blockade")
    {
        for (int i = 0; i < handCard.size(); i++)
        {
            if (handCard[i]->getCardType() == type::blockade)
            {
                cardPresent = true;
                Deck::getInstance().addCard(handCard[i]);
                handCard.erase(handCard.begin() + i);
                break;
            }
        }
        if (cardPresent)
        {
            order = new BlockadeOrder(this, target);
        }
        else
        {
            cout << "The card to issue Blockade order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }
    }
    else if (type == "airlift")
    {
        for (int i = 0; i < handCard.size(); i++)
        {
            if (handCard[i]->getCardType() == type::airlift)
            {
                cardPresent = true;
                Deck::getInstance().addCard(handCard[i]);
                handCard.erase(handCard.begin() + i);
                break;
            }
        }
        if (cardPresent)
        {
            order = new AirliftOrder(this, target, armyUnits, source);
        }
        else
        {
            cout << "The card to issue Airlift order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }
    }
    else if (type == "negotiate")
    {
        for (int i = 0; i < handCard.size(); i++)
        {
            if (handCard[i]->getCardType() == type::diplomacy)
            {
                cardPresent = true;
                Deck::getInstance().addCard(handCard[i]);
                handCard.erase(handCard.begin() + i);
                break;
            }
        }
        if (cardPresent)
        {
            order = new NegotiateOrder(this, player);
        }
        else
        {
            cout << "The card to issue Negotiate order was not found in the player's hand. No order was added to the orders list." << endl;
            return;
        }
    }
    else
    {
        cout << "Specified order is not allowed. No order was added to the orders list." << endl;
        return;
    }

    ordersList->addOrder(order);
}

OrdersList *Player::getOrdersList() const
{
    return ordersList;
}

vector<Card *> Player::getHandCards() const
{
    return handCard;
}

vector<Territory *> Player::getTerritories() const
{
    return territory;
}

int Player::getReinforcementPool() const
{
    return reinforcementPool;
}

bool Player::getDrawCard() const
{
    return drawCard;
}

void Player::setPlayerName(std::string newName)
{
    this->playerName = std::move(newName);
}

void Player::setOrdersList(OrdersList *newOrdersList)
{
    //  Deallocate the current OrdersList object
    delete this->ordersList;

    this->ordersList = newOrdersList;
}

void Player::setHandCards(vector<Card *> newHandCards)
{
    //  Deallocate the current vector of Cards
    for (auto *cardPtr : handCard)
    {
        delete cardPtr;
    }

    this->handCard = std::move(newHandCards);
}

void Player::setTerritories(vector<Territory *> newTerritories)
{
    //  Deallocate the current vector of Territories
    for (auto *territoryPtr : territory)
    {
        delete territoryPtr;
    }

    this->territory = std::move(newTerritories);
}

void Player::addToReinforcementPool(int units)
{
    reinforcementPool += units;
}

void Player::setDrawCard(bool drawCard)
{
    this->drawCard = drawCard;
}
