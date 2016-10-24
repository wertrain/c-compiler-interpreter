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
 * 優先順位
 */
static int kMinPriority = 0;
static int kMaxPriority = 8;

/**
 * 最後に処理した式のトークンの種類
 */
cci::token::TokenKind last_statement_token_kind = cci::token::kNon;


/**
 * 局所変数アドレス管理
 */
static int local_var_address = 0;

/**
 * 局所変数アドレス割り付け開始番地
 */
static const int kStartLocalAddress = 1 * sizeof(int);

/**
 * 内部関数のプロトタイプ（必要な関数のみ）
 */
void block(cci::token::Token &token, const bool is_function);
void Statement(cci::token::Token &token);

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
 * 二項演算子の優先順位を取得
 */
int GetPriority(const cci::token::TokenKind kind)
{
    switch(kind)
    {
    case cci::token::kMultiplication:
    case cci::token::kDivision:
    case cci::token::kModulo:
        return kMaxPriority - 1;
    case cci::token::kPlus:
    case cci::token::kMinus:
        return kMaxPriority - 2;
    case cci::token::kLess:
    case cci::token::kLessEqual:
    case cci::token::kGreat:
    case cci::token::kGreatEqual:
        return kMaxPriority - 3;
    case cci::token::kEqual:
    case cci::token::kNotEqual:
        return kMaxPriority - 4;
    case cci::token::kAnd:
        return kMaxPriority - 5;
    case cci::token::kOr:
        return kMaxPriority - 6;
    case cci::token::kAssignment:
        return kMaxPriority - 7;
    default:
        return 0;
    }
}

void CallFunction(const cci::symbol::SymbolData* data)
{

}

/**
 * 関数をシンボルに追加する
 */
bool EntryFunction(cci::token::Token &token, const cci::symbol::SymbolDataType type)
{
    local_var_address = kStartLocalAddress;
    const cci::symbol::SymbolData* find = cci::symbol::SearchSymbolByName(token.text_);
    if (find != nullptr && find->kind_ != cci::symbol::kFunction && find->kind_ != cci::symbol::kPrototype)
    {
        Notice(cci::notice::kErrorAlreadyUsedName, token.text_);
        return false;
    }

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

/**
 * 関数と変数の登録判定
 */
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

/**
 * 因子の処理
 */
void Factor(cci::token::Token &token)
{
    cci::token::TokenKind kind = token.kind_;
    switch (kind)
    {
    case cci::token::kPlus:
    case cci::token::kMinus:
        GetNextTokenInner(token);
        Factor(token);
        cci::code::GenerateCodeUnArray(kind);
        break;
    case cci::token::kIncrease:
    case cci::token::kDecrease:
        GetNextTokenInner(token);
        Factor(token);
        if (!cci::code::ToLeftValue())
        {
            Notice(cci::notice::kErrorInvalidLeftValue, token.text_);
        }
        cci::code::GenerateCodeUnArray(kind);
        break;
    case cci::token::kIntNum:
        cci::code::GenerateCode2(cci::code::kLdi, token.value_);
        GetNextTokenInner(token);
        break;
    case cci::token::kIdentifier:
        {
            const cci::symbol::SymbolData *data = cci::symbol::SearchSymbolByName(token.text_);
            switch (data->kind_)
            {
            case cci::symbol::kFunction:
            case cci::symbol::kPrototype:
                if (data->kind_ == cci::symbol::kVoid)
                {
                    // エラー
                }
                CallFunction(data);
                break;
            case cci::symbol::kVar:
            case cci::symbol::kParam:
                // 単純変数か判定
                if (data->arrayLength_ == 0)
                {
                    cci::code::GenerateCode3(cci::code::kLod, cci::symbol::GetCodeFlag(data), data->address_);
                    GetNextTokenInner(token);
                }
                else
                {
                    GetNextTokenInner(token);
                    if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
                    {
                        cci::code::GenerateCode3(cci::code::kLda, cci::symbol::GetCodeFlag(data), data->address_);
                        // ここから未実装
                    }
                    else
                    {
                        Notice(cci::notice::kErrorNotFoundArrayIndex);
                    }

                    if (token.kind_ == cci::token::kIncrease)
                    {
                        cci::code::ToLeftValue();
                        cci::code::GenerateCode1(cci::code::kInc);
                        cci::code::GenerateCode2(cci::code::kLdi, 1);
                        cci::code::GenerateCode1(cci::code::kSub);
                        GetNextTokenInner(token);
                    }
                    else if (token.kind_ == cci::token::kDecrease)
                    {
                        cci::code::ToLeftValue();
                        cci::code::GenerateCode1(cci::code::kDec);
                        cci::code::GenerateCode2(cci::code::kLdi, 1);
                        cci::code::GenerateCode1(cci::code::kAdd);
                        GetNextTokenInner(token);
                    }
                }
                break;
            }
        }
        break;
    default:
        Notice(cci::notice::kErrorInvalidFormat);
    }
}

/**
 * 項の処理
 */
void Term(cci::token::Token &token, const int priority)
{
    if (priority == kMaxPriority)
    {
        Factor(token);
        return;
    }
    Term(token, priority + 1);

    cci::token::TokenKind kind;
    while (priority == GetPriority(token.kind_))
    {
        kind = token.kind_;
        GetNextTokenInner(token);
        Term(token, priority + 1);
        cci::code::GenerateCodeBinary(kind);
    }
}

/**
 * 式の処理
 */
void Expression(cci::token::Token &token)
{
    Term(token, kMinPriority + 2);
    if (token.kind_ == '=') // '{' == cci::token::kAssignment
    {
        cci::code::ToLeftValue();
        GetNextTokenInner(token);
        Expression(token);
        cci::code::GenerateCode1(cci::code::kAssv);
    }
}

/**
 * 文の処理
 */
void Statement(cci::token::Token &token)
{
    cci::token::TokenKind kind = token.kind_;
    bool get_next = false;
    
    switch(kind)
    {
    // 識別子
    case cci::token::kIdentifier:
        {
            const cci::symbol::SymbolData* data = cci::symbol::SearchSymbolByName(token.text_);
            if (data == nullptr)
            {
                // 未定義の変数
            }
            else if ((data->kind_ == cci::symbol::kFunction || data->kind_ == cci::symbol::kPrototype) && data->dataType_ == cci::symbol::kVoid)
            {

            }
            else
            {

            }
        }
        break;
    // 非関数ブロック
    case '{': // '{' == cci::token::kLeftCurlyBracket
        block(token, false);
        break;
    // 文の終了
    case ';': // ';' == cci::token::kSemicolon
        get_next = true;
        break;
    // 意図しない終了
    case cci::token::kEof:
        Notice(cci::notice::kErrorUnintendedEof);
        break;
    // 無効な記述
    default:
        Notice(cci::notice::kErrorInvalidFormat);
        get_next = true;
    }

    if (get_next)
    {
        GetNextTokenInner(token);
    }
}

/**
 * ブロック処理
 */
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
        Statement(token);
    }
    last_statement_token_kind = kind;

    --block_nest_count;
    GetNextTokenInner(token);
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
