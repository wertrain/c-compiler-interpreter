#ifndef CCI_TOKEN_H_
#define CCI_TOKEN_H_

namespace cci {
namespace token {

static const int kTokenIdSize = 32;
static const int kTokenTextSize = 128;

/**
 * トークンの種類定義
 * 一文字の種類については判定時に kind == '(' などでも判定可能にするために
 * 対応した定数値を割り当てている
 */
enum TokenKind
{
    kNon,
    kLeftParenthesis    = '(',
    kRightParenthesis   = ')',
    kPlus               = '+',
    kMinus              = '-',
    kMultiplication     = '*',
    kDivision           = '/',
    kModulo             = '%',
    kSingleQuote        = '\'',
    kDoubleQuote        = '"',
    kAssignment         = '=',
    kColon              = ':',
    kSemicolon          = ';',
    kSharp              = '#',
    kComma              = ',',
    kBackslash          = '\\',
    kNot                = '!',
    kLess               = '<',
    kGreat              = '>',
    kEqual,             // ==
    kNotEqual,          // !=
    kLessEqual,         // <=
    kGreatEqual,        // >=
    kIf,                // if
    kElse,              // else
    kPuts,              // puts
    kInt,               // int
    kVoid,              // void
    kNull,              // NULL
    kString,            // (文字列定数)
    kId,                // (識別子)
    kOther,             // (その他)
    kEof,               // (終端)
    kSentinel
};

/**
 * トークン
 */
struct Token
{
    TokenKind kind_;
    char text_[kTokenTextSize];
    int value_;
};

/**
 * キーワード
 */
struct Keyword
{
    char* text_;
    TokenKind kind_;
};

/**
 * 初期化関数
 */
void Initialize(const char* name, const char* text, const int text_size);

/**
 * 終了関数
 */
void Finalize();

/**
 * 次のトークンの取得
 */
bool GetNext(Token& token);

} // namespace token
} // namespace cci

#endif  // CCI_TOKEN_H_
