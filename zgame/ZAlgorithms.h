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
