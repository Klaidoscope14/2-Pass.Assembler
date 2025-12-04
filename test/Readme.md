# Test Folder README

This folder contains **10 assembly test programs** designed to validate every component of the SIMPLE two-pass assembler. Each file exercises different parts of the assembler, from basic instruction handling to error detection, branching, `SET` pseudo-instruction handling, memory access, local variable manipulation, subroutines, loops, arithmetic, and small algorithms.

---

## 📁 Test Files Overview

### **1. test01.asm — Basic Valid Program**

* Simple arithmetic instructions
* Tests instruction encoding and listing output

### **2. test02.asm — Error Diagnostics Test**

* Contains duplicate labels
* Undefined labels
* Bad numeric constants
* Missing operands
* Extra tokens
* Illegal labels
* Unknown mnemonics

### **3. test03.asm — SET Pseudo Instruction**

* Tests `SET` label value assignment
* Tests forward references

### **4. test04.asm — Local Memory & Arithmetic**

* Exercises: `ldl`, `stl`, `adj`, local variables via SP
* Ensures assembler handles stack-based addressing

### **5. test05.asm — Summation Algorithm**

* Demonstrates loops and array access
* Tests correct branch offset computation

### **6. test06.asm — Bubble Sort (Required)**

* Full bubble sort algorithm in SIMPLE assembly
* Exercises nested loops, comparisons, memory loads/stores

### **7. test07.asm — Branching Logic**

* Tests `br`, `brz`, `brlz` behavior
* Checks handling of zero and negative values

### **8. test08.asm — Subroutine (call/return)**

* Tests `call` and `return` instructions
* Ensures correct operand resolution

### **9. test09.asm — Polynomial Evaluation**

* Demonstrates repeated-add multiplication
* Complex branching & label usage

### **10. test10.asm — Mean & Median Approximation**

* Array loops, repeated subtraction used as division
* Index-based memory access tests

---

## 🛠 Compilation Instructions

From the project root (where the Makefile is located):

### **Build the assembler:**

```sh
make
```

This produces an executable named:

```
./asm
```

---

## ▶ Running Test Assembly Files

Navigate to the test folder:

```sh
cd test
```

Run the assembler on any test file:

```sh
../asm test01.asm -o test01.obj -l test01.lst
```

Or run all tests (example commands):

```sh
../asm test02.asm -o test02.obj -l test02.lst
../asm test03.asm -o test03.obj -l test03.lst
../asm test04.asm -o test04.obj -l test04.lst
...
../asm test10.asm -o test10.obj -l test10.lst
```

---

## 📤 Output Files Generated

Every test produces:

* `*.obj` → **Binary object file** (big-endian machine words)
* `*.lst` → **Listing file** containing:

  * Address (hex)
  * Machine code word (hex)
  * Mnemonic
  * Operand

Example listing output:

```
00000000 00006500 ldc 0x65
00000001 00FFFF01 adc -1
00000002 00000011 br loop
```

---

## ✔ What These Tests Verify

These test programs fully exercise the assembler pipeline:

### **Pass 1 (Symbol Collection)**

* Correct detection of duplicate labels
* Forward references stored for resolution in Pass 2
* SET directives override label values

### **Pass 2 (Code Emission)**

* Correct operand resolution
* Correct relative-offset computation for branching
* Error detection for undefined labels
* Proper binary encoding (opcode + 24-bit operand)

### **Listing Generator**

* Correct address formatting
* Correct machine code formatting
* Mnemonic + operand printed in readable form