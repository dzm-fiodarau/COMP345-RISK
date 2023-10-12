#include "../headers/Orders.h"
#include "../headers/Player.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

void testOrdersLists()
{
    Player *player1 = new Player();
    Player *player2 = new Player();
    player1->handCard.push_back(new Card(type::blockade));
    player1->handCard.push_back(new Card(type::bomb));
    player1->handCard.push_back(new Card(type::airlift));
    player1->handCard.push_back(new Card(type::diplomacy));

    Continent *europe = new Continent("Europe", 100);

    Territory *france = new Territory("France", 5, 5, europe, player1, 5);
    Territory *belgium = new Territory("Belgium", 6, 4, europe, player1, 5);
    Territory *england = new Territory("England", 5, 3, europe, player2, 5);
    Territory *greece = new Territory("Greece", 10, 12, europe, player1, 5);

    france->addAdjacentTerritory(belgium);
    france->addAdjacentTerritory(england);
    belgium->addAdjacentTerritory(france);
    belgium->addAdjacentTerritory(england);
    england->addAdjacentTerritory(france);
    england->addAdjacentTerritory(belgium);

    player1->issueOrder("deploy", france, 2, NULL, NULL);
    player1->issueOrder("advance", belgium, 4, france, NULL);
    player1->issueOrder("bomb", england, 0, NULL, NULL);
    player1->issueOrder("blockade", france, 0, NULL, NULL);
    player1->issueOrder("airlift", england, 3, france, NULL);
    player1->issueOrder("negotiate", NULL, 0, NULL, player2);

    player2->issueOrder("deploy", france, 0, NULL, NULL);
    player2->issueOrder("invalid type", NULL, 0, NULL, NULL);
    player2->issueOrder("advance", belgium, 4, france, NULL);
    player2->issueOrder("bomb", england, 0, NULL, NULL);
    player2->issueOrder("blockade", france, 0, NULL, NULL);
    player2->issueOrder("airlift", england, 3, france, NULL);
    player2->issueOrder("negotiate", NULL, 0, NULL, player2);
    cout << "ORDERS PART\n-------------------------------------------------------------\n";

    // Verify OrdersList.move()
    cout << *(player2->getOrdersList()) << endl;
    player2->getOrdersList()->move(*(player2->getOrdersList()->orders.begin()++), 4); // Moves first element to index 4 of the list
    cout << *(player2->getOrdersList()) << endl;
    player2->getOrdersList()->move(*(player2->getOrdersList()->orders.begin()++), -2); // Invalid case
    player2->getOrdersList()->move(*(player1->getOrdersList()->orders.begin()++), 4);  // Invalid case
    cout << "\n";

    // Verify OrdersList.remove()
    player2->getOrdersList()->remove(*(player2->getOrdersList()->orders.begin()++)); // Removes first element of the list
    cout << *(player2->getOrdersList()) << endl;
    player2->getOrdersList()->remove(*(player1->getOrdersList()->orders.begin()++)); // Invalid case
    cout << "\n";

    // Verify validate() and execute() of different Order types
    // Calling execute() suffices because each execute uses validate() in its implementation
    cout << "Valid Orders:\n";
    list<Order *>::iterator itr;
    for (itr = player1->getOrdersList()->orders.begin(); itr != player1->getOrdersList()->orders.end(); ++itr)
    {
        (*itr)->execute();
    }
    cout << "\nInvalid Orders:\n";
    for (itr = player2->getOrdersList()->orders.begin(); itr != player2->getOrdersList()->orders.end(); ++itr)
    {
        (*itr)->execute();
    }

    cout << "\n-------------------------------------------------------------\n\n";
}