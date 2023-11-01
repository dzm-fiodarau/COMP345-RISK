#include "../headers/Orders.h"
#include "../headers/Player.h"
#include "../headers/Map.h"
#include "../headers/Cards.h"

/**
 * \brief Tests Order-related functionality specified for Assignment I
 */
void testOrdersLists()
{
     auto *player1 = new Player();
     auto *player2 = new Player();
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

     auto *europe = new Continent("Europe", 100);

     auto *france = new Territory("France", 5, 5, europe, player1, 5);
     auto *belgium = new Territory("Belgium", 6, 4, europe, player1, 5);
     auto *england = new Territory("England", 5, 3, europe, player2, 5);
     auto *greece = new Territory("Greece", 10, 12, europe, player1, 5);

     france->addAdjacentTerritory(belgium);
     france->addAdjacentTerritory(england);
     belgium->addAdjacentTerritory(france);
     belgium->addAdjacentTerritory(england);
     england->addAdjacentTerritory(france);
     england->addAdjacentTerritory(belgium);

     player1->issueOrder("deploy", france, 3, nullptr, nullptr);
     player1->issueOrder("advance", belgium, 4, france, nullptr);
     player1->issueOrder("bomb", england, 0, nullptr, nullptr);
     player1->issueOrder("blockade", belgium, 0, nullptr, nullptr);
     player1->issueOrder("airlift", greece, 3, france, nullptr);
     player1->issueOrder("negotiate", nullptr, 0, nullptr, player2);

     player2->issueOrder("deploy", france, 2, nullptr, nullptr);
     player2->issueOrder("advance", belgium, 4, france, nullptr);
     player2->issueOrder("bomb", greece, 0, nullptr, nullptr);
     player2->issueOrder("blockade", france, 0, nullptr, nullptr);
     player2->issueOrder("airlift", england, 2, france, nullptr);
     player2->issueOrder("negotiate", nullptr, 0, nullptr, player2);
     cout << "ORDERS PART\n-------------------------------------------------------------\n";

     OrdersList *player1_ordersList = player1->getOrdersList();
     OrdersList *player2_ordersList = player2->getOrdersList();

     // Verify OrdersList.move()
     cout << *(player2_ordersList) << endl;
     player2_ordersList->move((*player2_ordersList)[0], 4); // Moves first element to index 4 of the list
     cout << *(player2_ordersList) << endl;
     player2_ordersList->move((*player2_ordersList)[0], -2); //  Invalid case
     player2_ordersList->move((*player1_ordersList)[0], 4);  //  Invalid case
     cout << "\n";

     // Verify OrdersList.remove()
     player2_ordersList->remove((*player2_ordersList)[0]); // Removes first element of the list
     cout << *(player2->getOrdersList()) << endl;
     player2_ordersList->remove((*player1_ordersList)[0]); // Invalid case
     cout << "\n";

     //  Anonymous function that takes an order and calls its 'execute()' function
     auto executeOrderFunction = [](Order *order)
     { order->execute(); return; };

     // Verify validate() and execute() of different Order types
     // Calling execute() suffices because each execute uses validate() in its implementation
     cout << "Valid Orders:\n";
     player1->getOrdersList()->apply(executeOrderFunction);

     cout << "\nInvalid Orders:\n";
     player2->getOrdersList()->apply(executeOrderFunction);

     cout << "\n-------------------------------------------------------------\n\n";
}

/**
 * \brief Tests Order execution-related functionality specified for Assignment II
 */
void testOrderExecution()
{
     auto *player1 = new Player();
     auto *player2 = new Player();
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
     auto executeOrderFunction = [](Order *order)
     { cout<<order->execute()<<endl; return; };
     auto removeOrderFunction = [](Order *order)
     { order->execute(); return; };
     player2->getOrdersList()->apply(executeOrderFunction);

     cout << "\nOWNERSHIP OF TERRITORY TRANSFER:\n";
     while (player2->getOrdersList()->getNextOrder())
     {
     }
     england->setNumberOfArmies(100);
     player2->issueOrder("advance", france, 90, england, NULL);
     cout << "BEFORE:\n"
          << *player1 << *player2 << "Lucy's draw card at the end of turn indication: " << player2->getDrawCard() << endl
          << endl;
     cout << *england << endl
          << *france << endl
          << endl;
     player2->getOrdersList()->apply(executeOrderFunction);
     while (player2->getOrdersList()->getNextOrder())
     {
     }
     cout << "\nAFTER:\n"
          << *player1 << *player2 << "Lucy's draw card at the end of turn indication: " << player2->getDrawCard() << endl
          << endl;
     cout << *england << endl
          << *france << endl;

     cout << "\nBLOCKADE OWNERSHIP TRANSFER:\n";
     player1->issueOrder("blockade", greece, 0, NULL, NULL);
     cout << "BEFORE:\n"
          << *player1 << endl
          << *greece << endl
          << endl;
     player1->getOrdersList()->apply(executeOrderFunction);
     while (player1->getOrdersList()->getNextOrder())
     {
     }
     cout << "\nAFTER:\n"
          << *player1 << endl
          << *greece
          << endl;

     cout << "\nNEGOTIATE PREVENTS ATTACKS:\n";
     player1->issueOrder("negotiate", NULL, 0, NULL, player2);
     player2->issueOrder("advance", belgium, 80, france, NULL);
     cout << "BEFORE:\n"
          << *player1 << *player2
          << endl;
     cout << *belgium << endl
          << *france << endl
          << endl;
     player1->getOrdersList()->apply(executeOrderFunction);
     player2->getOrdersList()->apply(executeOrderFunction);
     while (player1->getOrdersList()->getNextOrder())
     {
     }
     while (player1->getOrdersList()->getNextOrder())
     {
     }
     cout << "\nAFTER:\n"
          << *player1 << *player2 << endl
          << endl;
     cout << *belgium << endl
          << *france << endl;
     cout << "\n-------------------------------------------------------------\n\n";
}