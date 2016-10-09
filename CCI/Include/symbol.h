#ifndef CCI_SYMBOL_H_
#define CCI_SYMBOL_H_

namespace cci {
namespace symbol {

enum SymbolKind
{
    kNumber = 0,
    kVar,
    kFunction,
    kPrototype,
    kParam
};

enum DataType
{
    kNon = 0,
    kVoid,
    kInteger,
};

struct SymbolData
{
    char* name_;        /** �ϐ���֐��̖��O */
    SymbolKind kind_;   /** ��� */
    DataType dataType_; /** �^�C�v */
    int arrayLength;    /** �z��, 0 �Ȃ�P���ϐ� */
    char level;         /** �L���̈�B�O���[�o���Ȃ�0, ���[�J���Ȃ�1 */
    char args;          /** �֐��̏ꍇ�̈����� */
    int address;        /** �ϐ��̃A�h���X */
};

} // namespace symbol
} // namespace cci

#endif // CCI_SYMBOL_H_
