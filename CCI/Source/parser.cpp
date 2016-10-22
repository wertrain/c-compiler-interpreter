#include "../include/parser.h"

#include <cassert>
#include "../include/token.h"
#include "../include/notice.h"
#include "../include/symbol.h"
#include "../include/code.h"

namespace
{

/**
 * 現在のネスト
 */
static int block_nest_count = 0;

/**
 * 内部処理をリセットする
 */
void ResetInner()
{
    block_nest_count = 0;
}

/**
 * 通知にメッセージを追加
 */
void Notice(const cci::notice::NoticeMessageId id)
{
    cci::notice::AddNotice(id, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

/**
 * 通知にメッセージを追加
 */
void Notice(const cci::notice::NoticeMessageId id, const char* text)
{
    cci::notice::AddNotice(id, text, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

/**
 * 次のトークンを取得する
 */
bool GetNextTokenInner(cci::token::Token &token)
{
    return (cci::token::GetNextToken(token) && token.kind_ != cci::token::kEof);
}

/**
 * トークンの種類から型を取得
 */
cci::symbol::SymbolDataType GetSymbolDataType(const cci::token::TokenKind kind)
{
    switch (kind)
    {
    case cci::token::kInt:
        return cci::symbol::kInteger;
    case cci::token::kVoid:
        return cci::symbol::kVoid;
    }
    return cci::symbol::kNon;
}

/**
 * 関数をシンボルに追加する
 */
bool EntryFunction(cci::token::Token &token, const cci::symbol::SymbolDataType type)
{
    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->kind_ = cci::symbol::kFunction; // 関数として追加
    tmpSymbolData->dataType_ = type;
    tmpSymbolData->level_ = block_nest_count;
    cci::symbol::Enter(tmpSymbolData);
    // ここから未実装
    return true;
}

/**
 * 変数をシンボルに追加する
 */
bool EntryVars(cci::token::Token &token, const cci::symbol::SymbolDataType type)
{
    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->kind_ = cci::symbol::kVar; // 変数として追加
    tmpSymbolData->dataType_ = type;
    tmpSymbolData->level_ = block_nest_count;

    // , で複数の変数宣言される場合があるので繰り返す
    while (1)
    {
        if (!GetNextTokenInner(token))
        {
            return false;
        }

        // 配列の宣言かチェック
        tmpSymbolData->arrayLength_ = 0;
        if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
        {
            if (!GetNextTokenInner(token))
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
            if (!GetNextTokenInner(token) || token.kind_ != cci::token::kIdentifier)
            {
                Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
                return false;
            }
            tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
            tmpSymbolData->kind_ = cci::symbol::kVar;
            tmpSymbolData->level_ = block_nest_count;
            tmpSymbolData->dataType_ = type;
            continue;
        }
        else
        {
            // 宣言エラー
            Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
            return false;
        }
    }

    return true;
}

void block(cci::token::Token &token, const bool is_function)
{
    if (!GetNextTokenInner(token))
    {
        return;
    }
    ++block_nest_count;
    
    // 関数なら先頭で定義されている変数をチェックする
    if (is_function)
    {
        cci::symbol::SymbolDataType type = GetSymbolDataType(token.kind_);
        while (type != cci::symbol::kNon)
        {
            EntryVars(token, type);
        }
    }

    // 文の処理
    cci::token::TokenKind kind = cci::token::kOther;
    while (token.kind_ != '}') // '}' == cci::token::kRightCurlyBracket
    {
        kind = token.kind_;
    }

    --block_nest_count;
    GetNextTokenInner(token);
}

bool DefineVarOrFunction(cci::token::Token &token, cci::symbol::SymbolDataType type)
{
    // 識別子かチェック
    if (!GetNextTokenInner(token) || token.kind_ != cci::token::kIdentifier)
    {
        Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
        return false;
    }

    // C のスタイルではこの時点では関数宣言なのか変数宣言なのか判定できないので
    // 次のトークンを取得してから判定する
    cci::token::Token next_token;
    if (!cci::token::CheckNextToken(next_token))
    {
        return false;
    }

    // 次のトークンが ( なら関数宣言
    if (next_token.kind_ == '(') // '(' == cci::token::kLeftParenthesis
    {
        return EntryFunction(token, type);
    }
    // それ以外では変数宣言
    else
    {
        return EntryVars(token, type);
    }
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

    while(GetNextTokenInner(token))
    {
        cci::symbol::SymbolDataType tmpSymbolDataType = cci::symbol::kNon;

        switch (token.kind_)
        {
        case cci::token::kSemicolon:
            // 何もせずに次へ
            continue;
        default:
            tmpSymbolDataType = GetSymbolDataType(token.kind_);
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
