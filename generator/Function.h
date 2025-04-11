#pragma once

#include <string>

#include <clang-c/Index.h>

namespace zlang
{

struct Function
{
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
};

} // namespace zlang
