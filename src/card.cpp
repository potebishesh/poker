#include <iostream>
#include <string>
#include "card.hpp"

Card::Card()
{
}

Card::Card(int value, SUIT suit)
{
    this->value = value;
    this->suit = suit;
}

int Card::compareCard(Card secondCard)
{
    int diff = 0;
    diff = this->value - secondCard.value;
    return diff;
}

int Card::getValue()
{

    return (value);
}

SUIT Card::getSuit()
{

    return (suit);
}

string Card::generateCardName()
{
    string suit;
    switch(this->suit)
    {
    case 0:
        suit = "D";
        break;
    case 1:
        suit = "H";
        break;
    case 2:
        suit = "C";
        break;
    case 3:
        suit = "S";
        break;
    default:
        std::cerr << "Invalid card suit." << std::endl;
    }
    string name = std::to_string(this->value) + "" + suit;
    return name;
}
