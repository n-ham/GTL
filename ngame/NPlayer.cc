#include "NPlayer.h"

using namespace std;

//constructer
GTL::NPlayer::NPlayer(int NoActions)
{
    noActions = NoActions;
    actions = std::vector<std::string>(noActions, "");
};

//input function
std::istream& GTL::operator>>(std::istream &is, NPlayer &player)
{
    is >> player.name >> player.actions;

    return is;
};

//output function
std::ostream& GTL::operator<<(std::ostream &os, const NPlayer &player)
{
    os << player.name << " " << player.actions;

    return os;
};
