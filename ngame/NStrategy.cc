#include "NStrategy.h"

using namespace std;

//constructs a pure strategy with each player playing their first strategy
GTL::NStrategy::NStrategy(const vector<int> &Dimensions)
{
    index = 0;
    dimensions = Dimensions;
    strategy = vector<int>(dimensions.size(), 0);
};

//constructs a pure strategy with the given strategy choice of each player
GTL::NStrategy::NStrategy(const vector<int> &Dimensions, const vector<int> &Strategy)
{
    dimensions = Dimensions;
    strategy = Strategy;
    calculateIndex();
};

//index function
int& GTL::NStrategy::operator[](int player) const
{
    return (int&) strategy[player];
};


void GTL::NStrategy::calculateIndex()
{
    int translate = 1;
    index = 0;

    for(int i=strategy.size()-1; i>=0; i--)
    {
        index += strategy[i]*translate;
        translate *= dimensions[i];
    }
};


//sets the pure strategy for the new vector of strategy choices
void GTL::NStrategy::set(const vector<int> &Strategy)
{
    strategy = Strategy;
    calculateIndex();
};

//resets to the first possible pure strategy combination
void GTL::NStrategy::reset()
{
    index = 0;
    strategy = vector<int>(dimensions.size(), 0);
};

//number of possible pure strategy combinations not including player
int GTL::NStrategy::noStrategiesExc(int excPlayer)
{
    int strategies = 1;

    for(int i=0; i<(int)dimensions.size(); i++)
        if(i != excPlayer)
            strategies *= dimensions[i];

    return strategies;
};

//number of possible pure strategy combinations not including players in the player vector
int GTL::NStrategy::noStrategiesExc(const vector<int> &excPlayers)
{
    int strategies = 1;
    vector<bool> include(dimensions.size(),1);

    for(int i=0; i<(int)excPlayers.size(); i++)
        include[excPlayers[i] ] = 0;

    for(int i=0; i<(int)dimensions.size(); i++)
        if(include[i])
            strategies *= dimensions[i];

    return strategies;
};

//number of pure strategies for incPlayer
int GTL::NStrategy::noStrategiesInc(int incPlayer)
{
    return dimensions[incPlayer];
};

//number of possible pure strategy combinations for the players in the player vector
int GTL::NStrategy::noStrategiesInc(const vector<int> &incPlayers)
{
    int strategies = 1;
    vector<bool> include(dimensions.size(),false);

    for(int i=0; i<(int)incPlayers.size(); i++)
        include[incPlayers[i] ] = true;

    for(int i=0; i<(int)dimensions.size(); i++)
        if(include[i])
            strategies *= dimensions[i];

    return strategies;
};

//number of possible pure strategy combinations
int GTL::NStrategy::noStrategies()
{
    int strategies = 1;

    for(int i=0; i<(int)dimensions.size(); i++)
        strategies *= dimensions[i];

    return strategies;
};

//increments to the next possible pure strategy combination keeping the strategy of player constant
void GTL::NStrategy::ppExc(int excPlayer)
{
    int i = dimensions.size() - 1,
        power = 1;

    for(; i>=0 && (i == excPlayer || strategy[i] == dimensions[i]-1); i--, power*=dimensions[i])
    {
        if(i != excPlayer)
        {
            strategy[i] = 0;
            index -= (dimensions[i]-1)*power;
        }
    }

    if(i>=0)
    {
        strategy[i]++;
        index += power;
    }
};

//increments to the next possible pure strategy combination keeping the strategy of a number of players constant
void GTL::NStrategy::ppExc(const vector<int> &excPlayers)
{
    int power = 1;
    int i = dimensions.size() - 1;
    vector<bool> include(strategy.size(), 1);

    for(int i=0; i<(int)excPlayers.size(); i++)
        include[excPlayers[i] ] = 0;

    for(;i>=0 && (!include[i] || strategy[i] == dimensions[i]-1); i--, power*=dimensions[i])
    {
        if(include[i])
        {
            strategy[i] = 0;
            index -= (dimensions[i]-1)*power;
        }
    }

    if(i>=0)
    {
        strategy[i]++;
        index += power;
    }
};

//increments to the next possible pure strategy combination varying only the strategies of player
void GTL::NStrategy::ppInc(int incPlayer)
{
    int power = 1;

    for(int i=dimensions.size()-1; i>incPlayer; i--)
        power *= dimensions[i];

    if(strategy[incPlayer] == dimensions[incPlayer]-1)
    {
        index -= strategy[incPlayer]*power;
        strategy[incPlayer] = 0;
    }
    else
    {
        index += power;
        strategy[incPlayer]++;
    }
};

//increments to the next possible pure strategy combination varying only strategies of players in the player vector
void GTL::NStrategy::ppInc(const vector<int> &incPlayers)
{
    int power = 1;
    int i = dimensions.size() - 1;
    vector<bool> include(strategy.size(), false);

    for(int i=0; i<(int)incPlayers.size(); i++)
        include[incPlayers[i] ] = true;

    while (i>=0 && (!include[i] || strategy[i] == dimensions[i]-1))
    {
        if(include[i])
        {
            strategy[i] = 0;
            index -= (dimensions[i]-1)*power;
        }

        power *= dimensions[i];

        i--;
    }

    if(i>=0)
    {
        strategy[i]++;
        index += power;
    }
};

//increments to the next possible pure strategy combination varying only the strategies of player
void GTL::NStrategy::operator++(int)
{
    int i = dimensions.size() - 1,
        power = 1;

    for(; i>=0 && strategy[i] == dimensions[i]-1; i--, power*=dimensions[i])
    {
        strategy[i] = 0;
        index -= (dimensions[i]-1)*power;
    }

    if(i>=0)
    {
        strategy[i]++;
        index += power;
    }
};


//input function
istream& GTL::operator>>(istream &is, NStrategy &strategy)
{
    int cStrategyChoice;
    //gets the pure strategy choices from the input stream for the no of players in the game
    strategy.strategy.clear();
    for(int i=0; i<(int)strategy.dimensions.size(); i++)
    {
        is >> cStrategyChoice;
        strategy.strategy.push_back(cStrategyChoice);
    }

    //updates the index value
    strategy.calculateIndex();

    return is;
};

//save function
ofstream& GTL::operator<<(ofstream &ofs, const NStrategy &strategy)
{
    for(int i=0; i<(int)strategy.dimensions.size(); i++)
        ofs << strategy[i] << " ";
    return ofs;
};

//output function
ostream& GTL::operator<<(ostream &os, const NStrategy &strategy)
{
    os << "ns(" << strategy[0];
    for(int i=1; i<(int)strategy.strategy.size(); i++)
        os << ", " << strategy[i];
    os << ")";

    return os;
};
