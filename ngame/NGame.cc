#include "NGame.h"

using namespace std;
using namespace GTL;

GTL::NGame::NGame()
{
    clear();
};

GTL::NGame::NGame(const string &filename)
{
    open(filename);
};

void GTL::NGame::open(const string &filename)
{
    clear();
    ifstream ifs(filename.c_str());
    ifs >> *this;
    ifs.close();
};

void GTL::NGame::save(const string &filename)
{
    ofstream ofs(filename.c_str());
    ofs << *this;
    ofs.close();
};

//Clears the contents of the game
void GTL::NGame::clear()
{
    name = "";
    dimensions.clear();
    players.clear();
    payoffs.clear();
};

//returns the players payoff for the given pure strategy
UTIL GTL::NGame::u(int player, const NStrategy &strategy)
{
    return payoffs[player][strategy];
};

//returns the payoffs for the given pure strategy
vector<UTIL> GTL::NGame::u(const NStrategy &strategy)
{
    vector<UTIL> utilities;
    for(int player=0; player<noPlayers; player++)
        utilities.push_back(payoffs[player][strategy]);
    return utilities;
};

//returns the players expected payoff from the given mixed strategy
UTIL GTL::NGame::u(int player, const NMStrategy &strategy)
{
    return sdot(strategy.pr, payoffs[player]);
};

//returns the expected payoffs from the given mixed strategy
vector<UTIL> GTL::NGame::u(const NMStrategy &strategy)
{
    return smdot(strategy.pr, payoffs);
};


//returns all pure strategy Nash equilibria of the strategic game
vector<NStrategy> GTL::NGame::psne()
{
    Tensor<int> maxCount(dimensions);
    NStrategy strategy(dimensions);
    vector<NStrategy> NE;
    UTIL uMax;

    //goes through each of the players in the game
    for(int p=0; p<noPlayers; p++)
    {
        //for each possible strategy combination excluding those of player p
        int noStrategies = strategy.noStrategiesExc(p);
        for(int s=0; s<noStrategies; s++)
        {
            //finds highest attainable utility for player n given strategies of other players in s
            uMax = u(p, strategy);
            strategy.ppInc(p);
            for(int a=1; a<dimensions[p]; a++)
            {
                uMax = max(uMax, u(p, strategy));
                strategy.ppInc(p);
            }

            //finds the strategies that give player p uMax given strategies of other players in s
            for(int a=0; a<dimensions[p]; a++)
            {
                if(u(p, strategy) == uMax)
                {
                    maxCount[strategy]++;

                    if(maxCount[strategy] == noPlayers) //s is a psne
                        NE.push_back(strategy);
                }
                strategy.ppInc(p);
            }
            strategy.ppExc(p);
        }
    }

    return NE;
};

//input function
istream& GTL::operator>>(istream &is, NGame &game)
{
    game.clear();

    //gets the name of the game
    while(game.name == "")
        if(!getline(is, game.name))
        {
            is.setstate(std::ios::failbit);
            return is;
        }

    //inputs the player information
    is >> game.noPlayers;

    game.dimensions = vector<int>(game.noPlayers, 0);
    for(int p=0; p<game.noPlayers; p++)
    {
        is >> game.dimensions[p];
        game.players.push_back(NPlayer(game.dimensions[p]));
    }

    for(int p=0; p<game.noPlayers; p++)
        is >> game.players[p];

    //inputs each players payoff tensor
    game.payoffs = vector<Tensor<UTIL> >(game.noPlayers, Tensor<UTIL>(game.dimensions));
    for(int p=0; p<game.noPlayers; p++)
        is >> game.payoffs[p].tensor;

    return is;
};

//save function
ofstream& GTL::operator<<(ofstream& ofs, NGame &game)
{
    //adds the name of the game and the number of players to the output file
    ofs << game.name << endl;
    ofs << game.noPlayers << endl;
    //ofs << game.dimensions << endl;

    //adds each of the players names and their pure strategies to the output file
    for(int p=0; p<game.noPlayers; p++)
        ofs << game.players[p] << endl;

    //adds each players payoff tensor values to the output file
    for(int p=0; p<game.noPlayers; p++)
        ofs << game.payoffs[p].tensor << endl;

    return ofs;
};

//output function
ostream& GTL::operator<<(ostream& os, NGame &game)
{
    //adds the name of the game to the output stream
    os << game.name << endl;

    NStrategy strategy(game.dimensions);
    vector<int> excPlayers = Vector(2, 0, 1);

    //for each of the players other than 0 and 1's strategy combinations
    int noStrategies = strategy.noStrategiesExc(excPlayers);
    for(int s=0; s<noStrategies; s++)
    {
        //outputs the strategies of players that aren't 0 or 1
        if(game.noPlayers > 2)
        {
            os << "Other players strategies: " << game.players[2].actions[strategy[2] ];
            for(int p=3; p<game.noPlayers; p++)
                os << ", " << game.players[p].actions[strategy[p] ];
            os << endl;
        }

        vector<vector<string> > output(game.dimensions[0]+1, vector<string>(game.dimensions[1]+1, ""));

        //prints the strategy choices of player 1
        for(int a1=0; a1<game.players[1].noActions; a1++)
            output[0][a1+1] = game.players[1].actions[a1];

        //gets the current lot of payoffs
        for(int a0=0; a0<game.players[0].noActions; a0++)
        {
            output[a0+1][0] = game.players[0].actions[strategy[0] ];
            for(int a1=0; a1<game.players[1].noActions; a1++)
            {
                output[a0+1][a1+1] += toString(game.u(0, strategy));
                for(int p=1; p<game.noPlayers; p++)
                    output[a0+1][a1+1] += "," + toString(game.u(p,strategy));
                strategy.ppInc(1);
            }
            strategy.ppInc(0);
        }
        strategy.ppExc(excPlayers);

        //works out the output width of each column
        vector<int> colWidth(output[0].size(), 0);
        for(int r=0; r<(int)output.size(); r++)
            for(int c=0; c<(int)output[r].size(); c++)
                colWidth[c] = max(colWidth[c], (int)output[r][c].size());

        //outputs the current lot of payoffs
        os.setf(ios::left);
        for(int r=0; r<(int)output.size(); r++)
        {
            for(int c=0; c<(int)output[r].size(); c++)
            {
                os.width(colWidth[c]);

                os << output[r][c] << " ";
            }
            os << endl;
        }
        os.unsetf(ios::left);
    }

    return os;
};
