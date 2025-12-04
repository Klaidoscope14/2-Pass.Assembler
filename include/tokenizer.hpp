#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

/* process_line: strip comments (;) and split label and rest.
   returns true if line contains content (label or statement).
   label_out empty if none. stmt_out empty if none.
*/
bool process_line(const std::string &line, std::string &label_out, std::string &stmt_out);

/* parse_token: returns next token from s starting at pos, advances pos */
std::string parse_token(const std::string &s, size_t &pos);

#endif 