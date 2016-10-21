#include "../include/parser.h"

#include <iostream>
#include "../include/token.h"
#include "../include/notice.h"
#include "../include/symbol.h"
#include "../include/code.h"

namespace
{

void Notice(const cci::notice::NoticeMessageId id)
{
    cci::notice::AddNotice(id, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

void Notice(const cci::notice::NoticeMessageId id, const char* text)
{
    cci::notice::AddNotice(id, text, cci::token::GetCurrentName(), cci::token::GetCurrentLineCount(), cci::token::GetCurrentCharCount());
}

bool GetNextToken(cci::token::Token &token)
{
    return (cci::token::GetNext(token) && token.kind_ != cci::token::kEof);
}

bool DefineVarOrFunction(cci::token::Token &token, cci::symbol::SymbolDataType type)
{
    // ���ʎq���`�F�b�N
    if (!GetNextToken(token) || token.kind_ != cci::token::kIdentifier)
    {
        Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
        return false;
    }
    // �^�͊m�肵�Ă���̂Ő�s���č쐬
    cci::symbol::SymbolData* tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
    tmpSymbolData->dataType_ = type;

    // C �̃X�^�C���ł͂��̎��_�ł͊֐��錾�Ȃ̂��ϐ��錾�Ȃ̂�����ł��Ȃ��̂�
    // ���̃g�[�N�����擾���Ă��画�肷��
    if (!GetNextToken(token))
    {
        return false;
    }
    // ���̃g�[�N���� ( �Ȃ�֐��錾
    if (token.kind_ == '(') // '(' == cci::token::kLeftParenthesis
    {
        // ��ނ��֐���
        tmpSymbolData->kind_ = cci::symbol::kFunction;
        cci::symbol::Enter(tmpSymbolData);
    }
    // ����ȊO�ł͕ϐ��錾
    else
    {
        // ��ނ�ϐ���
        tmpSymbolData->kind_ = cci::symbol::kVar;

        // , �ŕ����̕ϐ��錾�����ꍇ������̂ŌJ��Ԃ�
        while (1)
        {
            // �z��̐錾���`�F�b�N
            tmpSymbolData->arrayLength_ = 0;
            if (token.kind_ == '[') // '[' == cci::token::kLeftBracket
            {
                if (!GetNextToken(token))
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
                if (!GetNextToken(token) || token.kind_ != cci::token::kIdentifier)
                {
                    Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
                    return false;
                }
                tmpSymbolData = cci::symbol::CreateSymbolData(token.text_);
                tmpSymbolData->kind_ = cci::symbol::kVar;
                tmpSymbolData->dataType_ = type;

                // ���̊֐��̏����ɍ��킹�āA��������ǂ�ł���
                if (!GetNextToken(token))
                {
                    Notice(cci::notice::kErrorInvalidIdentifier);
                    return false;
                }
                continue;
            }
            else
            {
                // �錾�G���[
                Notice(cci::notice::kErrorInvalidIdentifier, token.text_);
                return false;
            }
        }
    }
    return true;
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

    while(cci::token::GetNext(token) && token.kind_ != cci::token::kEof)
    {
        cci::symbol::SymbolDataType tmpSymbolDataType = cci::symbol::kNon;

        switch (token.kind_)
        {
        case cci::token::kInt:
            tmpSymbolDataType = cci::symbol::kInteger;
            break;
        case cci::token::kVoid:
            tmpSymbolDataType = cci::symbol::kVoid;
            break;
        case cci::token::kSemicolon:
            // ���������Ɏ���
            continue;
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
