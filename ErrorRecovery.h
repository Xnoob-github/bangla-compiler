#ifndef ERROR_RECOVERY_H
#ifndef ERROR_RECOVERY_H
#define ERROR_RECOVERY_H

#include <iostream>
#include <vector>
#include <string>

// Member 4: Syntax Error Recovery Implementation (Panic Mode)
class ErrorRecovery {
public:
    static void synchronize(const std::vector<std::string>& tokens, size_t& currentIndex) {
        std::cerr << "[Syntax Error]: Unexpected token '" 
                  << (currentIndex < tokens.size() ? tokens[currentIndex] : "EOF") 
                  << "' at index " << currentIndex << ". Recovering parser state..." << std::endl;

        // Skip tokens until reaching a statement boundary (semicolon ;) or EOF
        while (currentIndex < tokens.size()) {
            if (tokens[currentIndex] == ";") {
                currentIndex++; // Move past the semicolon to resume parsing next statement
                std::cout << "[Error Recovery]: Resuming parsing after ';'" << std::endl;
                return;
            }
            currentIndex++;
        }
    }
};

#endif
