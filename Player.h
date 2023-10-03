#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Order {
private:
    string result;

public:
    Order(string r) : result(r) {}

    string getResult() const {
        return result;
    }
};

class Player {
private:
    string name;
    vector<string*> territory;
    vector<string*> handCard;
    vector<Order*> orderList;

public:
    // Constructors and Destructor
    Player();
    Player(string n, vector<string*> t, vector<string*> h, vector<Order*> o);
    Player(const Player& p);
    ~Player();

    // Member Functions
    void toAttack();
    void toDefend();
    void issueOrder(string o);
    vector<Order*> getOrderList();
    void printOrder();
};

#endif // PLAYER_H