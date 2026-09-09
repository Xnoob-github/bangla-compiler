#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

Lexer::Lexer(const string& source) : source(source), position(0), line(1) {}


    // ====================================
    // Check Bangla Character
    // ====================================
bool Lexer::isBanglaCharacter(unsigned char c) {

        /*
         * Bangla Unicode range:
         * U+0980 - U+09FF
         *
         * This function is mainly kept
         * for structure/documentation.
         */
        return false;
    }


    // ====================================
    // Check Next Character
    // ====================================
bool Lexer::peekNext(char expected) {

        if (position + 1 < source.length()) {

            return source[position + 1] == expected;
        }

        return false;
    }


    // ====================================
    // Add Token
    // ====================================
void Lexer::addToken(TokenType type, const string& value) {

        tokens.push_back(
            Token(type, value, line)
        );
    }


    // ====================================
    // Check Keyword
    // ====================================
TokenType Lexer::getKeywordType(const string& word) {

        if (word == "সংখ্যা") {
            return TokenType::KEYWORD_SHONGKHA;
        }

        if (word == "দশমিক") {
            return TokenType::KEYWORD_DOSHOMIK;
        }

        if (word == "যদি") {
            return TokenType::KEYWORD_JODI;
        }

        if (word == "নাহলে") {
            return TokenType::KEYWORD_NAHOLE;
        }

        if (word == "যতক্ষণ") {
            return TokenType::KEYWORD_JOTOKKHON;
        }

        return TokenType::IDENTIFIER;
    }


    // ====================================
    // Read Identifier / Keyword
    // ====================================
void Lexer::readIdentifier() {

        int start = position;

        while (position < source.length()) {

            unsigned char current =
                source[position];

            /*
             * Continue reading English letters,
             * digits and underscore.
             *
             * Bangla keywords are handled separately
             * below.
             */

            if (isalnum(current) ||
                current == '_') {

                position++;
            }

            else {

                break;
            }
        }


        /*
         * Handle Bangla UTF-8 text.
         *
         * Bangla characters use multiple bytes
         * in UTF-8, so we check known keywords
         * from the source.
         */

        vector<string> keywords = {
            "সংখ্যা",
            "দশমিক",
            "যদি",
            "নাহলে",
            "যতক্ষণ"
        };


        for (string keyword : keywords) {

            if (source.compare(
                    start,
                    keyword.length(),
                    keyword
                ) == 0) {

                position =
                    start + keyword.length();

                TokenType type =
                    getKeywordType(keyword);

                addToken(
                    type,
                    keyword
                );

                return;
            }
        }


        string word =
            source.substr(
                start,
                position - start
            );


        addToken(
            TokenType::IDENTIFIER,
            word
        );
    }


    // ====================================
    // Read Number
    // ====================================
void Lexer::readNumber() {

        int start = position;

        bool hasDecimal = false;


        // Read integer part
        while (
            position < source.length() &&
            isdigit(
                (unsigned char)source[position]
            )
        ) {

            position++;
        }


        // Check decimal point
        if (
            position < source.length() &&
            source[position] == '.'
        ) {

            hasDecimal = true;

            position++;


            // Read decimal part
            while (
                position < source.length() &&
                isdigit(
                    (unsigned char)source[position]
                )
            ) {

                position++;
            }
        }


        string number =
            source.substr(
                start,
                position - start
            );


        if (hasDecimal) {

            addToken(
                TokenType::DECIMAL,
                number
            );
        }

        else {

            addToken(
                TokenType::INTEGER,
                number
            );
        }
    }


    // ====================================
    // Tokenize
    // ====================================
