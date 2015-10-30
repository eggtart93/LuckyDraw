#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class FileIO
{
    public:
    FileIO() = default;
    FileIO(const std::string& in, const std::string& out);
    ~FileIO();

    std::vector<std::string> readData();
    //bool write(const std::string& data);
    bool writeLog(const std::string& info);



    private:
    std::ifstream inFile;
    std::ofstream outFile;
};

