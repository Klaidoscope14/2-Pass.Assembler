#include "util.hpp"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cctype>
using namespace std;

optional<int> parse_number(const string &s) {
    const char *cstr = s.c_str();
    char *end = nullptr;
    errno = 0;
    long v = strtol(cstr, &end, 0);
    if (end == cstr) return nullopt;
    if (errno) return nullopt;
    
    while (*end) {
        if (!isspace((unsigned char)*end)) return nullopt;
        end++;
    }
    return static_cast<int>(v);
}

void write_word_be(FILE *f, unsigned int word) {
    unsigned char b0 = (unsigned char)((word >> 24) & 0xFFu);
    unsigned char b1 = (unsigned char)((word >> 16) & 0xFFu);
    unsigned char b2 = (unsigned char)((word >>  8) & 0xFFu);
    unsigned char b3 = (unsigned char)((word      ) & 0xFFu);

    if (fputc(b0, f) == EOF) {}
    if (fputc(b1, f) == EOF) {}
    if (fputc(b2, f) == EOF) {}
    if (fputc(b3, f) == EOF) {}
}