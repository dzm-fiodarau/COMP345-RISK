#ifndef PLAYER_H // Check whether there is Player.h or not
#define PLAYER_H // If no, then create one

#include <iostream>
#include <vector>
#include <string>

//  Forward declaration of required classes from other header files. (included in .cpp file)
class OrdersList;   //  Orders.h
class Card;         //  Cards.h
class Territory;    //  Map.h



using namespace std;



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
     * \param   target Territory to which troops are to be moved
     * \param   armyUnits Number of army units to be moved
     * \param   source  Source of territory
     * \param   player  Player name
     */
    void issueOrder(const string& type, Territory *target, int armyUnits, Territory *source, Player *player);
    OrdersList *getOrdersList();

    // Operator Overloads
    Player &operator=(const Player &other);
    friend ostream &operator<<(ostream &out, const Player &player);
};

#endif // PLAYER_H
