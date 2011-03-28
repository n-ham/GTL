#ifndef ZALGORITHMS_H_
#define ZALGORITHMS_H_

#include "ZGame.h"

/*
    This file contains algorithms for calculating properties of finite
    2-player zero sum games.

    row player == maximising player, column player == minimising player
*/

namespace GTL
{
    //returns a new game after iteratively removing pure strategies strictly dominated by a pure strategy
    template <class U>
    ZGame<U> pure_iterated_elimination_of_pure_strategies(ZGame<U> &game)
    {
        ZStrategy strategyi(game.dimensions),
                  strategyj(game.dimensions);

        std::vector<std::list<int> > strategies(2, std::list<int>());
        std::list<int>::iterator it_i, it_j, it3;

        //starts with all strategies as not dominated
        for(int p=0; p<2; p++)
            for(int a=0; a<game.dimensions[p]; a++)
                strategies[p].push_back(a);

        //iteratively finds strictly dominated strategies
        bool changed = 1;
        while(changed)
        {
            changed = 0;

            for(int p=0; p<2; p++)
            {
                //finds dominated strategies
                for(it_i = ++strategies[p].begin(); it_i != strategies[p].end(); )
                {
                    for(it_j = strategies[p].begin(); it_j != it_i;)
                    {
                        bool dominatedi = 1,
                             dominatedj = 1;

                        strategyi.setp(p, *it_i);
                        strategyj.setp(p, *it_j);

                        //checks each of the other players stragies
                        for(it3 = strategies[(p+1)%2].begin(); it3 != strategies[(p+1)%2].end(); it3++)
                        {
                            strategyi.setp((p+1)%2, *it3);
                            strategyj.setp((p+1)%2, *it3);

                            if(game.u(p, strategyi) >= game.u(p, strategyj))
                                dominatedi = 0;
                            if(game.u(p, strategyj) >= game.u(p, strategyi))
                                dominatedj = 0;

                            if(!dominatedi && !dominatedj)
                                break;
                        }

                        //checks to see if one of the two strategies is dominated
                        if(dominatedi)
                        {
                            it_i = strategies[p].erase(it_i);
                            changed = 1;
                            break;
                        }
                        else if(dominatedj)
                        {
                            it_j = strategies[p].erase(it_j);
                            changed = 1;
                        }
                        else if(++it_j == it_i)
                        {
                            it_i++;
                            break;
                        }
                    }
                }
            }
        }

        return game.subGame(strategies);
    };

    template <class U>
    ZGame<U> mixed_iterated_elimination_of_pure_strategies(ZGame<U> &game)
    {


        return ZGame<U>();
    };

    //returns all pure strategy Nash equilibria using minimax
    template <class U>
    std::vector<ZStrategy> psne(ZGame<U> &game)
    {
        //finds the strategies for row/col that max/min their min/max
        std::vector<std::vector<int> > strategies(2, std::vector<int>());
        strategies[0] = amax(game.payoffs.minrows());
        strategies[1] = amin(game.payoffs.maxcols());

        //generates the set of NE from the optimal strategy sets
        std::vector<ZStrategy> NE;
        for(int p0=0; p0<(int)strategies[0].size(); p0++)
            for(int p1=0; p1<(int)strategies[1].size(); p1++)
                NE.push_back(ZStrategy(game.dimensions, strategies[0][p0], strategies[1][p1]));

        return NE;
    };

    //returns all mixed strategy Nash equilibrium using the simplex algorithm
    template <class U>
    std::vector<ZMStrategy<U> > msne(ZGame<U> &game);
}

#endif //ZALGORITHMS_H_
