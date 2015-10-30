#include "RandGenerator.h"

#include "Utility.hpp"

#include <algorithm>

//#define TRACE_ENABLE
#include "TraceLog.hpp"
RandGenerator::RandGenerator(bool repeatable)
: mEngine(),
  mRandDeque(),
  mHistory(),
  mDirtyCount(0),
  mIsRepeatable(repeatable),
  mHasToReset(false)
{}

void RandGenerator::addEntry(Entry* entry)
{
    mRandDeque.push_back(entry);
    mHasToReset = true;
}

void RandGenerator::deleteEntry(const Entry& entry)
{
//    auto it = mRandDeque.begin();
//    while (it != mRandDeque.end())
//    {
//        if (*(*it) == entry)
//        {
//            mRandDeque.erase(it);
//        }
//        ++it;
//    }
//
//    if (!mIsRepeatable)
//    {
//        auto it2 = mHistory.begin();
//        while (it2 != mHistory.end())
//        {
//            if (*(*it2) == entry)
//            {
//                mHistory.erase(it2);
//            }
//            ++it2;
//        }
//    }
}

RandGenerator::Entry* RandGenerator::getNextFromList()
{
    if (mRandDeque.empty())
    {
        TRACE("RandGenerator.cpp", "mRandDeque is empty");
        return nullptr;
    }

//    if (mHasToReset || mDirtyCount >= mRandDeque.size() + mHistory.size())
//    {
//        repopulate();
//    }

    ++mDirtyCount;
    if (!mIsRepeatable)
    {
        mHistory.push_back(mRandDeque.front());
        mRandDeque.pop_front();
        return mHistory.back();
    }
    else
    {
        return mRandDeque.front();
    }
}

void RandGenerator::eraseLastFromHistory()
{
    if (mHistory.empty()) { return; }

    mRandDeque.push_back(mHistory.back());
    mHistory.pop_back();
}

void RandGenerator::eraseFromHistory(const Entry& entry)
{
    /* TO-DO*/
}

void RandGenerator::reset(bool repeatable)
{
    repopulate();
    mHistory.clear();
    mIsRepeatable = repeatable;
}

void RandGenerator::clear()
{
    mRandDeque.clear();
    mHistory.clear();
}

bool RandGenerator::isRepeatable() const
{
    return mIsRepeatable;
}

bool RandGenerator::isAllPickedOut() const
{
    return mRandDeque.empty() && !mHistory.empty();
}

void RandGenerator::repopulate()
{
    std::vector<Entry*> buffer(mRandDeque.size() + mHistory.size());
    size_t pos;

    while (!mRandDeque.empty())
    {
        pos = mEngine.getNext(0, buffer.size());
        while (buffer[pos] != nullptr)
        {
            pos++;
            if (pos >= buffer.size()) { pos = 0; }
        }
        buffer[pos] = mRandDeque.front();
        mRandDeque.pop_front();
    }

    while (!mHistory.empty())
    {
        pos = mEngine.getNext(0, buffer.size());
        while (buffer[pos] != nullptr)
        {
            pos++;
            if (pos >= buffer.size()) { pos = 0; }
        }
        buffer[pos] = mHistory.back();
        mHistory.pop_back();
    }

    for (auto it = buffer.begin(); it != buffer.end(); ++it)
    {
        mRandDeque.push_front(*it);
    }

    mDirtyCount = 0;
    mHasToReset = false;
}

unsigned int RandGenerator::getRandUint(unsigned int floor, unsigned ceil)
{
    return mEngine.getNext(floor, ceil);
}