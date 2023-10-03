#include "Player.h"

using namespace std;

void testPlayers() {
    //test players
    vector<string*> territories = {new string("Territory1"), new string("Territory2")};
    vector<string*> handCards = {new string("Card1"), new string("Card2")};
    vector<Order*> orders = {new Order("Order1"), new Order("Order2")};

    Player player1("Player1", territories, handCards, orders);
    Player player2;

    player1.toAttack();
    player1.toDefend();
    player1.issueOrder("New Order");
    player1.printOrder();

    player2.toAttack();
    player2.toDefend();
    player2.issueOrder("Another Order");
    player2.printOrder();
}

int main() {
    testPlayers();
    return 0;
}
