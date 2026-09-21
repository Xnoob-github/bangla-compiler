#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "Token.h"
#include "ASTNodes.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    Token peek();
    bool isAtEnd();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& errorMessage);

    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<VarDeclNode> parseVarDeclaration();
    std::shared_ptr<ASTNode> parseAssignmentOrExpressionStatement();
    std::shared_ptr<BlockNode> parseBlock();
    std::shared_ptr<ASTNode> parseIfStatement();
    std::shared_ptr<ASTNode> parseWhileStatement();

    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseComparison();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
    std::shared_ptr<ASTNode> parsePrimary();

public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<BlockNode> parseProgram();
};

#endif
