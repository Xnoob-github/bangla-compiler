#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <iostream>
#include <memory>
#include "ASTNodes.h"
#include "SymbolTable.h"

class SemanticAnalyzer {
private:
    SymbolTable symbolTable;
    bool hasError;

public:
    SemanticAnalyzer() : hasError(false) {}

    bool getHasError() const { return hasError; }

    void reset() {
        hasError = false;
        symbolTable.reset();
    }

    DataType visit(std::shared_ptr<ASTNode> node) {
        if (!node) return TYPE_UNKNOWN;

        switch (node->type) {
            case NODE_DECLARATION: {
                auto decl = std::dynamic_pointer_cast<VarDeclNode>(node);
                if (!decl) return TYPE_UNKNOWN;

                if (!symbolTable.declareVariable(decl->varName, decl->varType)) {
                    std::cout << "[Semantic Error]: Variable '" << decl->varName << "' already declared in this scope!\n";
                    hasError = true;
                }

                if (decl->initExpr) {
                    DataType initType = visit(decl->initExpr);
                    if (decl->varType != initType && initType != TYPE_UNKNOWN) {
                        std::cout << "[Semantic Error]: Type Mismatch in initialization of variable '" << decl->varName << "'!\n";
                        hasError = true;
                    }
                }
                return decl->varType;
            }

            case NODE_ASSIGNMENT: {
                auto assign = std::dynamic_pointer_cast<AssignNode>(node);
                if (!assign) return TYPE_UNKNOWN;

                Symbol* sym = symbolTable.lookup(assign->varName);
                if (!sym) {
                    std::cout << "[Semantic Error]: Variable '" << assign->varName << "' used before declaration!\n";
                    hasError = true;
                    return TYPE_UNKNOWN;
                }

                DataType exprType = visit(assign->value);
                if (sym->type != exprType && exprType != TYPE_UNKNOWN) {
                    std::cout << "[Semantic Error]: Type Mismatch! Cannot assign to variable '" << assign->varName << "'!\n";
                    hasError = true;
                }
                return sym->type;
            }

            case NODE_VARIABLE: {
                auto var = std::dynamic_pointer_cast<VarNode>(node);
                if (!var) return TYPE_UNKNOWN;

                Symbol* sym = symbolTable.lookup(var->varName);
                if (!sym) {
                    std::cout << "[Semantic Error]: Variable '" << var->varName << "' undeclared!\n";
                    hasError = true;
                    return TYPE_UNKNOWN;
                }
                return sym->type;
            }

            case NODE_LITERAL: {
                auto lit = std::dynamic_pointer_cast<LiteralNode>(node);
                if (!lit) return TYPE_UNKNOWN;
                return lit->literalType;
            }

            case NODE_BINARY_OP: {
                auto binOp = std::dynamic_pointer_cast<BinaryOpNode>(node);
                if (!binOp) return TYPE_UNKNOWN;

                DataType leftType = visit(binOp->left);
                DataType rightType = visit(binOp->right);

                if (leftType != rightType && leftType != TYPE_UNKNOWN && rightType != TYPE_UNKNOWN) {
                    std::cout << "[Semantic Error]: Type Mismatch in Binary Operation!\n";
                    hasError = true;
                    return TYPE_UNKNOWN;
                }
                return leftType;
            }

            case NODE_BLOCK: {
                auto block = std::dynamic_pointer_cast<BlockNode>(node);
                if (!block) return TYPE_UNKNOWN;

                symbolTable.enterScope();
                for (auto& stmt : block->statements) {
                    visit(stmt);
                }
                symbolTable.exitScope();
                return TYPE_UNKNOWN;
            }

            case NODE_WHILE: {
                auto whileNode = std::dynamic_pointer_cast<WhileNode>(node);
                if (!whileNode) return TYPE_UNKNOWN;

                visit(whileNode->condition);
                visit(whileNode->body);
                return TYPE_UNKNOWN;
            }

            default:
                return TYPE_UNKNOWN;
        }
    }
};

#endif // SEMANTIC_ANALYZER_H