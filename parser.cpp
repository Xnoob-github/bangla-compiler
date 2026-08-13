#include <iostream>
#include <vector>
#include <string>

// Placeholder for Token and TokenType definitions (from Abid's Lexer)
struct Token {
    std::string type;
    std::string lexeme;
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    // Helper: Look at the current token without consuming it
    Token peek() {
        return tokens[current];
    }

    // Helper: Check if we have reached the end of the token list
    bool isAtEnd() {
        return peek().type == "EOF";
    }

    // Helper: Return the current token and move to the next one
    Token advance() {
        if (!isAtEnd()) current++;
        return tokens[current - 1];
    }

    void statement() {
        std::cout << "Parsing statement at token: " << peek().lexeme << std::endl;
        advance();
    }

public:
    // Constructor
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    // Main entry point for parsing
    void parse() {
        while (!isAtEnd()) {
            statement();
        }
    }
};