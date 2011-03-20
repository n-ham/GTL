#ifndef NMSTRATEGY_H_
#define NMSTRATEGY_H_

#include "../Const.h"
#include "../maths/Tensor.h"

//mixed strategies for normal form games
namespace GTL
{
    template <class P>
    struct NMStrategy
    {
        /*
            variables
        */
        std::vector<std::vector<P> > mstrategy;
        Tensor<P> pr;

        /*
            functions
        */
        //constructors
        NMStrategy<P>(const std::vector<int> &Dimensions)
        {
            pr.dimensions = Dimensions;

            reset();
        }

        NMStrategy<P>(const std::vector<int> &Dimensions, const std::vector<std::vector<P> > &Strategy)
        {
            pr.dimensions = Dimensions;
            mstrategy = Strategy;

            formp();
        };

        //index function
        std::vector<P>& operator[](int player) const
        {
            return (std::vector<P>&) mstrategy[player];
        };

        //set functions
        void set(const std::vector<std::vector<P> > &Strategy)
        {
            mstrategy = Strategy;

            formp();
        };

        //sets the mixed strategy so each player plays each strategy with equal weights
        void reset()
        {
            mstrategy.clear();
            for(int i=0; i<(int)pr.dimensions.size(); i++)
                mstrategy.push_back(std::vector<P>(pr.dimensions[i], (P)1/(P)pr.dimensions[i]));

            formp();
        };


       //forms the probability tensor from the players mixed strategies
        /*
            The tensor formed gives the probability of each outcome occuring
            when each player plays according to their specified mixed strategy.
        */
        void formp()
        {
            NStrategy strategy(pr.dimensions);
            int noStrategies = strategy.noStrategies();
            pr.tensor = std::vector<P>(noStrategies, 1);

            for(int s=0; s<noStrategies; s++, strategy++)
                for(int p=0; p<(int)pr.dimensions.size(); p++)
                    pr[strategy] *= mstrategy[p][strategy[p] ];
        };

    };

    //input function
    template <class P>
    std::istream& operator>>(std::istream &is, NMStrategy<P> &strategy)
    {
        //gets the "ms" string
        std::string junk;
        is >> junk;

        //reads each players mixed strategy
        for(int p=0; p<(int)strategy.pr.dimensions.size(); p++)
            for(int a=0; a<strategy.pr.dimensions[p]; a++)
                is >> strategy[p][a];

        //calculates the probability of each pure strategy happening
        strategy.formp();

        return is;
    };

    //output function
    template <class P>
    std::ostream& operator<<(std::ostream &os, const NMStrategy<P> &strategy)
    {
        if(strategy.mstrategy.size() > 0)
            os << join(' ', strategy.mstrategy[0]) << std::endl;
        for(int i=1; i<(int)strategy.mstrategy.size(); i++)
            os << "    " << join(' ', strategy.mstrategy[i]) << std::endl;

        return os;
    };

}

#endif //NMSTRATEGY_H_
