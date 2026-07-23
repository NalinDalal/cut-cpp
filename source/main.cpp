#include <fstream>
#include <iostream>
#include <string>

#include "cut-cpp/cut.hpp"

auto print_usage() -> void
{
  std::cout
      << "Usage: cut -f LIST [-d DELIM] [-s] [FILE...]\n"
      << "       cut -c LIST [-s] [FILE...]\n"
      << "\n"
      << "Print selected parts of lines from each FILE to standard output.\n"
      << "\n"
      << "Options:\n"
      << "  -f, --fields=LIST      select only these fields\n"
      << "  -c, --characters=LIST  select only these characters\n"
      << "  -d, --delimiter=DELIM  use DELIM instead of TAB as field "
         "delimiter\n"
      << "  -s, --only-delimited   do not print lines not containing "
         "delimiters\n"
      << "      --help             display this help and exit\n"
      << "\n"
      << "LIST is comma-separated numbers or ranges. Ranges are N-M (from N to "
         "M),\n"
      << "N- (from N to end), or -M (from start to M).\n"
      << "\n"
      << "Examples:\n"
      << "  cut -f 1,3          print fields 1 and 3\n"
      << "  cut -f 2-           print from field 2 to end\n"
      << "  cut -c 1-5          print characters 1 through 5\n"
      << "  cut -d : -f 3 file  print field 3 using : as delimiter\n";
}

auto main(int argc, char const* argv[]) -> int
{
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "--help") {
      print_usage();
      return 0;
    }
  }

  cut_parser parser;

  if (!parser.parse(argc, argv)) {
    if (!parser.error_message().empty()) {
      std::cerr << "cut: " << parser.error_message() << '\n';
      std::cerr << "Try 'cut --help' for more information.\n";
    }
    return 1;
  }

  auto const& opts = parser.options();
  cut_processor processor;

  bool has_file_args = false;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg[0] != '-') {
      has_file_args = true;
      break;
    }

    if (arg == "-d" || arg == "--delimiter" || arg == "-f" || arg == "--fields"
        || arg == "-c" || arg == "--characters")
    {
      i++;
    }
  }

  if (has_file_args) {
    for (int i = 1; i < argc; i++) {
      std::string arg = argv[i];

      if (arg[0] == '-') {
        if (arg == "-d" || arg == "--delimiter" || arg == "-f"
            || arg == "--fields" || arg == "-c" || arg == "--characters")
        {
          i++;
        }
        continue;
      }

      std::ifstream file(arg);

      if (!file.is_open()) {
        std::cerr << "cut: " << arg << ": No such file or directory\n";
        return 1;
      }

      if (!processor.process(file, std::cout, opts)) {
        std::cerr << "cut: " << processor.error_message() << '\n';
        return 1;
      }
    }
  } else {
    if (!processor.process(std::cin, std::cout, opts)) {
      std::cerr << "cut: " << processor.error_message() << '\n';
      return 1;
    }
  }

  return 0;
}
