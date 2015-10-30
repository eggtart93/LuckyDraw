#include "..\include\LotteryEntry.h"


LotteryEntry::LotteryEntry(const std::string& str)
    :  data(str)
{}

LotteryEntry::~LotteryEntry() {}

bool LotteryEntry::operator==(const LotteryEntry& other) const
{
    return (data == other.data);
}

bool LotteryEntry::operator!=(const LotteryEntry& other) const
{
    return (data != other.data);
}