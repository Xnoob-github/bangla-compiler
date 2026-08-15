#ifndef AST_NODES_H
#define AST_NODES_H

#include <string>
#include <vector>
#include <memory>

// Data Types Supported by BanglaCompiler
enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_UNKNOWN
};

// Types of AST Nodes
enum NodeType {
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_VARIABLE,
    NODE_LITERAL,
    NODE_BINARY_OP,
    NODE_BLOCK,
    NODE_WHILE
};

// Base Class for all AST Nodes
struct ASTNode {
    NodeType type;
    virtual ~ASTNode() = default;
    ASTNode(NodeType t) : type(t) {}
};

// Variable Declaration Node (e.g., shongkha x = 10;)
struct VarDeclNode : public ASTNode {
    std::string varName;
    DataType varType;
    std::shared_ptr<ASTNode> initExpr; // Initialization expression (Optional)

    VarDeclNode(std::string name, DataType t, std::shared_ptr<ASTNode> expr = nullptr)
        : ASTNode(NODE_DECLARATION), varName(name), varType(t), initExpr(expr) {}
};

// Assignment Node (e.g., x = 5;)
struct AssignNode : public ASTNode {
    std::string varName;
    std::shared_ptr<ASTNode> value;

    AssignNode(std::string name, std::shared_ptr<ASTNode> val)
        : ASTNode(NODE_ASSIGNMENT), varName(name), value(val) {}
};

// Variable Reference Node (e.g., using 'x' in expression)
struct VarNode : public ASTNode {
    std::string varName;

    VarNode(std::string name)
        : ASTNode(NODE_VARIABLE), varName(name) {}
};

// Constant Literal Node (e.g., 10, "hello")
struct LiteralNode : public ASTNode {
    DataType literalType;
    std::string value;

    LiteralNode(DataType t, std::string val)
        : ASTNode(NODE_LITERAL), literalType(t), value(val) {}
};

// Binary Operation Node (e.g., a + b)
struct BinaryOpNode : public ASTNode {
    char op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;

    BinaryOpNode(char operation, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r)
        : ASTNode(NODE_BINARY_OP), op(operation), left(l), right(r) {}
};

// Block Node for Scope handling (e.g., statements inside loops or functions)
struct BlockNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;

    BlockNode() : ASTNode(NODE_BLOCK) {}
};

// While Loop Node
struct WhileNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;

    WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> b)
        : ASTNode(NODE_WHILE), condition(cond), body(b) {}
};

#endif // AST_NODES_H