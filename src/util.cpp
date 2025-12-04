#include "util.hpp"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cctype>

std::optional<int> parse_number(const std::string &s) {
    const char *cstr = s.c_str();
    char *end = nullptr;
    errno = 0;
    long v = strtol(cstr, &end, 0);
    if (end == cstr) return std::nullopt;
    if (errno) return std::nullopt;
    
    while (*end) {
        if (!isspace((unsigned char)*end)) return std::nullopt;
        end++;
    }
    return static_cast<int>(v);
}

void write_word_be(std::FILE *f, unsigned int word) {
    unsigned char b0 = (unsigned char)((word >> 24) & 0xFFu);
    unsigned char b1 = (unsigned char)((word >> 16) & 0xFFu);
    unsigned char b2 = (unsigned char)((word >>  8) & 0xFFu);
    unsigned char b3 = (unsigned char)((word      ) & 0xFFu);

    if (std::fputc(b0, f) == EOF) {}
    if (std::fputc(b1, f) == EOF) {}
    if (std::fputc(b2, f) == EOF) {}
    if (std::fputc(b3, f) == EOF) {}
}