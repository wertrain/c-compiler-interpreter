#ifndef CCI_PARSER_H_
#define CCI_PARSER_H_

namespace cci {
namespace parser {

/**
 * コンパイルを行う
 * @param name ファイル名
 * @param text コンパイル対象のテキスト
 * @param text_size コンパイル対象のテキストのサイズ
 * @return 
 */
int compile(const char* name, const char* text, const int text_size);

} // namespace parser
} // namespace cci

#endif // CCI_PARSER_H_
