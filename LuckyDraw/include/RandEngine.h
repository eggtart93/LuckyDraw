#ifndef RANDENGINE_H
#define RANDENGINE_H

#include <random>

class RandEngine
{
    public:
        RandEngine();

        /*
            Parameters: 
            floor included lower bound
            ceil  excluded upper bound
        */
        unsigned int getNext(unsigned int floor, unsigned int ceil);
        void reset();

    private:
        std::mt19937 mRandom;
};

#endif // RANDENGINE_H
