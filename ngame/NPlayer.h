#ifndef NPLAYER_H_
#define NPLAYER_H_

#include "../Const.h"

namespace GTL
{
    struct NPlayer
    {
        std::string name;
        int noActions;
        std::vector<std::string> actions;

        NPlayer(int NoActions);
    };

    std::istream& operator>>(std::istream &is, NPlayer &player);
    std::ostream& operator<<(std::ostream &os, const NPlayer &player);
}

#endif //NPLAYER_H_