vector<Token> Lexer::tokenize() {

        while (position < source.length()) {

            char current =
                source[position];


            // ----------------------------
            // Ignore spaces
            // ----------------------------
            if (
                current == ' ' ||
                current == '\t' ||
                current == '\r'
            ) {

                position++;

                continue;
            }


            // ----------------------------
            // New line
            // ----------------------------
            if (current == '\n') {

                line++;

                position++;

                continue;
            }


            // ----------------------------
            // Bangla Keywords
            // ----------------------------
            bool foundKeyword = false;

            vector<string> keywords = {

                "সংখ্যা",
                "দশমিক",
                "যদি",
                "নাহলে",
                "যতক্ষণ"
            };


            for (string keyword : keywords) {

                if (
                    source.compare(
                        position,
                        keyword.length(),
                        keyword
                    ) == 0
                ) {

                    TokenType type =
                        getKeywordType(keyword);

                    addToken(
                        type,
                        keyword
                    );

                    position += keyword.length();

                    foundKeyword = true;

                    break;
                }
            }


            if (foundKeyword) {

                continue;
            }


            // ----------------------------
            // English Identifier
            // ----------------------------
            if (
                isalpha(
                    (unsigned char)current
                ) ||
                current == '_'
            ) {

                readIdentifier();

                continue;
            }


            // ----------------------------
            // Number
            // ----------------------------
            if (
                isdigit(
                    (unsigned char)current
                )
            ) {

                readNumber();

                continue;
            }


            // ----------------------------
            // Operators & Delimiters
            // ----------------------------
            switch (current) {


                // Addition
                case '+':

                    addToken(
                        TokenType::PLUS,
                        "+"
                    );

                    position++;

                    break;


                // Subtraction
                case '-':

                    addToken(
                        TokenType::MINUS,
                        "-"
                    );

                    position++;

                    break;


                // Multiplication
                case '*':

                    addToken(
                        TokenType::MULTIPLY,
                        "*"
                    );

                    position++;

                    break;


                // Division
                case '/':

                    addToken(
                        TokenType::DIVIDE,
                        "/"
                    );

                    position++;

                    break;


                // Assignment / Equal
                case '=':

                    if (peekNext('=')) {

                        addToken(
                            TokenType::EQUAL,
                            "=="
                        );

                        position += 2;
                    }

                    else {

                        addToken(
                            TokenType::ASSIGN,
                            "="
                        );

                        position++;
                    }

                    break;


                // Not Equal
                case '!':

                    if (peekNext('=')) {

                        addToken(
                            TokenType::NOT_EQUAL,
                            "!="
                        );

                        position += 2;
                    }

                    else {

                        cout
                            << "Lexer Error: Unexpected '!' "
                            << "at line "
                            << line
                            << endl;

                        position++;
                    }

                    break;


                // Less Than
                case '<':

                    if (peekNext('=')) {

                        addToken(
                            TokenType::LESS_EQUAL,
                            "<="
                        );

                        position += 2;
                    }

                    else {

                        addToken(
                            TokenType::LESS,
                            "<"
                        );

                        position++;
                    }

                    break;


                // Greater Than
                case '>':

                    if (peekNext('=')) {

                        addToken(
                            TokenType::GREATER_EQUAL,
                            ">="
                        );

                        position += 2;
                    }

                    else {

                        addToken(
                            TokenType::GREATER,
                            ">"
                        );

                        position++;
                    }

                    break;


                // Left Parenthesis
                case '(':

                    addToken(
                        TokenType::LEFT_PAREN,
                        "("
                    );

                    position++;

                    break;


                // Right Parenthesis
                case ')':

                    addToken(
                        TokenType::RIGHT_PAREN,
                        ")"
                    );

                    position++;

                    break;


                // Left Brace
                case '{':

                    addToken(
                        TokenType::LEFT_BRACE,
                        "{"
                    );

                    position++;

                    break;


                // Right Brace
                case '}':

                    addToken(
                        TokenType::RIGHT_BRACE,
                        "}"
                    );

                    position++;

                    break;


                // Semicolon
                case ';':

                    addToken(
                        TokenType::SEMICOLON,
                        ";"
                    );

                    position++;

                    break;


                // Unknown Character
                default:

                    cout
                        << "Lexer Error: Unknown character '"
                        << current
                        << "' at line "
                        << line
                        << endl;

                    position++;

                    break;
            }
        }


        // ----------------------------
        // EOF Token
        // ----------------------------
        tokens.push_back(
            Token(
                TokenType::EOF_TOKEN,
                "",
                line
            )
        );


    return tokens;
}


// ========================================
// MAIN FUNCT// int main(ION - Testing
// ========================================
// ) {

//     string code =

//         "সংখ্যা x = 10;\n"

//         "দশমিক y = 20.5;\n"

//         "x = x + 5 * 2;\n"

//         "যদি (x < y) {\n"

//         "    x = x + 1;\n"

//         "} নাহলে {\n"

//         "    x = x - 1;\n"

//         "}\n"

//         "যতক্ষণ (x < 50) {\n"

//         "    x = x + 2;\n"

//         "}";


//     // Create Lexer
//     Lexer lexer(code);


//     // Generate Tokens
//     vector<Token> tokens =
//         lexer.tokenize();


//     // Print Tokens
//     cout << "===== TOKENS =====" << endl;


//     for (Token token : tokens) {

//         cout
//             << "Line: "
//             << token.line

//             << " | Type: "
//             << tokenTypeToString(token.type)

//             << " | Value: "
//             << token.value

//             << endl;
//     }


//     return 0;
// }