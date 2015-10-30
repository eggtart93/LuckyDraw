#include "FileIO.h"

#include <chrono>
#include <ctime>
#include <locale>

//#define TRACE_ENABLE
#include "TraceLog.hpp"

#define MAX_NUM_CHARACTERS 80
FileIO::FileIO(const std::string& in, const std::string& out)
: inFile(in),
  outFile(out)
{}

FileIO::~FileIO()
{
    inFile.close();
    outFile.close();
}

std::vector<std::string> FileIO::readData()
{
    std::vector<std::string> data_list;
    if(inFile.is_open())
    {
        std::string line;
        while(std::getline(inFile, line))
        {
            size_t length = line.end() - line.begin();
            if(length > MAX_NUM_CHARACTERS)
            {
                line.erase(line.begin() + MAX_NUM_CHARACTERS, line.end());
            }

            data_list.push_back(line);
        }
    }
    else
    {
        TRACE("FileIO.cpp", "Read failure, cannot open file");
    }

    return data_list;
}

bool FileIO::writeLog(const std::string& info)
{
    if (outFile.is_open())
    {
        // convert clock to time, then to string
        std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string str = std::ctime(&tt);

        // erase new line character '\n'
        str.erase(str.end()-1);

        outFile << str;
        outFile << '\t';
        outFile << info;
        outFile << '\n';
        return true;
    }
    else
    {
        TRACE("FileIO.cpp", "cannot open file");
        return false;
    }
}

