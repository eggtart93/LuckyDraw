#include "Test.h"

//#include "RandomPick.hpp"
#include <SFML/System/String.hpp>
#include "LotteryDraw.h"
#include "Utility.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>

#include "FileIO.h"

uint16_t getDigits(int n)
{
    uint16_t digits = 0;
    while( n / 10 != 0)
    {
        n = n / 10;
        digits++;
    }
    return digits;
}


void testRandEngine()
{
    std::cout << "instantiate RandEngine..." << std::endl;
    RandEngine random;
    std::cout << "\nGenerate random 10 random integers from [3, 16): ";
    for(int i = 0; i < 10; ++i)
    {
        std::cout<< random.getNext(3, 16) << ", ";
    }

    std::cout << "\n\neset RandomEngine..." << std::endl;
    random.reset();

    std::cout <<"\ngenerate 1000000 ints from [0, 100) to test speed..." << std::endl;
    auto timeStart = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000000; ++i)
    {
        unsigned int n = random.getNext(0, 100);
    }
    auto timeEnd = std::chrono::high_resolution_clock::now();
    auto timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms" << std::endl;
}


void testLotteryDraw()
{
    size_t list_size = 10000;
    uint16_t max_digits = getDigits(list_size);
    std::vector<std::string> data_list(list_size);
    for (size_t i = 0; i < list_size; ++i)
    {
        data_list[i] = std::string(max_digits - getDigits(i), '0') + std::to_string(i);
    }

    std::cout << "instantiate LotteryDraw ..." << std::endl;
    LotteryDraw lottery;

    std::cout << "load data ..." << std::endl;
    lottery.load(data_list);

    std::cout << "pick " << list_size << " items: " << std::endl;
    auto timeStart = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < data_list.size(); ++i)
    {
        lottery.pick();
        //std::cout << lottery.pick(err)->data << std::endl;
    }
    auto timeEnd = std::chrono::high_resolution_clock::now();
    auto timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms"
              << ", Target time: " << 1000.f * (1.f / 60.f) << " ms" << std::endl;


    size_t history_size = lottery.getHistory().size();
    std::cout << "\nerase last " << history_size << " from history ..." << std::endl;
    timeStart = std::chrono::high_resolution_clock::now();
    for(uint16_t i = 0; i < history_size; ++i)
    {
        lottery.eraseLastFromHistory();
    }
    timeEnd = std::chrono::high_resolution_clock::now();
    timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms" << std::endl;

//
    std::cout << "\npick all again ..." << std::endl;
    timeStart = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < list_size; ++i)
    {
        lottery.pick();
        //std::cout << lottery.pick(err)->data << std::endl;
    }
    timeEnd = std::chrono::high_resolution_clock::now();
    timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms"
              << ", Target time: " << 1000.f * (1.f / 60.f) << " ms" << std::endl;


    //std::cout << "\nerase " << lottery.getHistory().size()
    //          << ", eraseFromHistory() ..." << std::endl;
    //timeStart = std::chrono::high_resolution_clock::now();
    //for(size_t i = 0; i < list_size; ++i)
    //{
    //    lottery.eraseFromHistory(*(lottery.getHistory().back()));
    //    //std::cout << lottery.pick(err)->data << std::endl;
    //}
    //timeEnd = std::chrono::high_resolution_clock::now();
    //timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    //std::cout << "Completed, time used: " << timeUsed << " ms" << std::endl;

    //std::cout << "\npick all again ..." << std::endl;
    //timeStart = std::chrono::high_resolution_clock::now();
    //for(size_t i = 0; i < list_size; ++i)
    //{
    //    LotteryDraw::ErrorCode err;
    //    lottery.pick(err);
    //    //std::cout << lottery.pick(err)->data << std::endl;
    //}
    //timeEnd = std::chrono::high_resolution_clock::now();
    //timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    //std::cout << "Completed, time used: " << timeUsed << " ms"
    //          << ", Target time: " << 1000.f * (1.f / 60.f) << " ms" << std::endl;





    std::cout << "\nreset to repeatable..." << std::endl;
    timeStart = std::chrono::high_resolution_clock::now();
    lottery.reset(true);
    timeEnd = std::chrono::high_resolution_clock::now();
    timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms" << std::endl;

    std::cout << "\npick twice amounts of total size ... " << std::endl;
    timeStart = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < 2 * list_size; ++i)
    {
        lottery.pick();
        //std::cout << lottery.pick(err)->data << std::endl;
    }
    timeEnd = std::chrono::high_resolution_clock::now();
    timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms"
              << ", Target time: " << 1000.f * (1.f / 60.f) << " ms" << std::endl;


    std::cout << "\nreset back to non repeatable..." << std::endl;
    lottery.reset(false);

    std::cout << "\npick " << list_size << "+1, should see one error ... " << std::endl;
    timeStart = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < list_size+1; ++i)
    {
        lottery.pick();
        //std::cout << lottery.pick(err)->data << std::endl;
    }
    timeEnd = std::chrono::high_resolution_clock::now();
    timeUsed = std::chrono::duration<double, std::milli>(timeEnd-timeStart).count();
    std::cout << "Completed, time used: " << timeUsed << " ms"
              << ", Target time: " << 1000.f * (1.f / 60.f) << " ms" << std::endl;




    std::cout << "\ndelete all entries ..." << std::endl;
    lottery.deleteAllEntries();

    std::vector<std::string> small_list(10);
    for(int i = 0; i < 10; ++i)
    {
        small_list[i] = std::to_string(i*10);
    }
    std::cout <<"\nadd 10 items ..." << std::endl;
    lottery.load(small_list);

    std::cout << "pick 5 items ..." << std::endl;
    for(size_t i = 0; i < 5; ++i)
    {
        std::cout << lottery.pick()->data << std::endl;
    }

    std::cout << "print history: " << std::endl;
    for(size_t i = 0; i < lottery.getHistory().size(); i++)
    {
        std::cout << (lottery.getHistory())[i] << std::endl;
    }
}


