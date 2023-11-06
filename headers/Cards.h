#ifndef CARDS_H
#define CARDS_H

#pragma once

#include <string>
#include <vector>



//  Forward declaration of required classes from other header files. (included in .cpp file)
class Player;           //  Player.h
class Deck;             //  Map.h
class Territory;        //  Map.h



// Enum of the different card types
enum class type
{
    bomb,
    reinforcement,
    blockade,
    airlift,
    diplomacy
};

/**
 * \class   Card
 * \brief   Class representing the card object. Each card has one of five possible type.
 */
class Card
{
public:
    /**
     * \brief   Empty constructor
     */
    Card();

    /**
     * \brief               Create a card of a specific type.
     * \param stateName     The type of the card.
     */
    explicit Card(type type);

    /**
     * \brief               Queue the effect of the card by creating an order to the player order list.
     */
    void play(Player*, Deck*, Territory*, int, Territory*);

    /**
     * \brief               Use to get the card type.
     * \return              Returns the card type.
     */
    type getCardType() const;

    /**
     * \brief   Deconstructs the Card object.
     */
    ~Card();

    /**
     * \brief               Compares if the two Cards have the same card type.
     * \param otherState    The other Card object to compare.
     * \return              True if the two Card objects are considered equal. False otherwise.
     */
    bool operator==(const Card &otherCard) const;

    /**
     * \brief               Assigns the card type of the passed Card object into the current card object.
     * \param otherState    The other Card object to copy the type from.
     * \return              Returns instance of current object with values changed.
     */
    Card &operator=(const Card &otherCard);

    /**
     * /brief               Prints the name of the card type.
     */
    friend std::ostream &operator<<(std::ostream &os, const Card &card);

    /**
     * /brief               Prints the name of a list of cards.
     */
    friend std::ostream &operator<<(std::ostream &os, std::vector<Card *> multipleCards);

private:
    // The card type
    type cardType;
};

class Deck
{
public:
    static Deck &getInstance();

    /**
     * \brief   Empty constructor
     */
    Deck();

    /**
     * \brief
     * \return
     */
    Card *draw() const;

    /**
     * \brief               Add a Card to the deck.
     * \param otherState    The Card to be added.
     */
    void addCard(Card *card);

    /**
     * \brief               Returns the number of cards currently in the deck.
     * \return              The number of cards in the deck.
     */
    int getDeckSize() const;

    /**
     * \brief   Deconstructs the Deck object.
     */
    ~Deck();

    /**
     * /brief               Prints the whole deck content
     */
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

    //  Deleted members
    Deck(const Deck &other) = delete;
    Deck &operator=(const Deck &other) = delete;
    Deck &operator=(const Deck &other) const = delete;

private:
    // The deck list of cards
    std::vector<Card *> cardList;
};

#endif // CARDS_H
