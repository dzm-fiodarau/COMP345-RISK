#ifndef PLAYER_H // check whether there is Player.h or not
#define PLAYER_H // if no, then create one

#include <iostream>
#include <vector>
#include <string>

#include "Orders.h"
#include "Map.h"
#include "Cards.h"

using namespace std;

class OrdersList;
class Player;
class Territory;

// player object
class Player
{
private:
    string playerName;
    OrdersList *ordersList;

public:
    vector<Card *> handCard;
    vector<Territory *> territory;

    // constructors & destructor
    Player();
    Player(string pn, vector<Territory *> t, vector<Card *> hc);
    Player(const Player &p);
    ~Player();

    // some member functions
    void toAttack();
    void toDefend();
    void issueOrder(string type, Territory *target, int armyUnits, Territory *source, Player *player);
    OrdersList *getOrdersList();
    void printOrder();
    string getPlayerName() const;
};

#endif // PLAYER_H
