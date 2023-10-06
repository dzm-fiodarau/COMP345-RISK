#include "../headers/Player.h"

// Default constructor
Player::Player()
{
	playerName = "";
	ordersList = new OrdersList(this);
}

// Constructor
Player::Player(string pn, vector<Territory *> t, vector<Card *> hd)
{
	playerName = pn;
	territory = t;
	handCard = hd;
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

// Method the get palyer name
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
void Player::issueOrder(string type, Territory *target, int armyUnits, Territory *source, Player *player)
{
	Order *order;
	if (type.compare("deploy") == 0)
	{
		order = new DeployOrder(ordersList, target, armyUnits);
	}
	else if (type.compare("advance") == 0)
	{
		order = new AdvanceOrder(ordersList, target, armyUnits, source);
	}
	else if (type.compare("bomb") == 0)
	{
		order = new BombOrder(ordersList, target);
	}
	else if (type.compare("blockade") == 0)
	{
		order = new BlockadeOrder(ordersList, target);
	}
	else if (type.compare("airlift") == 0)
	{
		order = new AirliftOrder(ordersList, target, armyUnits, source);
	}
	else if (type.compare("negotiate") == 0)
	{
		order = new NegotiateOrder(ordersList, player);
	}
	else
	{
		order = new Order(ordersList, type, target);
	}

	(*ordersList).orders.push_back(order);
}

OrdersList *Player::getOrdersList()
{
	return ordersList;
}
