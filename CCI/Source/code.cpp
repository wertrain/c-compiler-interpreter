#include "../include/code.h"

#include "../include/notice.h"
#include "../include/token.h"
#include "../include/notice.h"

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

static const int kIntSize = sizeof(int);

/**
 * スタック管理
 */
static const int kMaxStackSize = 128;
static const int kStackBottom = 0;
static int stack_array[kMaxStackSize + 1];

/**
 * メモリ管理
 */
static const int kMaxMemorySize = 0xffff;
static char memory_array[kMaxMemorySize + 1];
static int gloval_address = 1 * kIntSize;

static int base_register = 0;

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
    memset(stack_array, 0, sizeof(stack_array));
    memset(memory_array, 0, sizeof(memory_array));
    gloval_address = 1 * kIntSize;

    base_register = 0;
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

bool IsCode(const int index, const cci::code::OparationCode opcode, const int data)
{
    if (index < 0 || index >= cci::code::kMaxCodeSize)
    {
        return false;
    }
    return codedata_array[index].opcode_ == opcode && codedata_array[index].data_ == data;
}

int ToIntMemory(const int address)
{
    return (*(int*)(memory_array + address));
}

void Assign(const int address, const int data)
{
    int memory = ToIntMemory(address);
    memory = data;
}

void Addition(const int address, const int data)
{
    int memory = ToIntMemory(address);
    memory += data;
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

void BackPatch(const int index, const int address)
{
    if (index < 0 || index >= kMaxCodeSize)
    {
        return;
    }
    codedata_array[index].data_ = address;
}

void BackPatchReturnCode(const int address)
{
    // 直前が kRet 関連 kJmp なら不要なので削除
    for (int i = 0; i < codedata_count; ++i)
    {
        const int index = codedata_count - i - 1;
        if (IsCode(index, kJmp, kNoFixReturnAddress))
        {
            --codedata_count;
        }
        else
        {
            break;
        }
    }
    // 未定番地なら次番地を設定
    for (int i = 0; i < codedata_count; ++i)
    {
        const int index = codedata_count - i - 1;
        if (IsCode(index, kJmp, kNoFixReturnAddress))
        {
            codedata_array[index].data_ = codedata_count + 1;
        }
    }
}

bool ToLeftValue()
{
    const int index = codedata_count - 1;
    switch(codedata_array[index].opcode_)
    {
    case kVal: 
       --codedata_count;
       break;
    case kLod:
        codedata_array[index].opcode_ = kLda;
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

int Execute()
{
    const int kStartLocalMemory = ToIntMemory(0);
    int pc = 0;
    int stackTop = 0;

    while (1)
    {
        OparationCode opcode = codedata_array[pc].opcode_;
        int opdata = codedata_array[pc].data_;
        const int address = (codedata_array[pc].flag_ & 0x01) ? base_register + opdata : opdata;

        if (stackTop > kMaxStackSize)
        {
            cci::notice::AddNotice(cci::notice::kInternalErrorStackOverflow);
            return 0;
        }
        else if (stackTop < kStackBottom)
        {
            cci::notice::AddNotice(cci::notice::kInternalErrorStackUnderflow);
            return 0;
        }

        switch (opcode)
        {
        case kDel:
            --stackTop;
            break;
        case kStop:
            if (stackTop > 0)
            {
                //pop
            }
            else
            {
                return 0;
            }
            break;
        case kJmp:
            pc = opdata;
            break;
        case kJpt:
            if (stack_array[stackTop--])
            {
                pc = opdata;
            }
            break;
        case kJpf:
            if (!stack_array[stackTop--])
            {
                pc = opdata;
            }
            break;
        case kLib:
            break;
        case kLod:
             stack_array[++stackTop] = ToIntMemory(address);
             break;
        case kLda:
            stack_array[++stackTop] = address;
            break;
        case kLdi:
            stack_array[++stackTop] = opdata;
            break;
        case kSto:
            Assign(address, stack_array[stackTop]);
            --stackTop;
            break;
        case kAdbr:
            base_register += opdata;
            if (base_register < kStartLocalMemory)
            {
                cci::notice::AddNotice(cci::notice::kInternalErrorStackMemoryOver);
                return 0;
            }
            break;
        case kNop:
            ++pc;
            break;
        case kAss:
            Assign(stack_array[stackTop - 1], stack_array[stackTop]);
            stackTop -= 2;
            break;
        case kAssv:
            Assign(stack_array[stackTop - 1], stack_array[stackTop]);
            stack_array[stackTop - 1] = stack_array[stackTop];
            --stackTop;
            break;
        case kVal:
            stack_array[stackTop] = ToIntMemory(stack_array[stackTop]);
            break;
        case kEqcmp:
            if (opdata == stack_array[stackTop])
            {
                stack_array[stackTop] = 1;
            }
            else
            {
                stack_array[++stackTop] = 0;
            }
            break;
        case kCall:
            stack_array[++stackTop] = pc;
            pc = opdata;
            break;
        case kRet:
            pc = stack_array[stackTop--];
            break;
        case kInc:
            Addition(stack_array[stackTop], +1);
            stack_array[stackTop] = ToIntMemory(stack_array[stackTop]);
            break;
        case kDec:
            Addition(stack_array[stackTop], -1);
            stack_array[stackTop] = ToIntMemory(stack_array[stackTop]);
            break;
        case kNot:
            break;
        default:
            cci::notice::AddNotice(cci::notice::kInternalErrorInvalidCommand);
            return 0;
        }
    }
    return 0;
}

#ifdef _DEBUG

static const char* kOparationCodeText[] =
{
    "kNop", "kInc", "kDec", "kNeg", "kNot", "kAdd", "kSub", "kMul", "kDiv", "kMod", "kLess", "kLessEq", 
    "kGrt", "kGrtEq", "kEq", "kNotEq", "kAnd", "kOr", "kCall", "kDel", "kJmp", "kJpt", "kJpf", "kEqcmp",
    "kLod", "kLda", "kLdi", "kSto", "kAdbr", "kRet", "kAss", "kAssv", "kVal", "kLib", "kStop"
};

void DumpCodes()
{
    for (int i = 0; i < codedata_count; ++i)
    {
        std::cout << "[" << kOparationCodeText[codedata_array[i].opcode_] << "]" << " - ";
        std::cout << (codedata_array[i].flag_ == 0 ? "false" : "true") << " : ";
        std::cout << codedata_array[i].data_;
        std::cout << std::endl;
    }
}

#endif // _DEBUG

} // namespace code
} // namespace cci
