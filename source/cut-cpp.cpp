#include <string>

#include "cut-cpp/cut-cpp.hpp"

exported_class::exported_class()
    : m_name {"cut-cpp"}
{
}

auto exported_class::name() const -> char const*
{
  return m_name.c_str();
}
