#ifndef AST_NODES_H
#define AST_NODES_H

#include <string>
#include <vector>
#include <memory>

enum DataType {
    TYPE_INT,      // shongkha
    TYPE_STRING,   // lekha
    TYPE_UNKNOWN   // invalid / error
};

enum NodeType {
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_BINARY_OP,
    NODE_VARIABLE,
    NODE_LITERAL,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK
};

class ASTNode {
public:
    NodeType type;
    virtual ~ASTNode() = default;
};

class VarDeclNode : public ASTNode {
public:
    std::string varName;
    DataType varType;
    VarDeclNode(std::string name, DataType t) {
        type = NODE_DECLARATION;
        varName = name;
        varType = t;
    }
};

class VarNode : public ASTNode {
public:
    std::string varName;
    VarNode(std::string name) {
        type = NODE_VARIABLE;
        varName = name;
    }
};

class LiteralNode : public ASTNode {
public:
    DataType dataType;
    std::string value;
    LiteralNode(DataType t, std::string val) {
        type = NODE_LITERAL;
        dataType = t;
        value = val;
    }
};

class BinaryOpNode : public ASTNode {
public:
    char op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    BinaryOpNode(char o, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r) {
        type = NODE_BINARY_OP;
        op = o;
        left = l;
        right = r;
    }
};

class AssignNode : public ASTNode {
public:
    std::string varName;
    std::shared_ptr<ASTNode> expr;
    AssignNode(std::string name, std::shared_ptr<ASTNode> e) {
        type = NODE_ASSIGNMENT;
        varName = name;
        expr = e;
    }
};

class BlockNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> statements;
    BlockNode() { type = NODE_BLOCK; }
};

class IfNode : public ASTNode {
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> thenBlock;
    std::shared_ptr<ASTNode> elseBlock;
    IfNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> thenB, std::shared_ptr<ASTNode> elseB = nullptr) {
        type = NODE_IF;
        condition = cond;
        thenBlock = thenB;
        elseBlock = elseB;
    }
};

class WhileNode : public ASTNode {
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> b) {
        type = NODE_WHILE;
        condition = cond;
        body = b;
    }
};

#endif