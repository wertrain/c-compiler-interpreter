#include <iostream>
#include <cstring>

#define ENABLED_TEST

#ifdef ENABLED_TEST
#include "../Include/test.h"
#endif // ENABLED_TEST

int main (int argc, char *argv[])
{
#ifndef ENABLED_TEST
    if (argc == 1)
    {
        std::cout << "usage: CCI [--code]" << std::endl;
        std::cout << "    --code    Show generated code" << std::endl;
        return 0;
    }
#else
    cci::test::TestExcute();
    //cci::test::TestParser();
    //cci::test::TestSymbol();
    //cci::test::TestToken();
    //cci::test::TestCode();
#endif //ENABLED_TEST

    return 0;
}