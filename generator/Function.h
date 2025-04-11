#pragma once

#include <string>

#include <clang-c/Index.h>

#include "Parameter.h"

namespace zlang
{

struct Function
{
    Function(CXCursor cursor)
    {
        CXString functionName = clang_getCursorSpelling(cursor);
        m_name = clang_getCString(functionName);
        clang_disposeString(functionName);

        m_returnType = clang_getCursorResultType(cursor);
        CXString functionResultTypeCX = clang_getTypeSpelling(m_returnType);
        m_returnTypeString = clang_getCString(functionResultTypeCX);
        clang_disposeString(functionResultTypeCX);
    }

    void AddArgument(CXCursor cursor)
    {
        CXString argName = clang_getCursorDisplayName(cursor);
        string argNameString{clang_getCString(argName)};
        clang_disposeString(argName);

        CXType argCursorType = clang_getCursorType(cursor);
        string argTypeString = ctypeString(argCursorType);

        CXString pretty = clang_getCursorPrettyPrinted(cursor, nullptr);
        string prettyString{clang_getCString(pretty)};
        clang_disposeString(pretty);
    }

    const std::string &Name() const
    {
        return m_name;
    }

    CXType ReturnType() const
    {
        return m_returnType;
    }

    const std::string &ReturnTypeString() const
    {
        return m_returnTypeString;
    }

  private:
    std::string m_name;
    CXType m_returnType;
    std::string m_returnTypeString;
    std::vector<std::string> m_parameters;
};

} // namespace zlang
