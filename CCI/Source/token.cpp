#include "../include/token.h"

namespace cci {
namespace token {

/**
 * ������ރe�[�u��
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
 * �L�[���[�h�e�[�u��
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
 * �g�[�N���֘A�̏�����
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

} // namespace token
} // namespace cci
