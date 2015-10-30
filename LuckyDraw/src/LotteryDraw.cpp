#include "LotteryDraw.h"

#include <algorithm>

//#define TRACE_ENABLE
#include "TraceLog.hpp"

LotteryDraw::LotteryDraw()
    : mDatabase(),
      mHistory(),
      mDBCursor(0),
      mGenerator(false),
      mPicked(nullptr),
      mAlternEntry(new LotteryEntry())
{}


LotteryDraw::~LotteryDraw()
{
}

void LotteryDraw::load(const std::string& file)
{
    /* TO-DO*/
}

void LotteryDraw::load(const std::vector<std::string>& dataList)
{
    for(size_t i = 0; i < dataList.size(); ++i)
    {
        EntryPtr p_entry(new LotteryEntry(dataList[i]));
        addEntry(std::move(p_entry));
    }
    mGenerator.reset(false);
}

void LotteryDraw::addEntry(LotteryDraw::EntryPtr p_entry)
{
    mGenerator.addEntry(p_entry.get());
    mDatabase.push_back(std::move(p_entry));
}

void LotteryDraw::deleteEntry(const LotteryEntry& entry)
{
    /* TO-DO */
}

void LotteryDraw::deleteAllEntries()
{
    mHistory.clear();
    mDatabase.clear();
    mGenerator.clear();
}

const LotteryEntry* LotteryDraw::pick()
{
    mPicked = mGenerator.getNextFromList();

    if (mPicked == nullptr)
    {
        // TO-DO
        TRACE("LotteryDraw.cpp", "No entry has been picked");
        return mPicked;
    }

    mHistory.push_back(mPicked->data);
    return mPicked;
}

void LotteryDraw::clearHistory()
{
    mHistory.clear();
}

void LotteryDraw::eraseFromHistory(const LotteryEntry& entry)
{
    auto it = mHistory.begin();
    while(it != mHistory.end())
    {
        if(*it == entry.data)
        {
            mHistory.erase(it);
            mGenerator.eraseFromHistory(entry); // not implemented
            return;
        }
        ++it;
    }
    TRACE("LotteryDraw.cpp", "Erase did not happen!");
}

void LotteryDraw::eraseLastFromHistory()
{
    if (!mHistory.empty()) { mHistory.pop_back(); }
    mGenerator.eraseLastFromHistory();
    mPicked = nullptr;
}

void LotteryDraw::reset(bool repeatable)
{
    mHistory.clear();
    mGenerator.reset(repeatable);
}

const LotteryEntry* LotteryDraw::getNext()
{
    LotteryEntry* entry = mDatabase[mDBCursor++].get();
    if (mDBCursor >= mDatabase.size()) { mDBCursor = 0; }
    return entry;
}

const LotteryEntry* LotteryDraw::getPicked() const
{
    return mPicked;
}

const std::vector<std::string>& LotteryDraw::getHistory() const
{
    return mHistory;
}

const LotteryEntry* LotteryDraw::operator[](size_t index) const
{
    return mDatabase[index].get();
}

size_t LotteryDraw::getNumOfEntries() const
{
    return mDatabase.size();
}

bool LotteryDraw::isAllPickedOut() const
{
    return mGenerator.isAllPickedOut();
}

const std::string LotteryDraw::getHistoryInStr() const
{
    if (mHistory.empty()) { return ""; }

    std::string str;
    size_t i = 0;
    while (i < mHistory.size())
    {
        str += std::to_string(i + 1);
        str += ".";
        str += mHistory[i];
        str += '\n';
        ++i;
    }
    return str;
}

const LotteryEntry* LotteryDraw::getNextRand()
{
    std::string num;
    for (int i = 0; i < 4; ++i)
    {
        num += std::to_string(mGenerator.getRandUint(0, 9));
    }
    mAlternEntry->data = num;
    return mAlternEntry.get();
}