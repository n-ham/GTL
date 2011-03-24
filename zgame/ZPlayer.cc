#include "ZPlayer.h"

using namespace std;

//constructer
GTL::ZPlayer::ZPlayer(int NoActions)
{
    noActions = NoActions;
    actions = std::vector<std::string>(noActions, "");
};

//input function
std::istream& GTL::operator>>(std::istream &is, ZPlayer &player)
{
    is >> player.name >> player.actions;

    return is;
};

//output function
std::ostream& GTL::operator<<(std::ostream &os, const ZPlayer &player)
{
    os << player.name << " " << player.actions;

    return os;
};

