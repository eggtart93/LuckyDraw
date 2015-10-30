#include "RandEngine.h"

#include <chrono>

RandEngine::RandEngine()
: mRandom(std::chrono::system_clock::now().time_since_epoch().count())
{}

unsigned int RandEngine::getNext(unsigned int floor, unsigned int ceil)
{
    if(floor == ceil) { return floor; }
    else if(floor > ceil) { return 0; }
    else
    {
        unsigned int result = mRandom() % (ceil - floor);
        if (result < floor)
        {
            result += floor;
        }
        return result;
    }
}

void RandEngine::reset()
{
    unsigned long new_seed =  std::chrono::system_clock::now().time_since_epoch().count();
    mRandom.seed(new_seed);
}
