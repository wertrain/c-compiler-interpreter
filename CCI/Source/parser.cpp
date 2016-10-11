#include "../include/parser.h"

#include <iostream>
#include "../include/token.h"
#include "../include/notice.h"
#include "../include/symbol.h"

namespace cci {
namespace parser {

int compile(const char* name, const char* text, const int text_size)
{
    cci::notice::Initialize();
    cci::symbol::Initialize();

    cci::token::Initialize(name, text, text_size);
    cci::token::Token token;

    while(cci::token::GetNext(token) && token.kind_ != cci::token::kEof)
    {
        switch (token.kind_)
        {
        case cci::token::kInt:
        case cci::token::kVoid:
            break;
        }
    }

    cci::token::Finalize();
    cci::symbol::Finalize();
    cci::notice::Finalize();

    return 0;
}

} // namespace parser
} // namespace cci