void runUtilityTest()
{
    std::string orig_str("0189");
    std::string new_str;
    if (isStringOfNum(orig_str))
    {
        std::cout << "This is a string of numbers" << std::endl;
        new_str = insertSpaces(2, orig_str);
    }
    std::cout << new_str << ", length = "
              << new_str.size() << std::endl;
}

void runFileIOTest()
{
    FileIO fileIO("dat/db.dat", "dat/sys.log");

    fileIO.writeLog("File created");

    std::vector<std::string> data_read = fileIO.readData();
    for(auto it = data_read.begin(); it != data_read.end(); ++it)
    {
        fileIO.writeLog("picked" + (*it));
        std::cout << *it << std::endl;
    }

    fileIO.writeLog("Data read.");
}

std::wstring ReadUTF16(const std::string & filename)
{
    // Thanks to neminem for reminding us that we need
    // std::ios::binary for Windows
    std::ifstream file(filename.c_str(), std::ios::binary);
    std::stringstream ss;
    ss << file.rdbuf() << '\0';
    return std::wstring((wchar_t *)ss.str().c_str());
}

const std::string getHistoryInStr(std::vector<std::string>& lst)
{
    std::string str;
    size_t i = 0;
    while (i < lst.size())
    {
        str += std::to_string(i + 1);
        str += ". ";
        str += lst[i];
        str += '\n';
        ++i;
    }
    return str;
}

std::string trimTextContent(std::vector<std::string>& lst, std::vector<sf::String>& hiddenStrTop)
{
    sf::String str = getHistoryInStr(lst);
    size_t num_line = 0;
    for(size_t i = 0; i < str.getSize(); ++i) { if(str[i]=='\n') {num_line++;} }

    size_t pos = 0;
    while (num_line > 3 && !str.isEmpty())
    {
        sf::String buf;
        pos = str.find('\n', 0);
        if (pos == sf::String::InvalidPos) { break; }

        for (size_t i = 0; i <= pos; ++i) { buf += str[i]; }
        hiddenStrTop.push_back(buf);

        str.erase(0, pos + 1);
        num_line = 0;
        for(size_t i = 0; i < str.getSize(); ++i) { if(str[i]=='\n') {num_line++;} }
    }
    return str.toAnsiString();
}

void runTrimTextTest()
{
    std::vector<std::string> lst = {"0000", "0001", "0002", "0003", "0004", "0005", "0006", "0007"};
    std::vector<sf::String> hiddenStr;
    std::string new_str = trimTextContent(lst, hiddenStr);
    std::cout << new_str;
    std::cout << "end" << std::endl;

    std::cout << "\nHidden String: " << std::endl;
    for(size_t i = 0; i < hiddenStr.size(); ++i)
    {
        std::cout << hiddenStr[i].toAnsiString();
    }
    std::cout << "end" << std::endl;
}

void runTest()
{
    //testRandEngine();
    //testRandGenerator();
    //testRandomPick();
    //testLotteryDraw();
    //runUtilityTest();
    //runFileIOTest();
    runTrimTextTest();
}

