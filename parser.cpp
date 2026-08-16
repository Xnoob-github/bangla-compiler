#include "Parser.h"
#include <iostream>

Token Parser::peek() {
    if (current >= tokens.size()) {
        return Token{TokenType::EOF_TOKEN, "", 0};
    }
    return tokens[current];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::check(TokenType type) {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& errorMessage) {
    if (check(type)) return advance();
    std::cout << "[Parser Error] Line " << peek().line << ": " << errorMessage
              << " (got '" << peek().lexeme << "' / " << tokenTypeToString(peek().type) << ")\n";
    return peek();
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::shared_ptr<BlockNode> Parser::parseProgram() {
    auto block = std::make_shared<BlockNode>();
    while (!isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            block->statements.push_back(stmt);
        } else {
            if (!isAtEnd()) advance();
        }
    }
    return block;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::KEYWORD_SHONGKHA) || check(TokenType::KEYWORD_DOSHOMIK)) {
        return parseVarDeclaration();
    }
    if (check(TokenType::KEYWORD_JODI)) {
        return parseIfStatement();
    }
    if (check(TokenType::KEYWORD_JOTOKKHON)) {
        return parseWhileStatement();
    }
    if (check(TokenType::LEFT_BRACE)) {
        return parseBlock();
    }
    return parseAssignmentOrExpressionStatement();
}

std::shared_ptr<VarDeclNode> Parser::parseVarDeclaration() {
    Token typeToken = advance();
    DataType dType = (typeToken.type == TokenType::KEYWORD_SHONGKHA) ? TYPE_INT : TYPE_FLOAT;

    Token nameToken = consume(TokenType::IDENTIFIER, "Expected variable name after type keyword");

    auto node = std::make_shared<VarDeclNode>();
    node->varName = nameToken.lexeme;
    node->varType = dType;
    node->line = typeToken.line;

    if (match(TokenType::ASSIGN)) {
        node->initExpr = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseAssignmentOrExpressionStatement() {
    if (check(TokenType::IDENTIFIER) &&
        current + 1 < tokens.size() &&
        tokens[current + 1].type == TokenType::ASSIGN) {

        Token nameToken = advance();
        advance();

        auto assign = std::make_shared<AssignNode>();
        assign->varName = nameToken.lexeme;
        assign->line = nameToken.line;
        assign->expr = parseExpression();

        consume(TokenType::SEMICOLON, "Expected ';' after assignment");
        return assign;
    }

    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    return expr;
}

std::shared_ptr<BlockNode> Parser::parseBlock() {
    consume(TokenType::LEFT_BRACE, "Expected '{' to start block");
    auto block = std::make_shared<BlockNode>();

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) block->statements.push_back(stmt);
        else if (!isAtEnd()) advance();
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' to close block");
    return block;
}

std::shared_ptr<ASTNode> Parser::parseIfStatement() {
    Token kw = advance();
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'যদি'");
    auto cond = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    auto node = std::make_shared<IfNode>();
    node->line = kw.line;
    node->condition = cond;
    node->thenBlock = parseBlock();

    if (match(TokenType::KEYWORD_NAHOLE)) {
        node->elseBlock = parseBlock();
    }

    return node;
}

std::shared_ptr<ASTNode> Parser::parseWhileStatement() {
    Token kw = advance();
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'যতক্ষণ'");
    auto cond = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    auto node = std::make_shared<WhileNode>();
    node->line = kw.line;
    node->condition = cond;
    node->body = parseBlock();

    return node;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    return parseComparison();
}

std::shared_ptr<ASTNode> Parser::parseComparison() {
    auto left = parseTerm();

    while (check(TokenType::EQUAL) || check(TokenType::NOT_EQUAL) ||
           check(TokenType::LESS) || check(TokenType::GREATER) ||
           check(TokenType::LESS_EQUAL) || check(TokenType::GREATER_EQUAL)) {

        Token opTok = advance();
        auto right = parseTerm();

        auto node = std::make_shared<BinaryOpNode>();
        node->op = opTok.lexeme;
        node->left = left;
        node->right = right;
        node->line = opTok.line;
        left = node;
    }
    return left;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        Token opTok = advance();
        auto right = parseFactor();

        auto node = std::make_shared<BinaryOpNode>();
        node->op = opTok.lexeme;
        node->left = left;
        node->right = right;
        node->line = opTok.line;
        left = node;
    }
    return left;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
    auto left = parsePrimary();

    while (check(TokenType::MULTIPLY) || check(TokenType::DIVIDE)) {
        Token opTok = advance();
        auto right = parsePrimary();

        auto node = std::make_shared<BinaryOpNode>();
        node->op = opTok.lexeme;
        node->left = left;
        node->right = right;
        node->line = opTok.line;
        left = node;
    }
    return left;
}

std::shared_ptr<ASTNode> Parser::parsePrimary() {
    if (check(TokenType::INTEGER)) {
        Token t = advance();
        auto lit = std::make_shared<LiteralNode>();
        lit->dataType = TYPE_INT;
        lit->value = t.lexeme;
        lit->line = t.line;
        return lit;
    }

    if (check(TokenType::DECIMAL)) {
        Token t = advance();
        auto lit = std::make_shared<LiteralNode>();
        lit->dataType = TYPE_FLOAT;
        lit->value = t.lexeme;
        lit->line = t.line;
        return lit;
    }

    if (check(TokenType::STRING)) {
        Token t = advance();
        auto lit = std::make_shared<LiteralNode>();
        lit->dataType = TYPE_STRING;
        lit->value = t.lexeme;
        lit->line = t.line;
        return lit;
    }

    if (check(TokenType::IDENTIFIER)) {
        Token t = advance();
        auto var = std::make_shared<VarNode>();
        var->varName = t.lexeme;
        var->line = t.line;
        return var;
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    std::cout << "[Parser Error] Line " << peek().line
              << ": Unexpected token '" << peek().lexeme
              << "' (" << tokenTypeToString(peek().type) << ") in expression\n";

    auto lit = std::make_shared<LiteralNode>();
    lit->dataType = TYPE_UNKNOWN;
    lit->value = "";
    if (!isAtEnd()) advance();
    return lit;
}
