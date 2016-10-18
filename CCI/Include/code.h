#ifndef CCI_CODE_H_
#define CCI_CODE_H_

namespace cci {
namespace code {

enum OparationCode
{
    Nop,
    Stop
};

int GenerateCode3(const OparationCode op_code, const int flag, const int data);

} // namespace code
} // namespace cci

#endif // CCI_CODE_H_
