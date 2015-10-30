#pragma once

#include "RandGenerator.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class LotteryDraw
{
    public:
    typedef std::unique_ptr<LotteryEntry> EntryPtr;

    public:
    LotteryDraw();
    ~LotteryDraw();

    void load(const std::string& file);
    void load(const std::vector<std::string>& dataList);
    void addEntry(const EntryPtr entry);
    void deleteEntry(const LotteryEntry& entry); // not implemented
    void deleteAllEntries();

    const LotteryEntry* pick();

    void clearHistory();
    void eraseFromHistory(const LotteryEntry& entry); // not supported
    void eraseLastFromHistory();
    void reset(bool repeatable);

    const LotteryEntry* getPicked() const;
    const LotteryEntry* getNext();
    const std::vector<std::string>& getHistory() const;
    const std::string getHistoryInStr() const;
    const LotteryEntry* operator[](size_t index) const;
    size_t getNumOfEntries() const;
    bool isAllPickedOut() const;

    const LotteryEntry* getNextRand();

    private:
    std::vector<EntryPtr> mDatabase;
    std::vector<std::string> mHistory;
    size_t mDBCursor;
    RandGenerator mGenerator;
    LotteryEntry* mPicked;
    EntryPtr mAlternEntry;
};

