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

    std::string typeToString(DataType type) {
        if (type == TYPE_INT) return "shongkha";
        if (type == TYPE_STRING) return "lekha";
        return "UNKNOWN";
    }

public:
    SemanticAnalyzer() : hasError(false) {}
    bool getHasError() const { return hasError; }

    DataType visit(std::shared_ptr<ASTNode> node) {
        if (!node) return TYPE_UNKNOWN;

        switch (node->type) {
            case NODE_DECLARATION: {
                auto decl = std::dynamic_pointer_cast<VarDeclNode>(node);
                if (!symbolTable.declareVariable(decl->varName, decl->varType)) {
                    std::cout << "[Semantic Error]: Variable '" << decl->varName << "' already declared!\n";
                    hasError = true;
                }
                return decl->varType;
            }
            case NODE_VARIABLE: {
                auto var = std::dynamic_pointer_cast<VarNode>(node);
                Symbol* sym = symbolTable.lookupVariable(var->varName);
                if (!sym) {
                    std::cout << "[Semantic Error]: Variable '" << var->varName << "' used before declaration!\n";
                    hasError = true;
                    return TYPE_UNKNOWN;
                }
                return sym->type;
            }
            case NODE_LITERAL: {
                auto lit = std::dynamic_pointer_cast<LiteralNode>(node);
                return lit->dataType;
            }
            case NODE_BINARY_OP: {
                auto binOp = std::dynamic_pointer_cast<BinaryOpNode>(node);
                DataType leftType = visit(binOp->left);
                DataType rightType = visit(binOp->right);

                if (leftType == TYPE_INT && rightType == TYPE_INT) return TYPE_INT;
                std::cout << "[Semantic Error]: Type Mismatch in Binary Operation!\n";
                hasError = true;
                return TYPE_UNKNOWN;
            }
            case NODE_ASSIGNMENT: {
                auto assign = std::dynamic_pointer_cast<AssignNode>(node);
                Symbol* sym = symbolTable.lookupVariable(assign->varName);
                if (!sym) {
                    std::cout << "[Semantic Error]: Variable '" << assign->varName << "' used before declaration!\n";
                    hasError = true;
                    return TYPE_UNKNOWN;
                }
                DataType exprType = visit(assign->expr);
                if (sym->type != exprType && exprType != TYPE_UNKNOWN) {
                    std::cout << "[Semantic Error]: Cannot assign " << typeToString(exprType) 
                              << " to variable '" << assign->varName << "' of type " << typeToString(sym->type) << "!\n";
                    hasError = true;
                }
                return sym->type;
            }
            case NODE_BLOCK: {
                auto block = std::dynamic_pointer_cast<BlockNode>(node);
                symbolTable.enterScope();
                for (auto& stmt : block->statements) visit(stmt);
                symbolTable.exitScope();
                return TYPE_UNKNOWN;
            }
            case NODE_IF: {
                auto ifNode = std::dynamic_pointer_cast<IfNode>(node);
                visit(ifNode->condition);
                visit(ifNode->thenBlock);
                if (ifNode->elseBlock) visit(ifNode->elseBlock);
                return TYPE_UNKNOWN;
            }
            case NODE_WHILE: {
                auto whileNode = std::dynamic_pointer_cast<WhileNode>(node);
                visit(whileNode->condition);
                visit(whileNode->body);
                return TYPE_UNKNOWN;
            }
            default:
                return TYPE_UNKNOWN;
        }
    }
};

#endif