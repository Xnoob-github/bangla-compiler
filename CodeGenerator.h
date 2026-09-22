#pragma once

#include "ASTNodes.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

class CodeGenerator {
private:
    std::ofstream outFile;
    int indentLevel = 0;

    std::string getIndent() const {
        return std::string(indentLevel * 4, ' ');
    }

    std::string dataTypeToString(DataType type) const {
        (void)type;
        return "";
    }

    void emitBlock(const std::vector<std::shared_ptr<ASTNode>>& statements) {
        for (const auto& stmt : statements) {
            if (stmt) {
                generate(stmt.get());
            }
        }
    }

public:
    CodeGenerator() = default;

    explicit CodeGenerator(const std::string& filename) {
        open(filename);
    }

    void open(const std::string& filename) {
        outFile.close();
        outFile.open(filename, std::ios::out | std::ios::trunc);
    }

    ~CodeGenerator() {
        if (outFile.is_open()) {
            outFile.close();
        }
    }

    void generate(const std::shared_ptr<ASTNode>& node, const std::string& filename) {
        open(filename);
        generate(node.get());
    }

    void generate(ASTNode* node) {
        if (!node || !outFile.is_open()) {
            return;
        }

        if (auto block = dynamic_cast<BlockNode*>(node)) {
            emitBlock(block->statements);
            return;
        }

        if (auto varDecl = dynamic_cast<VarDeclNode*>(node)) {
            outFile << getIndent() << varDecl->varName;
            if (varDecl->initExpr) {
                outFile << " = ";
                generate(varDecl->initExpr.get());
            }
            outFile << "\n";
            return;
        }

        if (auto assign = dynamic_cast<AssignNode*>(node)) {
            outFile << getIndent() << assign->varName << " = ";
            if (assign->expr) {
                generate(assign->expr.get());
            }
            outFile << "\n";
            return;
        }

        if (auto var = dynamic_cast<VarNode*>(node)) {
            outFile << var->varName;
            return;
        }

        if (auto literal = dynamic_cast<LiteralNode*>(node)) {
            if (literal->dataType == TYPE_STRING) {
                outFile << "\"" << literal->value << "\"";
            } else {
                outFile << literal->value;
            }
            return;
        }

        if (auto binOp = dynamic_cast<BinaryOpNode*>(node)) {
            outFile << "(";
            if (binOp->left) {
                generate(binOp->left.get());
            }
            outFile << " " << binOp->op << " ";
            if (binOp->right) {
                generate(binOp->right.get());
            }
            outFile << ")";
            return;
        }

        if (auto ifNode = dynamic_cast<IfNode*>(node)) {
            outFile << getIndent() << "if ";
            if (ifNode->condition) {
                generate(ifNode->condition.get());
            }
            outFile << ":\n";

            ++indentLevel;
            if (ifNode->thenBlock) {
                emitBlock(ifNode->thenBlock->statements);
            }
            --indentLevel;

            if (ifNode->elseBlock) {
                outFile << getIndent() << "else:\n";
                ++indentLevel;
                emitBlock(ifNode->elseBlock->statements);
                --indentLevel;
            }

            return;
        }

        if (auto whileNode = dynamic_cast<WhileNode*>(node)) {
            outFile << getIndent() << "while ";
            if (whileNode->condition) {
                generate(whileNode->condition.get());
            }
            outFile << ":\n";

            ++indentLevel;
            if (whileNode->body) {
                emitBlock(whileNode->body->statements);
            }
            --indentLevel;

            return;
        }
    }
};