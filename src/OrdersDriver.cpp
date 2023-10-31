#include "../headers/Orders.h"
#include "../headers/Player.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

/**
 * \brief Tests Order-related functionality specified for Assignment I
 */
void testOrdersLists()
{
    Player *player1 = new Player();
    Player *player2 = new Player();
    player1->addToReinforcementPool(3);
    player2->addToReinforcementPool(2);
    player1->handCard.push_back(new Card(type::blockade));
    player1->handCard.push_back(new Card(type::bomb));
    player1->handCard.push_back(new Card(type::airlift));
    player1->handCard.push_back(new Card(type::diplomacy));
    player2->handCard.push_back(new Card(type::blockade));
    player2->handCard.push_back(new Card(type::bomb));
    player2->handCard.push_back(new Card(type::airlift));
    player2->handCard.push_back(new Card(type::diplomacy));

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

    player1->issueOrder("deploy", france, 3, NULL, NULL);
    player1->issueOrder("advance", belgium, 4, france, NULL);
    player1->issueOrder("bomb", england, 0, NULL, NULL);
    player1->issueOrder("blockade", belgium, 0, NULL, NULL);
    player1->issueOrder("airlift", greece, 3, france, NULL);
    player1->issueOrder("negotiate", NULL, 0, NULL, player2);

    player2->issueOrder("deploy", france, 2, NULL, NULL);
    player2->issueOrder("advance", belgium, 4, france, NULL);
    player2->issueOrder("bomb", greece, 0, NULL, NULL);
    player2->issueOrder("blockade", france, 0, NULL, NULL);
    player2->issueOrder("airlift", england, 2, france, NULL);
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
    cout << "Valid Orders (Player 1):\n";
    list<Order *>::iterator itr;
    for (itr = player1->getOrdersList()->orders.begin(); itr != player1->getOrdersList()->orders.end(); ++itr)
    {
        (*itr)->execute();
    }
    cout << "\nInvalid Orders (Player 2):\n";
    for (itr = player2->getOrdersList()->orders.begin(); itr != player2->getOrdersList()->orders.end(); ++itr)
    {
        (*itr)->execute();
    }

    cout << "\n-------------------------------------------------------------\n\n";
}

/**
 * \brief Tests Order execution-related functionality specified for Assignment II
 */
void testOrderExecution()
{
    Player *player1 = new Player();
    Player *player2 = new Player();
    player1->addToReinforcementPool(3);
    player1->setPlayerName("Shadow");
    player2->addToReinforcementPool(2);
    player2->setPlayerName("Lucy");
    player1->handCard.push_back(new Card(type::blockade));
    player1->handCard.push_back(new Card(type::bomb));
    player1->handCard.push_back(new Card(type::airlift));
    player1->handCard.push_back(new Card(type::diplomacy));
    player2->handCard.push_back(new Card(type::blockade));
    player2->handCard.push_back(new Card(type::bomb));
    player2->handCard.push_back(new Card(type::bomb));
    player2->handCard.push_back(new Card(type::airlift));
    player2->handCard.push_back(new Card(type::diplomacy));

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

    player2->issueOrder("deploy", france, 2, NULL, NULL);     // Invalid Deploy
    player2->issueOrder("advance", england, 5, france, NULL); // Invalid Advance: not owner of source
    player2->issueOrder("advance", greece, 5, england, NULL); // Invalid Advance: target not adjacent to source
    player2->issueOrder("airlift", england, 5, france, NULL); // Invalid Airlift
    player2->issueOrder("bomb", england, 0, NULL, NULL);      // Invalid Bomb: owner of bombed territory
    player2->issueOrder("bomb", greece, 0, NULL, NULL);       // Invalid Bomb: not adjacent to any owned territories
    player2->issueOrder("blockade", france, 0, NULL, NULL);   // Invalid Blockade
    player2->issueOrder("negotiate", NULL, 0, NULL, player2); // Invalid Negotiate

    cout << "Test Order Execution\n-------------------------------------------------------------\n";
    cout << *player1 << endl;
    cout << *player2 << endl;

    cout << "\nTRYING TO EXECUTE INVALID ORDERS (all orders on Lucy):\n";
    list<Order *>::iterator itr;
    for (itr = player2->getOrdersList()->orders.begin(); itr != player2->getOrdersList()->orders.end(); ++itr)
    {
        (*itr)->execute();
    }

    cout << "\nOWNERSHIP OF TERRITORY TRANSFER:\n";
    player2->getOrdersList()->orders.clear();
    england->setNumberOfArmies(100);
    player2->issueOrder("advance", france, 90, england, NULL);
    cout << "BEFORE:\n"
         << *player1 << *player2 << "Lucy's draw card at the end of turn indication: " << player2->getDrawCard() << endl
         << endl;
    cout << *england << endl
         << *france << endl
         << endl;
    for (itr = player2->getOrdersList()->orders.begin(); itr != player2->getOrdersList()->orders.end(); ++itr)
    {
        cout << endl
             << (*itr)->execute() << endl;
    }
    player2->getOrdersList()->orders.clear();
    cout << "\nAFTER:\n"
         << *player1 << *player2 << "Lucy's draw card at the end of turn indication: " << player2->getDrawCard() << endl
         << endl;
    cout << *england << endl
         << *france << endl;

    cout << "\nBLOCKADE OWNERSHIP TRANSFER:\n";
    player1->issueOrder("blockade", greece, 0, NULL, NULL);
    cout << "BEFORE:\n"
         << *player1 << endl
         << *greece
         << endl;
    for (itr = player1->getOrdersList()->orders.begin(); itr != player1->getOrdersList()->orders.end(); ++itr)
    {
        cout << endl
             << (*itr)->execute() << endl;
    }
    player1->getOrdersList()->orders.clear();
    cout << "\nAFTER:\n"
         << *player1 << endl
         << *greece
         << endl;

    cout << "\nNEGOTIATE PREVENTS ATTACKS:\n";
    player1->issueOrder("negotiate", NULL, 0, NULL, player2);
    player2->issueOrder("advance", belgium, 80, france, NULL);
    cout << "BEFORE:\n"
         << *player1 << *player2 << endl
         << endl;
    cout << *belgium << endl
         << *france << endl;
    for (itr = player1->getOrdersList()->orders.begin(); itr != player1->getOrdersList()->orders.end(); ++itr)
    {
        cout << endl
             << (*itr)->execute() << endl;
    }
    for (itr = player2->getOrdersList()->orders.begin(); itr != player2->getOrdersList()->orders.end(); ++itr)
    {
        cout << endl
             << (*itr)->execute() << endl;
    }
    player1->getOrdersList()->orders.clear();
    player2->getOrdersList()->orders.clear();
    cout << "\nAFTER:\n"
         << *player1 << *player2 << endl
         << endl;
    cout << *belgium << endl
         << *france << endl;
    cout << "\n-------------------------------------------------------------\n\n";
}