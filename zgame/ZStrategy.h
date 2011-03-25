#ifndef ZSTRATEGY_H
#define ZSTRATEGY_H

#include "../Const.h"

//pure strategies for two player zero sum games
namespace GTL
{
    struct ZStrategy
    {
        std::vector<int> dimensions, strategy;

        //constructers
        ZStrategy(const std::vector<int> &Dimensions);
        ZStrategy(const std::vector<int> &Dimensions, const std::vector<int> &Strategy);
        ZStrategy(const std::vector<int> &Dimensions, int s1, int s2);

        //index operator
        int& operator[](int player) const;

        //set functions
        void set(const std::vector<int> &Strategy);
        void set(int s1, int s2);
        void setp(int player, int s);
        void reset();

        //number of strategies functions
        int noStrategiesExc(int excPlayer);
        int noStrategiesInc(int incPlayer);
        int noStrategies();

        //increment operators
        void ppExc(int excPlayer);
        void ppInc(int incPlayer);
        void operator++();      //pre-fix
        void operator++(int);   //post-fix
    };

    std::istream& operator>>(std::istream &is, ZStrategy &strategy);        //input function
    std::ostream& operator<<(std::ostream &os, const ZStrategy &strategy);  //output function
}

#endif //ZSTRATEGY_H_
