#include "asm.hpp"
#include "instr.hpp"
#include "symtab.hpp"
#include "tokenizer.hpp"
#include "util.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

struct ListingEntry {
    unsigned addr;
    unsigned word;
    std::string mnemonic;
    std::string operand;
};

static std::vector<ListingEntry> listing;

static void add_listing(unsigned addr, unsigned word, const std::string &mn, const std::string &op) {
    listing.push_back({addr, word, mn, op});
}

/* compute branch offset: target - (instr_addr + 1) */
static int compute_offset_for_branch(int instr_addr, int target_addr) {
    return target_addr - (instr_addr + 1);
}

/* assemble_file: if emit==false => pass1 (discover labels); if emit==true => emit binary & listing */
int assemble_file(std::istream &in, std::FILE *obj, bool emit, std::ostream *lst, const std::string &in_name) {
    in.clear();
    in.seekg(0);
    std::string line;
    unsigned pc = 0;
    int lineno = 0;
    int errors = 0;

    while (std::getline(in, line)) {
        ++lineno;
        std::string label, stmt;
        if (!process_line(line, label, stmt)) continue;
        if (!label.empty()) {
            if (!emit) {
                bool dup = symtab_insert(label, static_cast<int>(pc), true);
                if (dup) {
                    std::cerr << in_name << ":" << lineno << ": error: duplicate label '" << label << "'\n";
                    ++errors;
                }
            } else {
                // nothing here; labels already recorded in pass1
            }
        }

        // parse statement
        size_t pos = 0;
        std::string tok = parse_token(stmt, pos);
        if (tok.empty()) continue;
        // uppercase mnemonic? spec uses case-insensitive for HALT maybe; keep case-sensitive for mnemonics except SET/HALT? We'll treat as exact except check "SET" as case-insensitive later
        if (tok == "SET" || tok == "set" || tok == "Set") {
            std::string op = parse_token(stmt, pos);
            if (op.empty()) {
                std::cerr << in_name << ":" << lineno << ": error: SET missing operand\n";
                ++errors;
            } else {
                auto val = parse_number(op);
                if (!val) {
                    std::cerr << in_name << ":" << lineno << ": error: bad number for SET: " << op << "\n";
                    ++errors;
                } else {
                    if (!emit) {
                        if (!label.empty()) {
                            symtab_insert(label, *val, true);
                        } else {
                            std::cerr << in_name << ":" << lineno << ": warning: SET without label ignored\n";
                        }
                    }
                }
            }
            continue;
        }

        const InstrDef *instr = lookup_instr(tok);
        if (!instr) {
            std::cerr << in_name << ":" << lineno << ": error: unknown mnemonic '" << tok << "'\n";
            ++errors;
            continue;
        }

        std::string operand = parse_token(stmt, pos);
        std::string extra = parse_token(stmt, pos);
        if (!extra.empty()) {
            std::cerr << in_name << ":" << lineno << ": error: extra tokens at end of line\n";
            ++errors;
        }

        if (instr->kind == OP_NONE) {
            if (!operand.empty()) {
                std::cerr << in_name << ":" << lineno << ": error: unexpected operand for '" << tok << "'\n";
                ++errors;
            }
            if (emit) {
                unsigned word = static_cast<unsigned>(instr->opcode & 0xFF);
                add_listing(pc, word, tok, "");
                write_word_be(obj, word);
            }
            ++pc;
        } else if (instr->kind == OP_VALUE || instr->kind == OP_ANY || instr->opcode == -1) {
            if (operand.empty()) {
                std::cerr << in_name << ":" << lineno << ": error: missing operand for '" << tok << "'\n";
                ++errors;
                ++pc;
                continue;
            }
            // label?
            if (std::isalpha((unsigned char)operand[0]) || operand[0]=='_') {
                if (!emit) {
                    symtab_ref(operand);
                } else {
                    auto val_opt = symtab_lookup(operand);
                    if (!val_opt) {
                        std::cerr << in_name << ":" << lineno << ": error: undefined label '" << operand << "'\n";
                        ++errors;
                        // fallback to zero
                        int val = 0;
                        if (instr->opcode == -1) {
                            unsigned word = static_cast<unsigned>(val);
                            add_listing(pc, word, "data", operand);
                            write_word_be(obj, word);
                        } else {
                            unsigned op24 = static_cast<unsigned>(val & 0xFFFFFF);
                            unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                            add_listing(pc, word, tok, operand);
                            write_word_be(obj, word);
                        }
                    } else {
                        int val = *val_opt;
                        if (instr->opcode == -1) {
                            unsigned word = static_cast<unsigned>(val);
                            add_listing(pc, word, "data", operand);
                            write_word_be(obj, word);
                        } else {
                            unsigned op24 = static_cast<unsigned>(val & 0xFFFFFF);
                            unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                            add_listing(pc, word, tok, operand);
                            write_word_be(obj, word);
                        }
                    }
                }
            } else {
                auto num_opt = parse_number(operand);
                if (!num_opt) {
                    std::cerr << in_name << ":" << lineno << ": error: bad number '" << operand << "'\n";
                    ++errors;
                    ++pc;
                    continue;
                }
                int num = *num_opt;
                if (emit) {
                    if (instr->opcode == -1) {
                        unsigned word = static_cast<unsigned>(num);
                        add_listing(pc, word, "data", operand);
                        write_word_be(obj, word);
                    } else {
                        unsigned op24 = static_cast<unsigned>(num & 0xFFFFFF);
                        unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                        add_listing(pc, word, tok, operand);
                        write_word_be(obj, word);
                    }
                }
            }
            ++pc;
        } else if (instr->kind == OP_OFFSET) {
            if (operand.empty()) {
                std::cerr << in_name << ":" << lineno << ": error: missing operand for '" << tok << "'\n";
                ++errors;
                ++pc;
                continue;
            }
            if (!emit) {
                if (std::isalpha((unsigned char)operand[0]) || operand[0]=='_') symtab_ref(operand);
            } else {
                if (std::isalpha((unsigned char)operand[0]) || operand[0]=='_') {
                    auto target_opt = symtab_lookup(operand);
                    if (!target_opt) {
                        std::cerr << in_name << ":" << lineno << ": error: undefined label '" << operand << "'\n";
                        ++errors;
                        int offset = 0;
                        unsigned op24 = static_cast<unsigned>(offset & 0xFFFFFF);
                        unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                        add_listing(pc, word, tok, operand);
                        write_word_be(obj, word);
                    } else {
                        int offset = compute_offset_for_branch(static_cast<int>(pc), *target_opt);
                        unsigned op24 = static_cast<unsigned>(offset & 0xFFFFFF);
                        unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                        add_listing(pc, word, tok, operand);
                        write_word_be(obj, word);
                    }
                } else {
                    auto num_opt = parse_number(operand);
                    if (!num_opt) {
                        std::cerr << in_name << ":" << lineno << ": error: bad number '" << operand << "'\n";
                        ++errors;
                        int num = 0;
                        unsigned op24 = static_cast<unsigned>(num & 0xFFFFFF);
                        unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                        add_listing(pc, word, tok, operand);
                        write_word_be(obj, word);
                    } else {
                        int num = *num_opt;
                        unsigned op24 = static_cast<unsigned>(num & 0xFFFFFF);
                        unsigned word = ((op24 << 8) & 0xFFFFFF00u) | static_cast<unsigned>(instr->opcode & 0xFF);
                        add_listing(pc, word, tok, operand);
                        write_word_be(obj, word);
                    }
                }
            }
            ++pc;
        } else {
            // unreachable
        }
    }

    // pass done
    if (emit && lst) {
        // Print listing. We will sort listing entries by address (they should already be in order).
        for (const auto &e : listing) {
            std::ostringstream os;
            os << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << e.addr;
            *lst << os.str() << " ";
            os.str(""); os.clear();
            os << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << e.word;
            *lst << os.str();
            if (!e.mnemonic.empty()) *lst << " " << e.mnemonic;
            if (!e.operand.empty()) *lst << " " << e.operand;
            *lst << std::dec << "\n";
        }
    }

    return errors == 0 ? 0 : 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: asm <input.asm> [-o out.obj] [-l out.lst]\n";
        return 1;
    }
    std::string infile = argv[1];
    std::string objname = "out.obj";
    std::string lstname = "out.lst";

    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == "-o" && i + 1 < argc) objname = argv[++i];
        else if (std::string(argv[i]) == "-l" && i + 1 < argc) lstname = argv[++i];
        else { std::cerr << "Unknown option '" << argv[i] << "'\n"; return 1; }
    }

    std::ifstream in(infile);
    if (!in) { std::perror(("fopen " + infile).c_str()); return 1; }

    instr_table_init();
    symtab_init();

    // PASS 1
    if (assemble_file(in, nullptr, false, nullptr, infile) != 0) {
        std::cerr << "Pass 1 failed (errors)\n";
        return 1;
    }

    // open binary and listing
    std::FILE *obj = std::fopen(objname.c_str(), "wb");
    if (!obj) { std::perror(("fopen " + objname).c_str()); return 1; }
    std::ofstream lst(lstname);
    if (!lst) { std::perror(("fopen " + lstname).c_str()); std::fclose(obj); return 1; }

    listing.clear();
    if (assemble_file(in, obj, true, &lst, infile) != 0) {
        std::cerr << "Pass 2 failed (errors)\n";
        std::fclose(obj);
        return 1;
    }

    // warnings: unused labels
    auto all = symtab_all();
    for (const auto &p : all) {
        if (p.second.defined && !p.second.referenced) {
            std::cout << "Warning: label '" << p.first << "' defined but not referenced\n";
        }
    }

    std::fclose(obj);
    std::cout << "Assembled " << infile << " -> " << objname << ", listing " << lstname << "\n";
    return 0;
}