#include <iostream>
#include "../Include/token.h"

int main (int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "usage: CCI [--code]" << std::endl;
        std::cout << "    --code    Show generated code" << std::endl;
        return 0;
    }

    cci::token::Initialize();

    return 0;
}