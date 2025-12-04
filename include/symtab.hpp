#ifndef SYMTAB_HPP
#define SYMTAB_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
using namespace std;

struct Symbol {
    int value;
    bool defined;
    bool referenced;
    Symbol() : value(0), defined(false), referenced(false) {}
    Symbol(int v, bool d) : value(v), defined(d), referenced(false) {}
};

void symtab_init();
bool symtab_insert(const string &name, int value, bool defined); // returns false on success, true on duplicate 
bool symtab_defined(const string &name);
optional<int> symtab_lookup(const string &name);
void symtab_ref(const string &name);
vector<pair<string, Symbol>> symtab_all(); // for warnings/listing
void symtab_clear();

#endif 