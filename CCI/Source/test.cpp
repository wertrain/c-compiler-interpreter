#include "../Include/test.h"

#include <iostream>
#include <cstring>
#include "../Include/token.h"
#include "../Include/notice.h"

namespace cci {
namespace test {

bool TestToken()
{
    const char* sample_text[] = 
    {
        {
            "if (dt <= size) dt = 100;\n\
            else dt = \'A\';\n\
            puts(\"abcd\");\n"
        },
        {
            "if (dt <= size) dt = 100;\n\
            else dt = \'AB\';\n\
            puts(\"abcd);\n"
        }
    };
    for (int i = 0; i < 2; ++i)
    {
        cci::notice::Initialize();
        cci::token::Initialize("sample_text", sample_text[i], strlen(sample_text[i]));
        cci::token::Token token;
        cci::token::GetNext(token);
        
        std::cout << "========================" << std::endl;
        std::cout << "Test:" << i << std::endl;
        while (token.kind_ != cci::token::kEof)
        {
            std::cout << token.text_ << " " << token.kind_ << " " << token.value_ << std::endl;
            cci::token::GetNext(token);
        }
        if (cci::notice::GetNoticeCount() > 0)
        {
            cci::notice::PrintNotice();
        }
        else
        {
            std::cout << "no error." << std::endl;
        }
        std::cout << "========================" << std::endl;
    }

    return true;
}

} // namespace test
} // namespace cci
