#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <optional>
#include <cstdio>
using namespace std;

optional<int> parse_number(const string &s);
void write_word_be(FILE *f, unsigned int word);

#endif 