#include "lexer.h"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

Lexer::Lexer(const std::string& source) : source(source) {}

bool Lexer::peekNext(char expected) const {
    if (position + 1 < source.length()) {
        return source[position + 1] == expected;
    }
    return false;
}

void Lexer::addToken(TokenType type, const std::string& value) {
    tokens.emplace_back(type, value, line);
}

TokenType Lexer::getKeywordType(const std::string& word) const {
    if (word == "সংখ্যা") return TokenType::KEYWORD_SHONGKHA;
    if (word == "দশমিক") return TokenType::KEYWORD_DOSHOMIK;
    if (word == "যদি") return TokenType::KEYWORD_JODI;
    if (word == "নাহলে") return TokenType::KEYWORD_NAHOLE;
    if (word == "যতক্ষণ") return TokenType::KEYWORD_JOTOKKHON;
    return TokenType::IDENTIFIER;
}

void Lexer::readIdentifier() {
    std::size_t start = position;

    while (position < source.length()) {
        unsigned char current = static_cast<unsigned char>(source[position]);
        if (std::isalnum(current) || current == '_') {
            ++position;
        } else {
            break;
        }
    }

    std::string word = source.substr(start, position - start);
    const std::vector<std::string> keywords = {
        "সংখ্যা", "দশমিক", "যদি", "নাহলে", "যতক্ষণ"
    };

    for (const std::string& keyword : keywords) {
        if (word == keyword) {
            addToken(getKeywordType(keyword), keyword);
            return;
        }
    }

    addToken(TokenType::IDENTIFIER, word);
}

void Lexer::readNumber() {
    std::size_t start = position;
    bool hasDecimal = false;

    while (position < source.length() && std::isdigit(static_cast<unsigned char>(source[position]))) {
        ++position;
    }

    if (position < source.length() && source[position] == '.') {
        hasDecimal = true;
        ++position;
        while (position < source.length() && std::isdigit(static_cast<unsigned char>(source[position]))) {
            ++position;
        }
    }

    std::string number = source.substr(start, position - start);
    if (hasDecimal) {
        addToken(TokenType::DECIMAL, number);
    } else {
        addToken(TokenType::INTEGER, number);
    }
}

std::vector<Token> Lexer::tokenize() {
    while (position < source.length()) {
        char current = source[position];

        if (current == ' ' || current == '\t' || current == '\r') {
            ++position;
            continue;
        }

        if (current == '\n') {
            ++line;
            ++position;
            continue;
        }

        const std::vector<std::string> keywords = {
            "সংখ্যা", "দশমিক", "যদি", "নাহলে", "যতক্ষণ"
        };

        bool foundKeyword = false;
        for (const std::string& keyword : keywords) {
            if (source.compare(position, keyword.length(), keyword) == 0) {
                addToken(getKeywordType(keyword), keyword);
                position += keyword.length();
                foundKeyword = true;
                break;
            }
        }

        if (foundKeyword) {
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(current)) || current == '_') {
            readIdentifier();
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(current))) {
            readNumber();
            continue;
        }

        switch (current) {
            case '+':
                addToken(TokenType::PLUS, "+");
                ++position;
                break;
            case '-':
                addToken(TokenType::MINUS, "-");
                ++position;
                break;
            case '*':
                addToken(TokenType::MULTIPLY, "*");
                ++position;
                break;
            case '/':
                addToken(TokenType::DIVIDE, "/");
                ++position;
                break;
            case '=':
                if (peekNext('=')) {
                    addToken(TokenType::EQUAL, "==");
                    position += 2;
                } else {
                    addToken(TokenType::ASSIGN, "=");
                    ++position;
                }
                break;
            case '!':
                if (peekNext('=')) {
                    addToken(TokenType::NOT_EQUAL, "!=");
                    position += 2;
                } else {
                    std::cout << "Lexer Error: Unexpected '!' at line " << line << std::endl;
                    ++position;
                }
                break;
            case '<':
                if (peekNext('=')) {
                    addToken(TokenType::LESS_EQUAL, "<=");
                    position += 2;
                } else {
                    addToken(TokenType::LESS, "<");
                    ++position;
                }
                break;
            case '>':
                if (peekNext('=')) {
                    addToken(TokenType::GREATER_EQUAL, ">=");
                    position += 2;
                } else {
                    addToken(TokenType::GREATER, ">");
                    ++position;
                }
                break;
            case '(':
                addToken(TokenType::LEFT_PAREN, "(");
                ++position;
                break;
            case ')':
                addToken(TokenType::RIGHT_PAREN, ")");
                ++position;
                break;
            case '{':
                addToken(TokenType::LEFT_BRACE, "{");
                ++position;
                break;
            case '}':
                addToken(TokenType::RIGHT_BRACE, "}");
                ++position;
                break;
            case ';':
                addToken(TokenType::SEMICOLON, ";");
                ++position;
                break;
            default:
                std::cout << "Lexer Error: Unknown character '" << current << "' at line " << line << std::endl;
                ++position;
                break;
        }
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "", line);
    return tokens;
}
