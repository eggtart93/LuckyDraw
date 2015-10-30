#ifndef RANDGENERATOR_H
#define RANDGENERATOR_H

#include "RandEngine.h"

#include "LotteryEntry.h"

#include <vector>
#include <deque>

class RandGenerator
{
    public:
    typedef LotteryEntry Entry;

    public:
        RandGenerator(bool repeatable);

        void addEntry(Entry* entry);
        void deleteEntry(const Entry& entry);
        Entry* getNextFromList();
        void eraseLastFromHistory();
        void eraseFromHistory(const Entry& entry);
        void clear();
        bool isRepeatable() const;
        bool isAllPickedOut() const;
        void reset(bool repeatable);

        unsigned int getRandUint(unsigned int floor, unsigned ceil);

    private:
    void repopulate();

    private:
        RandEngine mEngine;
        std::deque<Entry*> mRandDeque;
        std::vector<Entry*> mHistory;
        size_t mDirtyCount;
        bool mIsRepeatable;
        bool mHasToReset;
};

#endif // RANDGENERATOR_H
