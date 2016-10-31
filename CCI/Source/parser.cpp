#include "../include/parser.h"

#include <cassert>
#include <cstring>
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
 * �D�揇��
 */
static int kMinPriority = 0;
static int kMaxPriority = 8;

/**
 * �Ō�ɏ����������̃g�[�N���̎��
 */
cci::token::TokenKind last_statement_token_kind = cci::token::kNon;

/**
 * �Ǐ��ϐ��A�h���X�Ǘ�
 */
static int local_var_address = 0;

/**
 * �Ǐ��ϐ��A�h���X����t���J�n�Ԓn
 */
static const int kIntSize = sizeof(int);
static const int kStartLocalAddress = 1 * kIntSize;

/**
 * ���A�h���X�Ǘ�
 */
static const int kMaxGlobalMemorySize = 0xFFFF;
static char global_memory[kMaxGlobalMemorySize];
static int global_address = 1 * kIntSize;

/**
 * �����֐��̃v���g�^�C�v�i�K�v�Ȋ֐��̂݁j
 */
void block(cci::token::Token &token, const bool is_function);
void Statement(cci::token::Token &token);

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
 * �Ǐ����������m��
 */
int AllocLocal(const int size)
{
    if (size <= 0)
    {
        return 0;
    }
    local_var_address += size;
    return (local_var_address - size);
}

/**
 * �Ǐ����������N���A
 */
void ClearLocal()
{
    local_var_address = 0;
}

/**
 * ���A�h���X���m��
 */
