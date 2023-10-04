#include "../headers/Player.h"

int index; // used for for-loop

// default constructor
Player::Player()
{
	string playerName = "";
	vector<string *> terriorty;
	vector<string *> handCard;
	vector<Order *> orderList;
}

// constructor
Player::Player(string pn, vector<string *> t, vector<string *> hd, vector<Order *> ol)
{
	this->playerName = pn;
	this->territory = t;
	this->handCard = hd;
	this->orderList = ol;
}

// copy constructor
Player::Player(const Player &p)
{
	this->playerName = p.playerName;
	this->territory = p.territory;
	this->handCard = p.handCard;
	this->orderList = p.orderList;
}

// destructor
Player::~Player()
{
	playerName.clear();
	territory.clear();
	handCard.clear();
	for (auto ol : orderList)
	{
		delete ol; // delete each element of orderList
	}
	orderList.clear();
}

// method toAttack of Player
void Player::toAttack()
{
	for (index = 0; index < territory.size(); index++)
	{
		cout << *territory[index] << " ";
	}
	cout << endl;
}

// method toDefend of Player
void Player::toDefend()
{
	for (index = 0; index < territory.size(); index++)
	{
		cout << *territory[index] << " ";
	}
	cout << endl;
}

// methods below are used to get issueOrder
void Player::issueOrder(string io)
{
	Order *o = new Order(io);
	orderList.push_back(o);
}

vector<Order *> Player::getOrderList()
{
	return orderList;
}

void Player::printOrder()
{
	vector<Order *>::iterator it = orderList.begin();
	for (; it != orderList.end(); it++)
	{
		cout << (*it)->getResult() << " ";
	}
	cout << endl;
}
