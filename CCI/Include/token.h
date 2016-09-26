#ifndef CCI_TOKEN_H_
#define CCI_TOKEN_H_

namespace cci {
namespace token {

const int kIdSize = 32;
const int kTokenTextSize = 128;

/**
 * トークンの種類定義
 */
enum Kind
{
    kLeftParenthesis,
    kRightParenthesis,
    kPlus,
    kMinus,
    kMultiplication,
    kDivision,
    kEqual,
    kNotEqual,
    kLess,
    kLessEqual,
    kGreat,
    kGreatEqual,
    kSingleQuote,
    kDoubleQuote,
    kAssignment,
    kSemicolon,
    kIf,
    kElse,
    kPuts,
    kIndent,
    kIntValue,
    kStringValue,
    kLetter,
    kDigit,
    kNull,
    kEof,
    kOther,
    kSentinel
};

/**
 * トークン
 */
struct Token
{
    Kind kind_;
    char text_[kTokenTextSize];
    int value_;
};

/**
 * キーワード
 */
struct Keyword
{
    char* text_;
    Kind kind_;
};

/**
 * 初期化関数
 */
void Initialize();

/**
 * 次のトークンの取得
 */
Token GetNext();

} // namespace token
} // namespace cci

#endif  // CCI_TOKEN_H_
