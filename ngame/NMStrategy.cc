#include "NMStrategy.h"

using namespace std;

//constructs a mixed strategy with each player playing all strategies with equal weight
GTL::NMStrategy::NMStrategy(const vector<int> &Dimensions)
{
    pr.dimensions = Dimensions;

    reset();
}

//constructs a mixed strategy of given dimensions with the specified mixed strategies
GTL::NMStrategy::NMStrategy(const vector<int> &Dimensions, const vector<vector<PROB> > &Strategy)
{
    pr.dimensions = Dimensions;
    mstrategy = Strategy;

    formp();
};

//index function
vector<PROB>& GTL::NMStrategy::operator[](int player) const
{
    return (vector<PROB>&) mstrategy[player];
};

//sets the mixed strategy to the values given in Strategy
void GTL::NMStrategy::set(const vector<vector<PROB> > &Strategy)
{
    mstrategy = Strategy;

    formp();
};

//sets the mixed strategy so each player plays each strategy with equal weights
void GTL::NMStrategy::reset()
{
    mstrategy.clear();
    for(int i=0; i<(int)pr.dimensions.size(); i++)
        mstrategy.push_back(vector<PROB>(pr.dimensions[i], (PROB)1/(PROB)pr.dimensions[i]));

    formp();
};


//forms the probability tensor from the players mixed strategies
/*
    The tensor formed gives the probability of each outcome occuring
    when each player plays according to their specified mixed strategy.
*/
void GTL::NMStrategy::formp()
{
    NStrategy strategy(pr.dimensions);
    int noStrategies = strategy.noStrategies();
    pr.tensor = vector<PROB>(noStrategies, (PROB)1);

    for(int s=0; s<noStrategies; s++, strategy++)
        for(int p=0; p<(int)pr.dimensions.size(); p++)
            pr[strategy] *= mstrategy[p][strategy[p] ];
};


//input function
istream& GTL::operator>>(istream &is, NMStrategy &strategy)
{
    for(int p=0; p<(int)strategy.pr.dimensions.size(); p++)
        for(int a=0; a<strategy.pr.dimensions[p]; a++)
            is >> strategy[p][a];

    strategy.formp();

    return is;
};

//save function
ofstream& GTL::operator<<(ofstream &ofs, const NMStrategy &strategy)
{
    ofs << strategy.mstrategy << endl;

    return ofs;
};

//output function
ostream& GTL::operator<<(ostream &os, const NMStrategy &strategy)
{
    os << "nms(" << strategy[0];
    for(int i=1; i<(int)strategy.pr.dimensions.size(); i++)
        os << "; " << strategy[i];
    os << ")";

    return os;
};
