# Bangla Programming Language Compiler (বাংলা কম্পাইলার)

This project is a custom compiler implementation for a toy programming language written in Bangla syntax, developed in a C++ environment.

## 🚀 Features (Review 1)
- **Data Types (২টি):** `সংখ্যা` (Integer) & `লেখা` (String)
- **Operators:** Arithmetic (`+`, `-`, `*`, `/`)
- **Control Flow:** `যদি` - `নাহলে` (If-Else) and `যতক্ষণ` (While Loop)
- **Architecture:** Lexer -> Parser (AST) -> Symbol Table -> Semantic Analyzer -> Error Handler

## 🛠 Project Structure
- `Token.h` & `lexer.cpp` : Converts Bangla code into tokens.
- `parser.h` & `parser.cpp` : Parses tokens into an Abstract Syntax Tree (AST).
- `ASTNodes.h` : Defines nodes for AST.
- `SymbolTable.h` : Manages scope and variable declarations.
- `SemanticAnalyzer.h` : Performs type checking and semantic validation.
- `ErrorHandler.h` : Collects and prints compiler errors smoothly.
- `main.cpp` : Entry point running the compiler pipeline.

## 💻 How to Run
```bash
g++ -std=c++17 main.cpp parser.cpp lexer.cpp -o compiler
./compiler
