#include "../headers/Player.h"

void testPlayers()
{
    // Create territories for players
    Territory* territory1 = new Territory("Territory1", 5, 3, nullptr, nullptr, 0);
    Territory* territory2 = new Territory("Territory2", 4, 2, nullptr, nullptr, 0);

    // Create cards for players
    Card* card1 = new Card(type::bomb);
    Card* card2 = new Card(type::blockade);

    // Create player objects
    vector<Territory*> territories1 = {territory1};
    vector<Card*> handCards1 = {card1};
    Player player1("Player1", territories1, handCards1);

    vector<Territory*> territories2 = {territory2};
    vector<Card*> handCards2 = {card2};
    Player player2("Player2", territories2, handCards2);

    // Call toAttack() and toDefend() methods for player1 and player2
    cout << "Player 1 territories to attack: ";
    player1.toAttack();
    cout << "Player 1 territories to defend: ";
    player1.toDefend();

    cout << "Player 2 territories to attack: ";
    player2.toAttack();
    cout << "Player 2 territories to defend: ";
    player2.toDefend();

    // Issue orders for player1 and player2
    player1.issueOrder("deploy", territory1, 3, nullptr, nullptr);
    player1.issueOrder("advance", territory1, 2, territory2, nullptr);

    player2.issueOrder("deploy", territory2, 2, nullptr, nullptr);
    player2.issueOrder("advance", territory2, 1, territory1, nullptr);

    // Print orders for player1 and player2
    cout << "Player 1 orders: ";
    player1.printOrder();
    cout << "Player 2 orders: ";
    player2.printOrder();
}

int main()
{
    // Call the testPlayers function to demonstrate player features
    testPlayers();

    return 0;
}
