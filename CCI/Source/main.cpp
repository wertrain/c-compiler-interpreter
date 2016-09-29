#include <iostream>
#include <cstring>
#include "../Include/token.h"

int main (int argc, char *argv[])
{
#if 0
    if (argc == 1)
    {
        std::cout << "usage: CCI [--code]" << std::endl;
        std::cout << "    --code    Show generated code" << std::endl;
        return 0;
    }
#endif

    const char* sample_text = "if (dt <= size) dt = 100;";
    cci::token::Initialize(sample_text, strlen(sample_text));
    cci::token::Token token;
    while(cci::token::GetNext(token) && token.kind_ != cci::token::kEof) {
        std::cout << token.text_ << " " << token.kind_ << " " << token.value_ << std::endl;
    }
    return 0;
}