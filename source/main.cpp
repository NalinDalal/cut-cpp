#include <fstream>
#include <iostream>
#include <string>

#include "cut-cpp/cut.hpp"

auto main(int argc, char const* argv[]) -> int
{
  cut_parser parser;

  if (!parser.parse(argc, argv))
  {
    if (!parser.error_message().empty())
    {
      std::cerr << "cut: " << parser.error_message() << '\n';
      std::cerr << "Try 'cut --help' for more information.\n";
    }
    return 1;
  }

  auto const& opts = parser.options();
  cut_processor processor;

  bool has_file_args = false;

  for (int i = 1; i < argc; i++)
  {
    std::string arg = argv[i];

    if (arg[0] != '-')
    {
      has_file_args = true;
      break;
    }

    if (arg == "-d" || arg == "--delimiter" || arg == "-f" || arg == "--fields" || arg == "-c" || arg == "--characters")
    {
      i++;
    }
  }

  if (has_file_args)
  {
    for (int i = 1; i < argc; i++)
    {
      std::string arg = argv[i];

      if (arg[0] == '-')
      {
        if (arg == "-d" || arg == "--delimiter" || arg == "-f" || arg == "--fields" || arg == "-c" || arg == "--characters")
        {
          i++;
        }
        continue;
      }

      std::ifstream file(arg);

      if (!file.is_open())
      {
        std::cerr << "cut: " << arg << ": No such file or directory\n";
        return 1;
      }

      if (!processor.process(file, std::cout, opts))
      {
        std::cerr << "cut: " << processor.error_message() << '\n';
        return 1;
      }
    }
  }
  else
  {
    if (!processor.process(std::cin, std::cout, opts))
    {
      std::cerr << "cut: " << processor.error_message() << '\n';
      return 1;
    }
  }

  return 0;
}
