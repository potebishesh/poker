#ifndef game_h
#define game_h

#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>
#include "player.hpp"
#include "card.hpp"
#include "hand.hpp"
#include "deck.hpp"

using namespace std;


class playerNameWindow: public Gtk::Window
{
public:
    string playerName;

    playerNameWindow();

protected:
    void on_OK(); // get player's name from entry, opens playerWindow

    Gtk::Label nameLabel;
    Gtk::Entry Name;
    Gtk::Button OK;
    Gtk::VBox Box;
};

class playerWindow: public Gtk::Window
{
public:
    int temp1, temp5, temp25;
    player *Player;
    string chatMessage;

    playerWindow(player *p);

    //Signal handlers
    void on_Start();
    void on_Ante();
    void on_Menu();
    void on_Help();
    void on_Bet();
    void on_Call();
    void on_Raise();
    void on_Fold();
    void on_Exit();
    void on_CardSwap();
    void on_Send();
    void on_Check();
    void on_chip_1();
    void on_chip_5();
    void on_chip_25();
    void on_Clear();


    Gtk::Image* Card1;
    Gtk::Image* Card2;
    Gtk::Image* Card3;
    Gtk::Image* Card4;
    Gtk::Image* Card5;
    Gtk::CheckButton swap1;
    Gtk::CheckButton swap2;
    Gtk::CheckButton swap3;
    Gtk::CheckButton swap4;
    Gtk::CheckButton swap5;
    Gtk::Label Amount;
    Gtk::Entry Chat;
    Gtk::Label* chatLabel;
    Gtk::Label* currBetLabel;
    Gtk::Label* totPotLabel;
    Gtk::Label* currTurnLabel;
    Gtk::Label currentBet;
    Gtk::Label totalPot;
    Gtk::Label* balanceLabel;
    Gtk::Label yourBet;
    Gtk::Label chatHistory;
    Gtk::Button Start;
    Gtk::Button Ante;
    Gtk::Button chip1;
    Gtk::Button chip5;
    Gtk::Button chip25;
    Gtk::Button clear;
    Gtk::Button Help;
    Gtk::Button Check;
    Gtk::Button Bet;
    Gtk::Button Call;
    Gtk::Button Raise;
    Gtk::Button Fold;
    Gtk::Button Exit;
    Gtk::Button CardSwap;
    Gtk::Button Send;
    Gtk::VBox Box;
    Gtk::HBox checkBox;
    Gtk::HBox startBox;
    Gtk::HBox chipBox;
    Gtk::HBox actionBox;
    Gtk::HBox cardsBox;

};


int compareHand(hand H1, hand H2);
string getRank(hand H1);

#endif
