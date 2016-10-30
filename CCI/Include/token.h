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
    kLeftBracket        = '[',
    kRightBracket       = ']',
    kLeftCurlyBracket   = '{', // MEMO: kLeftBrace �̕��������H
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
    kPrintf,            // �g�ݍ��݊֐�: �t�H�[�}�b�g��������o�͂���
    kInput,             // �g�ݍ��݊֐�: ���͂��󂯕t����
    kExit,              // �g�ݍ��݊֐�: �v���O�������I������
    kInt,               // �^: int
    kVoid,              // �^: void
    kNull,              // NULL
    kIntNum,            // (���l�萔)
    kString,            // (������萔)
    kIdentifier,        // (���ʎq)
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
bool GetNextToken(Token& token);

/**
 * �O�̃g�[�N���ɖ߂��i�A���g�p�s�j
 */
bool PreviousToken();

/**
 * �|�C���^��i�߂��Ɏ��̃g�[�N�����擾����
 */
bool CheckNextToken(Token& token);

/**
 * ���ݏ������̖��O���擾����
 * @return ���݂̖��O
 */
const char* GetCurrentName();

/**
 * ���ݏ������̍s�����擾����
 * @return ���݂̍s��
 */
int GetCurrentLineCount();

/**
 * ���ݏ������̕����񐔂��擾����
 * @return ���݂̗�
 */
int GetCurrentCharCount();

} // namespace token
} // namespace cci

#endif  // CCI_TOKEN_H_
