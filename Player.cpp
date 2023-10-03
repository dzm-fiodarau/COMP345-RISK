#include "Player.h"

using namespace std;
int index;	//used for for-loop

//default constructor
Player::Player()
{
	string name;
	vector<string*> terriorty;
	vector<string*> handCard;
	vector<Order*> orderList;
}

//constructor
Player::Player(string n, vector<string*> t, vector<string*> hd,vector<Order*> ol)
{
	this->name = n;
	this->territory = t;
	this->handCard = hd;
	this->orderList = ol;
}

//copy constructor
Player::Player(const Player& p)
{
	this->name = p.name;
	this->territory = p.territory;
	this->handCard = p.handCard;
	this->orderList = p.orderList;
}

//destructor
Player::~Player()
{
	name.clear();
	territory.clear();
	handCard.clear();
	for (auto ol: orderList)
	{
		delete ol;
	}
	orderList.clear();
}

void Player::toAttack()
{
	for (index=0; index < territory.size(); index++) 
	{
		cout << *territory[index] << " ";
	}
	cout << endl;
}

void Player::toDefend()
{
	for (index=0; index < territory.size(); index++)
	{
		cout << *territory[index] << " ";
	}
	cout << endl;
}

//get issueOrder
void Player::issueOrder(string io)
{
	Order *a = new Order(io);
	orderList.push_back(a);
}

vector<Order*> Player::getOrderList()
{
	return orderList;
}

void Player::printOrder()
{
	vector<Order*>::iterator it = orderList.begin();
	for (; it != orderList.end(); it++)
	{
		cout << (*it)->getResult() << " ";
	}
	cout << endl;
}