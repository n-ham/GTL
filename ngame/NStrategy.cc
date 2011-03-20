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
    return product(dimensions);
};

//increments to the next possible pure strategy combination keeping the strategy of player constant
void GTL::NStrategy::ppExc(int excPlayer)
{
    for(int p=dimensions.size()-1, power = 1; p>=0; p--, power*=dimensions[p])
    {
        if(strategy[p] == dimensions[p]-1)
        {
            if(p != excPlayer)
            {
                strategy[p] = 0;
                index -= (dimensions[p]-1)*power;
            }
        }
        else if(p != excPlayer)
        {
            strategy[p]++;
            index += power;
            break;
        }
    }
};

//increments to the next possible pure strategy combination keeping the strategy of a number of players constant
void GTL::NStrategy::ppExc(const vector<int> &excPlayers)
{
    vector<bool> include(strategy.size(), 1);
    for(int i=0; i<(int)excPlayers.size(); i++)
        include[excPlayers[i] ] = 0;

    for(int p=dimensions.size()-1, power=1; p>=0; p--, power*=dimensions[p])
    {
        if(strategy[p] == dimensions[p]-1)
        {
            if(include[p])
            {
                strategy[p] = 0;
                index -= (dimensions[p]-1)*power;
            }
        }
        else if(include[p])
        {
            strategy[p]++;
            index += power;
            break;
        }
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
    vector<bool> include(strategy.size(), 0);
    for(int i=0; i<(int)incPlayers.size(); i++)
        include[incPlayers[i] ] = 1;

    for(int i=dimensions.size()-1, power=1; i>=0; i--, power*=dimensions[i])
    {
        if(strategy[i] == dimensions[i]-1)
        {
            if(include[i])
            {
                strategy[i] = 0;
                index -= (dimensions[i]-1)*power;
            }
        }
        else if(include[i])
        {
            strategy[i]++;
            index += power;
            break;
        }
    }
};

//(prefix) increments to the next possible pure strategy combination
void GTL::NStrategy::operator++()
{
    for(int i=dimensions.size()-1, power=1; i>=0; i--, power*=dimensions[i])
    {
        if(strategy[i] == dimensions[i]-1)
        {
            strategy[i] = 0;
            index -= (dimensions[i]-1)*power;
        }
        else
        {
            strategy[i]++;
            index += power;
            break;
        }
    }
};

//(postfix) increments to the next possible pure strategy combination
void GTL::NStrategy::operator++(int)
{
    ++(*this);
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

//output function
ostream& GTL::operator<<(ostream &os, const NStrategy &strategy)
{
    os << join(' ', strategy.strategy);

    return os;
};
