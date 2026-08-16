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
        if (type == TYPE_INT) return "সংখ্যা (int)";
        if (type == TYPE_FLOAT) return "দশমিক (float)";
        if (type == TYPE_STRING) return "string";
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
                    std::cout << "[Semantic Error] Line " << decl->line
                              << ": Variable '" << decl->varName << "' already declared!\n";
                    hasError = true;
                }

                if (decl->initExpr) {
                    DataType initType = visit(decl->initExpr);
                    if (initType != TYPE_UNKNOWN && initType != decl->varType) {
                        std::cout << "[Semantic Error] Line " << decl->line
                                  << ": Cannot initialize variable '" << decl->varName
                                  << "' of type " << typeToString(decl->varType)
                                  << " with value of type " << typeToString(initType) << "!\n";
                        hasError = true;
                    }
                }

                return decl->varType;
            }

            case NODE_VARIABLE: {
                auto var = std::dynamic_pointer_cast<VarNode>(node);
                Symbol* sym = symbolTable.lookupVariable(var->varName);
                if (!sym) {
                    std::cout << "[Semantic Error] Line " << var->line
                              << ": Variable '" << var->varName << "' used before declaration!\n";
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

                bool leftNumeric = (leftType == TYPE_INT || leftType == TYPE_FLOAT);
                bool rightNumeric = (rightType == TYPE_INT || rightType == TYPE_FLOAT);

                if (leftNumeric && rightNumeric) {
                    return (leftType == TYPE_FLOAT || rightType == TYPE_FLOAT) ? TYPE_FLOAT : TYPE_INT;
                }

                if (leftType != TYPE_UNKNOWN && rightType != TYPE_UNKNOWN) {
                    std::cout << "[Semantic Error] Line " << binOp->line
                              << ": Type mismatch in binary operation '" << binOp->op
                              << "' (" << typeToString(leftType) << " vs " << typeToString(rightType) << ")\n";
                    hasError = true;
                }
                return TYPE_UNKNOWN;
            }

            case NODE_ASSIGNMENT: {
                auto assign = std::dynamic_pointer_cast<AssignNode>(node);
                Symbol* sym = symbolTable.lookupVariable(assign->varName);

                if (!sym) {
                    std::cout << "[Semantic Error] Line " << assign->line
                              << ": Variable '" << assign->varName << "' used before declaration!\n";
                    hasError = true;
                    visit(assign->expr);
                    return TYPE_UNKNOWN;
                }

                DataType exprType = visit(assign->expr);
                if (exprType != TYPE_UNKNOWN && sym->type != exprType) {
                    std::cout << "[Semantic Error] Line " << assign->line
                              << ": Cannot assign " << typeToString(exprType)
                              << " to variable '" << assign->varName
                              << "' of type " << typeToString(sym->type) << "!\n";
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