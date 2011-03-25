#include "ZStrategy.h"

using namespace std;

GTL::ZStrategy::ZStrategy(const std::vector<int> &Dimensions)
{
    dimensions = Dimensions;
    strategy = std::vector<int>(2, 0);
};

GTL::ZStrategy::ZStrategy(const std::vector<int> &Dimensions, const std::vector<int> &Strategy)
{
    dimensions = Dimensions;
    strategy = Strategy;
};

GTL::ZStrategy::ZStrategy(const std::vector<int> &Dimensions, int s1, int s2)
{
    dimensions = Dimensions;
    strategy = std::vector<int>(1, s1);
    strategy.push_back(s2);
};

//index function
int& GTL::ZStrategy::operator[](int player) const
{
    return (int&) strategy[player];
};

//set functions
void GTL::ZStrategy::set(const std::vector<int> &Strategy)
{
    strategy = Strategy;
};

void GTL::ZStrategy::set(int s1, int s2)
{
    strategy[0] = s1;
    strategy[1] = s2;
};

void GTL::ZStrategy::setp(int player, int s)
{
    strategy[player] = s;
};

//reset function
void GTL::ZStrategy::reset()
{
    strategy[0] = strategy[1] = 0;
};


int GTL::ZStrategy::noStrategiesExc(int excPlayer)
{
    return dimensions[(excPlayer+1)%2];
};

int GTL::ZStrategy::noStrategiesInc(int incPlayer)
{
    return dimensions[incPlayer];
};

int GTL::ZStrategy::noStrategies()
{
    return dimensions[0]*dimensions[1];
};

void GTL::ZStrategy::ppExc(int excPlayer)
{
    ++strategy[(excPlayer+1)%2] %= dimensions[(excPlayer+1)%2];
};

void GTL::ZStrategy::ppInc(int incPlayer)
{
    ++strategy[incPlayer] %= dimensions[incPlayer];
};

//pre-fix increment operator
void GTL::ZStrategy::operator++()
{
    for(int p=1; p>=0; p--)
    {
        if(strategy[p]+1 == dimensions[p])
            strategy[p] = 0;
        else
        {
            strategy[p]++;
            break;
        }
    }
};

//post-fix increment operator
void GTL::ZStrategy::operator++(int)
{
    ++(*this);
};

//input function
istream& GTL::operator>>(istream &is, ZStrategy &strategy)
{
    is >> strategy[0] >> strategy[1];
    return is;
};

//output function
ostream& GTL::operator<<(ostream &os, const ZStrategy &strategy)
{
    os << join(' ', strategy.strategy);

    return os;
};
