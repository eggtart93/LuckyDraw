#include <string>

void trace(const std::string& file, int line, const std::string& message);
void traceNop(const std::string&, const std::string&);

#ifndef TRACE_ENABLE
#define TRACE(param1, param2)  traceNop(param1, param2)// comment out this line to enable trace log
#else
#define TRACE(file, info) trace(file, __LINE__, info)
//#define TRACE_FULL(message) trace(__FILE__, __LINE__, message)
#endif
