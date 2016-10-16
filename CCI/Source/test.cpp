#include "../Include/test.h"

#include <iostream>
#include <cstring>
#include <cassert>
#include "../Include/token.h"
#include "../Include/notice.h"
#include "../Include/symbol.h"
#include "../Include/parser.h"

namespace cci {
namespace test {

void Test(const bool test, const long line, const char* file)
{
    if (!test)
    {
        std::cout << "Test Failed!"
                  << " Line: " << line
                  << " File: " << file
                  << std::endl;
        assert(test);
    }
    else
    {
        std::cout << "Test Succeeded!"
                  << " Line: " << line
                  << " File: " << file
                  << std::endl;
    }
}

bool TestSymbol()
{
    cci::symbol::Initialize();

    const int sample_text_num = 10;
    const char* sample_text[sample_text_num] = {
        {"aiueo"},
        {"oeiua"},
        {"abcde"},
        {"edcba"},
        {"fghij"},
        {"jihgf"},
        {"qwerty"},
        {"asdfgh"},
        {"zxcvbn"},
        {"123456"}
    };
    for (int i = 0; i < sample_text_num; ++i)
    {
        cci::symbol::Enter(cci::symbol::CreateSymbolData(sample_text[i]));
    }

    Test(nullptr != cci::symbol::SearchSymbolByName(sample_text[0]), __LINE__, __FILE__);
    Test(nullptr != cci::symbol::SearchSymbolByName(sample_text[1]), __LINE__, __FILE__);
    Test(nullptr == cci::symbol::SearchSymbolByName("notfoundtest"), __LINE__, __FILE__);

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
        if (cci::notice::GetNoticeAmountCount() > 0)
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

bool TestParser()
{
    const int sample_text_num = 2;
    const char* sample_text[sample_text_num] = 
    {
        {
            "int dt;\n\
            dt = 100;\n"
        },
        {
            "int func();\n\
            func();\n"
        }
    };
    for (int i = 0; i < sample_text_num; ++i)
    {
        std::cout << "========================" << std::endl;
        std::cout << "sample_text_" << i << ":" << std::endl;
        cci::parser::compile("sample_text", sample_text[i], strlen(sample_text[i]));
    }
    return true;
}

} // namespace test
} // namespace cci
