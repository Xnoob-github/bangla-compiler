#ifndef AST_NODES_H
#define AST_NODES_H

#include <string>
#include <vector>
#include <memory>

enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_UNKNOWN
};

enum NodeType {
    NODE_BLOCK,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_VARIABLE,
    NODE_LITERAL,
    NODE_BINARY_OP,
    NODE_IF,
    NODE_WHILE
};

struct ASTNode {
    NodeType type;
    int line = 0;
    virtual ~ASTNode() = default;
};

struct BlockNode : ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;
    BlockNode() { type = NODE_BLOCK; }
};

struct VarDeclNode : ASTNode {
    std::string varName;
    DataType varType;
    std::shared_ptr<ASTNode> initExpr;
    VarDeclNode() { type = NODE_DECLARATION; }
};

struct AssignNode : ASTNode {
    std::string varName;
    std::shared_ptr<ASTNode> expr;
    AssignNode() { type = NODE_ASSIGNMENT; }
};

struct VarNode : ASTNode {
    std::string varName;
    VarNode() { type = NODE_VARIABLE; }
};

struct LiteralNode : ASTNode {
    std::string value;
    DataType dataType;
    LiteralNode() { type = NODE_LITERAL; }
};

struct BinaryOpNode : ASTNode {
    std::string op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    BinaryOpNode() { type = NODE_BINARY_OP; }
};

struct IfNode : ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<BlockNode> thenBlock;
    std::shared_ptr<BlockNode> elseBlock;
    IfNode() { type = NODE_IF; }
};

struct WhileNode : ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<BlockNode> body;
    WhileNode() { type = NODE_WHILE; }
};

#endif
