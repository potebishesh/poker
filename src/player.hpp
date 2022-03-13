#ifndef player_h
#define player_h

#include <iostream>
#include <string>
#include <vector>



using namespace std;


class player
{
public:
    string id;
    int balance;
    int chip1;
    int chip5;
    int chip25;

    bool status;
    bool turn;
    bool swapped;
    bool checked;

    string playerName;

    player(string PlayerName);

};

#endif
