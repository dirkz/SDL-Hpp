#pragma once

#include <string>

namespace zlang
{

struct Argument
{
    Argument(CXCursor cursor, const std::string &name, const std::string &declaration)
        : m_name{name}, m_declaration{declaration}
    {
        CXType cursorType = clang_getCursorType(cursor);
        m_isPointer = cursorType.kind == CXType_Pointer;
        if (m_isPointer)
        {
            CXType pointeeType = clang_getPointeeType(cursorType);
            CXString cxs = clang_getTypeSpelling(pointeeType);
            m_pointeeTypeString = clang_getCString(cxs);
            clang_disposeString(cxs);
        }
    }

    const std::string &Name() const
    {
        return m_name;
    }

    const std::string &Declaration() const
    {
        return m_declaration;
    }

    bool IsPointer() const
    {
        return m_isPointer;
    }

    const std::string &PointeeTypeString() const
    {
        return m_pointeeTypeString;
    }

  private:
    std::string m_name;
    std::string m_declaration;
    bool m_isPointer;
    std::string m_pointeeTypeString;
};

} // namespace zlang