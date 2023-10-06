#include "../headers/Player.h"

void testPlayers()
{
    //      PHASE 1: SETUP
    // Create cards for players
    Card* card1 = new Card(type::bomb);
    Card* card2 = new Card(type::blockade);

    // Create player objects
    vector<Territory*> territories1;
    vector<Card*> handCards1 = {card1};
    Player* player1 = new Player("Player1", territories1, handCards1);

    vector<Territory*> territories2;
    vector<Card*> handCards2 = {card2};
    Player* player2 = new Player("Player2", territories2, handCards2);

    // Creating continents
    Continent* continent1 = new Continent("Africa", 0);
    Continent* continent2 = new Continent("Europe", 0);

    // Create territories for players
    Territory* territory1 = new Territory("Territory1", 5, 3, continent1, player1, 0);
    Territory* territory2 = new Territory("Territory2", 4, 2, continent2, player2, 0);

    // Add territories the player's 'owned territories' list
    player1->territory.push_back(territory1);
    player2->territory.push_back(territory2);



    //      PHASE 2: "PLAYING" THE GAME
    // Call toAttack() and toDefend() methods for player1 and player2
    cout << "Player 1 territories to attack: ";
    player1->toAttack();
    cout << "Player 1 territories to defend: ";
    player1->toDefend();

    cout << "Player 2 territories to attack: ";
    player2->toAttack();
    cout << "Player 2 territories to defend: ";
    player2->toDefend();

    // Issue orders for player1 and player2
    player1->issueOrder("deploy", territory1, 3, nullptr, nullptr);
    player1->issueOrder("advance", territory1, 2, territory2, nullptr);

    player2->issueOrder("deploy", territory2, 2, nullptr, nullptr);
    player2->issueOrder("advance", territory2, 1, territory1, nullptr);

    // Print orders for player1 and player2
    cout << "Player 1 orders: ";
    player1->printOrder();
    cout << "Player 2 orders: ";
    player2->printOrder();
}
