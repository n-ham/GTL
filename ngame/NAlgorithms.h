#ifndef NALGORITHMS_H_
#define NALGORITHMS_H_

#include "NGame.h"

/*
    This file contains algorithms for calculating properties of finite
    n-player normal form games.
*/

namespace GTL
{
    //returns all pure strategy Nash equilibria of the given game
    template <class U>
    std::vector<NStrategy> psne(NGame<U> &game)
    {
        Tensor<int> maxCount(game.dimensions);
        NStrategy strategy(game.dimensions);
        std::vector<NStrategy> NE;
        U uMax;

        //goes through each of the players in the game
        for(int p=0; p<game.noPlayers; p++)
        {
            //for each possible strategy combination excluding those of player p
            for(int s=0, noStrategies=strategy.noStrategiesExc(p); s<noStrategies; s++)
            {
                //finds highest attainable utility for player n given the other strategies
                uMax = game.u(p, strategy);
                strategy.ppInc(p);
                for(int a=1; a<game.dimensions[p]; a++)
                {
                    uMax = max(uMax, game.u(p, strategy));
                    strategy.ppInc(p);
                }

                //finds the strategies that give player p uMax
                for(int a=0; a<game.dimensions[p]; a++)
                {
                    if(game.u(p, strategy) == uMax)
                    {
                        maxCount[strategy]++;

                        if(maxCount[strategy] == game.noPlayers) //strategy is a psne
                            NE.push_back(strategy);
                    }
                    strategy.ppInc(p);
                }
                strategy.ppExc(p);
            }
        }

        return NE;
    };

    //returns all mixed strategy Nash equilibria of the given game
    template <class U>
    std::vector<NMStrategy<U> > msne(); //haha, good luck writing this
}

#endif //NALGORITHMS_H_

