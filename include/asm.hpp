#ifndef ASM_HPP
#define ASM_HPP

#include <cstdio>
#include <string>
using namespace std;

const int MAX_LINE = 512;
const int MAX_TOKEN = 128;

enum OperandKind { 
    OP_NONE, 
    OP_VALUE, 
    OP_OFFSET, 
    OP_ANY 
};

struct InstrDef {
    string name;
    int opcode;
    OperandKind kind;
    InstrDef() : name(), opcode(-1), kind(OP_NONE) {}
    InstrDef(const char* n, int o, OperandKind k) : name(n), opcode(o), kind(k) {}
};

#endif 