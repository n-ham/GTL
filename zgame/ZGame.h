#ifndef ZGAME_H
#define ZGAME_H

#include "../Const.h"

#include "ZPlayer.h"
#include "ZStrategy.h"
#include "ZMStrategy.h"

#include "../maths/Matrix.h"

//Zero-Sum Two Player Games
namespace GTL
{
    //T == type of payoffs
    template <class U>
    struct ZGame
    {
        std::string name;                       //name of the game
        std::vector<int> dimensions;            //game dimensions
        std::vector<ZPlayer> players;           //players in the game
        Matrix<U> payoffs;                      //player 0's payoffs

        bool formatOutput;

        ZGame<U>()
        {
            clear();
        };

        ZGame<U>(const std::string &filename)
        {
            clear();
            open(filename);
        };

        void clear()
        {
            name = "";
            formatOutput = 1;
            dimensions.clear();
            players.clear();
            payoffs.clear();
        };

        void open(const std::string &filename)
        {
            clear();
            std::ifstream ifs(filename.c_str());
            ifs >> *this;
            ifs.close();
        };

        void save(const std::string &filename)
        {
            std::ofstream ofs(filename.c_str());
            print_unformatted(ofs);
            ofs.close();
        };

        U u(int player, const ZStrategy &strategy)
        {
            if(player = 0)
                return payoffs[strategy[0] ][strategy[1] ];
            else
                return -payoffs[strategy[0] ][strategy[1] ];
        };

        std::vector<U> u(const ZStrategy &strategy)
        {
            std::vector<U> utilities(1, payoffs[strategy[0] ][strategy[1] ]);
            utilities.push_back(-utilities[0]);
            return utilities;
        };

        U u(int player, const ZMStrategy<U> &s)
        {
            if(player == 0)
                return sdot(s.pr, payoffs);
            return -sdot(s.pr, payoffs);
        };

        std::vector<U> u(const ZMStrategy<U> &s)
        {
            std::vector<U> ans(1, sdot(s.pr, payoffs));
            ans.push_back(-ans[0]);
            return ans;
        };

        void print_unformatted(std::ostream &os)
        {
            os << "name " << name << std::endl;
            os << "dimensions" << dimensions[0] << " " << dimensions[1] << std::endl;

            //adds each of the players to the output stream
            for(int p=0; p<2; p++)
                os << "player " << players[p] << std::endl;

            os << "payoffs" << std::endl;
            os << payoffs << std::endl;

            os << "end" << std::endl;
        };

        void print_formatted(std::ostream &os)
        {
            os << name << std::endl;
            std::string output[dimensions[0]+1][dimensions[1]+1];

            for(int acol=0; acol<dimensions[1]; acol++)
                output[0][acol+1] = players[1].actions[acol];

            for(int arow=0; arow<dimensions[0]; arow++)
            {
                output[arow+1][0] = players[0].actions[arow];
                for(int acol=0; acol<dimensions[1]; acol++)
                    output[arow+1][acol+1] += toString(payoffs[arow][acol]) + "," + toString(-payoffs[arow][acol]);
            }

            //works out the output width of each column
            std::vector<int> colWidth(dimensions[1], 0);
            for(int r=0; r<dimensions[0]+1; r++)
                for(int c=0; c<dimensions[1]+1; c++)
                    colWidth[c] = std::max(colWidth[c], (int)output[r][c].size());

            //outputs the current lot of payoffs
            os.setf(std::ios::left);
            for(int r=0; r<dimensions[0]+1; r++)
            {
                for(int c=0; c<dimensions[1]+1; c++)
                {
                    os.width(colWidth[c]);

                    os << output[r][c] << "  ";
                }
                os << std::endl;
            }
            os.unsetf(std::ios::left);
        };

    };

    //input function
    template <class U>
    std::istream& operator>>(std::istream &is, ZGame<U> &game)
    {
        game.clear();
        std::string lineType, junk;

        int p=0;
        while(is >> lineType)
        {
            if(lineType == "name")
                getline(is.ignore(1), game.name);
            else if(lineType == "dimensions")
            {
                game.dimensions = std::vector<int>(2, 0);
                for(int p=0; p<2; p++)
                {
                    is >> game.dimensions[p];
                    game.players.push_back(ZPlayer(game.dimensions[p]));
                }
                game.payoffs = Matrix<U>(game.dimensions[0], game.dimensions[1], 0);
            }
            else if(lineType == "player")
                is >> game.players[p++];
            else if(lineType == "payoffs")
                is >> game.payoffs;
            else if(lineType == "end")
                break;
            else
                getline(is, junk);
        }

        return is;
    };

    //output function
    template <class U>
    std::ostream& operator<<(std::ostream &os, ZGame<U> &game)
    {
        if(game.formatOutput)
            game.print_formatted(os);
        else
            game.print_unformatted(os);

        return os;
    };
}

#endif
