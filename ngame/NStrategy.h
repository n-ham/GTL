#ifndef NSTRATEGY_H_
#define NSTRATEGY_H_

#include "../Const.h"

//mixed strategies for normal form games
namespace GTL
{
    struct NStrategy
    {
        /*
            variables
        */
        int index;                       //tensor index
        std::vector<int> dimensions;     //game dimensions
        std::vector<int> strategy;       //strategy choices

        /*
            functions
        */
        //constructors
        NStrategy(const std::vector<int> &Dimensions);
        NStrategy(const std::vector<int> &Dimensions, const std::vector<int> &Strategy);

        //index functions
        int& operator[](int player) const;
        void calculateIndex();

        //set functions
        void set(const std::vector<int> &Strategy);
        void reset();

        //number of strategies functions
        int noStrategiesExc(int excPlayer);
        int noStrategiesExc(const std::vector<int> &excPlayers);
        int noStrategiesInc(int incPlayer);
        int noStrategiesInc(const std::vector<int> &incPlayers);
        int noStrategies();

        //increment functions
        void ppExc(int excPlayer);
        void ppExc(const std::vector<int> &excPlayers);
        void ppInc(int incPlayer);
        void ppInc(const std::vector<int> &incPlayers);
        void operator++();
        void operator++(int);
    };

    std::istream& operator>>(std::istream &is, NStrategy &strategy);               //input function
    std::ostream& operator<<(std::ostream &os, const NStrategy &strategy);         //output function
}

#endif //NSTRATEGY_H_
