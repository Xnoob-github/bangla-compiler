#include <iostream>
#include <memory>          // <-- Ei include-ta missing chilo
#include "ASTNodes.h"
#include "SemanticAnalyzer.h"

int main() {
    SemanticAnalyzer analyzer;

    std::cout << "--- Starting C++ Semantic Analysis ---\n";

    // shongkha x;
    // x = 10 + 5;
    auto stmt1 = std::make_shared<VarDeclNode>("x", TYPE_INT);
    analyzer.visit(stmt1);

    auto stmt2 = std::make_shared<AssignNode>("x", 
        std::make_shared<BinaryOpNode>('+', 
            std::make_shared<LiteralNode>(TYPE_INT, "10"), 
            std::make_shared<LiteralNode>(TYPE_INT, "5")
        )
    );
    analyzer.visit(stmt2);

    std::cout << "-----------------------------------\n";
    if (!analyzer.getHasError()) {
        std::cout << "SUCCESS: C++ Code Compiled & Analyzed with 0 Errors!\n";
    } else {
        std::cout << "FAILED: Errors found!\n";
    }
    std::cout << "-----------------------------------\n";

    return 0;
}