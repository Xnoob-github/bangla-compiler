#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Token.h"
#include "ASTNodes.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "Parser.h"
#include "Lexer.cpp" // আপনার Lexer হেডার ফাইল

// টেস্ট কেস স্ট্রাকচার
struct TestCase {
    std::string title;
    std::string code;
};

void runPipeline(const std::string& testName, const std::string& sourceCode) {
    std::cout << "\n========================================\n";
    std::cout << "Running: " << testName << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Source Code:\n" << sourceCode << "\n";
    std::cout << "----------------------------------------\n";

    try {
        // ১. Lexical Analysis
        std::cout << "[1] Running Lexer...\n";
        Lexer lexer(sourceCode);
        std::vector<Token> tokens = lexer.tokenize(); 

        // ২. Syntax Analysis (Parser)
        std::cout << "[2] Running Parser...\n";
        Parser parser(tokens);
        std::shared_ptr<BlockNode> programAST = parser.parseProgram();

        // ৩. Semantic Analysis
        std::cout << "[3] Running Semantic Analyzer...\n";
        SemanticAnalyzer analyzer;
        analyzer.visit(programAST);

        // ৪. স্ট্যাটাস রিপোর্ট Check
        if (analyzer.getHasError()) {
            std::cout << "\n[RESULT] ❌ Compilation failed due to semantic errors.\n";
        } else {
            std::cout << "\n[RESULT] ✅ Compilation successful!\n";
        }

    } catch (const std::exception& e) {
        std::cout << "\n[RESULT] ❌ Compilation failed with Exception: " << e.what() << "\n";
    }
}

int main() {
    // ইনভ্যালিড টেস্ট কেসের তালিকা
    std::vector<TestCase> invalidTests = {
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
        }
    };

    // সব ইনভ্যালিড কোড একের পর এক টেস্ট পাইপলাইনে পাঠানো
    for (const auto& test : invalidTests) {
        runPipeline(test.title, test.code);
    }

    return 0;
}
