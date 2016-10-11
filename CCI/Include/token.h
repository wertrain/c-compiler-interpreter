#ifndef CCI_TOKEN_H_
#define CCI_TOKEN_H_

namespace cci {
namespace token {

static const int kTokenIdSize = 32;
static const int kTokenTextSize = 128;

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
void Initialize(const char* name, const char* text, const int text_size);

/**
 * �I���֐�
 */
void Finalize();

/**
 * ���̃g�[�N���̎擾
 */
bool GetNext(Token& token);

} // namespace token
} // namespace cci

#endif  // CCI_TOKEN_H_
