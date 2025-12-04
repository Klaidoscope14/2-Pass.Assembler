#ifndef INSTR_HPP
#define INSTR_HPP

#include "asm.hpp"
#include <optional>

void instr_table_init();
const InstrDef* lookup_instr(const std::string &mnemonic);

#endif 