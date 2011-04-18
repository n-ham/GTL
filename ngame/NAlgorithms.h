#ifndef NALGORITHMS_H_
#define NALGORITHMS_H_

#include "../Const.h"
#include "NGame.h"

/*
    This file contains algorithms for calculating properties of finite
    n-player normal form games.
*/

namespace GTL
{
    //returns a new game after iteratively removing pure strategies strictly dominated by a pure strategy
    template <class U>
    NGame<U> pure_iterated_elimination_of_pure_strategies(NGame<U> &game)
    {
        NStrategy strategyi(game.dimensions),
                  strategyj(game.dimensions);

        std::vector<std::list<int> > strategies(game.noPlayers, std::list<int>());
        std::list<int>::iterator it_i, it_j;
        std::vector<std::list<int>::iterator> its(game.noPlayers, std::list<int>::iterator());

        //starts with all strategies as not dominated
        for(int p=0; p<game.noPlayers; p++)
            for(int a=0; a<game.dimensions[p]; a++)
                strategies[p].push_back(a);


        //iteratively finds strictly dominated strategies
        bool changed = 1;
        while(changed)
        {
            changed = 0;

            for(int p=0; p<game.noPlayers; p++)
            {
                //finds dominated strategies
                for(it_i = strategies[p].begin(); it_i != --strategies[p].end(); )
                {
                    for(it_j = (++it_i)--; it_j != strategies[p].end();)
                    {
                        bool dominatedi = 1,
                             dominatedj = 1;

                        //checks each of the other players stragies
                        int noStrategies = 1;
                        for(int p2=0; p2<game.noPlayers; p2++)
                        {
                            its[p2] = strategies[p2].begin();
                            if(p2 != p)
                                noStrategies *= strategies[p2].size();
                        }
                        for(int s=0; s<noStrategies; s++)
                        {
                            its[p] = it_i;
                            strategyi.set(its);
                            its[p] = it_j;
                            strategyj.set(its);

                            if(game.u(p, strategyi) >= game.u(p, strategyj))
                                dominatedi = 0;
                            if(game.u(p, strategyj) >= game.u(p, strategyi))
                                dominatedj = 0;

                            if(!dominatedi && !dominatedj)
                                break;

                            //increments the iterators
                            for(int p2=game.noPlayers-1; p2>=0; p2--)
                            {
                                if(p2 != p)
                                {
                                    if(++its[p2] != strategies[p2].end())
                                        break;
                                    its[p2] = strategies[p2].begin();
                                }
                            }
                        }

                        //checks to see if one of the two strategies is dominated
                        if(dominatedi)
                        {
                            //std::cout << "deleted " << *it_i << " for player " << p << std::endl;
                            it_i = strategies[p].erase(it_i);
                            changed = 1;
                            break;
                        }
                        else if(dominatedj)
                        {
                            //std::cout << "deleted " << *it_j << " for player " << p << std::endl;
                            it_j = strategies[p].erase(it_j);
                            changed = 1;
                        }
                        else if(++it_j == strategies[p].end())
                            ++it_i;
                    }
                }
            }
        }
        return game.subGame(strategies);
    };

    //returns all pure strategy Nash equilibria of the given game
    template <class U>
    std::vector<NStrategy> pure_strategy_Nash_equilibria(NGame<U> &game)
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
                    uMax = std::max(uMax, game.u(p, strategy));
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
    std::vector<NMStrategy<U> > mixed_strategy_Nash_equilibria(NGame<U> &game); //haha, good luck writing this
}

#endif //NALGORITHMS_H_

