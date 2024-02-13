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
gcc vm.c 
./a.out myinput.txt 
Example Instruction File:

# Example
7 0 45  // Jump to instruction at index 45 if the top of the stack is less than 0
7 0 6   // Jump to instruction at index 6 if the top of the stack is less than 0
6 0 4   // Increment the stack pointer by 4
1 0 4   // Push literal 4 onto the stack
1 0 3   // Push literal 3 onto the stack
2 0 3   // Return from procedure
4 1 4   // Store the value at the top of the stack in the stack location at offset 4 from 1 lexicographical level down
1 0 14  // Push literal 14 onto the stack
3 1 4   // Load value from the stack location at offset 4 from 1 lexicographical level down
2 0 7   // Jump to instruction at index 7
8 0 39  // Jump to instruction at index 39 if the top of the stack is 0
1 0 7   // Push literal 7 onto the stack
7 0 42  // Jump to instruction at index 42 if the top of the stack is less than 0
1 0 5   // Push literal 5 onto the stack
2 0 0   // Return from procedure
6 0 5   // Increment the stack pointer by 5
9 0 2   // System call to read an integer as a character value from standard input
5 0 6   // Jump to instruction at index 6 if the top of the stack is not 0
9 0 1   // System call to output the value at the top of the stack to standard output as a character
9 0 3   // System call to halt the P-machine
