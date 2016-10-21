#include "../include/code.h"

#include "../include/notice.h"

namespace
{

/**
 * ÉRÅ[Éhäiî[ïîï™ (16 byte)
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

int GenerateCode2(const OparationCode opcode, const int flag)
{
    return GenerateCode(opcode, flag, 0);
}

int GenerateCode1(const OparationCode opcode)
{
    return GenerateCode(opcode, 0, 0);
}

} // namespace code
} // namespace cci
