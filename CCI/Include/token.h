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
    kLeftBracket        = '[',
    kRightBracket       = ']',
    kLeftCurlyBracket   = '{', // MEMO: kLeftBrace の方がいい？
    kRightCurlyBracket  = '}',
    kEqual,             // ==
    kNotEqual,          // !=
    kLessEqual,         // <=
    kGreatEqual,        // >=
    kAnd,               // &&
    kOr,                // ||
    kIncrease,          // ++
    kDecrease,          // --
    kIf,                // if
    kElse,              // else
    kReturn,            // return
    kPrintf,            // 組み込み関数: フォーマット文字列を出力する
    kInput,             // 組み込み関数: 入力を受け付ける
    kExit,              // 組み込み関数: プログラムを終了する
    kInt,               // 型: int
    kVoid,              // 型: void
    kNull,              // NULL
    kIntNum,            // (数値定数)
    kString,            // (文字列定数)
    kIdentifier,        // (識別子)
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
bool GetNextToken(Token& token);

/**
 * 前のトークンに戻す（連続使用不可）
 */
bool PreviousToken();

/**
 * ポインタを進めずに次のトークンを取得する
 */
bool CheckNextToken(Token& token);

/**
 * 現在処理中の名前を取得する
 * @return 現在の名前
 */
const char* GetCurrentName();

/**
 * 現在処理中の行数を取得する
 * @return 現在の行数
 */
int GetCurrentLineCount();

/**
 * 現在処理中の文字列数を取得する
 * @return 現在の列数
 */
int GetCurrentCharCount();

} // namespace token
} // namespace cci

#endif  // CCI_TOKEN_H_
