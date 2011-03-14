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
        int index;                //index of strategies position in a payoff tensor
        std::vector<int> strategy;          //strategy choices
        std::vector<int> dimensions;        //dimensions of the game

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
        void operator++(int);
    };

    std::istream& operator>>(std::istream &is, NStrategy &strategy);               //input function
    std::ofstream& operator<<(std::ofstream &ofs, const NStrategy &strategy);      //save function
    std::ostream& operator<<(std::ostream &os, const NStrategy &strategy);         //output function
}

#endif //NSTRATEGY_H_

