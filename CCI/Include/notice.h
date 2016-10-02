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
    "無効な文字列."
    "不明な文字.",
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
 * 通知の追加
 */
void AddNotice(const char* message, const char* filename, const int line, const int character);

/**
 * 通知の追加
 */
void AddNotice(const NoticeMessageId id, const char* filename, const int line, const int character);
 
/**
 * 通知の表示
 */
void PrintNotice();

/**
 * 通知のクリア
 */
void ClearNotice();

/**
 * 通知数の取得
 */
int GetNoticeCount();

} // namespace notice
} // namespace cci

#endif  // CCI_NOTICE_H_
