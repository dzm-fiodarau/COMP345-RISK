#ifndef PLAYER_H    //check whether there is Player.h or not
#define PLAYER_H    //if no, then create one

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//order object
class Order {
private:
    string result;

public:
    Order(string r) : result(r) {}

    string getResult() const {
        return result;
    }
};

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
};

#endif //PLAYER_H
