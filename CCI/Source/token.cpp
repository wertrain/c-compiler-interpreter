#include "../include/token.h"

#include <cstring>

namespace cci {
namespace token {

/**
 * 文字種類テーブル
 */
static Kind char_type_table[256];
static inline void SetCharTypeTable(int start, int end, Kind value)
{
    for (int i = start; i < end; ++i)
    {
        char_type_table[i] = value;
    }
}

/**
 * キーワードテーブル
 */
Keyword keyword_table[] = 
{
    {"if", kIf},
    {"else", kElse},
    {"(", kLeftParenthesis},
    {")", kRightParenthesis},
    {"+", kPlus},
    {"-", kMinus},
    {"*", kMultiplication},
    {"/", kDivision},
    {"==", kEqual},
    {"!=", kNotEqual},
    {"<", kLess},
    {"<=", kLessEqual},
    {">", kGreat},
    {">=", kGreatEqual},
    {"=", kAssignment},
    {";", kSemicolon},
    {"", kSentinel},
};

/**
 * トークン関連の初期化
 */
void Initialize()
{
    SetCharTypeTable( 0, 256, kOther);
    SetCharTypeTable('0', '9', kDigit);
    SetCharTypeTable('A', 'Z', kLetter);
    SetCharTypeTable('a', 'z', kLetter);
    SetCharTypeTable('a', 'z', kLetter);
    char_type_table['_'] = kLetter;
    char_type_table['='] = kAssignment;
    char_type_table['('] = kLeftParenthesis;
    char_type_table[')'] = kRightParenthesis;
    char_type_table['<'] = kLess;
    char_type_table['>'] = kGreat;
    char_type_table['+'] = kPlus;
    char_type_table['-'] = kMinus;
    char_type_table['*'] = kMultiplication;
    char_type_table['/'] = kDivision;
    char_type_table['\''] = kSingleQuote;
    char_type_table['"'] = kDoubleQuote;
    char_type_table[';'] = kSemicolon;
}

int NextChar()
{
    int c = 0;
    //if (c == EOF
}

bool IsOperator(int c0, int c1)
{
    char text[4];
    text[0] = c0;
    text[1] = c1;
    text[2] = '\0';
    return strstr(" <= >= == != ", text) != NULL;
}

bool SetKind(Token& token)
{
    char* text = token.text_;
    for (int i = 0; keyword_table[i].kind_ != kSentinel; ++i)
    {
        if (strcmp(text, keyword_table[i].text_) == 0)
        {
            token.kind_ = keyword_table[i].kind_;
            return true;
        }
    }
    if (char_type_table[*text] == kLetter)
    {
        token.kind_ = kIndent;
    }
    else if (char_type_table[*text] == kDigit)
    {
        token.kind_ = kIntValue;
    }
    else
    {
        return false;
    }
    return true;
}

} // namespace token
} // namespace cci
