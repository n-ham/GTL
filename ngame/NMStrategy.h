#ifndef NMSTRATEGY_H_
#define NMSTRATEGY_H_

#include "../Const.h"
#include "../maths/Tensor.h"

//mixed strategies for normal form games
namespace GTL
{
    struct NMStrategy
    {
        std::vector<std::vector<PROB> > mstrategy;
        Tensor<PROB> pr;

        //constructors
        NMStrategy(const std::vector<int> &Dimensions);
        NMStrategy(const std::vector<int> &Dimensions, const std::vector<std::vector<PROB> > &Strategy);

        //index function
        std::vector<PROB>& operator[](int player) const;

        //set functions
        void set(const std::vector<std::vector<PROB> > &Strategy);
        void reset();   //assigns each players strategy an equal weighting

        //forms the probability tensor
        void formp();
        void formp2();
    };

    std::istream& operator>>(std::istream &is, NMStrategy &strategy);           //input function
    std::ofstream& operator<<(std::ofstream &ofs, const NMStrategy &strategy);  //save function
    std::ostream& operator<<(std::ostream &os, const NMStrategy &strategy);     //print function
}

#endif //NMSTRATEGY_H_
