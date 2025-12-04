#ifndef INSTR_HPP
#define INSTR_HPP

#include "asm.hpp"
#include <optional>
using namespace std;

void instr_table_init();
const InstrDef* lookup_instr(const string &mnemonic);

#endif 