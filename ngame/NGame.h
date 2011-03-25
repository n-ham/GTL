#ifndef NGAME_H_
#define NGAME_H_

#include "../Const.h"

#include "NPlayer.h"
#include "NStrategy.h"
#include "NMStrategy.h"

#include "../maths/Tensor.h"
#include "../maths/Matrix.h"

namespace GTL
{
    /*
        Struct implementing finite n-player normal form games
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
        int rowPlayer, colPlayer;

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
            print_unformatted(ofs, *this);
            ofs.close();
        };

        /*
            (expected) utility functions
        */
        //returns the players payoff for the given pure strategy
        U u(int player, const NStrategy &strategy) const
        {
            return payoffs[player][strategy];
        };

        //returns the payoffs for the given pure strategy
        std::vector<U> u(const NStrategy &strategy) const
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
            {
                is >> game.noPlayers;
                game.rowPlayer = game.noPlayers-2;
                game.colPlayer = game.noPlayers-1;
            }
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
    std::ostream& operator<<(std::ostream& os, const NGame<U> &game)
    {
        if(game.formatOutput)
            print_formatted(os, game);
        else
            print_unformatted(os, game);

        return os;
    };

    template <class U>
    void print_unformatted(std::ostream &os, const NGame<U> &game)
    {
        //adds the name of the game and the number of players to the output file
        os << "name " << game.name << std::endl;
        os << "players " << game.noPlayers << std::endl;
        os << "dimensions " << game.dimensions << std::endl;

        //adds each of the players names and their pure strategies to the output file
        for(int p=0; p<game.noPlayers; p++)
            os << "player " << game.players[p] << std::endl;

        //adds each players payoff tensor values to the output file
        for(int p=0; p<game.noPlayers; p++)
            os << "payoffs " << game.payoffs[p].tensor << std::endl;

        os << "end" << std::endl;
    };

    template <class U>
    void print_formatted(std::ostream &os, const NGame<U> &game)
    {
        //adds the name of the game to the output stream
        os << game.name << std::endl;
        os << "row player: " << game.rowPlayer << std::endl;
        os << "col player: " << game.colPlayer << std::endl;

        NStrategy strategy(game.dimensions);
        std::vector<int> excPlayers = Vector(2, game.rowPlayer, game.colPlayer);

        //for each of the players other than rowPlayer and colPlayers strategy combinations
        int noStrategies = strategy.noStrategiesExc(excPlayers);
        for(int s=0; s<noStrategies; s++)
        {
            //outputs the strategies of players that aren't rowPlayer or colPlayer
            if(game.noPlayers > 2)
            {
                std::vector<std::string> otherStrategies(game.noPlayers, "");
                for(int p=0; p<game.noPlayers; p++)
                {
                    if(p == game.rowPlayer || p == game.colPlayer)
                        otherStrategies[p] = "_";
                    else
                        otherStrategies[p] = game.players[p].actions[strategy[p] ];
                }

                os << "Other players strategies: " << join(',', otherStrategies) << std::endl;
            }

            std::vector<std::vector<std::string> > output(game.dimensions[0]+1,
                                                          std::vector<std::string>(game.dimensions[1]+1, ""));

            //gets the strategy choices of colPlayer
            for(int a1=0; a1<game.players[game.colPlayer].noActions; a1++)
                output[0][a1+1] = game.players[game.colPlayer].actions[a1];

            //gets the current lot of payoffs
            for(int a0=0; a0<game.players[game.rowPlayer].noActions; a0++)
            {
                //adds the strategy choice of rowPlayer
                output[a0+1][0] = game.players[game.rowPlayer].actions[a0];

                //adds the current row of payoffs
                for(int a1=0; a1<game.players[game.colPlayer].noActions; a1++)
                {
                    output[a0+1][a1+1] += toString(game.u(0,strategy));
                    for(int p=1; p<game.noPlayers; p++)
                        output[a0+1][a1+1] += "," + toString(game.u(p,strategy));
                    strategy.ppInc(game.colPlayer);
                }
                strategy.ppInc(game.rowPlayer);
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
                    os << output[r][c] << "  ";
                }
                os << std::endl;
            }
            os.unsetf(std::ios::left);
        }

    };
}

#endif //NGAME_H_
