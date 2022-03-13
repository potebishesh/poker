#ifndef hand_h
#define hand_h

#include <iostream>
#include <string>
#include <vector>
#include "card.hpp"


using namespace std;

class hand
{
public:
    vector <Card> handOfCards;

    hand(); // initailly set all five cards to NULL, before dealer deals

    void sequenceHand();//puts the hand in sequence

    bool isPair();
    bool isTwoPair();
    bool isThreeOfAKind();
    bool isFlush();
    bool isStraight();
    bool isFullHouse();
    bool isFourOfAKind();
    bool isRoyalFlush();

    int pairVal();
    int twoPairHigh();
    int twoPairLow();
    int threeOfAKindVal();
    int fourOfAKindVal();
};

#endif
