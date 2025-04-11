#pragma once

#include <string>

#include <clang-c/Index.h>

#include "Argument.h"

namespace zlang
{

struct Function
{
    Function(CXCursor cursor)
    {
        CXString functionName = clang_getCursorSpelling(cursor);
        m_name = clang_getCString(functionName);
        clang_disposeString(functionName);

        m_namespacedName = m_name.substr(0, 4);

        m_returnType = clang_getCursorResultType(cursor);
        CXString functionResultTypeCX = clang_getTypeSpelling(m_returnType);
        m_returnTypeString = clang_getCString(functionResultTypeCX);
        clang_disposeString(functionResultTypeCX);
    }

    void AddArgument(CXCursor cursor)
    {
        CXString argName = clang_getCursorDisplayName(cursor);
        std::string argNameString{clang_getCString(argName)};
        clang_disposeString(argName);

        CXString pretty = clang_getCursorPrettyPrinted(cursor, nullptr);
        std::string prettyString{clang_getCString(pretty)};
        clang_disposeString(pretty);

        m_arguments.emplace_back(argNameString, prettyString);
    }

    const std::string &Name() const
    {
        return m_name;
    }

    const std::string &NamespacedName() const
    {
        return m_namespacedName;
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
    std::string m_namespacedName;
    CXType m_returnType;
    std::string m_returnTypeString;
    std::vector<Argument> m_arguments;
};

} // namespace zlang
