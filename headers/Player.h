#ifndef PLAYER_H // Check whether there is Player.h or not
#define PLAYER_H // If no, then create one

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

/**
 * \class   Player
 * \brief   Class that contains player information
 */
class Player
{
private:
    string playerName;
    OrdersList *ordersList;

public:
    vector<Card *> handCard;
    vector<Territory *> territory;

    // Default constructor
    Player();
    // Constructor
    Player(string pn, vector<Territory *> t, vector<Card *> hc);
    // Copy constructor
    Player(const Player &p);
    // Destructor
    ~Player();

    // Some member functions
    string getPlayerName() const;
    void toAttack();
    void toDefend();
    /**
     * \brief   Get issueOrder
     * \param   type Type of the Order object created
     * \param   target Territory to which troups are to be moved
     * \param   armyUnits Number of army units to be moved
     * \param   source  Source of territory
     * \param   player  Player name
     */
    void issueOrder(string type, Territory *target, int armyUnits, Territory *source, Player *player);
    OrdersList *getOrdersList();

    // Operator Overloads
    Player &operator=(const Player &other);
    friend ostream &operator<<(ostream &out, const Player &player);
};

#endif // PLAYER_H
