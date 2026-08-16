#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Token.h"
#include "ASTNodes.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "parser.h"
#include "lexer.h"

struct TestCase {
    std::string title;
    std::string code;
};

void printAST(const std::shared_ptr<ASTNode>& node, int indent = 0) {
    if (!node) return;
    std::string pad(indent * 2, ' ');

    switch (node->type) {
        case NODE_BLOCK: {
            auto block = std::dynamic_pointer_cast<BlockNode>(node);
            std::cout << pad << "Block:\n";
            for (auto& stmt : block->statements) printAST(stmt, indent + 1);
            break;
        }
        case NODE_DECLARATION: {
            auto decl = std::dynamic_pointer_cast<VarDeclNode>(node);
            std::cout << pad << "VarDecl: " << decl->varName
                      << " (type=" << decl->varType << ")\n";
            if (decl->initExpr) printAST(decl->initExpr, indent + 1);
            break;
        }
        case NODE_ASSIGNMENT: {
            auto assign = std::dynamic_pointer_cast<AssignNode>(node);
            std::cout << pad << "Assign: " << assign->varName << " =\n";
            printAST(assign->expr, indent + 1);
            break;
        }
        case NODE_VARIABLE: {
            auto var = std::dynamic_pointer_cast<VarNode>(node);
            std::cout << pad << "Var: " << var->varName << "\n";
            break;
        }
        case NODE_LITERAL: {
            auto lit = std::dynamic_pointer_cast<LiteralNode>(node);
            std::cout << pad << "Literal: " << lit->value << " (type=" << lit->dataType << ")\n";
            break;
        }
        case NODE_BINARY_OP: {
            auto bin = std::dynamic_pointer_cast<BinaryOpNode>(node);
            std::cout << pad << "BinaryOp: " << bin->op << "\n";
            printAST(bin->left, indent + 1);
            printAST(bin->right, indent + 1);
            break;
        }
        case NODE_IF: {
            auto ifNode = std::dynamic_pointer_cast<IfNode>(node);
            std::cout << pad << "If:\n";
            std::cout << pad << "  Condition:\n";
            printAST(ifNode->condition, indent + 2);
            std::cout << pad << "  Then:\n";
            printAST(ifNode->thenBlock, indent + 2);
            if (ifNode->elseBlock) {
                std::cout << pad << "  Else:\n";
                printAST(ifNode->elseBlock, indent + 2);
            }
            break;
        }
        case NODE_WHILE: {
            auto whileNode = std::dynamic_pointer_cast<WhileNode>(node);
            std::cout << pad << "While:\n";
            std::cout << pad << "  Condition:\n";
            printAST(whileNode->condition, indent + 2);
            std::cout << pad << "  Body:\n";
            printAST(whileNode->body, indent + 2);
            break;
        }
    }
}

void runPipeline(const std::string& testName, const std::string& sourceCode) {
    std::cout << "\n========================================\n";
    std::cout << "Running: " << testName << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Source Code:\n" << sourceCode << "\n";
    std::cout << "----------------------------------------\n";

    try {
        std::cout << "[1] Running Lexer...\n";
        Lexer lexer(sourceCode);
        std::vector<Token> tokens = lexer.tokenize();

        std::cout << "----- Tokens -----\n";
        for (const Token& tok : tokens) {
            std::cout << "Line: " << tok.line
                      << " | Type: " << tokenTypeToString(tok.type)
                      << " | Value: " << tok.lexeme << "\n";
        }

        std::cout << "\n[2] Running Parser...\n";
        Parser parser(tokens);
        std::shared_ptr<BlockNode> programAST = parser.parseProgram();

        std::cout << "----- AST -----\n";
        printAST(programAST);

        std::cout << "\n[3] Running Semantic Analyzer...\n";
        SemanticAnalyzer analyzer;
        analyzer.visit(programAST);

        if (analyzer.getHasError()) {
            std::cout << "\n[RESULT] Compilation failed due to semantic errors.\n";
        } else {
            std::cout << "\n[RESULT] Compilation successful!\n";
        }

    } catch (const std::exception& e) {
        std::cout << "\n[RESULT] Compilation failed with Exception: " << e.what() << "\n";
    }
}

int main() {
    std::vector<TestCase> tests = {
        {
            "Undeclared Variable Assignment",
            "z = 100;"
        },
        {
            "Redeclaration Error",
            "সংখ্যা x = 10;\n"
            "সংখ্যা x = 50;"
        },
        {
            "Type Mismatch Error",
            "সংখ্যা a = 5;\n"
            "a = \"বাংলা\";"
        },
        {
            "Valid Program",
            "সংখ্যা x = 10;\n"
            "দশমিক y = 20.5;\n"
            "x = x + 5 * 2;\n"
            "যদি (x < 100) {\n"
            "    x = x + 1;\n"
            "} নাহলে {\n"
            "    x = x - 1;\n"
            "}\n"
            "যতক্ষণ (x < 50) {\n"
            "    x = x + 2;\n"
            "}"
        }
    };

    for (const auto& test : tests) {
        runPipeline(test.title, test.code);
    }

    return 0;
}
