#include "../include/code.h"

#include <iostream>

namespace cci {
namespace code {

struct CodeData
{
    OparationCode opcode_;
    int flag_;
    int data_;
};

int GenerateCode3(const OparationCode op_code, const int flag, const int data)
{
    std::cout << sizeof(CodeData) << std::endl;
    return 0;
}

} // namespace code
} // namespace cci
