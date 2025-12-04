#include "symtab.hpp"
#include <algorithm>

static std::unordered_map<std::string, Symbol> table;

void symtab_init() {
    table.clear();
}

bool symtab_insert(const std::string &name, int value, bool defined) {
    auto it = table.find(name);
    if (it != table.end()) {
        if (defined && it->second.defined) {
            return true;
        }
        if (defined) {
            it->second.value = value;
            it->second.defined = true;
        }
        return false;
    }
    table.emplace(name, Symbol(value, defined));
    return false;
}

bool symtab_defined(const std::string &name) {
    auto it = table.find(name);
    return (it != table.end()) && it->second.defined;
}

std::optional<int> symtab_lookup(const std::string &name) {
    auto it = table.find(name);
    if (it == table.end()) return std::nullopt;
    return it->second.value;
}

void symtab_ref(const std::string &name) {
    auto it = table.find(name);
    if (it == table.end()) {
        Symbol s;
        s.referenced = true;
        table.emplace(name, s);
    } else {
        it->second.referenced = true;
    }
}

std::vector<std::pair<std::string, Symbol>> symtab_all() {
    std::vector<std::pair<std::string, Symbol>> out;
    out.reserve(table.size());
    for (auto &kv : table) out.emplace_back(kv);
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b){ return a.first < b.first; });
    return out;
}

void symtab_clear() {
    table.clear();
}