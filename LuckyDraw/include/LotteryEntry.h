#pragma once

#include <string>

class LotteryDraw;

class LotteryEntry
{
    public:
    friend class LotteryDraw;

    public:
    LotteryEntry() = default;
    LotteryEntry(const std::string& str);
    ~LotteryEntry();

    bool operator==(const LotteryEntry& other) const;
    bool operator!=(const LotteryEntry& other) const;

    std::string data;
};

