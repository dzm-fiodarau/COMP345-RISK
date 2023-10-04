#include <iostream>
#include <vector>

#include "Cards.h"

void testCards();

int main() {
    testCards();
    return 0;

}
void testCards()
{
    //Create an empty deck
    Deck* deck = &Deck::getInstance();
    //Create an empty card hand
    std::vector<Card*> hand;

    //Add some cards in the deck
    for (size_t i = 0; i < 3; i++)
    {
        deck->addCard(new Card(type::blockade));
        deck->addCard(new Card(type::bomb));
        deck->addCard(new Card(type::airlif));
        deck->addCard(new Card(type::diplomacy));
        deck->addCard(new Card(type::reinforcement));
    }
    //Print the deck content
    std::cout << *deck << std::endl;

    //Draw one card in the deck
    Card* card = deck->draw();
    //Print the card
    std::cout << *card << std::endl;
    // Add it to the hand
    hand.push_back(card);
    //Print the hand and make sure it is the same card drawn
    std::cout << hand << std::endl;
    //Print the deck content to make sure it has one less card
    std::cout << *deck << std::endl;
    
    //Draw some cards from the deck and add them to the hand
    hand.push_back(deck->draw());
    hand.push_back(deck->draw());
    hand.push_back(deck->draw());
    //Print the cards in the hand
    std::cout << hand << std::endl;
}
