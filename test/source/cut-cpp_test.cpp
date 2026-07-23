#include <string>

#include "cut-cpp/cut-cpp.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("cut-cpp") == exported.name() ? 0 : 1;
}
