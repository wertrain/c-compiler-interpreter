#include "../include/token.h"

#include <cstring>
#include <cctype>

namespace cci {
namespace token {

static const int kEndOfText = (-1);

/**
 * ����Ώۂ̃e�L�X�g
 */
static const char* target_text = nullptr;
static const char* target_text_current_char = nullptr;
/**
 * ������ރe�[�u��
 */
static Kind char_type_table[256];

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

int NextChar()
{
    if (target_text_current_char == nullptr)
    {
        return 0;
    }

    const int c = *target_text_current_char;
    ++target_text_current_char;
    return c;
}

int PrevChar()
{
    if (target_text_current_char == nullptr)
    {
        return 0;
    }

    const int c = *target_text_current_char;
    --target_text_current_char;
    return c;
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
        token.kind_ = kId;
    }
    else if (char_type_table[*text] == kDigit)
    {
        token.kind_ = kIntNum;
    }
    else
    {
        return false;
    }
    return true;
}

/**
 * �g�[�N���֘A�̏�����
 */
void Initialize(const char* text)
{
    for (int i = 0; i < 256; ++i)
    {
        char_type_table[i] = kOther;
    }
    for (int i = '0'; i <= '9'; ++i)
    {
        char_type_table[i] = kDigit;
    }
    for (int i = 'A'; i <= 'Z'; ++i)
    {
        char_type_table[i] = kLetter;
    }
    for (int i = 'a'; i <= 'z'; ++i)
    {
        char_type_table[i] = kLetter;
    }
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

    target_text = target_text_current_char = text;
}

/**
 * ���̃g�[�N���̎擾
 */
bool GetNext(Token& token)
{
    token.kind_ = kNull;
    memset(token.text_, '\0', sizeof(token.text_));
    token.value_ = 0;

    char* token_text_pointer = token.text_;
    char* token_text_pointer_id_max = token.text_ + kTokenIdSize;
    char* token_text_pointer_text_max = token.text_ + kTokenTextSize;
    int num = 0, sq_count = 0;

    int ch = NextChar();
    while (isspace(ch))
    {
        ch = NextChar();
    }
    if (ch == 0)
    {
        token.kind_ = kEof;
        return true;
    }

    switch (char_type_table[ch])
    {
    case kLetter:
        while (char_type_table[ch] == kLetter || char_type_table[ch] == kDigit)
        {
            if (token_text_pointer < token_text_pointer_id_max)
            {
                *token_text_pointer++ = ch;
            }
            ch = NextChar();
        }
        PrevChar();
        *token_text_pointer = '\0';
        break;
    case kDigit:
        num = 0;
        while(char_type_table[ch] == kDigit)
        {
            num = num * 10 + (ch - '0');
            ch = NextChar();
        }
        PrevChar();
        token.kind_ = kIntNum;
        token.value_ = num;
        break;
    case kSingleQuote:
        ch = NextChar();
        while(ch != kEndOfText && ch != '\n' && ch != '\'')
        {
            if (++sq_count == 1)
            {
                *token_text_pointer++ = token.value_ = ch;
            }
            else
            {
                return false;
            }
        }
        break;
    case kDoubleQuote:
        ch = NextChar();
        while(ch != kEndOfText && ch != '\n' && ch != '"')
        {
            if (token_text_pointer < token_text_pointer_text_max)
            {
                *token_text_pointer++ = ch;
            }
            else 
            {
                return false;
            }
            ch = NextChar();
        }
        *token_text_pointer = '\0';

        if (ch == '"')
        {
            return false;
        }
        token.kind_ = kString;
        break;
    default:
        *token_text_pointer++ = ch;
        ch = NextChar();
        if (IsOperator(*(token_text_pointer-1), ch))
        {
            *token_text_pointer++ = ch;
            ch = NextChar();
        }
        PrevChar();
        *token_text_pointer = '\0';
    }
    if (token.kind_ == kNull)
    {
        SetKind(token);
    }
    if (token.kind_ == kOther)
    {
        return false;
    }
    return true;
}

} // namespace token
} // namespace cci
