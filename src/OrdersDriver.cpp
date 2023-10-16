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

    player1->issueOrder("deploy", france, 2, NULL, nullptr);
    player1->issueOrder("advance", belgium, 4, france, nullptr);
    player1->issueOrder("bomb", england, 0, NULL, nullptr);
    player1->issueOrder("blockade", france, 0, NULL, nullptr);
    player1->issueOrder("airlift", england, 3, france, nullptr);
    player1->issueOrder("negotiate", NULL, 0, NULL, nullptr);

    player2->issueOrder("deploy", france, 0, NULL, nullptr);
    player2->issueOrder("invalid type", NULL, 0, NULL, nullptr);
    player2->issueOrder("advance", belgium, 4, france, nullptr);
    player2->issueOrder("bomb", england, 0, NULL, nullptr);
    player2->issueOrder("blockade", france, 0, NULL, nullptr);
    player2->issueOrder("airlift", england, 3, france, nullptr);
    player2->issueOrder("negotiate", NULL, 0, NULL, nullptr);
    cout << "ORDERS PART\n-------------------------------------------------------------\n";


    OrdersList* player1_ordersList = player1->getOrdersList();
    OrdersList* player2_ordersList = player2->getOrdersList();


    // Verify OrdersList.move()
    cout << *(player2_ordersList) << endl;
    player2_ordersList->move((*player2_ordersList)[0], 4);   // Moves first element to index 4 of the list
    cout << *(player2_ordersList) << endl;
    player2_ordersList->move((*player2_ordersList)[0], -2);  //  Invalid case
    player2_ordersList->move((*player1_ordersList)[0], 4);   //  Invalid case
    cout << "\n";


    // Verify OrdersList.remove()
    player2_ordersList->remove((*player2_ordersList)[0]);   // Removes first element of the list
    cout << *(player2->getOrdersList()) << endl;
    player2_ordersList->remove((*player1_ordersList)[0]);   // Invalid case
    cout << "\n";


    //  Anonymous function that takes an order and calls its 'execute()' function
    auto executeOrderFunction = [] (Order* order) { order->execute(); return; };

    // Verify validate() and execute() of different Order types
    // Calling execute() suffices because each execute uses validate() in its implementation
    cout << "Valid Orders:\n";
    player1->getOrdersList()->apply(executeOrderFunction);

    cout << "\nInvalid Orders:\n";
    player2->getOrdersList()->apply(executeOrderFunction);

    cout << "\n-------------------------------------------------------------\n\n";
}