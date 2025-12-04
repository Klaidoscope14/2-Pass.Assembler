#include "symtab.hpp"
#include <algorithm>
using namespace std;

static unordered_map<string, Symbol> table;

void symtab_init() {
    table.clear();
}

bool symtab_insert(const string &name, int value, bool defined) {
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

bool symtab_defined(const string &name) {
    auto it = table.find(name);
    return (it != table.end()) && it->second.defined;
}

optional<int> symtab_lookup(const string &name) {
    auto it = table.find(name);
    if (it == table.end()) return nullopt;
    return it->second.value;
}

void symtab_ref(const string &name) {
    auto it = table.find(name);
    if (it == table.end()) {
        Symbol s;
        s.referenced = true;
        table.emplace(name, s);
    } else {
        it->second.referenced = true;
    }
}

vector<pair<string, Symbol>> symtab_all() {
    vector<pair<string, Symbol>> out;
    out.reserve(table.size());
    for (auto &kv : table) out.emplace_back(kv);
    sort(out.begin(), out.end(), [](const auto &a, const auto &b){ return a.first < b.first; });
    return out;
}

void symtab_clear() {
    table.clear();
}