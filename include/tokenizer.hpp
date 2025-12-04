#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
using namespace std;

/* process_line: strip comments (;) and split label and rest.
   returns true if line contains content (label or statement).
   label_out empty if none. stmt_out empty if none.
*/
bool process_line(const string &line, string &label_out, string &stmt_out);

/* parse_token: returns next token from s starting at pos, advances pos */
string parse_token(const string &s, size_t &pos);

#endif 