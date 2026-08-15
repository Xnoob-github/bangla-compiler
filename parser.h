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
    bool match(const std::string& type);
    Token consume(const std::string& type, const std::string& errorMessage);
    
    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<VarDeclNode> parseVarDeclaration();
    std::shared_ptr<ASTNode> parseAssignmentOrExpression();

public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<BlockNode> parseProgram();
};

#endif