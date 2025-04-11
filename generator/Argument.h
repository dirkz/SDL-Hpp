#pragma once

#include <string>

namespace zlang
{

struct Argument
{
    Argument(const std::string &name, const std::string &declaration)
        : m_name{name}, m_declaration{declaration}
    {
    }

    const std::string &Name() const
    {
        return m_name;
    }

    const std::string &Declaration() const
    {
        return m_declaration;
    }

  private:
    std::string m_name;
    std::string m_declaration;
};

} // namespace zlang