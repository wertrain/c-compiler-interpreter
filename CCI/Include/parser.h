#ifndef CCI_PARSER_H_
#define CCI_PARSER_H_

namespace cci {
namespace parser {

/**
 * �R���p�C�����s��
 * @param name �t�@�C����
 * @param text �R���p�C���Ώۂ̃e�L�X�g
 * @param text_size �R���p�C���Ώۂ̃e�L�X�g�̃T�C�Y
 * @return 
 */
int compile(const char* name, const char* text, const int text_size);

} // namespace parser
} // namespace cci

#endif // CCI_PARSER_H_
