#include "../Include/test.h"

#include <iostream>
#include <cstring>
#include "../Include/token.h"
#include "../Include/notice.h"

namespace cci {
namespace test {

bool TestToken()
{
    const char* sample_text = 
    {
        "if (dt <= size) dt = 100;\n\
        else dt = \'AB\';\n\
        puts(\"abcd);\n"
    };

    cci::notice::Initialize();

    cci::token::Initialize("sample_text", sample_text, strlen(sample_text));
    
    cci::token::Token token;
    cci::token::GetNext(token);
    while (token.kind_ != cci::token::kEof)
    {
        std::cout << token.text_ << " " << token.kind_ << " " << token.value_ << std::endl;
        cci::token::GetNext(token);
    }

    if (cci::notice::GetNoticeCount() > 0)
    {
        cci::notice::PrintNotice();
    }

    return true;
}

} // namespace test
} // namespace cci
