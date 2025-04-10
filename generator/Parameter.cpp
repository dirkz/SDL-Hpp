#include "Parameter.h"

namespace zlang
{

Parameter::Parameter(CXCursor cursor, CXType cursorType)
{
    CXString displayName = clang_getCursorDisplayName(cursor);
    m_name = clang_getCString(displayName);
    clang_disposeString(displayName);
}

} // namespace zlang