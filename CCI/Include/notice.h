#ifndef CCI_NOTICE_H_
#define CCI_NOTICE_H_

namespace cci {
namespace notice {

/**
 * 通知格納最大数
 */
const int kMaxNotice = 128;

/**
 * メッセージサイズ
 */
const int kNoticeMessageSize = 256;

/**
 * ファイル名サイズ
 */
const int kNoticeFilenameSize = 256;

/**
 * エラー/警告リスト
 */
enum NoticeMessageId
{
    kErrorNone,
    kErrorTooLongCharacter,
    kErrorMissingSingleQuote,
    kErrorMissingDoubleQuote,
    kErrorInvalidString,
    kErrorUnknownCharacter,
    kErrorNotFoundArrayIndex,
    kErrorInvalidIdentifier,
    kErrorInvalidFormat,
    kErrorInvalidLeftValue,
    kErrorInvalidType,
    kErrorUnintendedEof,
    kErrorAlreadyUsedName,
    kErrorInvalidMainFunction,
    kInternalErrorOverflowCodeData,
    kInternalErrorNotEnoughMemory,
    kInternalErrorStackOverflow,
    kInternalErrorStackUnderflow,
    kInternalErrorStackMemoryOver,
    kInternalErrorInvalidCommand,
    kErrorUnknown
};
 
/**
 * エラー/警告メッセージ
 */
static const char* kNoticeMessageList[] =
{
    "",
    "2文字以上の文字定数.",
    "'が見つからない.",
    "\"が見つからない.",
    "無効な文字列.",
    "不明な文字.",
    "配列の添え字が見つからない.",
    "無効な識別子.",
    "無効な記述.",
    "無効な左辺値.",
    "無効な型.",
    "意図しない終了.",
    "この名前は既に使用させれている.",
    "main関数の書式が不正.",
    "コードが格納できる最大サイズを超えた.",
    "メモリが不足している.",
    "スタックオーバーフロー.",
    "スタックアンダーフロー.",
    "スタックメモリーオーバー.",
    "無効な命令.",
    "不明なエラー."
};

/**
 * 通知タイプ
 */
enum NoticeType
{
    kError,
    kWarning,
    kMax
};

/**
 * 通知パラメータ
 */
struct NoticeParam
{
    NoticeType type_;
    int line_;
    int character_;
    char filename_[kNoticeFilenameSize];
    char message_[kNoticeMessageSize];
};

/**
 * 通知関連の初期化
 */
bool Initialize();

/**
 * 通知関連の終了
 */
void Finalize();

/**
 * 通知の追加
 */
void AddNotice(const char* message, const char* filename, const int line, const int character);

/**
 * 通知の追加
 */
void AddNotice(const NoticeMessageId id, const char* filename, const int line, const int character);

/**
 * 通知の追加
 */
void AddNotice(const NoticeMessageId id, const char* text, const char* filename, const int line, const int character);

/**
 * 通知の追加
 */
void AddNotice(const NoticeMessageId id);
 
/**
 * 通知の表示
 */
void PrintNotice();

/**
 * 通知のクリア
 */
void ClearNotice();

/**
 * タイプ別通知数の取得
 */
int GetNoticeCount(const NoticeType type);

/**
 * 通知数の取得
 */
int GetNoticeAmountCount();

} // namespace notice
} // namespace cci

#endif  // CCI_NOTICE_H_
