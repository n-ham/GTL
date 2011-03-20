#ifndef NPLAYER_H_
#define NPLAYER_H_

#include "../Const.h"

namespace GTL
{
    /*
        This struct stores information for a player in a normal form game.
    */
    struct NPlayer
    {
        std::string name;
        int noActions;
        std::vector<std::string> actions;

        NPlayer(int NoActions);
    };

    std::istream& operator>>(std::istream &is, NPlayer &player);        //input function
    std::ostream& operator<<(std::ostream &os, const NPlayer &player);  //output function
}

#endif //NPLAYER_H_
