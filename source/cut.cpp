#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "cut-cpp/cut.hpp"

auto cut_parser::parse(int argc, char const* argv[]) -> bool
{
  int i = 1;
  bool mode_set = false;

  while (i < argc) {
    std::string arg = argv[i];

    if (arg == "-d" || arg == "--delimiter") {
      if (i + 1 >= argc) {
        m_error = "option requires an argument -- 'd'";
        return false;
      }
      i++;
      std::string delim_arg = argv[i];
      if (delim_arg.empty()) {
        m_error = "delimiter cannot be empty";
        return false;
      }
      m_options.delimiter = delim_arg[0];
      if (delim_arg.size() > 1) {
        if (delim_arg == "\\t") {
          m_options.delimiter = '\t';
        } else if (delim_arg == "\\n") {
          m_options.delimiter = '\n';
        } else if (delim_arg == "\\0") {
          m_options.delimiter = '\0';
        } else {
          m_error = "delimiter must be a single character";
          return false;
        }
      }
    } else if (arg == "-f" || arg == "--fields") {
      if (mode_set && m_options.mode != cut_mode::fields) {
        m_error = "cannot specify both -f and -c";
        return false;
      }
      m_options.mode = cut_mode::fields;
      mode_set = true;

      if (i + 1 >= argc) {
        m_error = "Missing field specification";
        return false;
      }
      i++;
      if (!parse_field_spec(argv[i])) {
        return false;
      }
    } else if (arg == "-c" || arg == "--characters") {
      if (mode_set && m_options.mode != cut_mode::characters) {
        m_error = "cannot specify both -f and -c";
        return false;
      }
      m_options.mode = cut_mode::characters;
      mode_set = true;

      if (i + 1 >= argc) {
        m_error = "Missing byte specification";
        return false;
      }
      i++;
      if (!parse_char_spec(argv[i])) {
        return false;
      }
    } else if (arg == "-s" || arg == "--only-delimited") {
      m_options.suppress_non_delimited = true;
    } else if (arg == "--help") {
      m_error = "";
      return false;
    } else if (arg[0] == '-') {
      m_error = "invalid option -- '" + arg.substr(1, 1) + "'";
      return false;
    } else {
      break;
    }

    i++;
  }

  if (!mode_set) {
    m_error = "you must specify one of -f or -c";
    return false;
  }

  return true;
}

auto cut_parser::error_message() const -> std::string const&
{
  return m_error;
}

auto cut_parser::options() const -> cut_options const&
{
  return m_options;
}

auto cut_parser::parse_field_spec(std::string const& spec) -> bool
{
  m_options.ranges.clear();

  if (spec.empty() || spec.front() == ',' || spec.back() == ',') {
    m_error = "invalid field specification: '" + spec + "'";
    return false;
  }

  std::istringstream stream(spec);
  std::string token;

  while (std::getline(stream, token, ',')) {
    if (token.empty()) {
      m_error = "invalid field specification: '" + spec + "'";
      return false;
    }

    int start = 0;
    int end = 0;

    if (!parse_range(token, start, end)) {
      m_error = "invalid field specification: '" + spec + "'";
      return false;
    }

    m_options.ranges.emplace_back(start, end);
  }

  if (m_options.ranges.empty()) {
    m_error = "invalid field specification: '" + spec + "'";
    return false;
  }

  return true;
}

auto cut_parser::parse_char_spec(std::string const& spec) -> bool
{
  m_options.ranges.clear();

  if (spec.empty() || spec.front() == ',' || spec.back() == ',') {
    m_error = "invalid byte specification: '" + spec + "'";
    return false;
  }

  std::istringstream stream(spec);
  std::string token;

  while (std::getline(stream, token, ',')) {
    if (token.empty()) {
      m_error = "invalid byte specification: '" + spec + "'";
      return false;
    }

    int start = 0;
    int end = 0;

    if (!parse_range(token, start, end)) {
      m_error = "invalid byte specification: '" + spec + "'";
      return false;
    }

    m_options.ranges.emplace_back(start, end);
  }

  if (m_options.ranges.empty()) {
    m_error = "invalid byte specification: '" + spec + "'";
    return false;
  }

  return true;
}

