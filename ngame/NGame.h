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
    template <class U>
    struct NGame
    {
        /*
            variables
        */
        std::string name;                     //name of the game
        int noPlayers;                        //number of players
        std::vector<int> dimensions;          //dimensions of the game
        std::vector<NPlayer> players;         //players in the game
        std::vector<Tensor<U> > payoffs;      //players payoff tensors

        bool formatOutput;

        /*
            functions
        */
        //constructers
        NGame<U>()
        {
            clear();
        };

        NGame<U>(const std::string &filename)
        {
            clear();
            open(filename);
        };

        //clear function
        void clear()
        {
            name = "";
            formatOutput = 1;
            dimensions.clear();
            players.clear();
            payoffs.clear();
        };

        //single io functions
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
            print(ofs);
            ofs.close();
        };

        /*
            (expected) utility functions
        */
        //returns the players payoff for the given pure strategy
        U u(int player, const NStrategy &strategy)
        {
            return payoffs[player][strategy];
        };

        //returns the payoffs for the given pure strategy
        std::vector<U> u(const NStrategy &strategy)
        {
            std::vector<U> utilities;
            for(int player=0; player<noPlayers; player++)
                utilities.push_back(payoffs[player][strategy]);
            return utilities;
        };

        //returns the players expected payoff from the given mixed strategy
        U u(int player, const NMStrategy<U> &strategy)
        {
            return sdot(strategy.pr, payoffs[player]);
        };

        //returns the expected payoffs from the given mixed strategy
        std::vector<U> u(const NMStrategy<U> &strategy)
        {
            return smdot(strategy.pr, payoffs);
        };

        void print(std::ostream &os)
        {
            //adds the name of the game and the number of players to the output file
            os << "name " << name << std::endl;
            os << "players " << noPlayers << std::endl;
            os << "dimensions " << dimensions << std::endl;

            //adds each of the players names and their pure strategies to the output file
            for(int p=0; p<noPlayers; p++)
                os << "player " << players[p] << std::endl;

            //adds each players payoff tensor values to the output file
            for(int p=0; p<noPlayers; p++)
                os << "payoffs " << payoffs[p].tensor << std::endl;

            os << "end" << std::endl;
        };

        void print_readable(std::ostream &os)
        {
            //adds the name of the game to the output stream
            os << name << std::endl;

            NStrategy strategy(dimensions);
            std::vector<int> excPlayers = Vector(2, 0, 1);

            //for each of the players other than 0 and 1's strategy combinations
            int noStrategies = strategy.noStrategiesExc(excPlayers);
            for(int s=0; s<noStrategies; s++)
            {
                //outputs the strategies of players that aren't 0 or 1
                if(noPlayers > 2)
                {
                    os << "Other players strategies: " << players[2].actions[strategy[2] ];
                    for(int p=3; p<noPlayers; p++)
                        os << ", " << players[p].actions[strategy[p] ];
                    os << std::endl;
                }

                std::vector<std::vector<std::string> > output(dimensions[0]+1,
                                                              std::vector<std::string>(dimensions[1]+1, ""));

                //gets the strategy choices of player 1
                for(int a1=0; a1<players[1].noActions; a1++)
                    output[0][a1+1] = players[1].actions[a1];

                //gets the current lot of payoffs
                for(int a0=0; a0<players[0].noActions; a0++)
                {
                    output[a0+1][0] = players[0].actions[strategy[0] ];
                    for(int a1=0; a1<players[1].noActions; a1++)
                    {
                        output[a0+1][a1+1] += toString(u(0, strategy));
                        for(int p=1; p<noPlayers; p++)
                            output[a0+1][a1+1] += "," + toString(u(p,strategy));
                        strategy.ppInc(1);
                    }
                    strategy.ppInc(0);
                }
                strategy.ppExc(excPlayers);

                //works out the output width of each column
                std::vector<int> colWidth(output[0].size(), 0);
                for(int r=0; r<(int)output.size(); r++)
                    for(int c=0; c<(int)output[r].size(); c++)
                        colWidth[c] = std::max(colWidth[c], (int)output[r][c].size());

                //outputs the current lot of payoffs
                os.setf(std::ios::left);
                for(int r=0; r<(int)output.size(); r++)
                {
                    for(int c=0; c<(int)output[r].size(); c++)
                    {
                        os.width(colWidth[c]);

                        os << output[r][c] << " ";
                    }
                    os << std::endl;
                }
                os.unsetf(std::ios::left);
            }

        };

    };

    /*
        io functions
    */
    //input function
    template <class U>
    std::istream& operator>>(std::istream &is, NGame<U> &game)
    {
        game.clear();
        std::string lineType, junk;

        int p=0, t=0;
        while(is >> lineType)
        {
            if(lineType == "name")
                getline(is.ignore(1), game.name);
            else if(lineType == "players")
                is >> game.noPlayers;
            else if(lineType == "dimensions")
            {
                game.dimensions = std::vector<int>(game.noPlayers, 0);
                for(int p=0; p<game.noPlayers; p++)
                {
                    is >> game.dimensions[p];
                    game.players.push_back(NPlayer(game.dimensions[p]));
                }
                game.payoffs = std::vector<Tensor<U> >(game.noPlayers, Tensor<U>(game.dimensions));
            }
            else if(lineType == "player")
                is >> game.players[p++];
            else if(lineType == "payoffs")
                is >> game.payoffs[t++].tensor;
            else if(lineType == "end")
                break;
            else
                getline(is, junk);
        }

        //creates generic information if none was provided
        if(game.name == "")
            game.name = join("x", game.dimensions) + " normal form game";
        for(; p<game.noPlayers; p++)
        {
            game.players[p].name = "p" + toString(p);
            for(int a=0; a<game.players[p].noActions; a++)
                game.players[p].actions[a] = toString(a);
        }

        return is;
    };

    //output function
    template <class U>
    std::ostream& operator<<(std::ostream& os, NGame<U> &game)
    {
        if(game.formatOutput)
            game.print_readable(os);
        else
            game.print(os);

        return os;
    };
}

#endif //NGAME_H_
