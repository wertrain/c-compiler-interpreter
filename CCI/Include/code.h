#ifndef CCI_CODE_H_
#define CCI_CODE_H_

namespace cci {
namespace code {

/**
 * コード格納最大数
 */
const int kMaxCodeSize = 1024 * 16;

/**
 * エラー定数
 */
const int kError = -1;

/**
 * 処理コード
 */
enum OparationCode
{
    kNop, kInc, kDec, kNeg, kNot, kAdd, kSub, kMul, kDiv, kMod, kLess, kLessEq, 
    kGrt, kGrtEq, kEq, kNotEq, kAnd, kOr, kCall, kDel, kJmp, kJpt, kJpf, kEqcmp,
    kLod, kLda, kLdi, kSto, kAdbr, kRet, kAss, kAssv, kVal, kLib, kStop
};

/**
 * コード関連の初期化
 */
bool Initialize();

/**
 * コード関連の終了
 */
void Finalize();

/**
 * コードを生成する
 * @param op_code 処理コード
 * @param flag フラグ
 * @param data データ
 * @return 現在のコード数、追加できなければ cci::code::kError を返す
 */
int GenerateCode3(const OparationCode opcode, const int flag, const int data);

/**
 * コードを生成する
 * @param op_code 処理コード
 * @param flag フラグ
 * @return 現在のコード数、追加できなければ cci::code::kError を返す
 */
int GenerateCode2(const OparationCode opcode, const int data);

/**
 * コードを生成する
 * @param op_code 処理コード
 * @return 現在のコード数、追加できなければ cci::code::kError を返す
 */
int GenerateCode1(const OparationCode opcode);

/**
 * コードを生成する
 * @param kind トークン種類
 */
void GenerateCodeBinary(const int kind);

/**
 * コードを生成する
 * @param kind トークン種類
 */
void GenerateCodeUnArray(const int kind);

/**
 * 左辺値にする
 */
bool ToLeftValue();

} // namespace code
} // namespace cci

#endif // CCI_CODE_H_
