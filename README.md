# Simple Virtual Machine Interpreter

This repository contains a basic implementation of a virtual machine (VM) interpreter in C++. The VM executes a custom assembly-like language where instructions and data are encoded as 3-digit integers (ranging from 000 to 999). It simulates a simple processor with 10 registers, 1000 memory locations, and a set of basic operations including arithmetic, memory access, branching, and halting.
The program loads a sequence of integers from a user-specified file into memory, interprets them as instructions starting from memory address 0, executes until a halt instruction is encountered, and then displays the final state of the registers along with the total number of instructions executed.
All arithmetic operations are performed modulo 1000 to keep values within the 3-digit range.

## Features
- Memory Model: 1000 locations, each holding a 3-digit value parsed into operation code (op), and two operands/register indices (x, y)
- Registers: 10 general-purpose registers (0-9), initialized to 0.
- Instruction Encoding: Each instruction is a 3-digit number where:
First digit: Operation code (0-9).
Second digit: Typically a destination register (d) or similar.
Third digit: Typically a source register (s), value (n), or address component (a).
- Execution: Sequential fetch-execute cycle with program counter (PC) starting at 0.
- Input: Loads program from a text file containing space- or newline-separated integers.
- Output: Post-execution dump of register values and instruction count.

## Example Program File
```bash
210
311
100
```
- This sets register 1 to 0 (210), multiplies it by 1 (311), then halts (100).
- Output: All registers 0, instructions executed: 3.

Note: Ensure the file path is relative to the current working directory. If the file can't be opened, an error message with the current directory will be displayed.

## Instruction Set
Instructions are encoded as op d s/n/a (where op is operation, d is destination register, s is source register, n is immediate value, a is address register).

Opcode

0 - Jump to address in register d if register s != 0 | Encoding: 0ds

1 - Stop execution | Encoding: 100

2 - Set register d to immediate value n | Encoding: 2dn

3 - Multiply register d by immediate n | Encoding: 3dn

4 - Add immmediate n to register d | Encoding: 4dn

5 - Set register d to value of register s | Encoding: 5ds

6 - Multiply register d by register s | Encoding: 6ds

7 - Add register s to register d | Encoding: 7ds

8 - Load value from memory at the location stored in register a into register d | Encoding: 8da

9 - Write the value stored in register s into memory at the location stored into register a | Encoding: 9sa

Notes:
- Registers are indexed 0-9; values outside this range may cause undefined behavior.
- Memory addresses 0-999; PC out-of-bounds check aborts execution.
- All operations wrap around via modulo 1000 to maintain 3-digit consistency.

## Limitations
- No support for floating-point or advanced data types—everything is integer-based.
- File input assumes well-formed integers; malformed input may lead to incomplete loading.
- No debugging tools or error handling beyond basic file open and PC bounds checks.
- Program size currently limited to 1000 instructions/data slots.
- User input for file path is blocking and console-only.
- No command-line arguments for file path (uses interactive prompt).
