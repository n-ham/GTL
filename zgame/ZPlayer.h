#ifndef ZPLAYER_H_
#define ZPLAYER_H_

#include "../Const.h"

namespace GTL
{
    /*
        This struct stores information for a player in a normal form game.
    */
    struct ZPlayer
    {
        std::string name;
        int noActions;
        std::vector<std::string> actions;

        ZPlayer(int NoActions);
    };

    std::istream& operator>>(std::istream &is, ZPlayer &player);        //input function
    std::ostream& operator<<(std::ostream &os, const ZPlayer &player);  //output function
}

#endif //NPLAYER_H_

