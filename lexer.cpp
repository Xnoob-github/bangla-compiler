#include "lexer.h"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

Lexer::Lexer(const std::string& source) : source(source), position(0), line(1) {}

bool Lexer::peekNext(char expected) {
    return position + 1 < source.length() && source[position + 1] == expected;
}

void Lexer::addToken(TokenType type, const std::string& value) {
    tokens.emplace_back(type, value, line);
}

TokenType Lexer::getKeywordType(const std::string& word) {
    if (word == "সংখ্যা") return TokenType::KEYWORD_SHONGKHA;
    if (word == "দশমিক") return TokenType::KEYWORD_DOSHOMIK;
    if (word == "যদি") return TokenType::KEYWORD_JODI;
    if (word == "নাহলে") return TokenType::KEYWORD_NAHOLE;
    if (word == "যতক্ষণ") return TokenType::KEYWORD_JOTOKKHON;
    return TokenType::IDENTIFIER;
}

void Lexer::readIdentifier() {
    const std::size_t start = position;
    while (position < source.length()) {
        unsigned char ch = static_cast<unsigned char>(source[position]);
        if (std::isalnum(ch) || ch == '_') {
            ++position;
        } else {
            break;
        }
    }

    const std::string word = source.substr(start, position - start);
    if (word == "সংখ্যা" || word == "দশমিক" || word == "যদি" || word == "নাহলে" || word == "যতক্ষণ") {
        addToken(getKeywordType(word), word);
        return;
    }

    addToken(TokenType::IDENTIFIER, word);
}

void Lexer::readNumber() {
    const std::size_t start = position;
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

    const std::string number = source.substr(start, position - start);
    addToken(hasDecimal ? TokenType::DECIMAL : TokenType::INTEGER, number);
}

std::vector<Token> Lexer::tokenize() {
    while (position < source.length()) {
        const char current = source[position];

        if (current == ' ' || current == '\t' || current == '\r') {
            ++position;
            continue;
        }

        if (current == '\n') {
            ++line;
            ++position;
            continue;
        }

        bool matchedKeyword = false;
        for (const std::string& keyword : {"সংখ্যা", "দশমিক", "যদি", "নাহলে", "যতক্ষণ"}) {
            if (source.compare(position, keyword.length(), keyword) == 0) {
                addToken(getKeywordType(keyword), keyword);
                position += keyword.length();
                matchedKeyword = true;
                break;
            }
        }
        if (matchedKeyword) {
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

        if (current == '"') {
            const std::size_t start = position + 1;
            ++position;
            while (position < source.length() && source[position] != '"') {
                if (source[position] == '\\' && position + 1 < source.length()) {
                    ++position;
                }
                ++position;
            }

            if (position >= source.length()) {
                std::cout << "Lexer Error: Unterminated string at line " << line << std::endl;
                break;
            }

            const std::string literal = source.substr(start, position - start);
            addToken(TokenType::STRING, literal);
            ++position;
            continue;
        }

        switch (current) {
        case '+': addToken(TokenType::PLUS, "+"); ++position; break;
        case '-': addToken(TokenType::MINUS, "-"); ++position; break;
        case '*': addToken(TokenType::MULTIPLY, "*"); ++position; break;
        case '/': addToken(TokenType::DIVIDE, "/"); ++position; break;
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
        case '(': addToken(TokenType::LEFT_PAREN, "("); ++position; break;
        case ')': addToken(TokenType::RIGHT_PAREN, ")"); ++position; break;
        case '{': addToken(TokenType::LEFT_BRACE, "{"); ++position; break;
        case '}': addToken(TokenType::RIGHT_BRACE, "}"); ++position; break;
        case ';': addToken(TokenType::SEMICOLON, ";"); ++position; break;
        default:
            std::cout << "Lexer Error: Unknown character '" << current << "' at line " << line << std::endl;
            ++position;
            break;
        }
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "", line);
    return tokens;
}