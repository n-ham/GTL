#ifndef ENGINE_H_
#define ENGINE_H_

//#include "../Const.h"

#include "../tools/Process.h"
#include "../ngame/NGame.h"

namespace GTL
{
    struct Engine
    {

        Engine()
        {

        };

        //plays a single game returning the payoffs to each bot
        template <class U>
        std::vector<U> playGame(NGame<U> game, const std::vector<std::string> &playerFiles)
        {
            Process process;
            game.formatOutput = 0;

            //lets each player pick their move
            std::vector<int> strategyChoices(game.noPlayers, 0);
            for(int p=0; p<game.noPlayers; p++)
            {
                //Process process;
                process.start(playerFiles[p]);
                process << game << std::endl;
                process >> strategyChoices[p];
                process.stop();
            }

            return game.u(NStrategy(game.dimensions, strategyChoices));
        };
    };
}

#endif //ENGINE_H_

