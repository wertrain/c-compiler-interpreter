#ifndef CCI_TOKEN_H_
#define CCI_TOKEN_H_

namespace cci {
namespace token {

static const int kTokenIdSize = 32;
static const int kTokenTextSize = 128;

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
    kId,
    kIntNum,
    kString,
    kLetter,
    kDigit,
    kNull,
    kEof,
    kOther,
    kInt,
    kVoid,
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
