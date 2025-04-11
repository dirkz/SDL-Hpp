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

        if (m_name.starts_with("SDL_"))
        {
            m_hasSdlPrefix = true;
            m_namespacedName = m_name.substr(4);
            m_isHidden = false;
        }
        else
        {
            m_isHidden = m_name.starts_with("_");
            m_hasSdlPrefix = false;
            m_namespacedName = m_name;
        }

        m_returnType = clang_getCursorResultType(cursor);
        CXString functionResultTypeCX = clang_getTypeSpelling(m_returnType);
        m_returnTypeString = clang_getCString(functionResultTypeCX);
        clang_disposeString(functionResultTypeCX);

        m_returnsBool = m_returnType.kind == CXType_Bool;

        // TODO: This is not very reliable.
        m_returnsPointer = m_returnType.kind = CXType_Pointer;
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

    bool ReturnsBool() const
    {
        return m_returnsBool;
    }

    bool ReturnsPointer() const
    {
        return m_returnsPointer;
    }

    bool HasSDLPrefix() const
    {
        return m_hasSdlPrefix;
    }

    bool IsHidden() const
    {
        return m_isHidden;
    }

  private:
    std::string m_name;
    std::string m_namespacedName;
    CXType m_returnType;
    std::string m_returnTypeString;
    std::vector<Argument> m_arguments;
    bool m_returnsBool;
    bool m_returnsPointer;
    bool m_hasSdlPrefix;
    bool m_isHidden;
};

} // namespace zlang
