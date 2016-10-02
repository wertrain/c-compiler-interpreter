#include "../Include/test.h"

#include <iostream>
#include <cstring>
#include "../Include/token.h"

namespace cci {
namespace test {

bool TestToken()
{
    const char* sample_text = 
    {
        "if (dt <= size) dt = 100;\
        else dt = \'A\';\
        puts(\"abcd\");"
    };

    cci::token::Initialize(sample_text, strlen(sample_text));
    cci::token::Token token;
    while(cci::token::GetNext(token) && token.kind_ != cci::token::kEof)
    {
        std::cout << token.text_ << " " << token.kind_ << " " << token.value_ << std::endl;
    }

    return true;
}

} // namespace test
} // namespace cci
