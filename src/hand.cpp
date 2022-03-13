#include <iostream>
#include <vector>
#include <string>

#include "hand.hpp"

hand::hand()
{

}

void hand::sequenceHand()
{
    // This methods puts hand in sequence based on card value, assumes a hand of cards has five cards.
    Card temp;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < (5-i-1) ; j++)
        {
            if(handOfCards.at(j).value > handOfCards.at(j+1).value)
            {
                temp = handOfCards.at(j);
                handOfCards.at(j) = handOfCards.at(j+1);
                handOfCards.at(j+1) = temp;
            }
        }
    }
}


// All of the methods below this assumes hand is put in sequence already.
bool hand::isPair()
{
    // This method checks if a hand has a pair, returns true if it has a pair.
    if(handOfCards.at(0).getValue() == handOfCards.at(1).getValue() || handOfCards.at(1).getValue()==handOfCards.at(2).getValue() || handOfCards.at(2).getValue()==handOfCards.at(3).getValue() ||
            handOfCards.at(3).getValue()==handOfCards.at(4).getValue())
        return true;
    return false;
}

int hand::pairVal()
{
    // This method returns the value of the card that has a pair.
    if(handOfCards.at(0).getValue() == handOfCards.at(1).getValue())
        return handOfCards.at(0).getValue();
    if(handOfCards.at(1).getValue() == handOfCards.at(2).getValue())
        return handOfCards.at(1).getValue();
    if(handOfCards.at(2).getValue() == handOfCards.at(3).getValue())
        return handOfCards.at(2).getValue();
    if(handOfCards.at(3).getValue() == handOfCards.at(4).getValue())
        return handOfCards.at(3).getValue();
    return 0;
}

bool hand::isTwoPair()
{
    // This method checks if a hand has two pairs, returns true of it has two pairs.
    if(handOfCards.at(0).getValue()==handOfCards.at(1).getValue() && handOfCards.at(2).getValue()==handOfCards.at(3).getValue())
        return true;
    if(handOfCards.at(1).getValue()==handOfCards.at(2).getValue() && handOfCards.at(3).getValue()==handOfCards.at(4).getValue())
        return true;
    if (handOfCards.at(0).getValue()==handOfCards.at(1).getValue() && handOfCards.at(3).getValue()==handOfCards.at(4).getValue())
        return true;
    return false;
}

int hand::twoPairHigh()
{

    // This method returns the value of the card that has a pair (bigger pari of the two pairs).
    return handOfCards.at(3).getValue();

}
int hand::twoPairLow()
{
    // This method returns the value of the card that has a pair (smaller pair of the two pairs).
    return handOfCards.at(1).getValue();
}

bool hand::isThreeOfAKind()
{
    // This method checks if a hand has three of a kind, returns true if it has three of a kind.

    if(handOfCards.at(0).getValue()==handOfCards.at(1).getValue() && handOfCards.at(1).getValue()==handOfCards.at(2).getValue())
        return true;
    if(handOfCards.at(1).getValue()==handOfCards.at(2).getValue() && handOfCards.at(2).getValue()==handOfCards.at(3).getValue())
        return true;
    if(handOfCards.at(2).getValue()==handOfCards.at(3).getValue() && handOfCards.at(3).getValue()==handOfCards.at(4).getValue())
        return true;
    return false;
}

int hand::threeOfAKindVal()
{
    // This method returns the card value of three of a kind.
    return handOfCards.at(2).getValue();
}

bool hand::isFlush()
{
    // This method checks if a hand is a flush, returns true if it is a flush.

    for (int count=0; count<4; count++)
    {
        if(handOfCards.at(count).getSuit()!=handOfCards.at(count+1).getSuit())
        {
            return false;
        }
    }
    return true;
}

bool hand::isStraight()
{
    // This method checks if a hand is a straight, returns true if it is a straight.
    for(int count=0; count<4; count++)
    {
        if((handOfCards.at(count).getValue()+1) != handOfCards.at(count+1).getValue())
        {
            return false;
        }
    }

    return true;
}

bool hand::isFullHouse()
{
    // This method checks if a hand is a full house, returns true if it is a full house.

    if(handOfCards.at(0).getValue()==handOfCards.at(1).getValue() && handOfCards.at(1).getValue()==handOfCards.at(2).getValue() && handOfCards.at(3).getValue()==handOfCards.at(4).getValue())
        return true;
    if(handOfCards.at(2).getValue()==handOfCards.at(3).getValue() && handOfCards.at(3).getValue()==handOfCards.at(4).getValue() && handOfCards.at(0).getValue()==handOfCards.at(1).getValue())
        return true;
    return false;
}

bool hand::isFourOfAKind()
{
    // This method checks if a hand has four of a kind, returns true if it has four of a kind.
    if(handOfCards.at(0).getValue()==handOfCards.at(1).getValue() && handOfCards.at(1).getValue()==handOfCards.at(2).getValue() && handOfCards.at(2).getValue()==handOfCards.at(3).getValue())
        return true;
    else if (handOfCards.at(1).getValue()==handOfCards.at(2).getValue() && handOfCards.at(2).getValue()==handOfCards.at(3).getValue() && handOfCards.at(3).getValue()==handOfCards.at(4).getValue())
        return true;
    else
        return false;
}

int hand::fourOfAKindVal()
{
    // This method return the card value of four of a kind.
    return handOfCards.at(1).getValue();
}

bool hand::isRoyalFlush()
{
    // This method checks if a hand is a royal flush, returns true if it is a royal flush.
    for (int count=0; count<4; count++)
    {
        if(handOfCards.at(count).getSuit()!=handOfCards.at(count+1).getSuit())
        {
            return false;
        }
    }


    if(handOfCards.at(0).getValue() == 1 && handOfCards.at(1).getValue() == 10 && handOfCards.at(2).getValue() == 11 && handOfCards.at(3).getValue() == 12 && handOfCards.at(4).getValue() == 13)
    {
        return true;
    }

    return false;

}

