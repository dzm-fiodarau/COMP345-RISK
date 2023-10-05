#include "../headers/Player.h"

int index; // used for for-loop

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

// TO DO by Ziyi Tu: Reimplement method
// destructor
Player::~Player()
{
	playerName.clear();
	// territory.clear();
	// handCard.clear();
	//  TO DO: call destructor of ordersList object
}

// TO DO by Ziyi Tu: Reimplement method
// method toAttack of Player
void Player::toAttack()
{
	// for (index = 0; index < territory.size(); index++)
	// {
	// 	cout << *territory[index] << " ";
	// }
	// cout << endl;
}

// TO DO by Ziyi Tu: Reimplement method
// method toDefend of Player
void Player::toDefend()
{
	// for (index = 0; index < territory.size(); index++)
	// {
	// 	cout << *territory[index] << " ";
	// }
	// cout << endl;
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

// TO DO by Ziyi Tu: Reimplement method
void Player::printOrder()
{
	// vector<Order *>::iterator it = orderList.begin();
	// for (; it != orderList.end(); it++)
	// {
	// 	//  CHANGED 10/4/2023
	// 	//  cout << (*it)->getResult() << " ";
	// 	cout << (*it)->execute() << " ";
	// }
	// cout << endl;
}

string Player::getPlayerName() const
{
	return playerName;
}
