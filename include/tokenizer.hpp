#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
using namespace std;

bool process_line(const string &line, string &label_out, string &stmt_out);
string parse_token(const string &s, size_t &pos);

#endif 