auto cut_parser::parse_range(std::string const& range_str, int& start, int& end)
    -> bool
{
  if (range_str.empty()) {
    return false;
  }

  size_t dash_pos = range_str.find('-');

  if (dash_pos == std::string::npos) {
    try {
      start = std::stoi(range_str);
      end = start;
    } catch (...) {
      return false;
    }

    if (start < 1) {
      return false;
    }

    return true;
  }

  std::string start_str = range_str.substr(0, dash_pos);
  std::string end_str = range_str.substr(dash_pos + 1);

  if (start_str.empty()) {
    start = 1;
  } else {
    try {
      start = std::stoi(start_str);
    } catch (...) {
      return false;
    }
  }

  if (end_str.empty()) {
    end = -1;
  } else {
    try {
      end = std::stoi(end_str);
    } catch (...) {
      return false;
    }
  }

  if (start < 1 || (end != -1 && end < start)) {
    return false;
  }

  return true;
}

auto cut_processor::process(std::istream& input,
                            std::ostream& output,
                            cut_options const& opts) -> bool
{
  std::string line;

  while (std::getline(input, line)) {
    std::string result;

    if (opts.mode == cut_mode::fields) {
      result = extract_fields(line, opts);
    } else {
      result = extract_characters(line, opts);
    }

    if (!result.empty() || !opts.suppress_non_delimited) {
      output << result << '\n';
    }
  }

  return true;
}

auto cut_processor::error_message() const -> std::string const&
{
  return m_error;
}

auto cut_processor::split_by_delimiter(std::string const& line, char delimiter)
    -> std::vector<std::string>
{
  std::vector<std::string> fields;
  std::istringstream stream(line);
  std::string field;

  if (delimiter == '\t') {
    while (std::getline(stream, field, '\t')) {
      fields.push_back(field);
    }
  } else {
    size_t start = 0;
    size_t pos = 0;

    while ((pos = line.find(delimiter, start)) != std::string::npos) {
      fields.push_back(line.substr(start, pos - start));
      start = pos + 1;
    }

    fields.push_back(line.substr(start));
  }

  return fields;
}

auto cut_processor::extract_fields(std::string const& line,
                                   cut_options const& opts) -> std::string
{
  bool has_delimiter = line.find(opts.delimiter) != std::string::npos;

  if (!has_delimiter && opts.suppress_non_delimited) {
    return "";
  }

  auto fields = split_by_delimiter(line, opts.delimiter);
  std::string result;
  bool first = true;

  for (auto const& [start, end] : opts.ranges) {
    for (int i = start; i <= end || end == -1; i++) {
      int idx = i - 1;

      if (idx >= 0 && idx < static_cast<int>(fields.size())) {
        if (!first) {
          result += opts.delimiter;
        }

        result += fields[idx];
        first = false;
      } else if (end == -1) {
        break;
      }
    }
  }

  return result;
}

auto cut_processor::extract_characters(std::string const& line,
                                       cut_options const& opts) -> std::string
{
  std::string result;

  for (auto const& [start, end] : opts.ranges) {
    for (int i = start; i <= end || end == -1; i++) {
      int idx = i - 1;

      if (idx >= 0 && idx < static_cast<int>(line.size())) {
        result += line[idx];
      } else if (end == -1) {
        break;
      }
    }
  }

  return result;
}

auto cut_processor::is_in_range(int index,
                                std::vector<std::pair<int, int>> const& ranges)
    -> bool
{
  for (auto const& [start, end] : ranges) {
    if (index >= start && (end == -1 || index <= end)) {
      return true;
    }
  }

  return false;
}
