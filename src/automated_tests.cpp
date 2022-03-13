//#include <map>

#include "game.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "hand.hpp"


#define BOOST_TEST_MODULE pokertest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (poker_tests)


BOOST_AUTO_TEST_CASE (check_order_of_sequenced_hand)
{
    // checking if sequenceHand put the hand in order
    Deck D;
    hand H;

    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());

    H.sequenceHand();

    BOOST_CHECK (H.handOfCards.at(0).getValue() <= H.handOfCards.at(1).getValue() && H.handOfCards.at(1).getValue() <= H.handOfCards.at(2).getValue() &&
                 H.handOfCards.at(2).getValue() <= H.handOfCards.at(3).getValue() && H.handOfCards.at(3).getValue() <= H.handOfCards.at(4).getValue());
}


BOOST_AUTO_TEST_CASE (suit_count)
{
    //checking counts of all the suits in a shuffled deck is 13 each
    Deck D;
    D.shuffle_deck();

    int s, c, d, h;
    s = 0;
    c = 0;
    d = 0;
    h = 0;

    for(int i = 0; i < (int)D.deckOfCards.size(); i++)
    {
        if(D.deckOfCards.at(i).getSuit() == 0)
            s++;
        else if(D.deckOfCards.at(i).getSuit() == 1)
            c++;
        else if(D.deckOfCards.at(i).getSuit() == 2)
            d++;
        else if(D.deckOfCards.at(i).getSuit() == 3)
            h++;

    }

    BOOST_CHECK_EQUAL(s, 13);
    BOOST_CHECK_EQUAL(c, 13);
    BOOST_CHECK_EQUAL(d, 13);
    BOOST_CHECK_EQUAL(h, 13);

}

BOOST_AUTO_TEST_CASE (two_pair_high)
{
    //checking if higher card value generated of a two pair is greater than lower card value generated of the two pairs.
    Deck D;
    hand H;

    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.handOfCards.push_back(D.get_card());
    H.sequenceHand();

    while (!H.isTwoPair() || H.isFourOfAKind())
    {
        for(int i = 0; i < 5; i++)
        {
            if(D.deckOfCards.empty())
            {
                D.reset_deck();
                D.shuffle_deck();
            }
            H.handOfCards.at(i) = D.get_card();

        }

        H.sequenceHand();

    }

    BOOST_CHECK (H.twoPairHigh() > H.twoPairLow());


}


BOOST_AUTO_TEST_CASE (compare_hand_function)
{
    //check if compare_hand works by checking the return value after passing a flush and a straight

    Deck D;
    hand H1, H2;

    H1.handOfCards.push_back(D.get_card());
    H1.handOfCards.push_back(D.get_card());
    H1.handOfCards.push_back(D.get_card());
    H1.handOfCards.push_back(D.get_card());
    H1.handOfCards.push_back(D.get_card());
    H1.sequenceHand();

    while (!H1.isStraight() || H1.isFlush())
    {
        for(int i = 0; i < 5; i++)
        {
            if(D.deckOfCards.empty())
            {
                D.reset_deck();
                D.shuffle_deck();
            }
            H1.handOfCards.at(i) = D.get_card();

        }

        H1.sequenceHand();

    }

    D.reset_deck();

    H2.handOfCards.push_back(D.get_card());
    H2.handOfCards.push_back(D.get_card());
    H2.handOfCards.push_back(D.get_card());
    H2.handOfCards.push_back(D.get_card());
    H2.handOfCards.push_back(D.get_card());
    H2.sequenceHand();

    while (!H2.isFlush() || H2.isStraight())
    {
        for(int i = 0; i < 5; i++)
        {
            if(D.deckOfCards.empty())
            {
                D.reset_deck();
                D.shuffle_deck();
            }
            H2.handOfCards.at(i) = D.get_card();

        }

        H2.sequenceHand();

    }

    BOOST_CHECK (compareHand(H1, H2) < 0);


}

BOOST_AUTO_TEST_CASE (deck_total)
{
    //check if shuffled deck has a sum value of 364

    Deck D;
    D.shuffle_deck();
    int sum = 0;

    for(int i = 0; i < (int)D.deckOfCards.size(); i++)
    {
        sum = sum + D.deckOfCards.at(i).getValue();
    }

    BOOST_CHECK_EQUAL (sum, 364);

}


BOOST_AUTO_TEST_SUITE_END( )
