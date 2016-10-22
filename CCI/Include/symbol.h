#ifndef CCI_SYMBOL_H_
#define CCI_SYMBOL_H_

namespace cci {
namespace symbol {

/**
 * �V���{���i�[�ő吔
 */
const int kMaxSymbol = 1024;

enum SymbolKind
{
    kNumber = 0,
    kVar,
    kFunction,
    kPrototype,
    kParam
};

enum SymbolDataType
{
    kNon = 0,
    kVoid,
    kInteger,
};

struct SymbolData
{
    char* name_;        /** �ϐ���֐��̖��O */
    SymbolKind kind_;   /** ��� */
    SymbolDataType dataType_; /** �^�C�v */
    int arrayLength_;   /** �z��, 0 �Ȃ�P���ϐ� */
    char level_;        /** �L���̈�, �O���[�o���Ȃ�0, ���[�J���Ȃ�1 */
    char args_;         /** �֐��̏ꍇ�̈����� */
    int address_;       /** �ϐ��̃A�h���X */
};

SymbolData* CreateSymbolData(const char* name);

bool Enter(const SymbolData *data);

/**
 * �V���{���֘A�̏�����
 */
bool Initialize();

/**
 * �V���{���֘A�̏I��
 */
void Finalize();

/**
 * �V���{������������
 * @param name �������閼�O
 * @return ���������V���{���A������Ȃ���� nullptr ��Ԃ�
 */
const SymbolData* SearchSymbolByName(const char* name);

/**
 * cci::code �Ŏg�p����t���O���擾����
 * @param data ���肷��f�[�^
 * @return �Ǐ��ϐ��Ȃ� 1 ����ȊO�Ȃ� 0
 */
int GetCodeFlag(const SymbolData *data);

} // namespace symbol
} // namespace cci

#endif // CCI_SYMBOL_H_
