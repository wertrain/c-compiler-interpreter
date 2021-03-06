#include "../Include/notice.h"

#include <iostream>
#include <cstring>

namespace cci {
namespace notice {

static NoticeParam notice_list[kMaxNotice];
static int notice_count = 0;

/**
 * 内部エラー文字列
 */
const char* kInternalErrorName = "Error Internal";

bool Initialize()
{
    ClearNotice();
    return true;
}

void Finalize()
{

}

void AddNotice(const char* message, const char* filename, const int line, const int character)
{
    strcpy_s(notice_list[notice_count].message_, kNoticeMessageSize, message);
    strcpy_s(notice_list[notice_count].filename_, kNoticeFilenameSize, filename);
    notice_list[notice_count].line_ = line;
    notice_list[notice_count].character_ = character;
    ++notice_count;
}

void AddNotice(const NoticeMessageId id, const char* filename, const int line, const int character)
{
    AddNotice(kNoticeMessageList[id], filename, line, character);
}

void AddNotice(const NoticeMessageId id, const char* text, const char* filename, const int line, const int character)
{
    char message[kNoticeMessageSize];
    sprintf_s(message, kNoticeMessageSize, "%s:%s", text, kNoticeMessageList[id]);
    AddNotice(message, filename, line, character);
}

void AddNotice(const NoticeMessageId id)
{
    AddNotice(kNoticeMessageList[id], "", 0, 0);
}

void PrintNotice()
{
    if (notice_count == 0)
    {
        return;
    }

    for (int i = 0; i < notice_count; ++i)
    {
        if (notice_list[i].type_ == kError)
        {
             std::cout << "Error: ";
        }
        else if (notice_list[i].type_ == kWarning)
        {
             std::cout << "Warning: ";
        }
        std::cout << notice_list[i].filename_ << ":" << 
            notice_list[i].line_ << " " << notice_list[i].message_ << std::endl;
    }
}

int GetNoticeCount(const NoticeType type)
{
    int count = 0;
    for (int i = 0; i < notice_count; ++i)
    {
        if (notice_list[i].type_ == type)
        {
             ++count;
        }
    }
    return count;
}

int GetNoticeAmountCount()
{
    return notice_count;
}

void ClearNotice()
{
    for (int i = 0; i < kMaxNotice; ++i)
    {
        notice_list[i].type_ = kMax;
        notice_list[i].filename_[0] = '\0';
        notice_list[i].message_[0] = '\0';
        notice_list[i].line_ = 0;
        notice_list[i].character_ = 0;
    }
    notice_count = 0;
}

} // namespace notice
} // namespace cci
