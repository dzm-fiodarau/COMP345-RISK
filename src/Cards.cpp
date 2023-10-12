#include <iostream>
#include <vector>
#include <random>

#include "../headers/Cards.h"
#include "../headers/Player.h"
#include "../headers/Map.h"


using namespace std;


/** \brief Convert the type enum to a string */
static std::string convertTypeToString(type type) {
    switch (type)
    {
        //  No need for breaks since return exits the function anyway
    case type::bomb:
       return "bomb";
    case type::reinforcement:
        return "reinforcement";
    case type::blockade:
        return "blockade";
    case type::airlift:
        return "airlift";
    case type::diplomacy:
        return "diplomacy";
    default:
        return "invalid";
    }
}

Card::Card() = default;

Card::Card(type type)
{
    cardType = type;
}

void Card::play(Player* player, Deck* deck, Territory* target, int armyUnits, Territory* source)
{
    player->issueOrder(convertTypeToString(this->getCardType()), target, armyUnits, source, player);

    deck->addCard(this);
}

type Card::getCardType() const
{
    return cardType;
}

Card::~Card()
{
}

bool Card::operator==(const Card &otherCard) const
{
    if (this->cardType != otherCard.cardType)
    {
        return false;
    }

    return true;
}

Card &Card::operator=(const Card &otherCard)
{
    if (this != &otherCard)
    {
        this->cardType = otherCard.cardType;
    }

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << "Card of type: ";
    os << convertTypeToString(card.getCardType()) << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Card *> multipleCards)
{
    os << "Current hand of size " << multipleCards.size() << " with content of:" << std::endl;

    for (Card *card : multipleCards)
    {
        os << convertTypeToString(card->getCardType()) << std::endl;;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const Deck &deck)
{
    os << "Current deck of size " << deck.cardList.size() << " with content of:" << std::endl;

    for (Card *card : deck.cardList)
    {
        os << *card;
    }

    return os;
}

Deck &Deck::getInstance()
{
    static Deck instance;
    return instance;
}

Deck::Deck()
{
    for (size_t i = 0; i < 3; i++)
    {
        addCard(new Card(type::blockade));
        addCard(new Card(type::bomb));
        addCard(new Card(type::airlift));
        addCard(new Card(type::diplomacy));
        addCard(new Card(type::reinforcement));
    }
}

Card *Deck::draw()
{
    std::random_device r;
    std::mt19937 rng(r());
    // The number generated will be between 0 and the length of the deck size.
    std::uniform_int_distribution<int> uniform_dist(0, cardList.size() - 1);

    int randNum = uniform_dist(rng);

    // The card that got randomly chosen
    Card *cardSelected = cardList[randNum];

    // Delete it from the deck
    cardList.erase(cardList.begin() + randNum);

    // Return the card
    return cardSelected;
}

void Deck::addCard(Card *card)
{
    cardList.push_back(card);
}

int Deck::getDeckSize() const
{
    return cardList.size();
}

Deck::~Deck()
{
    for (Card *card : cardList)
    {
        delete card;
    }
}
