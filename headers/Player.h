#ifndef PLAYER_H    //check whether there is Player.h or not
#define PLAYER_H    //if no, then create one

#include "Orders.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

//player object
class Player {
private:
    string playerName;
    vector<string*> territory;
    vector<string*> handCard;
    vector<Order*> orderList;

public:
    //constructors & destructor
    Player();
    Player(string pn, vector<string*> t, vector<string*> hc, vector<Order*> ol);
    Player(const Player& p);
    ~Player();

    //some member functions
    void toAttack();
    void toDefend();
    void issueOrder(string io);
    vector<Order*> getOrderList();
    void printOrder();

    //  CHANGED 10/4/2023
    //  Added function since the class 'Map' is dependent on being able to access the player name.
    string getPlayerName() const;
};

#endif //PLAYER_H
