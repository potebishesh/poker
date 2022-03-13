//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <algorithm>
#include <vector>
#include "asio.hpp"
#include "chat_message.hpp"
#include "game.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "json.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using asio::ip::tcp;




//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

Deck* deck = NULL;


int gameStatus; // -1 means not started, 1 means round 1, 2 means round to swap, 3 means final round
int currentBet;
int totalPot;
int turn;
int swapCount;
std::vector <string> idlist;
std::map<std::string, std::string> playerInfo;
std::map<std::string, hand> handInfo;

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        for (auto msg: recent_msgs_)
            participant->deliver(msg);
    }

    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant: participants_)
            participant->deliver(msg);
    }

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
    : public chat_participant,
      public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room)
        : socket_(std::move(socket)),
          room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        asio::async_read(socket_,
                         asio::buffer(read_msg_.data(), chat_message::header_length),
                         [this, self](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_msg_.decode_header())
            {
                // clear out the old buffer from the last read
                // a '\0' is a good value to make sure a string
                // is terminated
                for (unsigned int i=0; i<chat_message::max_body_length; i++)
                {
                    read_msg_.body() [i] = '\0';
                }


                do_read_body();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        asio::async_read(socket_,
                         asio::buffer(read_msg_.body(), read_msg_.body_length()),
                         [this, self](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {

                nlohmann::json to_dealer = nlohmann::json::parse(std::string(read_msg_.body()));

                nlohmann::json to_player;  // represents the entire game state.  sent to all players

                to_player["chat"] = to_dealer["chat"];

                if(to_dealer["event"] != "chat")
                {
                    to_player["current_bet"] = to_dealer["total_bet"];
                    /*
                    dealer decides what to do based on the "event" choosed by the user below this
                    event could be "start", "ante", "call", "bet", "raise", "check", "fold"
                    based on the event, the turn is determined, round of the game is determined,
                    winner is determined, etc.
                    */
                    if(to_dealer["event"] == "start")
                    {	
                    	/*
                    	if the event received is start, upon no interference, game status is updated,
                    	current bet is initialized to 0, cards are dealt, sequenced and send back to
                    	all players, players are only able to read cards sent to them based on uuid 
                    	even though all the players are sent the same instruction 
                    	*/
		
                        if(gameStatus >= 1)
                        {
                            to_player["chat"] = "Game is already started.";
                        }
                        else if(idlist.size() <= 1)
                        {
                            to_player["chat"] = "Not enough players to start the game.";
                        }
                        else
                        {
                            gameStatus = 1;
                            to_player["current_bet"] = 0;

                            for(int i = 0; i < (int)idlist.size(); i++)
                            {
                                hand H;

                                for(int j = 0; j < 5; j++)
                                {
                                    if(deck->deckOfCards.size() == 0)
                                    {
                                        deck->reset_deck();
                                        deck->shuffle_deck();
                                    }
                                    H.handOfCards.push_back(deck->get_card());

                                }


                                H.sequenceHand();

                                handInfo.insert(pair<std::string, hand> (idlist.at(i), H));

                                to_player["hand"][idlist.at(i)] = {{"card1",H.handOfCards.at(0).generateCardName()},
                                    {"card2",H.handOfCards.at(1).generateCardName()},
                                    {"card3",H.handOfCards.at(2).generateCardName()},
                                    {"card4",H.handOfCards.at(3).generateCardName()},
                                    {"card5",H.handOfCards.at(4).generateCardName()}
                                };

                            }
                        }
                    }
                    else if(to_dealer["event"] == "ante")
                    {
                    /*
                    if the event recieved is ante, active player's list is updated, total pot is updated
                    $1 per active player
                    */
                        totalPot++;
                        idlist.push_back(to_dealer["from"]["uuid"]);

                        playerInfo.insert(pair<std::string,std::string> (std::string(to_dealer["from"]["uuid"]), std::string(to_dealer["from"]["name"])));
                    }
                    else if(to_dealer["event"] == "call")
                    {
                    /*
                   	if the received event is call, turn is switched to next player, total pot is updated
                   	based on the amount called by the player, game staus is updated and if it is the last 
                   	call of the game, the winner is decided by comparision of hands of all active players. 
                   	Also, total pot amountis sent to the players, so winner's total balance is updated, 
                   	and other variables arereset to start a new game.
                    */
                        int tempPot = to_dealer["total_bet"];
                        totalPot = totalPot + tempPot;

                        if (turn >= (int)idlist.size() - 1)
                        {
                            gameStatus++;
                            turn = 0;
                            if(gameStatus == 2)
                            {
                                to_player["chat"] = std::string(to_player["chat"]) + ". Swapping round begins.";
                                to_player["current_bet"] = 0;
                            }
                            else if(gameStatus >= 4)
                            {
                                std::string best;

                                std::map<std::string, hand>::iterator it = handInfo.begin();


                                while (it != handInfo.end())
                                {
                                    it->second.sequenceHand();
                                    it++;
                                }

                                best = idlist.at(0);
                                for(int i = 1; i < (int)(idlist.size()); i++)
                                {
                                    if(compareHand(handInfo.at(best), handInfo.at(idlist.at(i))) < 0)
                                    {
                                        best = idlist.at(i);
                                    }
                                }

                                to_player["winner"] = playerInfo.at(best);
                                to_player["winner_hand"] = getRank(handInfo.at(best));
                                to_player["prize"]["amount"] = std::to_string(totalPot);
                                to_player["prize"]["uuid"] = best;
                                to_player["current_bet"] = 0;

                                totalPot = 0;
                                currentBet = 0;
                                gameStatus = -1;
                                swapCount = 0;


                                idlist.clear();
                                playerInfo.clear();
                                handInfo.clear();

                            }
                        }

                        else
                        {
                            turn++;
                        }

                    }
                    else if(to_dealer["event"] == "check") {
                    	/*
                    	if the received event is check, the turn is switched to next player.
                    	*/
                        std::rotate(idlist.begin(), idlist.begin() + turn, idlist.end());
                        std::rotate(idlist.begin(), idlist.begin() + 1, idlist.end());
                        turn = 0;
                        to_player["current_bet"] = 0;

                    }
                    else if(to_dealer["event"] == "bet")
                    {
                    	/*
                    	if the received event is bet, total pot is updated by the amount of bet 
                    	done by the player, and turn is switched to next player.	
                    	*/
                        int tempPot = to_dealer["total_bet"];
                        totalPot = totalPot + tempPot;

                        turn++;

                    }
                    else if(to_dealer["event"] == "fold")
                    {	
                    	/*
                    	if the received event is fold, turn is switched to next player, the player who fold
                    	is removed from active players list. If only one player remain after a fold, the last
                    	player standing wins automatically regardless of the rank of his hand. Also, total pot 
                    	amount is sent to the players, so winner's total balance is updated, and other variables
                    	are reset to start a new game.
                    	*/
                        int temp = turn;
                        if(turn == 0)
                        {
                            to_player["current_bet"] = 0;
                        }

                        if(turn == ((int)idlist.size() - 1))
                        {
                            gameStatus++;
                            to_player["current_bet"] = 0;
                            turn = 0;
                        }

                        idlist.erase(idlist.begin() + temp);

                        playerInfo.erase(std::string(to_dealer["from"]["uuid"]));
                        handInfo.erase(std::string(to_dealer["from"]["uuid"]));


                        if((int)idlist.size() == 1)
                        {
                            handInfo.at(idlist.at(0)).sequenceHand();
                            to_player["winner"] = playerInfo.at(idlist.at(0));
                            to_player["winner_hand"] = "with all others folded";
                            to_player["prize"]["amount"] = std::to_string(totalPot);
                            to_player["prize"]["uuid"] = idlist.at(0);
                            to_player["current_bet"] = 0;

                            totalPot = 0;
                            currentBet = 0;
                            gameStatus = -1;
                            swapCount = 0;

                            idlist.clear();
                            playerInfo.clear();
                            handInfo.clear();


                        }


                    }
                    else if(to_dealer["event"] == "raise")
                    {
                    	/*
                    	if the received event is raise, total pot is updated by the amount of raised 
                    	by the player, and turn is switched to next player.	
                    	*/
                        int tempPot = to_dealer["total_bet"];
                        totalPot = totalPot + tempPot;

                        std::rotate(idlist.begin(), idlist.begin() + turn,idlist.end());

                        turn = 1;
                    }
                    else if(to_dealer["event"] == "swap")
                    {
                    	/*
                    	if received event is swap, dealer reads the cards that player wants to switch,
                    	and get new card from the card to replace the old cards. if all swaps are
                    	completed, game status is updated.
                    	*/
                        to_player["current_bet"] = 0;
                        swapCount++;
                        std::rotate(idlist.begin(), idlist.begin() + turn, idlist.end());
                        std::rotate(idlist.begin(), idlist.begin() + 1, idlist.end());
                        turn = 0;

                        string cardSwaps = to_dealer["swap_cards"];

                        if(atoi(cardSwaps.c_str()) != 0)

                        {
                            string temp = to_dealer["from"]["uuid"];
                            for(int i = 0; i < (int)cardSwaps.length(); i++)
                            {
                                if(deck->deckOfCards.size() == 0)
                                {
                                    deck->reset_deck();
                                    deck->shuffle_deck();
                                }

                                handInfo.at(temp).handOfCards.at(atoi(cardSwaps.substr(i,1).c_str())-1) = deck->get_card();
                                to_player["hand"][temp]["card" + cardSwaps.substr(i,1)] = handInfo.at(temp).handOfCards.at(atoi(cardSwaps.substr(i,1).c_str())-1).generateCardName();
                            }
                        }

                        if(swapCount == (int)idlist.size())
                        {

                            gameStatus++;

                            to_player["chat"] = std::string(to_player["chat"]) + ". Swapping round ends.";
                        }


                    }
                    if(gameStatus != -1)
                    {
                    	/*
                    	at any time, the game is being played (after start and before end), turn is sent to the players.
                    	*/
                        to_player["turn"]["name"] = playerInfo.at(idlist.at(turn));
                        to_player["turn"]["uuid"] = idlist.at(turn);
                    }

                }
                /*
                total pot, game round, player count are send to players regardless of the status of the game
                */

                to_player["total_pot"] = std::to_string(totalPot);

                to_player["game_round"] = std::to_string(gameStatus);

                to_player["player_count"] = std::to_string(idlist.size());

                std::string t = to_player.dump();
                chat_message sending;
                if (t.size() < chat_message::max_body_length)
                {
                    memcpy( sending.body(), t.c_str(), t.size() );
                    sending.body_length(t.size());
                    sending.encode_header();
                    room_.deliver(sending);
                }
                do_read_header();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_write()
    {
        auto self(shared_from_this());
        asio::async_write(socket_,
                          asio::buffer(write_msgs_.front().data(),
                                       write_msgs_.front().length()),
                          [this, self](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------

class chat_server
{
public:
    chat_server(asio::io_context& io_context,
                const tcp::endpoint& endpoint)
        : acceptor_(io_context, endpoint)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<chat_session>(std::move(socket), room_)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    chat_room room_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
    deck = new Deck();
    deck->shuffle_deck();
    totalPot = 0;
    gameStatus = -1;
    currentBet = 0;
    turn = 0;

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        asio::io_context io_context;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
