#include "../include/parser.h"

#include <iostream>
#include "../include/token.h"
#include "../include/notice.h"
#include "../include/symbol.h"

namespace
{

void Notice(const cci::notice::NoticeMessageId id)
{
    cci::notice::AddNotice(id, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

bool GetNextToken(cci::token::Token &token)
{
    return (cci::token::GetNext(token) && token.kind_ != cci::token::kEof);
}

void DefineVarOrFunction(cci::token::Token &token, cci::symbol::DataType type)
{
    // 識別子かチェック
    if (token.kind_ != cci::token::kIdentifier)
    {
        Notice(cci::notice::kErrorInvalidIdentifier);
        return;
    }

    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->dataType_ = type;
            
    // C のスタイルではこの時点では関数宣言なのか変数宣言なのか判定できないので
    // 次のトークンを取得してから判定する
    if (!GetNextToken(token))
    {
        return;
    }
    // 次のトークンが ( なら関数宣言
    if (token.kind_ == '(') // '(' == cci::token::kLeftParenthesis
    {
        cci::symbol::Enter(tmpSymbolData, cci::symbol::kFunction);
    }
    // それ以外では変数宣言
    else
    {
        // 配列の宣言かチェック
        tmpSymbolData->arrayLength_ = 0;
        if (!GetNextToken(token))
        {
            return;
        }
        if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
        {
            if (!GetNextToken(token))
            {
                return;
            }
            // 配列の添え字がない
            if (token.kind_ == ']') // ']' == cci::token::kRightBracket
            {
                Notice(cci::notice::kErrorNotFoundArrayIndex);
                return;
            }
            // ここから未実装
        }
        cci::symbol::Enter(tmpSymbolData, cci::symbol::kVar);
    }
}

};

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
        cci::symbol::DataType tmpSymbolDataType = cci::symbol::kNon;

        switch (token.kind_)
        {
        case cci::token::kInt:
            tmpSymbolDataType = cci::symbol::kInteger;
            break;
        case cci::token::kVoid:
            tmpSymbolDataType = cci::symbol::kVoid;
            break;
        case cci::token::kSemicolon:
            // 何もせずに次へ
            continue;
        }

        if (tmpSymbolDataType != cci::symbol::kNon)
        {
            DefineVarOrFunction(token, tmpSymbolDataType);
        }
    }

    cci::token::Finalize();
    cci::symbol::Finalize();
    cci::notice::Finalize();

    return 0;
}

} // namespace parser
} // namespace cci
