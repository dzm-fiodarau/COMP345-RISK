#include "../headers/Player.h"

// default constructor
Player::Player()
{
	playerName = "";
	ordersList = new OrdersList(this);
}

// constructor
Player::Player(string pn, vector<Territory *> t, vector<Card *> hd)
{
	playerName = pn;
	territory = t;
	handCard = hd;
	ordersList = new OrdersList(this);
}

// copy constructor
Player::Player(const Player &p)
{
	playerName = p.playerName;
	territory = p.territory;
	handCard = p.handCard;
	ordersList = p.ordersList;
}

// destructor
Player::~Player()
{
	playerName.clear();
	delete ordersList;
}

// method toAttack of Player
void Player::toAttack()
{
    cout << playerName << " is selecting territories to attack:" << endl;

    for (Territory* terr : territory)
    {
        cout << "Attacking " << terr->getName() << endl;
    }
}

// method toDefend of Player
void Player::toDefend()
{
    cout << playerName << " is selecting territories to defend:" << endl;

    for (Territory* terr : territory)
    {
		cout << "Defending " << terr->getName() << endl;
    }
}

// methods below are used to get issueOrder
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

void Player::printOrder()
{
    cout << "Orders List for " << playerName << ":" << endl;

    // Output all orders of player
    for (Order* order : ordersList->orders) 
    {
        cout << *order << endl; // Assume that the Order class implements the appropriate output operator overloads
    }
}

string Player::getPlayerName() const
{
	return playerName;
}
