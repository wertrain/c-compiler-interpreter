#ifndef CCI_CODE_H_
#define CCI_CODE_H_

namespace cci {
namespace code {

/**
 * �R�[�h�i�[�ő吔
 */
const int kMaxCodeSize = 1024 * 16;

/**
 * �G���[�萔
 */
const int kError = -1;

/**
 * �����R�[�h
 */
enum OparationCode
{
    kNop, kInc, kDec, kNeg, kNot, kAdd, kSub, kMul, kDiv, kMod, kLess, kLessEq, 
    kGrt, kGrtEq, kEq, kNotEq, kAnd, kOr, kCall, kDel, kJmp, kJpt, kJpf, kEqcmp,
    kLod, kLda, kLdi, kSto, kAdbr, kRet, kAss, kAssv, kVal, kLib, kStop
};

/**
 * �R�[�h�֘A�̏�����
 */
bool Initialize();

/**
 * �R�[�h�֘A�̏I��
 */
void Finalize();

/**
 * �R�[�h�𐶐�����
 * @param op_code �����R�[�h
 * @param flag �t���O
 * @param data �f�[�^
 * @return ���݂̃R�[�h���A�ǉ��ł��Ȃ���� cci::code::kError ��Ԃ�
 */
int GenerateCode3(const OparationCode opcode, const int flag, const int data);

/**
 * �R�[�h�𐶐�����
 * @param op_code �����R�[�h
 * @param flag �t���O
 * @return ���݂̃R�[�h���A�ǉ��ł��Ȃ���� cci::code::kError ��Ԃ�
 */
int GenerateCode2(const OparationCode opcode, const int data);

/**
 * �R�[�h�𐶐�����
 * @param op_code �����R�[�h
 * @return ���݂̃R�[�h���A�ǉ��ł��Ȃ���� cci::code::kError ��Ԃ�
 */
int GenerateCode1(const OparationCode opcode);

/**
 * �R�[�h�𐶐�����
 * @param kind �g�[�N�����
 */
void GenerateCodeBinary(const int kind);

/**
 * �R�[�h�𐶐�����
 * @param kind �g�[�N�����
 */
void GenerateCodeUnArray(const int kind);

/**
 * ���Ӓl�ɂ���
 */
bool ToLeftValue();

/**
 * ���̒l���c�� kAssv �� kAss �ɕύX����
 * �܂��́A kDel ��ǉ�����
 */
void RemoveValue();

#ifdef _DEBUG

/**
 * �z����e���_���v����
 */
void DumpCodes();

#endif // _DEBUG

} // namespace code
} // namespace cci

#endif // CCI_CODE_H_
