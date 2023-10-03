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
    Deck* deck = &Deck::getInstance();

    //Add some cards in the deck
    for (size_t i = 0; i < 3; i++)
    {
        deck->addCard(new Card(type::blockade));
        deck->addCard(new Card(type::bomb));
        deck->addCard(new Card(type::airlif));
        deck->addCard(new Card(type::diplomacy));
        deck->addCard(new Card(type::reinforcement));
    }

    std::cout << *deck << std::endl;

    std::vector<Card*> hand;  

    Card* card = deck->draw();

    hand.push_back(card);
    deck->draw();
    deck->draw();

    std::cout << *card << std::endl;
}
