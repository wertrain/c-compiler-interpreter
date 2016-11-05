#ifndef CCI_TEST_H_
#define CCI_TEST_H_

namespace cci {
namespace test {

void Test(const bool test, const long line, const char* file);

bool TestSymbol();
bool TestToken();
bool TestParser();
bool TestCode();
bool TestExcute();

} // namespace test
} // namespace cci

#endif // CCI_TEST_H_
