// This file describes the format of the JSON strings to be used
// for communicating between the dealer and the player.
// Compliance is mandatory, but not all fields are required
//


#include <iostream>

#include "json.hpp"
int main(int argc,char *argv[])
{
    nlohmann::json to_dealer;
    to_dealer["from"] = { {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"}, {"name","Bud"} };
    to_dealer["event"] = "stand";        // "stand","hit","fold","raise","join","request_cards"
    to_dealer["cards_requested"] = 3;    // optional, number of cards requested, 1 to 5
    to_dealer["current_bet"] = 1.00;
    to_dealer["total_bet"] = 5.00;
    to_dealer["chat"] = "this is a string of text representing a chat message";
    std::cout << "to dealer:" << std::endl;
    std::cout << to_dealer.dump(2) << std::endl;

    nlohmann::json to_player;  // represents the entire game state.  sent to all players
    to_player["turn"] = "3f96b414-9ac9-40b5-8007-90d0e771f0d0";   // UUID of the current player.
    to_player["chat"] = {"this is one line","this is another","and so is this"};
    to_player["dealer_comment"] = "fred has raised and received 2 new cards";
    to_player["recommended_play"] = "you should fold";
    to_player["hand"] =
    {
        {{"bet",1},{"current_bet",10}, {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"}, {"name","Bud"},{"cards",{"acespades","10hearts","9clubs","2diamonds","kinghearts"}}},
        {{"bet",2},{"current_bet",1}, {"uuid","3f96b414-9ac9-40b5-8007-20d0e771f0d0"}, {"name","Donald"},{"cards",{"acehearts","10spades","9clubs","2clubs","jackhearts"}}},
        {{"bet",5},{"current_bet",5}, {"uuid","3f96b414-9ac9-40b5-8007-30d0e771f0d0"}, {"name","Ann"},{"cards",{"aceclubs","10diamonds","9clubs","2hearts","queenhearts"}}},
        {{"bet",10},{"current_bet",0}, {"uuid","3f96b414-9ac9-40b5-8007-40d0e771f0d0"}, {"name","Melania"},{"cards",{"acediamonds","10clubs","9clubs","2spades","kinghearts"}}}
    };

    std::cout << "to player:" << std::endl;
    std::cout << to_player.dump(2) << std::endl;
    return 0;
}
