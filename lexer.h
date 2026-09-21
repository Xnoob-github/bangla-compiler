#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;
    std::size_t position = 0;
    int line = 1;

    bool peekNext(char expected) const;
    void addToken(TokenType type, const std::string& value);
    TokenType getKeywordType(const std::string& word) const;
    void readIdentifier();
    void readNumber();
};

#endif