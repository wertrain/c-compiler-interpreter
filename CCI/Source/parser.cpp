#include "../include/parser.h"

#include <iostream>
#include "../include/token.h"
#include "../include/notice.h"
#include "../include/symbol.h"
#include "../include/code.h"

namespace
{

void Notice(const cci::notice::NoticeMessageId id)
{
    cci::notice::AddNotice(id, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

void Notice(const cci::notice::NoticeMessageId id, const char* text)
{
    cci::notice::AddNotice(id, text, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

bool GetNextToken(cci::token::Token &token)
{
    return (cci::token::GetNext(token) && token.kind_ != cci::token::kEof);
}

bool DefineVarOrFunction(cci::token::Token &token, cci::symbol::SymbolDataType type)
{
    // 識別子かチェック
    if (!GetNextToken(token) || token.kind_ != cci::token::kIdentifier)
    {
        Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
        return false;
    }
    // 型は確定しているので先行して作成
    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->dataType_ = type;

    // C のスタイルではこの時点では関数宣言なのか変数宣言なのか判定できないので
    // 次のトークンを取得してから判定する
    if (!GetNextToken(token))
    {
        return false;
    }
    // 次のトークンが ( なら関数宣言
    if (token.kind_ == '(') // '(' == cci::token::kLeftParenthesis
    {
        // 種類を関数に
        tmpSymbolData->kind_ = cci::symbol::kFunction;
        cci::symbol::Enter(tmpSymbolData);
    }
    // それ以外では変数宣言
    else
    {
        // 種類を変数に
        tmpSymbolData->kind_ = cci::symbol::kVar;

        // , で複数の変数宣言される場合があるので繰り返す
        while (1)
        {
            // 配列の宣言かチェック
            tmpSymbolData->arrayLength_ = 0;
            if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
            {
                if (!GetNextToken(token))
                {
                    return false;
                }
                // 配列の添え字がない
                if (token.kind_ == ']') // ']' == cci::token::kRightBracket
                {
                    Notice(cci::notice::kErrorNotFoundArrayIndex);
                    return false;
                }
                // ここから未実装
            }
            cci::symbol::Enter(tmpSymbolData);

            // 変数宣言の終了
            if (token.kind_ == ';') // ';' == cci::token::kSemicolon
            {
                break;
            }
            // 変数宣言を続ける
            else if (token.kind_ == ',') // ',' == cci::token::kComma
            {
                if (!GetNextToken(token) || token.kind_ != cci::token::kIdentifier)
                {
                    Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
                    return false;
                }
                tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
                tmpSymbolData->kind_ = cci::symbol::kVar;
                tmpSymbolData->dataType_ = type;

                // この関数の処理に合わせて、もう一つ先を読んでおく
                if (!GetNextToken(token))
                {
                    Notice(cci::notice::kErrorInvalidIdentifier);
                    return false;
                }
                continue;
            }
            else
            {
                // 宣言エラー
                Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
                return false;
            }
        }
    }
    return true;
}

};

namespace cci {
namespace parser {

int compile(const char* name, const char* text, const int text_size)
{
    cci::notice::Initialize();
    cci::symbol::Initialize();
    cci::code::Initialize();

    cci::code::GenerateCode2(cci::code::kCall, -1); // main 関数の呼び出し、-1は仮設定
    cci::code::GenerateCode1(cci::code::kStop); // プログラム終了

    cci::token::Initialize(name, text, text_size);
    cci::token::Token token;

    while(cci::token::GetNext(token) && token.kind_ != cci::token::kEof)
    {
        cci::symbol::SymbolDataType tmpSymbolDataType = cci::symbol::kNon;

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
    cci::code::Finalize();
    cci::symbol::Finalize();
    cci::notice::Finalize();

    return 0;
}

} // namespace parser
} // namespace cci
