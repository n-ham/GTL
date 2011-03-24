#ifndef ZMSTRATEGY_H_
#define ZMSTRATEGY_H_

#include "../Const.h"

//pure strategies for two player zero sum games
namespace GTL
{
    template <class P>
    struct ZMStrategy
    {
        std::vector<std::vector<P> > mstrategy;
        Matrix<P> pr;

        ZMStrategy(const std::vector<int> &dimensions)
        {
            pr.rows = dimensions[0];
            pr.cols = dimensions[1];

            reset();
        };

        ZMStrategy(const std::vector<int> &dimensions, const std::vector<std::vector<P> > &strategy)
        {
            pr.rows = dimensions[0];
            pr.cols = dimensions[1];

            mstrategy = strategy;
            formp();
        };

        //index function
        std::vector<P>& operator[](int player) const
        {
            return (std::vector<P>&) mstrategy[player];
        };

        void set(const std::vector<std::vector<P> > &strategy)
        {
            mstrategy = strategy;
            formp();
        };

        void reset()
        {
            mstrategy.clear();
            mstrategy.push_back(std::vector<P>(pr.rows, (P)1/(P)pr.rows));
            mstrategy.push_back(std::vector<P>(pr.cols, (P)1/(P)pr.cols));
            formp();
        };

        //forms the probability matrix from the players mixed strategies
        /*
            The matrix formed gives the probability of each outcome occuring
            when each player plays according to their specified mixed strategy.
        */
        void formp()
        {
            pr.matrix = std::vector<std::vector<P> >(pr.rows, mstrategy[1]);
            for(int r=0; r<pr.rows; r++)
                for(int c=0; c<pr.cols; c++)
                    pr.matrix[r][c] *= mstrategy[0][c];
        }
    };

    //input function
    template <class P>
    std::istream operator>>(std::istream &is, ZMStrategy<P> &strategy)
    {
        for(int p=0; p<2; p++)
            for(int a=0; a<(int)strategy[p].size(); a++)
                is >> strategy[p][a];

        return is;
    };

    //output function
    template <class P>
    std::ostream& operator<<(std::ostream &os, const ZMStrategy<P> &strategy)
    {
        for(int p=0; p<2; p++)
            os << join(' ', strategy.mstrategy[p]) << std::endl;
        return os;
    };
}

#endif //ZMSTRATEGY_H_
