#include "../include/parser.h"

#include <cassert>
#include "../include/token.h"
#include "../include/notice.h"
#include "../include/symbol.h"
#include "../include/code.h"

namespace
{

/**
 * ���݂̃l�X�g
 */
static int block_nest_count = 0;

/**
 * �������������Z�b�g����
 */
void ResetInner()
{
    block_nest_count = 0;
}

/**
 * �ʒm�Ƀ��b�Z�[�W��ǉ�
 */
void Notice(const cci::notice::NoticeMessageId id)
{
    cci::notice::AddNotice(id, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

/**
 * �ʒm�Ƀ��b�Z�[�W��ǉ�
 */
void Notice(const cci::notice::NoticeMessageId id, const char* text)
{
    cci::notice::AddNotice(id, text, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

/**
 * ���̃g�[�N�����擾����
 */
bool GetNextTokenInner(cci::token::Token &token)
{
    return (cci::token::GetNextToken(token) && token.kind_ != cci::token::kEof);
}

/**
 * �g�[�N���̎�ނ���^���擾
 */
cci::symbol::SymbolDataType GetSymbolDataType(const cci::token::TokenKind kind)
{
    switch (kind)
    {
    case cci::token::kInt:
        return cci::symbol::kInteger;
    case cci::token::kVoid:
        return cci::symbol::kVoid;
    }
    return cci::symbol::kNon;
}

/**
 * �֐����V���{���ɒǉ�����
 */
bool EntryFunction(cci::token::Token &token, const cci::symbol::SymbolDataType type)
{
    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->kind_ = cci::symbol::kFunction; // �֐��Ƃ��Ēǉ�
    tmpSymbolData->dataType_ = type;
    tmpSymbolData->level_ = block_nest_count;
    cci::symbol::Enter(tmpSymbolData);
    // �������疢����
    return true;
}

/**
 * �ϐ����V���{���ɒǉ�����
 */
bool EntryVars(cci::token::Token &token, const cci::symbol::SymbolDataType type)
{
    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->kind_ = cci::symbol::kVar; // �ϐ��Ƃ��Ēǉ�
    tmpSymbolData->dataType_ = type;
    tmpSymbolData->level_ = block_nest_count;

    // , �ŕ����̕ϐ��錾�����ꍇ������̂ŌJ��Ԃ�
    while (1)
    {
        if (!GetNextTokenInner(token))
        {
            return false;
        }

        // �z��̐錾���`�F�b�N
        tmpSymbolData->arrayLength_ = 0;
        if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
        {
            if (!GetNextTokenInner(token))
            {
                return false;
            }
            // �z��̓Y�������Ȃ�
            if (token.kind_ == ']') // ']' == cci::token::kRightBracket
            {
                Notice(cci::notice::kErrorNotFoundArrayIndex);
                return false;
            }
            // �������疢����
        }
        cci::symbol::Enter(tmpSymbolData);

        // �ϐ��錾�̏I��
        if (token.kind_ == ';') // ';' == cci::token::kSemicolon
        {
            break;
        }
        // �ϐ��錾�𑱂���
        else if (token.kind_ == ',') // ',' == cci::token::kComma
        {
            if (!GetNextTokenInner(token) || token.kind_ != cci::token::kIdentifier)
            {
                Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
                return false;
            }
            tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
            tmpSymbolData->kind_ = cci::symbol::kVar;
            tmpSymbolData->level_ = block_nest_count;
            tmpSymbolData->dataType_ = type;
            continue;
        }
        else
        {
            // �錾�G���[
            Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
            return false;
        }
    }

    return true;
}

void block(cci::token::Token &token, const bool is_function)
{
    if (!GetNextTokenInner(token))
    {
        return;
    }
    ++block_nest_count;
    
    // �֐��Ȃ�擪�Œ�`����Ă���ϐ����`�F�b�N����
    if (is_function)
    {
        cci::symbol::SymbolDataType type = GetSymbolDataType(token.kind_);
        while (type != cci::symbol::kNon)
        {
            EntryVars(token, type);
        }
    }

    // ���̏���
    cci::token::TokenKind kind = cci::token::kOther;
    while (token.kind_ != '}') // '}' == cci::token::kRightCurlyBracket
    {
        kind = token.kind_;
    }

    --block_nest_count;
    GetNextTokenInner(token);
}

bool DefineVarOrFunction(cci::token::Token &token, cci::symbol::SymbolDataType type)
{
    // ���ʎq���`�F�b�N
    if (!GetNextTokenInner(token) || token.kind_ != cci::token::kIdentifier)
    {
        Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
        return false;
    }

    // C �̃X�^�C���ł͂��̎��_�ł͊֐��錾�Ȃ̂��ϐ��錾�Ȃ̂�����ł��Ȃ��̂�
    // ���̃g�[�N�����擾���Ă��画�肷��
    cci::token::Token next_token;
    if (!cci::token::CheckNextToken(next_token))
    {
        return false;
    }

    // ���̃g�[�N���� ( �Ȃ�֐��錾
    if (next_token.kind_ == '(') // '(' == cci::token::kLeftParenthesis
    {
        return EntryFunction(token, type);
    }
    // ����ȊO�ł͕ϐ��錾
    else
    {
        return EntryVars(token, type);
    }
}

};

namespace cci {
namespace parser {

int compile(const char* name, const char* text, const int text_size)
{
    cci::notice::Initialize();
    cci::symbol::Initialize();
    cci::code::Initialize();

    cci::code::GenerateCode2(cci::code::kCall, -1); // main �֐��̌Ăяo���A-1�͉��ݒ�
    cci::code::GenerateCode1(cci::code::kStop); // �v���O�����I��

    cci::token::Initialize(name, text, text_size);
    cci::token::Token token;

    while(GetNextTokenInner(token))
    {
        cci::symbol::SymbolDataType tmpSymbolDataType = cci::symbol::kNon;

        switch (token.kind_)
        {
        case cci::token::kSemicolon:
            // ���������Ɏ���
            continue;
        default:
            tmpSymbolDataType = GetSymbolDataType(token.kind_);
        }

        if (tmpSymbolDataType != cci::symbol::kNon)
        {
            DefineVarOrFunction(token, tmpSymbolDataType);
        }
    }

    cci::token::Finalize();
    cci::code::Finalize();
    cci::symbol::Finalize();
    cci::notice::Finalize();

    return 0;
}

} // namespace parser
} // namespace cci
