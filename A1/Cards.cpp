#include <iostream>
#include <vector> 
#include <random>

#include "Cards.h"

using namespace std;

Card::Card()
{
    //TODO: Think about creating an no-data/empty card type.
}

Card::Card(type type)
{
    cardType = type;
}

void Card::play()
{
    //TODO: Implement method with the orders.
}

type Card::getCardType() const
{
    return cardType;
}

Card::~Card()
{
}

bool Card::operator==(const Card& otherCard) const
{
    if (this->cardType != otherCard.cardType) {
        return false;
    }

    return true;
}

Card& Card::operator=(const Card& otherCard)
{
    if (this != &otherCard) {
        this->cardType = otherCard.cardType;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << "Card of type: ";
    switch (card.getCardType()) {
    case type::bomb:
        os << "bomb";
        break;
    case type::reinforcement:
        os << "reinforcement";
        break;
    case type::blockade:
        os << "blockade";
        break;
    case type::airlif:
        os << "airlif";
        break;
    case type::diplomacy:
        os << "diplomacy";
        break;
    default:
        os << "invalid";
        break;
    }

    os << std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const Deck& deck)
{
    os << "Current deck content:" << std::endl;

    for (Card* card : deck.cardList)
    {
        os << *card;
    }

    return os;
}

Deck& Deck::getInstance() {
    static Deck instance;
    return instance;
}

//TODO: Determine what are the default cards in the starting deck
Deck::Deck() {
}

Card* Deck::draw()
{
    std::random_device r;
    std::mt19937 rng(r());
    std::uniform_int_distribution<int> uniform_dist(0, cardList.size() - 1);
    
    int randNum = uniform_dist(rng);

    Card* cardSelected = cardList[randNum];

    cardList.erase(cardList.begin() + randNum);
    return cardSelected;
}

void Deck::addCard(Card* card)
{
    cardList.push_back(card);
}

Deck::~Deck()
{
}
