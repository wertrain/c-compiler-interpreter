#include "../Include/test.h"

#include <iostream>
#include <cstring>
#include "../Include/token.h"
#include "../Include/notice.h"
#include "../Include/symbol.h"

namespace cci {
namespace test {

bool TestSymbol()
{
    cci::symbol::Initialize();

    const int sample_text_num = 8;
    const char* sample_text[sample_text_num] = {
        {"CBR250"},
        {"CB250F"},
        {"CB400SF"},
        {"VTR250"},
        {"250CBR"},
        {"250CBRF"},
        {"250VTR"},
        {"400CBSF"}
    };
    for (int i = 0; i < sample_text_num; ++i)
    {
        cci::symbol::Enter(cci::symbol::CreateSymbolData(sample_text[i]), cci::symbol::kVar);
    }

    cci::symbol::Finalize();

    return true;
}

bool TestToken()
{
    const int sample_text_num = 2;
    const char* sample_text[sample_text_num] = 
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
    for (int i = 0; i < sample_text_num; ++i)
    {
        cci::notice::Initialize();
        cci::token::Initialize("sample_text", sample_text[i], strlen(sample_text[i]));
        cci::token::Token token;
        cci::token::GetNext(token);
        
        std::cout << "========================" << std::endl;
        std::cout << "sample_text_" << i << ":" << std::endl;
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
    }

    return true;
}

} // namespace test
} // namespace cci
