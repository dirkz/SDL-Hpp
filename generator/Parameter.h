#pragma once

#include <string>

#include <clang-c/Index.h>

namespace zlang
{

struct Parameter
{
    Parameter(CXCursor cursor, CXType cursorType);

  private:
    std::string m_name;
};

} // namespace zlang
