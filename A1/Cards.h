#pragma once

#include <string>
#include <vector> 

// Enum of the different card types
enum class type { bomb, reinforcement, blockade, airlif, diplomacy };

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
    Card(type type);

    /**
     * \brief               Queue the effect of the card by creating an order to the player order list.
     */
    void play();

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
    bool operator==(const Card& otherCard) const;

    /**
     * \brief               Assigns the card type of the passed Card object into the current card object.
     * \param otherState    The other Card object to copy the type from.
     * \return              Returns instance of current object with values changed.
     */
    Card& operator=(const Card& otherCard);

    /**
     * /brief               Prints the name of the card type.
     */
    friend std::ostream& operator<<(std::ostream& os, const Card& card);

private:
    // The card type
    type cardType;
};

class Deck
{
public:
    static Deck& getInstance();

    Deck();

    Card* draw();

    void addCard(Card* card);

    ~Deck();

    /**
     * /brief               Prints the whole deck content
     */
    friend std::ostream& operator<<(std::ostream& os, const Deck& deck);

private:

    std::vector<Card*> cardList;
};
