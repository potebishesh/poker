#include <iostream>
#include <vector>
#include <string>
#include "player.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>



player::player(string playerName)
{
    //player constructor: sets player name, status to false, generates id from uuid, initializes balance and chips, sets swapped, turn, checked to false.
    this->playerName = playerName;
    this->status = false;
    id = boost::uuids::to_string(boost::uuids::random_generator()());
    balance = 100;
    chip1 = 25;
    chip5 = 10;
    chip25 = 1;
    swapped = false;
    turn = false;
    checked = false;
}
