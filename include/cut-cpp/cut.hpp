#pragma once

#include <functional>
#include <string>
#include <vector>

#include "cut-cpp/cut-cpp_export.hpp"

enum class cut_mode
{
  fields,
  characters
};

struct cut_options
{
  cut_mode mode = cut_mode::fields;
  char delimiter = '\t';
  std::vector<std::pair<int, int>> ranges;
  bool suppress_non_delimited = false;
};

class CUT_CPP_EXPORT cut_parser
{
public:
  cut_parser() = default;

  auto parse(int argc, char const* argv[]) -> bool;
  auto error_message() const -> std::string const&;

  auto options() const -> cut_options const&;

private:
  auto parse_field_spec(std::string const& spec) -> bool;
  auto parse_char_spec(std::string const& spec) -> bool;
  auto parse_range(std::string const& range_str, int& start, int& end) -> bool;

  cut_options m_options;
  std::string m_error;
};

class CUT_CPP_EXPORT cut_processor
{
public:
  cut_processor() = default;

  auto process(std::istream& input, std::ostream& output, cut_options const& opts) -> bool;
  auto error_message() const -> std::string const&;

private:
  auto split_by_delimiter(std::string const& line, char delimiter) -> std::vector<std::string>;
  auto extract_fields(std::string const& line, cut_options const& opts) -> std::string;
  auto extract_characters(std::string const& line, cut_options const& opts) -> std::string;
  auto is_in_range(int index, std::vector<std::pair<int, int>> const& ranges) -> bool;

  std::string m_error;
};
