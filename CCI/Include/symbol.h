#ifndef CCI_SYMBOL_H_
#define CCI_SYMBOL_H_

namespace cci {
namespace symbol {

/**
 * シンボル格納最大数
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

enum DataType
{
    kNon = 0,
    kVoid,
    kInteger,
};

struct SymbolData
{
    char* name_;        /** 変数や関数の名前 */
    SymbolKind kind_;   /** 種類 */
    DataType dataType_; /** タイプ */
    int arrayLength_;   /** 配列長, 0 なら単純変数 */
    char level_;        /** 記憶領域, グローバルなら0, ローカルなら1 */
    char args_;         /** 関数の場合の引数個数 */
    int address_;       /** 変数のアドレス */
};

SymbolData* CreateSymbolData(const char* name);

SymbolData* Enter(const SymbolData *data, SymbolKind kind);

/**
 * シンボル関連の初期化
 */
bool Initialize();

/**
 * シンボル関連の終了
 */
void Finalize();

} // namespace symbol
} // namespace cci

#endif // CCI_SYMBOL_H_
