#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    int position;
    int line;

    bool isBanglaCharacter(unsigned char c);
    bool peekNext(char expected);
    void addToken(TokenType type, const std::string& value);
    TokenType getKeywordType(const std::string& word);
    void readIdentifier();
    void readNumber();

public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();
};

#endif