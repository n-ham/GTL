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

        //increment operators
        void operator++();      //pre-fix
        void operator++(int);   //post-fix

        //index function
        int& operator[](int player) const;
    };

    std::istream& operator>>(std::istream &is, ZStrategy &strategy);        //input function
    std::ostream& operator<<(std::ostream &os, const ZStrategy &strategy);  //output function
}

#endif //ZSTRATEGY_H_
