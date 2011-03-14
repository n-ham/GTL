#ifndef NGAME_H_
#define NGAME_H_

#include "NPlayer.h"
#include "NStrategy.h"
#include "NMStrategy.h"
#include "../maths/Tensor.h"
#include "../maths/Matrix.h"

namespace GTL
{
    /*
        This struct implements finite n-player normal form games
    */
    struct NGame
    {
        std::string name;                     //name of the game
        int noPlayers;                        //number of players
        std::vector<int> dimensions;          //dimensions of the game
        std::vector<NPlayer> players;         //players in the game
        std::vector<Tensor<UTIL> > payoffs;   //players payoff tensors

        //constructers
        NGame();
        NGame(const string &filename);

        //clear function
        void clear();

        //single io functions
        void open(const string &filename);
        void save(const string &filename);

        //utility functions
        UTIL u(int player, const NStrategy &strategy);
        std::vector<UTIL> u(const NStrategy &strategy);
        UTIL u(int player, const NMStrategy &strategy);
        std::vector<UTIL> u(const NMStrategy &strategy);

        //returns all pure strategy Nash equilibria of the strategic game
        std::vector<NStrategy> psne();
        std::vector<NMStrategy> msne(); //haha, good luck writing this
    };

    //io functions
    std::istream& operator>>(std::istream &is, NGame &game);        //input function
    std::ofstream& operator<<(std::ofstream& ofs, NGame &game);     //save function
    std::ostream& operator<<(std::ostream& os, NGame &game);        //output function
}

#endif //NGAME_H_
