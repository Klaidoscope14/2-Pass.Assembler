#include "tokenizer.hpp"
#include <cctype>
using namespace std;

static string trim_right(const string &s) {
    size_t n = s.size();
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t')) n--;
    return s.substr(0, n);
}

bool process_line(const string &line, string &label_out, string &stmt_out) {
    string buf = line;
    size_t sc = buf.find(';');

    if (sc != string::npos) buf = buf.substr(0, sc);
    buf = trim_right(buf);

    size_t i = 0;
    while (i < buf.size() && isspace((unsigned char)buf[i])) i++;
    if (i >= buf.size()) { 
        label_out.clear(); 
        stmt_out.clear(); 
        return false; 
    }

    // check label at start: letters/_ then alnum/_
    size_t j = i;
    if (j < buf.size() && (isalpha((unsigned char)buf[j]) || buf[j]=='_')) {
        j++;

        while (j < buf.size() && (isalnum((unsigned char)buf[j]) || buf[j]=='_')) j++;

        if (j < buf.size() && buf[j] == ':') {
            label_out = buf.substr(i, j - i);
            j++;

            while (j < buf.size() && isspace((unsigned char)buf[j])) j++;

            if (j < buf.size()) stmt_out = buf.substr(j); else stmt_out.clear();
            return true;
        }
    }

    label_out.clear();
    stmt_out = buf.substr(i);
    return true;
}

string parse_token(const string &s, size_t &pos) {
    size_t n = s.size();
    while (pos < n && isspace((unsigned char)s[pos])) pos++;
    if (pos >= n) return "";
    size_t start = pos;
    while (pos < n && !isspace((unsigned char)s[pos]) && s[pos] != ',') pos++;
    return s.substr(start, pos - start);
}