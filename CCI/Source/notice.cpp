#include "../Include/notice.h"

#include <iostream>
#include <cstring>

namespace cci {
namespace notice {

static NoticeParam notice_list[kMaxNotice];
static int notice_count = 0;

bool Initialize()
{
    notice_count = 0;
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

} // namespace notice
} // namespace cci
