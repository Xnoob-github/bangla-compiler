#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    KEYWORD_SHONGKHA,
    KEYWORD_DOSHOMIK,
    KEYWORD_JODI,
    KEYWORD_NAHOLE,
    KEYWORD_JOTOKKHON,
    IDENTIFIER,
    INTEGER,
    DECIMAL,
    STRING,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    EQUAL,
    NOT_EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,
    EOF_TOKEN,
    UNKNOWN
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD_SHONGKHA:  return "KEYWORD_SHONGKHA";
        case TokenType::KEYWORD_DOSHOMIK:  return "KEYWORD_DOSHOMIK";
        case TokenType::KEYWORD_JODI:      return "KEYWORD_JODI";
        case TokenType::KEYWORD_NAHOLE:    return "KEYWORD_NAHOLE";
        case TokenType::KEYWORD_JOTOKKHON: return "KEYWORD_JOTOKKHON";
        case TokenType::IDENTIFIER:        return "IDENTIFIER";
        case TokenType::INTEGER:           return "INTEGER";
        case TokenType::DECIMAL:           return "DECIMAL";
        case TokenType::STRING:            return "STRING";
        case TokenType::PLUS:              return "PLUS";
        case TokenType::MINUS:             return "MINUS";
        case TokenType::MULTIPLY:          return "MULTIPLY";
        case TokenType::DIVIDE:            return "DIVIDE";
        case TokenType::ASSIGN:            return "ASSIGN";
        case TokenType::EQUAL:             return "EQUAL";
        case TokenType::NOT_EQUAL:         return "NOT_EQUAL";
        case TokenType::LESS:              return "LESS";
        case TokenType::GREATER:           return "GREATER";
        case TokenType::LESS_EQUAL:        return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL:     return "GREATER_EQUAL";
        case TokenType::LEFT_PAREN:        return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:       return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:        return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:       return "RIGHT_BRACE";
        case TokenType::SEMICOLON:         return "SEMICOLON";
        case TokenType::EOF_TOKEN:         return "EOF";
        default:                          return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type = TokenType::UNKNOWN, std::string lexeme = "", int line = 0)
        : type(type), lexeme(lexeme), line(line) {}
};

#endif