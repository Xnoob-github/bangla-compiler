#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// ========================================
// Token Types
// ========================================
enum class TokenType {

    // Keywords
    INT,
    FLOAT,
    IF,
    ELSE,
    WHILE,

    // Identifier and Literals
    IDENTIFIER,
    INTEGER,
    DECIMAL,

    // Arithmetic Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    // Assignment
    ASSIGN,

    // Comparison Operators
    EQUAL,
    NOT_EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,

    // Delimiters
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,

    // End of File
    EOF_TOKEN
};


// ========================================
// Convert Token Type to String
// ========================================
string tokenTypeToString(TokenType type) {

    switch (type) {

        case TokenType::INT:
            return "INT";

        case TokenType::FLOAT:
            return "FLOAT";

        case TokenType::IF:
            return "IF";

        case TokenType::ELSE:
            return "ELSE";

        case TokenType::WHILE:
            return "WHILE";

        case TokenType::IDENTIFIER:
            return "IDENTIFIER";

        case TokenType::INTEGER:
            return "INTEGER";

        case TokenType::DECIMAL:
            return "DECIMAL";

        case TokenType::PLUS:
            return "PLUS";

        case TokenType::MINUS:
            return "MINUS";

        case TokenType::MULTIPLY:
            return "MULTIPLY";

        case TokenType::DIVIDE:
            return "DIVIDE";

        case TokenType::ASSIGN:
            return "ASSIGN";

        case TokenType::EQUAL:
            return "EQUAL";

        case TokenType::NOT_EQUAL:
            return "NOT_EQUAL";

        case TokenType::LESS:
            return "LESS";

        case TokenType::GREATER:
            return "GREATER";

        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";

        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";

        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";

        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";

        case TokenType::LEFT_BRACE:
            return "LEFT_BRACE";

        case TokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";

        case TokenType::SEMICOLON:
            return "SEMICOLON";

        case TokenType::EOF_TOKEN:
            return "EOF";

        default:
            return "UNKNOWN";
    }
}


// ========================================
// Token Class
// ========================================
struct Token {

    TokenType type;
    string value;
    int line;

    Token(TokenType type, string value, int line) {

        this->type = type;
        this->value = value;
        this->line = line;
    }
};


// ========================================
// Lexer Class
// ========================================
class Lexer {

private:

    string source;

    vector<Token> tokens;

    int position;
    int line;


public:

    // ====================================
    // Constructor
    // ====================================
    Lexer(string source) {

        this->source = source;

        position = 0;
        line = 1;
    }


    // ====================================
    // Check Bangla Character
    // ====================================
    bool isBanglaCharacter(unsigned char c) {

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
    bool peekNext(char expected) {

        if (position + 1 < source.length()) {

            return source[position + 1] == expected;
        }

        return false;
    }


    // ====================================
    // Add Token
    // ====================================
    void addToken(TokenType type, string value) {

        tokens.push_back(
            Token(type, value, line)
        );
    }


    // ====================================
    // Check Keyword
    // ====================================
    TokenType getKeywordType(string word) {

        if (word == "সংখ্যা") {
            return TokenType::INT;
        }

        if (word == "দশমিক") {
            return TokenType::FLOAT;
        }

        if (word == "যদি") {
            return TokenType::IF;
        }

        if (word == "নাহলে") {
            return TokenType::ELSE;
        }

        if (word == "যতক্ষণ") {
            return TokenType::WHILE;
        }

        return TokenType::IDENTIFIER;
    }


    // ====================================
    // Read Identifier / Keyword
    // ====================================
    void readIdentifier() {

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
    void readNumber() {

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
    vector<Token> tokenize() {

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
};


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