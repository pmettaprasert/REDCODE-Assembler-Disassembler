# Assembler and Disassembler for REDCODE in C++

## Overview

This project includes a C++ implementation of an assembler and disassembler to translate assembly language instructions to machine code and vice versa. The main purpose is to demonstrate the use of file I/O, string manipulation, and bitwise operations to encode and decode instructions.


## File Structure

- `main.cpp`: The driver program to test the assembler and disassembler functionalities.
- `assembler.h`: Header file containing the assembler code which reads assembly language instructions and translates them to machine code.
- `disassembler.h`: Header file containing the disassembler code which translates machine code back into readable assembly language instructions.

## Core Features

- Load programs from files and insert them into a memory block.
- Disassemble machine instructions from memory blocks to assembly language.
- Dump memory content for verification and debugging purposes.
- Handle different addressing modes and operations defined in Redcode.
