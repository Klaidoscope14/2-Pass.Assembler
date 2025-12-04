#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <optional>
#include <cstdio>

std::optional<int> parse_number(const std::string &s);
void write_word_be(std::FILE *f, unsigned int word);

#endif 