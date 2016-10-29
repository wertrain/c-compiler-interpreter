#include "../include/code.h"

#include "../include/notice.h"
#include "../include/token.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

namespace
{

/**
 * コード格納部分 (16 byte)
 */
struct CodeData
{
    cci::code::OparationCode opcode_;
    int flag_;
    int data_;
    int reserved_;
};

static CodeData codedata_array[cci::code::kMaxCodeSize];
static int codedata_count = 0;

void ResetInner()
{
    codedata_count = 0;
    for (int i = 0; i < cci::code::kMaxCodeSize; ++i)
    {
        codedata_array[i].opcode_ = cci::code::kNop;
        codedata_array[i].flag_ = 0;
        codedata_array[i].data_ = 0;
        codedata_array[i].reserved_ = 0;
    }
}

int GenerateCode(const cci::code::OparationCode opcode, const int flag, const int data)
{
    if (codedata_count >= cci::code::kMaxCodeSize)
    {
        cci::notice::AddNotice(cci::notice::kInternalErrorOverflowCodeData);
        return cci::code::kError;
    }

    codedata_array[codedata_count].opcode_ = opcode;
    codedata_array[codedata_count].flag_ = flag;
    codedata_array[codedata_count].data_ = data;
    ++codedata_count;

    return codedata_count;
}

} // namespace

namespace cci {
namespace code {

bool Initialize()
{
    ResetInner();
    return true;
}

void Finalize()
{

}

int GenerateCode3(const OparationCode opcode, const int flag, const int data)
{
    return GenerateCode(opcode, flag, data);
}

int GenerateCode2(const OparationCode opcode, const int data)
{
    return GenerateCode(opcode, 0, data);
}

int GenerateCode1(const OparationCode opcode)
{
    return GenerateCode(opcode, 0, 0);
}

void GenerateCodeBinary(const int kind)
{
    OparationCode code = kNop;
    switch(kind)
    {
    case cci::token::kPlus:             code = kAdd;    break;
    case cci::token::kMinus:            code = kSub;    break;
    case cci::token::kMultiplication:   code = kMul;    break;
    case cci::token::kDivision:         code = kDiv;    break;
    case cci::token::kLess:             code = kLess;   break;
    case cci::token::kLessEqual:        code = kLessEq; break;
    case cci::token::kGreat:            code = kGrt;    break;
    case cci::token::kGreatEqual:       code = kGrtEq;  break;
    case cci::token::kEqual:            code = kEq;     break;
    case cci::token::kNotEqual:         code = kNotEq;  break;
    case cci::token::kAnd:              code = kAnd;    break;
    case cci::token::kOr:               code = kOr;     break;
    case cci::token::kModulo:           code = kMod;    break;
    }
    GenerateCode1(code);
}

void GenerateCodeUnArray(const int kind)
{
    OparationCode code = kNop;
    switch(kind)
    {
    case cci::token::kPlus:      return;
    case cci::token::kMinus:     code = kNeg;    break;
    case cci::token::kNot:       code = kNot;    break;
    case cci::token::kIncrease:  code = kInc;    break;
    case cci::token::kDecrease:  code = kDec;    break;
    }
    GenerateCode1(code);
}

bool ToLeftValue()
{
    switch(codedata_array[codedata_count].opcode_)
    {
    case kVal: 
       --codedata_count;
       break;
    case kLod:
        codedata_array[codedata_count - 1].opcode_ = kLda;
        break;
    // 不正な左辺値
    default:
        return false;
    }
    return true;
}

void RemoveValue()
{
    const int index = codedata_count - 1;
    if (codedata_array[index].opcode_ == kAssv)
    {
        codedata_array[index].opcode_ = kAss;
    }
    else
    {
        GenerateCode1(kDel);
    }
}

#ifdef _DEBUG

static const char* kOparationCodeText[] =
{
    "kNop", "kInc", "kDec", "kNeg", "kNot", "kAdd", "kSub", "kMul", "kDiv", "kMod", "kLess", "kLessEq", 
    "kGrt", "kGrtEq", "kEq", "kNotEq", "kAnd", "kOr", "kCall", "kDel", "kJmp", "kJpt", "kJpf", "kEqcmp",
    "kLod", "kLda", "kLdi", "kSto", "kAdbr", "kRet", "kAss", "kAssv", "kVal", "kLib", "kStop"
};

void DumpCodes(const cci::code::OparationCode opcode, const int flag, const int data)
{
    for (int i = 0; i < codedata_count; ++i)
    {
        int index = codedata_count - i;
        std::cout << "[" << kOparationCodeText[codedata_array[index].opcode_] << "]" << " - ";
        std::cout << codedata_array[index].flag_ << " - ";;
        std::cout << codedata_array[index].data_;
        std::cout << std::endl;
    }
}

#endif // _DEBUG

} // namespace code
} // namespace cci
