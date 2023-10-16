#include <utility>

#include "../headers/Player.h"
#include "../headers/Orders.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

// Default constructor
Player::Player()
{
	playerName = "";
	ordersList = new OrdersList(this);
}

// Constructor
Player::Player(string pn, vector<Territory *> t, vector<Card *> hd)
{
	playerName = std::move(pn);
	territory = std::move(t);
	handCard = std::move(hd);
	ordersList = new OrdersList(this);
}

// Copy constructor
Player::Player(const Player &p)
{
	playerName = p.playerName;
	territory = p.territory;
	handCard = p.handCard;
	ordersList = p.ordersList;
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
        delete ordersList; // Free the existing memory
        ordersList = new OrdersList(*other.ordersList); // Creating a new OrdersList with copied contents
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

    for (Territory* terr : territory)
    {
        cout << "Attacking " << terr->getName() << endl;
    }
}

// Method toDefend of Player
void Player::toDefend()
{
    cout << playerName << " is selecting territories to defend:" << endl;

    for (Territory* terr : territory)
    {
		cout << "Defending " << terr->getName() << endl;
    }
}

// Methods below are used to get issueOrder
void Player::issueOrder(const string& type, Territory *target, int armyUnits, Territory *source, Player *player)
{
	Order *order;
	if (type == "deploy")
	{
		order = new DeployOrder(this, target, armyUnits);
	}
	else if (type == "advance")
	{
		order = new AdvanceOrder(this, target, armyUnits, source);
	}
	else if (type == "bomb")
	{
		order = new BombOrder(this, target);
	}
	else if (type == "blockade")
	{
		order = new BlockadeOrder(this, target);
	}
	else if (type == "airlift")
	{
		order = new AirliftOrder(this, target, armyUnits, source);
	}
	else if (type == "negotiate")
	{
		order = new NegotiateOrder(this, player);
	}
	else
	{
        //  TODO Issue some message that the order issued is invalid, either try again or notify nothing is added to queue
        //  No order is issued if order is invalid
		//  order = new Order(ordersList, type, target);
        return;
	}

	ordersList->addOrder(order);
}

OrdersList *Player::getOrdersList()
{
	return ordersList;
}
