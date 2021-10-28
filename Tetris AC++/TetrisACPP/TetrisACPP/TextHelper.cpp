#include "TextHelper.h"
#include <sstream>
const char* TextHelper::intToTest(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str().c_str();
}
