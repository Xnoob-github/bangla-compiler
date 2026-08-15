#include "Parser.h"
#include <iostream>

Token Parser::peek() {
    if (current >= tokens.size()) {
        return Token{"EOF", ""};
    }
    return tokens[current];
}

bool Parser::isAtEnd() {
    return peek().type == "EOF";
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::match(const std::string& type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(const std::string& type, const std::string& errorMessage) {
    if (peek().type == type) return advance();
    std::cerr << "[Parser Error]: " << errorMessage << " at '" << peek().lexeme << "'\n";
    return peek();
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::shared_ptr<BlockNode> Parser::parseProgram() {
    auto block = std::make_shared<BlockNode>();
    block->type = NODE_BLOCK;
    
    while (!isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            block->statements.push_back(stmt);
        }
    }
    return block;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    if (peek().type == "KEYWORD_SHONGKHA" || peek().type == "KEYWORD_LEKHA") {
        return parseVarDeclaration();
    }
    return parseAssignmentOrExpression();
}

std::shared_ptr<VarDeclNode> Parser::parseVarDeclaration() {
    Token typeToken = advance(); // shongkha or lekha
    DataType dType = (typeToken.lexeme == "shongkha") ? TYPE_INT : TYPE_STRING;
    
    Token nameToken = consume("IDENTIFIER", "Expected variable name after type");
    consume("SEMICOLON", "Expected ';' after variable declaration");

    auto node = std::make_shared<VarDeclNode>();
    node->type = NODE_DECLARATION;
    node->varName = nameToken.lexeme;
    node->varType = dType;
    return node;
}

std::shared_ptr<ASTNode> Parser::parseAssignmentOrExpression() {
    Token currentTok = advance();
    auto lit = std::make_shared<LiteralNode>();
    lit->type = NODE_LITERAL;
    lit->dataType = TYPE_INT;
    return lit;
}