#ifndef CCI_TOKEN_H_
#define CCI_TOKEN_H_

namespace cci {
namespace token {

const int kIdSize = 32;
const int kTokenTextSize = 128;

/**
 * �g�[�N���̎�ޒ�`
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
 * �g�[�N��
 */
struct Token
{
    Kind kind_;
    char text_[kTokenTextSize];
    int value_;
};

/**
 * �L�[���[�h
 */
struct Keyword
{
    char* text_;
    Kind kind_;
};

/**
 * �������֐�
 */
void Initialize();

/**
 * ���̃g�[�N���̎擾
 */
Token GetNext();

} // namespace token
} // namespace cci

#endif  // CCI_TOKEN_H_
