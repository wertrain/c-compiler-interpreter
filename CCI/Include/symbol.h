#ifndef CCI_SYMBOL_H_
#define CCI_SYMBOL_H_

namespace cci {
namespace symbol {

/**
 * シンボル格納最大数
 */
const int kMaxSymbol = 1024;

/**
 * シンボルの種類
 */
enum SymbolKind
{
    kNumber = 0,
    kVar,
    kFunction,
    kPrototype,
    kParam
};

/**
 * シンボルのデータタイプ
 */
enum SymbolDataType
{
    kNon = 0,
    kVoid,
    kInteger,
};

/**
 * シンボルデータ構造体
 */
struct SymbolData
{
    char* name_;        /** 変数や関数の名前 */
    SymbolKind kind_;   /** 種類 */
    SymbolDataType dataType_; /** タイプ */
    int arrayLength_;   /** 配列長, 0 なら単純変数 */
    char level_;        /** 記憶領域, グローバルなら0, ローカルなら1 */
    char args_;         /** 関数の場合の引数個数 */
    int address_;       /** 変数のアドレス */
};

/**
 * 指定された名前でシンボルデータを作成
 * 返り値であるシンボルデータ構造体のポインタは
 * Finalize 時に削除されるので delete 等は不要
 * @param name 追加するシンボルの名前
 * @return シンボルデータのポインタ
 */
SymbolData* CreateSymbolData(const char* name);

/**
 * シンボルを登録
 * @param data 登録するデータ
 * @return 登録に成功したか
 */
bool Enter(const SymbolData *data);

/**
 * シンボル関連の初期化
 */
bool Initialize();

/**
 * シンボル関連の終了
 */
void Finalize();

/**
 * シンボルを検索する
 * @param name 検索する名前
 * @return 見つかったシンボル、見つからなければ nullptr を返す
 */
const SymbolData* SearchSymbolByName(const char* name);

/**
 * cci::code で使用するフラグを取得する
 * @param data 判定するデータ
 * @return 局所変数なら 1 それ以外なら 0
 */
int GetCodeFlag(const SymbolData *data);

/**
 * 局所記号表の開始
 */
void OpenLocalSymbol();

/**
 * 局所記号表の終了
 */
void CloseLocalSymbol(const SymbolData* data);

#ifdef _DEBUG

/**
 * テーブル内容をダンプする
 */
void DumpTable();

#endif // _DEBUG

} // namespace symbol
} // namespace cci

#endif // CCI_SYMBOL_H_
