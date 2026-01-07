# C Compiler (Lexical, Syntaxic & Semantic Analyzer)

## Overview
This project is a C compiler prototype written in C.  
It focuses on the **analysis phases** of compilation rather than code generation.  
The compiler reads a C-like source file and validates it through lexical, syntaxic, and semantic analysis using predefined dictionaries and rules.

## Compiler Phases

### 1. Lexical Analysis
- Reads the source code character by character
- Verifies valid characters using a dictionary file
- Splits the source code into tokens
- Detects invalid symbols and malformed identifiers

### 2. Syntaxic Analysis
- Checks the structure of instructions
- Validates correct ordering of tokens
- Detects syntax errors such as missing symbols or incorrect expressions

### 3. Semantic Analysis
- Verifies meaning and correctness of declarations
- Checks variable usage and consistency
- Detects semantic errors such as undeclared identifiers

## Project Files
- `main.c` – Core compiler implementation
- `code.txt` – Source code input to be analyzed
- `dictionnaire.txt` – Dictionary of valid characters/tokens
- `bugs.txt` – Output file containing detected errors
- `main.exe` – Compiled executable (Windows)
- `.vscode/` – VS Code configuration files

## How It Works
1. The compiler reads the source code from `code.txt`
2. It compares characters and tokens against `dictionnaire.txt`
3. Errors are detected during analysis phases
4. All detected errors are written to `bugs.txt`

## credit
------------All rights reserved. © MOATAZ Mohamed------------
