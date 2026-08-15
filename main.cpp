#include <iostream>
#include <vector>
#include <memory>
#include "Token.h"
#include "ASTNodes.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "Parser.h" // Assuming you saved your parser as Parser.h (or include parser implementation)

// Declare your lexer function if it's defined in lexer.cpp
// std::vector<Token> tokenize(const std::string& sourceCode);

int main() {
    // 1. Example token stream (or call your actual lexer function here)
    std::vector<Token> tokens = {
        {"KEYWORD_SHONGKHA", "shongkha"},
        {"IDENTIFIER", "x"},
        {"SEMICOLON", ";"},
        {"EOF", ""}
    };

    std::cout << "--- Phase 1: Lexical Analysis Completed --- \n";

    // 2. Parse tokens into an Abstract Syntax Tree (AST)
    Parser parser(tokens);
    std::shared_ptr<BlockNode> programAST = parser.parseProgram();

    std::cout << "--- Phase 2: Syntax Analysis (Parser) Completed --- \n";

    // 3. Run Semantic Analysis & Symbol Table Verification
    SemanticAnalyzer analyzer;
    analyzer.visit(programAST);

    std::cout << "--- Phase 3: Semantic Analysis Completed --- \n";

    // 4. Final status report
    if (analyzer.getHasError()) {
        std::cout << "Compilation failed due to semantic errors.\n";
    } else {
        std::cout << "Compilation successful! No semantic errors found.\n";
    }

    return 0;
}