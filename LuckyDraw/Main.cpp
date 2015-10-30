#include "Application.h"

//#define TEST

#ifndef TEST
int main()
{
    Application app;
    app.run();
    return EXIT_SUCCESS;
}
#else
#include "Test.h"
int main()
{
    runTest();
    return EXIT_SUCCESS;
}
#endif
