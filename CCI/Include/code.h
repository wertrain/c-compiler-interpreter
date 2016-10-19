#ifndef CCI_CODE_H_
#define CCI_CODE_H_

namespace cci {
namespace code {

/**
 * コード格納最大数
 */
const int kMaxCodeSize = 1024 * 4;

/**
 * 処理コード
 */
enum OparationCode
{
    Nop,
    Stop
};

/**
 * コードを生成する
 * @param op_code 処理コード
 * @param flag フラグ
 * @param data データ
 */
int GenerateCode3(const OparationCode op_code, const int flag, const int data);

} // namespace code
} // namespace cci

#endif // CCI_CODE_H_
