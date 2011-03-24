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

//index function
int& GTL::ZStrategy::operator[](int player) const
{
    return (int&) strategy[player];
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
