#include <iostream>
#include <string>
#include <vector>
#include "game.hpp"



string getRank(hand H1) {
    // This function returns the rank of a hand as a string.
    if(H1.isRoyalFlush())
        return "a Royal Flush";
    else if(H1.isStraight() && H1.isFlush())
        return "a Straight Flush";
    else if(H1.isFourOfAKind())
        return "Four of a kind";
    else if(H1.isFullHouse())
        return "a Full House";
    else if(H1.isFlush())
        return "a Flush";
    else if(H1.isStraight())
        return "a Straight";
    else if(H1.isThreeOfAKind())
        return "three of a kind";
    else if(H1.isTwoPair())
        return "two pairs";
    else if(H1.isPair())
        return "a pair";
    else
        return "a high card";


}

int compareHand(hand H1, hand H2) {
    /* This function returns positive integer if hand H1 is better than H2,
    	returns negative if hand H2 is better than hand H1. It compares ranks
    	of two hands to determine better hand. If they are tied, determines better
     	hand by comparing card values.
    */
    int score1 = 0;
    int score2 = 0;


    if(H1.isRoyalFlush())
        score1 = 10;
    else if(H1.isStraight() && H1.isFlush())
        score1 = 9;
    else if(H1.isFourOfAKind())
        score1 = 8;
    else if(H1.isFullHouse())
        score1 = 7;
    else if(H1.isFlush())
        score1 = 6;
    else if(H1.isStraight())
        score1 = 5;
    else if(H1.isThreeOfAKind())
        score1 = 4;
    else if(H1.isTwoPair())
        score1 = 3;
    else if(H1.isPair())
        score1 = 2;
    else
        score1 = 1;

    if(H2.isRoyalFlush())
        score2 = 10;
    else if(H2.isStraight() && H1.isFlush())
        score2 = 9;
    else if(H2.isFourOfAKind())
        score2 = 8;
    else if(H2.isFullHouse())
        score2 = 7;
    else if(H2.isFlush())
        score2 = 6;
    else if(H2.isStraight())
        score2 = 5;
    else if(H2.isThreeOfAKind())
        score2 = 4;
    else if(H2.isTwoPair())
        score2 = 3;
    else if(H2.isPair())
        score2 = 2;
    else
        score2 = 1;

    if(score1 != score2)
        return score1 - score2;
    else
    {
        if(score1 == 8)
        {
            if(H1.fourOfAKindVal() != H2.fourOfAKindVal())
                return H1.fourOfAKindVal() - H2.fourOfAKindVal();
        }
        else if(score1 == 4)
        {
            if(H1.threeOfAKindVal() != H2.threeOfAKindVal())
                return H1.threeOfAKindVal() - H2.threeOfAKindVal();
        }
        else if(score1 == 3)
        {
            if(H1.twoPairHigh() != H2.twoPairHigh())
                return H1.twoPairHigh() - H2.twoPairHigh();
            else if(H1.twoPairLow() != H2.twoPairLow())
                return H1.twoPairLow() - H2.twoPairLow();
        }
        else if(score1 == 2)
        {
            if(H1.pairVal() != H2.pairVal())
                return H1.pairVal() - H2.pairVal();
        }
    }

    if(H1.handOfCards.at(4).getValue() != H2.handOfCards.at(4).getValue())
        return H1.handOfCards.at(4).getValue() - H2.handOfCards.at(4).getValue();

    if(H1.handOfCards.at(3).getValue() != H2.handOfCards.at(3).getValue())
        return H1.handOfCards.at(3).getValue() - H2.handOfCards.at(3).getValue();

    if(H1.handOfCards.at(2).getValue() != H2.handOfCards.at(2).getValue())
        return H1.handOfCards.at(2).getValue() - H2.handOfCards.at(2).getValue();

    if(H1.handOfCards.at(1).getValue() != H2.handOfCards.at(1).getValue())
        return H1.handOfCards.at(1).getValue() - H2.handOfCards.at(1).getValue();

    if(H1.handOfCards.at(0).getValue() != H2.handOfCards.at(0).getValue())
        return H1.handOfCards.at(0).getValue() - H2.handOfCards.at(0).getValue();

    return 0;
}
