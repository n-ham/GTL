#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>

namespace GTL
{

    struct Timer
    {
        timeval timer;
        double startTime, currentTime;

        Timer()
        {
            gettimeofday(&timer, NULL);
            startTime = timer.tv_sec+(timer.tv_usec/1000000.0);
        };

        void restart_timer()
        {
            gettimeofday(&timer, NULL);
            startTime = timer.tv_sec+(timer.tv_usec/1000000.0);
        };

        double get_time()
        {
            gettimeofday(&timer, NULL);
            currentTime = timer.tv_sec+(timer.tv_usec/1000000.0);
            return currentTime-startTime;
        };
    };
}

#endif //TIMER_H_
