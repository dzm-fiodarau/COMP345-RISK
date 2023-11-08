#include <iostream>
#include <vector>

#include "../headers/Cards.h"
#include "../headers/Map.h"
#include "../headers/player/Player.h"

void testCards()
{
    std::cout << "-------------------------------------------" << std::endl;

    Continent* europe = new Continent("Europe", 100);
    Player* player1 = new Player();
    Territory* france = new Territory("France", 5, 5, europe, player1, 5);

    // Create a deck filled with some cards
    Deck *deck = &Deck::getInstance();
    // Create an empty card hand
    std::vector<Card *> hand;
 
    // Print the initial deck content
    std::cout << *deck << std::endl;

    // Draw one card in the deck
    Card *card = deck->draw();
    std::cout << "Card drawn from deck: " << std::endl;
    std::cout << *card << std::endl;

    // Add it to the hand
    hand.push_back(card);
    // Print the hand and make sure it is the same card drawn
    std::cout << hand << std::endl;

    // Draw some cards from the deck and add them to the hand
    hand.push_back(deck->draw());
    hand.push_back(deck->draw());
    // Print the cards in the hand
    std::cout << hand << std::endl;

    //Play The first card of hand, which creates an order
    // 'armyUnits' & 'source' are placeholder values below:
    hand[1]->play(player1, deck, france, -1, nullptr);

    // Print final deck content
    std::cout << *deck << std::endl;
}
