#ifndef SYMTAB_HPP
#define SYMTAB_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

struct Symbol {
    int value;
    bool defined;
    bool referenced;
    Symbol() : value(0), defined(false), referenced(false) {}
    Symbol(int v, bool d) : value(v), defined(d), referenced(false) {}
};

void symtab_init();
bool symtab_insert(const std::string &name, int value, bool defined); // returns false on success, true on duplicate def
bool symtab_defined(const std::string &name);
std::optional<int> symtab_lookup(const std::string &name);
void symtab_ref(const std::string &name);
std::vector<std::pair<std::string, Symbol>> symtab_all(); // for warnings/listing
void symtab_clear();

#endif 