int AllocGlobal(const int size)
{
    if (size <= 0)
    {
        return global_address;
    }
    global_address = (global_address + 3) / 4 * 4;
    if (global_address + size > kMaxGlobalMemorySize)
    {
        Notice(cci::notice::kInternalErrorNotEnoughMemory);
        return 0;
    }
    std::memset(global_memory + global_address, 0, size);
    global_address += size;
    return (global_address - size);
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
 * �񍀉��Z�q�̗D�揇�ʂ��擾
 */
int GetPriority(const cci::token::TokenKind kind)
{
    switch(kind)
    {
    case cci::token::kMultiplication:
    case cci::token::kDivision:
    case cci::token::kModulo:
        return kMaxPriority - 1;
    case cci::token::kPlus:
    case cci::token::kMinus:
        return kMaxPriority - 2;
    case cci::token::kLess:
    case cci::token::kLessEqual:
    case cci::token::kGreat:
    case cci::token::kGreatEqual:
        return kMaxPriority - 3;
    case cci::token::kEqual:
    case cci::token::kNotEqual:
        return kMaxPriority - 4;
    case cci::token::kAnd:
        return kMaxPriority - 5;
    case cci::token::kOr:
        return kMaxPriority - 6;
    case cci::token::kAssignment:
        return kMaxPriority - 7;
    default:
        return 0;
    }
}

/**
 * �֐��Ăяo��
 */
void CallFunction(const cci::symbol::SymbolData* data)
{

}

/**
 * �V�X�e���֐��Ăяo��
 */
bool CallSystemFunction(cci::token::Token &token)
{
    switch(token.kind_)
    {
    case cci::token::kPrintf:
    case cci::token::kInput:
    case cci::token::kExit:
        break;
    default:
        return false;
    }
    return true;
}

/**
 * �֐����V���{���ɒǉ�����
 */
bool EntryFunction(cci::token::Token &token, const cci::symbol::SymbolDataType type)
{
    local_var_address = kStartLocalAddress;
    const cci::symbol::SymbolData* find = cci::symbol::SearchSymbolByName(token.text_);
    if (find != nullptr && find->kind_ != cci::symbol::kFunction && find->kind_ != cci::symbol::kPrototype)
    {
        Notice(cci::notice::kErrorAlreadyUsedName, token.text_);
        return false;
    }

    cci::symbol::SymbolData* funcSymbolData = cci::symbol::CreateSymbolData(token.text_);
    funcSymbolData->kind_ = cci::symbol::kFunction; // �֐��Ƃ��Ēǉ�
    funcSymbolData->dataType_ = type;
    funcSymbolData->level_ = block_nest_count;
    cci::symbol::Enter(funcSymbolData);

    if (!GetNextTokenInner(token) || token.kind_ != '(' || // '(' == cci::token::kLeftParenthesis
        !GetNextTokenInner(token)) 
    {
        return false;
    }

    cci::symbol::OpenLocalSymbol();
    switch (token.kind_)
    {
    case cci::token::kVoid:
        GetNextTokenInner(token);
        break;
    case ')': // ')' == cci::token::kRightParenthesis
        if (!GetNextTokenInner(token)) 
        {
            return false;
        }
        break;
    default:
        // �֐������̓o�^
        while (1)
        {
            const cci::symbol::SymbolDataType type = GetSymbolDataType(token.kind_);
            if (type == cci::symbol::kNon)
            {
                Notice(cci::notice::kErrorInvalidType, token.text_);
                return false;
            }

            if (!GetNextTokenInner(token)) 
            {
                return false;
            }
            cci::symbol::SymbolData* paramSymbolData = cci::symbol::CreateSymbolData(token.text_);
            paramSymbolData->kind_ = cci::symbol::kParam; // �֐������Ƃ��Ēǉ�
            paramSymbolData->dataType_ = type;
            paramSymbolData->level_ = block_nest_count;

            ++(funcSymbolData->args_);

            if (!GetNextTokenInner(token)) 
            {
                return false;
            }
            if (token.kind_ == ')' &&  // ')' == cci::token::kRightParenthesis
                GetNextTokenInner(token))
            {
                return true;
            }
            if (!GetNextTokenInner(token)) 
            {
                return false;
            }
        }
        if (!GetNextTokenInner(token) || token.kind_ != ')' || // ')' == cci::token::kRightParenthesis
            !GetNextTokenInner(token)) 
        {
            return false;
        }
        // �錾�����Ȃ�v���g�^�C�v�ɕύX
        if (token.kind_ == ';') // cci::token::kSemicolon
        {
            funcSymbolData->kind_ = cci::symbol::kPrototype;
        }
    }
    cci::symbol::CloseLocalSymbol(funcSymbolData);

    switch (token.kind_)
    {
    case cci::token::kVoid:
        GetNextTokenInner(token);
        break;
    case '{': // '{' == cci::token::kLeftCurlyBracket
        // main �֐����`�F�b�N
        if (strcmp("main", funcSymbolData->name_) == 0)
        {
            if (funcSymbolData->dataType_ != cci::symbol::kInteger || funcSymbolData->args_ != 0)
            {
                Notice(cci::notice::kErrorInvalidMainFunction);
            }
            cci::code::BackPatch(0, funcSymbolData->address_);
        }
        // �֐������J�n
        cci::code::GenerateCode2(cci::code::kAdbr, 0);
        cci::code::GenerateCode3(cci::code::kSto, cci::code::kLocalFlag, 0);
        for (int i = 0; i < funcSymbolData->args_; ++i)
        {
            const int index = funcSymbolData->args_ - i;
            cci::code::GenerateCode3(cci::code::kSto, cci::code::kLocalFlag, (funcSymbolData + i)->address_);
        }
        // �֐����u���b�N�����J�n
        block(token, true);
        // �֐������I��
        cci::code::BackPatch(funcSymbolData->address_, -local_var_address);
        if (last_statement_token_kind != cci::token::kReturn)
        {
            //
        }
        cci::code::BackPatchReturnCode(funcSymbolData->address_);
        cci::code::GenerateCode3(cci::code::kLod, cci::code::kLocalFlag, 0);
        cci::code::GenerateCode2(cci::code::kAdbr, local_var_address);
        cci::code::GenerateCode1(cci::code::kRet);
        break;
    }

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

/**
 * �֐��ƕϐ��̓o�^����
 */
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

/**
 * ���q�̏���
 */
void Factor(cci::token::Token &token)
{
    cci::token::TokenKind kind = token.kind_;
    switch (kind)
    {
    case cci::token::kPlus:
    case cci::token::kMinus:
        GetNextTokenInner(token);
        Factor(token);
        cci::code::GenerateCodeUnArray(kind);
        break;
    case cci::token::kIncrease:
    case cci::token::kDecrease:
        GetNextTokenInner(token);
        Factor(token);
        if (!cci::code::ToLeftValue())
        {
            Notice(cci::notice::kErrorInvalidLeftValue, token.text_);
        }
        cci::code::GenerateCodeUnArray(kind);
        break;
    case cci::token::kIntNum:
        cci::code::GenerateCode2(cci::code::kLdi, token.value_);
        GetNextTokenInner(token);
        break;
    case cci::token::kIdentifier:
        {
            const cci::symbol::SymbolData *data = cci::symbol::SearchSymbolByName(token.text_);
            switch (data->kind_)
            {
            case cci::symbol::kFunction:
            case cci::symbol::kPrototype:
                if (data->kind_ == cci::symbol::kVoid)
                {
                    // �G���[
                }
                CallFunction(data);
                break;
            case cci::symbol::kVar:
            case cci::symbol::kParam:
                // �P���ϐ�������
                if (data->arrayLength_ == 0)
                {
                    cci::code::GenerateCode3(cci::code::kLod, cci::symbol::GetCodeFlag(data), data->address_);
                    GetNextTokenInner(token);
                }
                else
                {
                    GetNextTokenInner(token);
                    if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
                    {
                        cci::code::GenerateCode3(cci::code::kLda, cci::symbol::GetCodeFlag(data), data->address_);
                        // �������疢����
                    }
                    else
                    {
                        Notice(cci::notice::kErrorNotFoundArrayIndex);
                    }

                    if (token.kind_ == cci::token::kIncrease)
                    {
                        cci::code::ToLeftValue();
                        cci::code::GenerateCode1(cci::code::kInc);
                        cci::code::GenerateCode2(cci::code::kLdi, 1);
                        cci::code::GenerateCode1(cci::code::kSub);
                        GetNextTokenInner(token);
                    }
                    else if (token.kind_ == cci::token::kDecrease)
                    {
                        cci::code::ToLeftValue();
                        cci::code::GenerateCode1(cci::code::kDec);
                        cci::code::GenerateCode2(cci::code::kLdi, 1);
                        cci::code::GenerateCode1(cci::code::kAdd);
                        GetNextTokenInner(token);
                    }
                }
                break;
            }
        }
        break;
    // �g�ݍ��݊֐��̏���
    case cci::token::kPrintf:
    case cci::token::kInput:
    case cci::token::kExit:
        CallSystemFunction(token);
        break;
    default:
        Notice(cci::notice::kErrorInvalidFormat);
    }
}

/**
 * ���̏���
 */
void Term(cci::token::Token &token, const int priority)
{
    if (priority == kMaxPriority)
    {
        Factor(token);
        return;
    }
    Term(token, priority + 1);

    cci::token::TokenKind kind;
    while (priority == GetPriority(token.kind_))
    {
        kind = token.kind_;
        GetNextTokenInner(token);
        Term(token, priority + 1);
        cci::code::GenerateCodeBinary(kind);
    }
}

/**
 * ���̏���
 */
void Expression(cci::token::Token &token)
{
    Term(token, kMinPriority + 2);
    if (token.kind_ == '=') // '{' == cci::token::kAssignment
    {
        cci::code::ToLeftValue();
        GetNextTokenInner(token);
        Expression(token);
        cci::code::GenerateCode1(cci::code::kAssv);
    }
}

/**
 * ���̏���
 */
void Statement(cci::token::Token &token)
{
    cci::token::TokenKind kind = token.kind_;
    bool get_next = false;
    
    switch(kind)
    {
    // ���ʎq
    case cci::token::kIdentifier:
        {
            const cci::symbol::SymbolData* data = cci::symbol::SearchSymbolByName(token.text_);
            if (data == nullptr)
            {
                // ����`�̕ϐ�
            }
            else if ((data->kind_ == cci::symbol::kFunction || data->kind_ == cci::symbol::kPrototype) && data->dataType_ == cci::symbol::kVoid)
            {
                CallFunction(data);
                get_next = true;
            }
            else
            {
                Expression(token);
                cci::code::RemoveValue();
            }
        }
        break;
    // ��֐��u���b�N
    case '{': // '{' == cci::token::kLeftCurlyBracket
        block(token, false);
        break;
    // ���̏I��
    case ';': // ';' == cci::token::kSemicolon
        get_next = true;
        break;
    // �Ӑ}���Ȃ��I��
    case cci::token::kEof:
        Notice(cci::notice::kErrorUnintendedEof);
        break;
    // �����ȋL�q
    default:
        Notice(cci::notice::kErrorInvalidFormat);
        get_next = true;
    }

    if (get_next)
    {
        GetNextTokenInner(token);
    }
}

/**
 * �u���b�N����
 */
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
            if (!GetNextTokenInner(token))
            {
                return;
            }
            EntryVars(token, type);
            type = GetSymbolDataType(token.kind_);
        }
    }

    // ���̏���
    cci::token::TokenKind kind = cci::token::kOther;
    while (token.kind_ != '}') // '}' == cci::token::kRightCurlyBracket
    {
        kind = token.kind_;
        Statement(token);
    }
    last_statement_token_kind = kind;

    --block_nest_count;
    GetNextTokenInner(token);
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

    //cci::symbol::DumpTable();
    cci::code::DumpCodes();

    cci::token::Finalize();
    cci::code::Finalize();
    cci::symbol::Finalize();
    cci::notice::Finalize();

    return 0;
}

} // namespace parser
} // namespace cci
