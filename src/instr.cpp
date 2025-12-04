#include "instr.hpp"
#include <vector>
#include <cstring>

static std::vector<InstrDef> instrs;

void instr_table_init() {
    instrs.clear();
    instrs.emplace_back("data", -1, OP_VALUE);
    instrs.emplace_back("ldc", 0, OP_VALUE);
    instrs.emplace_back("adc", 1, OP_VALUE);
    instrs.emplace_back("ldl", 2, OP_OFFSET);
    instrs.emplace_back("stl", 3, OP_OFFSET);
    instrs.emplace_back("ldnl",4, OP_OFFSET);
    instrs.emplace_back("stnl",5, OP_OFFSET);
    instrs.emplace_back("add", 6, OP_NONE);
    instrs.emplace_back("sub", 7, OP_NONE);
    instrs.emplace_back("shl", 8, OP_NONE);
    instrs.emplace_back("shr", 9, OP_NONE);
    instrs.emplace_back("adj", 10, OP_VALUE);
    instrs.emplace_back("a2sp",11, OP_NONE);
    instrs.emplace_back("sp2a",12, OP_NONE);
    instrs.emplace_back("call",13, OP_OFFSET);
    instrs.emplace_back("return",14, OP_NONE);
    instrs.emplace_back("brz",15, OP_OFFSET);
    instrs.emplace_back("brlz",16, OP_OFFSET);
    instrs.emplace_back("br",17, OP_OFFSET);
    instrs.emplace_back("HALT",18, OP_NONE);
    instrs.emplace_back("SET", -2, OP_VALUE);
}

const InstrDef* lookup_instr(const std::string &mnemonic) {
    for (size_t i = 0; i < instrs.size(); ++i) {
        if (instrs[i].name == mnemonic) return &instrs[i];
    }
    return nullptr;
}