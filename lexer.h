#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "Token.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;
    std::size_t position;
    int line;

    bool peekNext(char expected);
    void addToken(TokenType type, const std::string& value);
    TokenType getKeywordType(const std::string& word);
    void readIdentifier();
    void readNumber();
};

#endif