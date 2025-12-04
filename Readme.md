# 2 - Pass Assembler (C++)

This project implements an **advanced two-pass assembler** for the SIMPLE instruction set. It is written entirely in **portable C++ (C++17)** without using non-standard headers like `bits/stdc++.h`, making it compatible with **clang** and **gcc**.

---

## Project Structure

```
simple-asm/
├── Makefile
├── README.md
├── include/
│   ├── asm.hpp
│   ├── instr.hpp
│   ├── symtab.hpp
│   ├── tokenizer.hpp
│   └── util.hpp
├── src/
│   ├── main.cpp
│   ├── instr.cpp
│   ├── symtab.cpp
│   ├── tokenizer.cpp
│   └── util.cpp
└── test/
    └── example.asm
```

---

## Features Implemented

### ✔ Two-pass assembly

* **Pass 1:** Collects labels, detects duplicates, registers forward references.
* **Pass 2:** Emits machine code, resolves labels, performs error checks.

### ✔ Fully compliant with specification

Implements every instruction from the SIMPLE ISA including:

* `ldc`, `adc`, `ldl`, `stl`, `ldnl`, `stnl`
* `add`, `sub`, `shl`, `shr`
* `adj`, `a2sp`, `sp2a`
* `call`, `return`
* Branch instructions: `br`, `brz`, `brlz`
* Pseudo-ops: `data`, **`SET`**

### ✔ Advanced listing file

Shows:

* Address (hex)
* Machine code word (hex)
* Mnemonic + operand

### ✔ Binary object file (big-endian)

Each instruction is encoded as:

```
[ 24-bit operand ][ 8-bit opcode ]
```

### ✔ Robust error handling

Detects:

* Duplicate labels
* Undefined labels
* Invalid numbers
* Extra tokens on line
* Missing operands

---

## Building the Assembler

Ensure you have **clang++** or **g++** installed.

Run:

```
make
```

This produces the executable:

```
./asm
```

---

## ▶ Using the Assembler

Basic usage:

```
./asm input.asm
```

Custom output paths:

```
./asm input.asm -o output.obj -l listing.lst
```

If not provided:

```
out.obj and out.lst
```

are created by default.

---

## Input Assembly Format

### Labels

```
start:
```

Labels point to the **address of the next instruction/data word**.

### Instructions

```
ldc 5
adc -3
br loop
```

### Data

```
array: data 10
```

### SET pseudo-op

```
value: SET 42
```

Assigns a custom address value to a label.

---

## Outputs

### 1. Binary object file (`*.obj`)

A sequence of **big-endian 32-bit words**.

### 2. Listing file (`*.lst`)

Example:

```
00000000 00006500 ldc 0x65
00000001 00FFFF01 adc -1
00000002 00000011 br loop
```

---

## Testing

Place your `.asm` test files into the `test/` folder.
Example:

```
./asm test/example.asm -o example.obj -l example.lst
```

---

## Notes

* Branch offsets use the formula:

```
offset = target_address - (current_pc + 1)
```

* Unknown labels cause pass-2 errors.
* Undefined but referenced labels in pass-1 are added as placeholders.