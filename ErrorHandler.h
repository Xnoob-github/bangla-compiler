#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <iostream>
#include <string>
#include <vector>

struct CompilerError {
    std::string phase;
    std::string message;
    int line;
};

class ErrorHandler {
private:
    std::vector<CompilerError> errors;

public:
    void reportError(const std::string& phase, const std::string& message, int line = -1) {
        errors.push_back({phase, message, line});
    }

    bool hasErrors() const {
        return !errors.empty();
    }

    void printErrors() const {
        std::cout << "\n========================================\n";
        std::cout << "         COMPILER ERROR REPORT          \n";
        std::cout << "========================================\n";
        for (const auto& err : errors) {
            std::cout << "[" << err.phase << " Error]";
            if (err.line != -1) {
                std::cout << " at line " << err.line;
            }
            std::cout << ": " << err.message << "\n";
        }
        std::cout << "========================================\n\n";
    }

    void reset() {
        errors.clear();
    }
};

#endif
