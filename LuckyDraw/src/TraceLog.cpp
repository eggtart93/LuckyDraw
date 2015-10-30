#include "TraceLog.hpp"
#include <iostream>

void trace(const std::string& file, int line, const std::string& message)
{
    std::cout << "TRACE: " << file <<",\n\tLine[" << line << "] : "
              << message << std::endl;
}

void traceNop(const std::string&, const std::string&) {}