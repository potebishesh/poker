#ifndef card_h
#define card_h

#include <iostream>
#include <string>



using namespace std;

typedef enum {D,H,C,S} SUIT;
class Card
{
public:
    int value;
    SUIT suit;

    Card();
    Card(int value, SUIT suit);
    int compareCard(Card secondCard);
    string generateCardName();

    int getValue();

    SUIT getSuit();


};

#endif
