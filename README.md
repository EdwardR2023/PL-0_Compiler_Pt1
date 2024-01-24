# Compiler_In_C_Pt1.
This project is an implementation of a P-machine, a theoretical stack-based virtual machine commonly used in compiler design and computer science education. The P-machine executes a sequence of instructions, manipulating a stack and performing various operations defined by opcodes.

# Features:
Stack-based virtual machine
Implemented opcodes for operations like literal push, return, load, store, call, increment, jump, jump conditional, and system calls
Reads instructions from a file and executes them
Supports basic arithmetic and control flow operations
Simple demonstration of a stack-based execution environment
Usage:

Compile the source code.
Run the executable with a file containing P-machine instructions.
Example Instruction File:

# Example
1 0 5   // Push literal 5 onto the stack
2 0 0   // Return from procedure
7 0 9   // Jump to instruction at index 9 if the top of the stack is less than 0
