```cpp
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <variant>

#include "ASTNodes.h"
#include "SymbolTable.h"

class Interpreter {
private:
    std::shared_ptr<SymbolTable> env;

    double toDouble(const Value& val) {
        if (std::holds_alternative<int>(val))
            return std::get<int>(val);

        if (std::holds_alternative<double>(val))
            return std::get<double>(val);

        return 0.0;
    }

    bool isTruthy(const Value& val) {
        return toDouble(val) != 0.0;
    }

public:

    Interpreter() {
        env = std::make_shared<SymbolTable>();
    }

    Value evaluate(const std::shared_ptr<ASTNode>& node) {

        if (!node)
            return Value(0);

        switch (node->type) {

            case NODE_BLOCK: {

                auto block =
                    std::dynamic_pointer_cast<BlockNode>(node);

                auto previousEnv = env;

                env = std::make_shared<SymbolTable>(previousEnv);

                for (auto& stmt : block->statements) {
                    evaluate(stmt);
                }

                env = previousEnv;

                return Value(0);
            }

            case NODE_DECLARATION: {

                auto decl =
                    std::dynamic_pointer_cast<VarDeclNode>(node);

                Value val;

                if (decl->varType == TYPE_FLOAT)
                    val = Value(0.0);
                else
                    val = Value(0);

                if (decl->initExpr)
                    val = evaluate(decl->initExpr);

                env->declare(
                    decl->varName,
                    decl->varType,
                    val
                );

                return val;
            }

            case NODE_ASSIGNMENT: {

                auto assign =
                    std::dynamic_pointer_cast<AssignNode>(node);

                Value val =
                    evaluate(assign->expr);

                if (!env->assign(assign->varName, val)) {

                    throw std::runtime_error(
                        "Runtime Error: Undefined variable '" +
                        assign->varName + "'"
                    );
                }

                return val;
            }

            case NODE_VARIABLE: {

                auto var =
                    std::dynamic_pointer_cast<VarNode>(node);

                Symbol* sym =
                    env->lookup(var->varName);

                if (!sym) {

                    throw std::runtime_error(
                        "Runtime Error: Undefined variable '" +
                        var->varName + "'"
                    );
                }

                return sym->value;
            }

            case NODE_LITERAL: {

                auto lit =
                    std::dynamic_pointer_cast<LiteralNode>(node);

                if (lit->dataType == TYPE_INT)
                    return Value(std::stoi(lit->value));

                if (lit->dataType == TYPE_FLOAT)
                    return Value(std::stod(lit->value));

                return Value(lit->value);
            }

            case NODE_BINARY_OP: {

                auto bin =
                    std::dynamic_pointer_cast<BinaryOpNode>(node);

                Value leftVal =
                    evaluate(bin->left);

                Value rightVal =
                    evaluate(bin->right);

                bool isFloat =
                    std::holds_alternative<double>(leftVal) ||
                    std::holds_alternative<double>(rightVal);

                double l = toDouble(leftVal);
                double r = toDouble(rightVal);

                if (bin->op == "+") {

                    if (isFloat)
                        return Value(l + r);

                    return Value(
                        (int)l + (int)r
                    );
                }

                if (bin->op == "-") {

                    if (isFloat)
                        return Value(l - r);

                    return Value(
                        (int)l - (int)r
                    );
                }

                if (bin->op == "*") {

                    if (isFloat)
                        return Value(l * r);

                    return Value(
                        (int)l * (int)r
                    );
                }

                if (bin->op == "/") {

                    if (r == 0)
                        throw std::runtime_error(
                            "Runtime Error: Division by zero."
                        );

                    if (isFloat)
                        return Value(l / r);

                    return Value(
                        (int)l / (int)r
                    );
                }

                if (bin->op == "<")
                    return Value((int)(l < r));

                if (bin->op == ">")
                    return Value((int)(l > r));

                if (bin->op == "<=")
                    return Value((int)(l <= r));

                if (bin->op == ">=")
                    return Value((int)(l >= r));

                if (bin->op == "==")
                    return Value((int)(l == r));

                if (bin->op == "!=")
                    return Value((int)(l != r));

                throw std::runtime_error(
                    "Runtime Error: Unknown operator '" +
                    bin->op + "'"
                );
            }

            case NODE_IF: {

                auto ifNode =
                    std::dynamic_pointer_cast<IfNode>(node);

                Value cond =
                    evaluate(ifNode->condition);

                if (isTruthy(cond)) {

                    evaluate(ifNode->thenBlock);

                } else if (ifNode->elseBlock) {

                    evaluate(ifNode->elseBlock);
                }

                return Value(0);
            }

            case NODE_WHILE: {

                auto whileNode =
                    std::dynamic_pointer_cast<WhileNode>(node);

                while (
                    isTruthy(
                        evaluate(whileNode->condition)
                    )
                ) {

                    evaluate(whileNode->body);
                }

                return Value(0);
            }

            default:

                throw std::runtime_error(
                    "Runtime Error: Unknown AST node."
                );
        }
    }

    void dumpState() {

        std::cout
            << "===== Environment State ====="
            << std::endl;
    }
};

#endif
```
