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

//  README
//  CHANGED 10/4/2023
//  Changed so that the project can compile. Issue: the placeholder classes were interfering with the linking during
//  compile time. Therefore, project could not compile.

//  CHANGES
//  - Added include directive: #include "Orders.h"
//  - Removed the placeholder class and corresponding methods in the .cpp file
//  - Added and implemented the method "string getPlayerName() const" since the "Map" class in "Map.h" is dependent
//    on being able to get the name of the player
//  - Adjusted method call in the .cpp file to match the actual method in the Order class
//          from '(*it)->getResult()' to '(*it)->execute()'

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

    //  CHANGED 10/4/2023
    //  Added function since the class 'Map' is dependent on being able to access the player name.
    string getPlayerName() const;
};

#endif // PLAYER_H
