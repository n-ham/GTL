#include "NPlayer.h"

using namespace std;

GTL::NPlayer::NPlayer(int NoActions)
{
    noActions = NoActions;
    actions = vector<string>(noActions, "");
};

istream& GTL::operator>>(istream &is, NPlayer &player)
{
    is >> player.name >> player.actions;

    return is;
};

ostream& GTL::operator<<(ostream &os, const NPlayer &player)
{
    os << player.name << " " << player.actions;

    return os;
};
