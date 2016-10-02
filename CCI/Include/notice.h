#ifndef CCI_NOTICE_H_
#define CCI_NOTICE_H_

namespace cci {
namespace notice {

/**
 * �ʒm�i�[�ő吔
 */
const int kMaxNotice = 128;

/**
 * ���b�Z�[�W�T�C�Y
 */
const int kNoticeMessageSize = 256;

/**
 * �t�@�C�����T�C�Y
 */
const int kNoticeFilenameSize = 256;

/**
 * �G���[/�x�����X�g
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
 * �G���[/�x�����b�Z�[�W
 */
static const char* kNoticeMessageList[] =
{
    "",
    "2�����ȏ�̕����萔.",
    "'��������Ȃ�.",
    "\"��������Ȃ�.",
    "�����ȕ�����."
    "�s���ȕ���.",
};


enum NoticeType
{
    kError,
    kWarning,
    kMax
};

struct NoticeParam
{
    NoticeType type_;
    int line_;
    int character_;
    char filename_[kNoticeFilenameSize];
    char message_[kNoticeMessageSize];
};

/**
 * �ʒm�֘A�̏�����
 */
bool Initialize();

/**
 * �ʒm�̒ǉ�
 */
void AddNotice(const char* message, const char* filename, const int line, const int character);
void AddNotice(const NoticeMessageId id, const char* filename, const int line, const int character);
 
/**
 * �ʒm�̕\��
 */
void PrintNotice();

/**
 * �ʒm�̃N���A
 */
void ClearNotice();

/**
 * �ʒm���̎擾
 */
int GetNoticeCount();

} // namespace notice
} // namespace cci

#endif  // CCI_NOTICE_H_