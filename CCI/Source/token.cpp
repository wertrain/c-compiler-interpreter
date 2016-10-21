#include "../include/token.h"

#include <cstring>
#include <cctype>
#include "../include/notice.h"

namespace cci {
namespace token {

static const int kEndOfText = (-1);

/**
 * 分解対象のテキスト
 */
static const char* target_text = nullptr;
static int target_text_size = 0;
static const char* target_text_current_char = nullptr;

/**
 * 分解位置
 */
static int current_line_count = 0;
static int current_character_count = 0;

/**
 * ターゲット名
 */
const int kTargetNameSize = 256;
static char target_name[kTargetNameSize];

/**
 * 文字の種類
 * Kind との違いに注意
 */
enum CharType
{
    kCharLetter,
    kCharDigit,
    kCharOther,
    kCharLeftParenthesis,
    kCharRightParenthesis,
    kCharPlus,
    kCharMinus,
    kCharMultiplication,
    kCharDivision,
    kCharSingleQuote,
    kCharDoubleQuote,
    kCharAssignment,
    kCharSemicolon,
    kCharLess,
    kCharGreat,
    kCharMax
};

/**
 * 文字種類テーブル
 */
static CharType char_type_table[256];

/**
 * キーワードテーブル
 */
Keyword keyword_table[] = 
{
    {"if", kIf},
    {"else", kElse},
    {"int", kInt},
    {"void", kVoid},
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
    {":", kColon},
    {",", kComma},
    {"", kSentinel},
};

int NextChar()
{
    if (target_text_current_char >= target_text + target_text_size)
    {
        return kEndOfText;
    }

    const int c = *target_text_current_char;
    ++target_text_current_char;

    if (c == '\n')
    {
        ++current_line_count;
        current_character_count = 0;
    }
    else
    {
        ++current_character_count;
    }
    return c;
}

int PrevChar()
{
    if (target_text_current_char < target_text)
    {
        return target_text[0];
    }

    const int c = *target_text_current_char;
    --target_text_current_char;

    if (c == '\n')
    {
        --current_line_count;
        current_character_count = 0;
    }
    else
    {
        --current_character_count;
    }
    return c;
}

bool IsOperator2(int c0, int c1)
{
    char text[4];
    text[0] = c0;
    text[1] = c1;
    text[2] = '\0';
    return strstr("<= >= == !=", text) != NULL;
}

void SetKind(Token& token)
{
    char* text = token.text_;
    for (int i = 0; keyword_table[i].kind_ != kSentinel; ++i)
    {
        if (strcmp(text, keyword_table[i].text_) == 0)
        {
            token.kind_ = keyword_table[i].kind_;
            return;
        }
    }
    if (char_type_table[*text] == kCharLetter)
    {
        token.kind_ = kIdentifier;
    }
    else if (char_type_table[*text] == kCharDigit)
    {
        token.kind_ = kIntNum;
    }
    else
    {
        token.kind_ = kOther;
    }
}

void Notice(const cci::notice::NoticeMessageId id)
{
    cci::notice::AddNotice(id, target_name, current_line_count, current_character_count);
}

void Initialize(const char* name, const char* text, const int text_size)
{
    current_line_count = 0;
    current_character_count = 0;

    strcpy_s(target_name, kTargetNameSize, name);

    for (int i = 0; i < 256; ++i)
    {
        char_type_table[i] = kCharOther;
    }
    for (int i = '0'; i <= '9'; ++i)
    {
        char_type_table[i] = kCharDigit;
    }
    for (int i = 'A'; i <= 'Z'; ++i)
    {
        char_type_table[i] = kCharLetter;
    }
    for (int i = 'a'; i <= 'z'; ++i)
    {
        char_type_table[i] = kCharLetter;
    }
    char_type_table['_'] = kCharLetter;
    char_type_table['='] = kCharAssignment;
    char_type_table['('] = kCharLeftParenthesis;
    char_type_table[')'] = kCharRightParenthesis;
    char_type_table['<'] = kCharLess;
    char_type_table['>'] = kCharGreat;
    char_type_table['+'] = kCharPlus;
    char_type_table['-'] = kCharMinus;
    char_type_table['*'] = kCharMultiplication;
    char_type_table['/'] = kCharDivision;
    char_type_table['\''] = kCharSingleQuote;
    char_type_table['"'] = kCharDoubleQuote;
    char_type_table[';'] = kCharSemicolon;

    target_text = target_text_current_char = text;
    target_text_size = text_size;
}

void Finalize()
{

}

bool GetNext(Token& token)
{
    token.kind_ = kNon;
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
    if (ch == kEndOfText)
    {
        token.kind_ = kEof;
        return true;
    }

    switch (char_type_table[ch])
    {
    case kCharLetter:
        while (char_type_table[ch] == kCharLetter || char_type_table[ch] == kCharDigit)
        {
            if (token_text_pointer < token_text_pointer_id_max)
            {
                *token_text_pointer++ = ch;
            }
            ch = NextChar();
        }
        if (ch != kEndOfText)
        {
            PrevChar();
        }
        *token_text_pointer = '\0';
        break;
    case kCharDigit:
        num = 0;
        while(char_type_table[ch] == kCharDigit)
        {
            num = num * 10 + (ch - '0');
            ch = NextChar();
        }
        if (ch != kEndOfText)
        {
            PrevChar();
        }
        token.kind_ = kIntNum;
        token.value_ = num;
        break;
    case kCharSingleQuote:
        ch = NextChar();
        while(ch != kEndOfText && ch != '\n' && ch != '\'')
        {
            if (++sq_count == 1)
            {
                *token_text_pointer++ = token.value_ = ch;
                ch = NextChar();
            }
            else
            {
                // kErrorTooLongCharacterを二つ拾ってしまうので、改行か'まで文字を送ってしまう
                while(ch != kEndOfText && ch != '\n' && ch != '\'')
                {
                    ch = NextChar();
                }
                Notice(cci::notice::kErrorTooLongCharacter);
                return false;
            }
        }
        *token_text_pointer = '\0';
        if (ch != '\'')
        {
            Notice(cci::notice::kErrorMissingSingleQuote);
            return false;
        }
        token.kind_ = kIntNum;
        break;
    case kCharDoubleQuote:
        ch = NextChar();
        while(ch != kEndOfText && ch != '\n' && ch != '"')
        {
            if (token_text_pointer < token_text_pointer_text_max)
            {
                *token_text_pointer++ = ch;
                ch = NextChar();
            }
            else 
            {
                Notice(cci::notice::kErrorInvalidString);
                return false;
            }
        }
        *token_text_pointer = '\0';
        if (ch != '"')
        {
            Notice(cci::notice::kErrorMissingDoubleQuote);
            return false;
        }
        token.kind_ = kString;
        break;
    default:
        *token_text_pointer++ = ch;
        ch = NextChar();
        if (IsOperator2(*(token_text_pointer-1), ch))
        {
            *token_text_pointer++ = ch;
            ch = NextChar();
        }
        if (ch != kEndOfText)
        {
            PrevChar();
        }
        *token_text_pointer = '\0';
    }
    if (token.kind_ == kNon)
    {
        SetKind(token);
    }
    return true;
}

const char* GetCurrentName()
{
    return target_name;
}

int GetCurrentLineCount()
{
    return current_line_count;
}

int GetCurrentCharCount()
{
    return current_character_count;
}

} // namespace token
} // namespace cci
