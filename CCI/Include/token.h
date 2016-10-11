#ifndef CCI_TOKEN_H_
#define CCI_TOKEN_H_

namespace cci {
namespace token {

static const int kTokenIdSize = 32;
static const int kTokenTextSize = 128;

/**
 * �g�[�N���̎�ޒ�`
 * �ꕶ���̎�ނɂ��Ă͔��莞�� kind == '(' �Ȃǂł�����\�ɂ��邽�߂�
 * �Ή������萔�l�����蓖�ĂĂ���
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
    kString,            // (������萔)
    kId,                // (���ʎq)
    kOther,             // (���̑�)
    kEof,               // (�I�[)
    kSentinel
};

/**
 * �g�[�N��
 */
struct Token
{
    TokenKind kind_;
    char text_[kTokenTextSize];
    int value_;
};

/**
 * �L�[���[�h
 */
struct Keyword
{
    char* text_;
    TokenKind kind_;
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
