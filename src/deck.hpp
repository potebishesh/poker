#ifndef deck_h
#define deck_h

#include <iostream>
#include <string>
#include <vector>
#include "card.hpp"

using namespace std;


class Deck {
public:
    vector <Card> deckOfCards;

    Deck(); // initialize deck of 52 cards

    void shuffle_deck(); // shuffles the vector of initially 52 cards using using pre-defined function random_shuffle()
    Card get_card(); //gets the card needs to be distributed or swapped during the game
    void reset_deck();

};


#endif
