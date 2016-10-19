#ifndef CCI_CODE_H_
#define CCI_CODE_H_

namespace cci {
namespace code {

/**
 * �R�[�h�i�[�ő吔
 */
const int kMaxCodeSize = 1024 * 4;

/**
 * �����R�[�h
 */
enum OparationCode
{
    Nop,
    Stop
};

/**
 * �R�[�h�𐶐�����
 * @param op_code �����R�[�h
 * @param flag �t���O
 * @param data �f�[�^
 */
int GenerateCode3(const OparationCode op_code, const int flag, const int data);

} // namespace code
} // namespace cci

#endif // CCI_CODE_H_
