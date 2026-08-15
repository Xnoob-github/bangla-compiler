# Bangla Programming Language Compiler

A front-end compiler pipeline for a custom Bangla programming language implemented in C++. Built as part of Review 1.

## 🚀 Features Supported (Review 1)
- **Data Types:** 2 Primitive types (`সংখ্যা`, `লেখা`)
- **Operators:** Arithmetic (`+`, `-`, `*`, `/`) and Assignment (`=`)
- **Control Flow:** Conditional Statements (`যদি` / `অথবা`) and Loops (`যতক্ষণ`)
- **Architecture:** Lexer, Recursive Descent Parser, AST Generation, Symbol Table, and Type Checker.
- **Error Recovery:** Basic Panic-Mode Syntax Error Recovery (skips to next `;`).

## 📁 Project Structure
- `lexer.cpp` - Tokenizer for custom Bangla keywords and symbols.
- `parser.cpp` - Syntax Analyzer enforcing CFG rules and precedence.
- `ASTNodes.h` - Abstract Syntax Tree node definitions.
- `SymbolTable.h` & `SemanticAnalyzer.h` - Scope and type checking.
- `ErrorRecovery.h` - Error recovery mechanisms.
- `test_valid.bangla` & `test_invalid.bangla` - Test suites.

## 👥 Team Responsibilities
- **Member 1:** Lexical Analysis (`lexer.cpp`)
- **Member 2:** Syntax Analysis (`parser.cpp`)
- **Member 3:** Semantic Analysis & Symbol Table (`ASTNodes.h`, `SymbolTable.h`, `SemanticAnalyzer.h`)
- **Member 4:** Test Cases, Error Recovery & Documentation (`test_*.bangla`, `ErrorRecovery.h`, `README.md`)

## 🛠️ Build & Run Instructions
1. Compile the main program:
   ```bash
   g++ -std=c++17 main.cpp lexer.cpp parser.cpp -o bangla_compiler
