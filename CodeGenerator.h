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
        switch (type) {
            case TYPE_INT: return "সংখ্যা";
            case TYPE_FLOAT: return "দশমিক";
            case TYPE_STRING: return "টেক্সট";
            default: return "অজানা";
        }
    }

    void emitBlock(const std::vector<std::shared_ptr<ASTNode>>& statements) {
        for (const auto& stmt : statements) {
            if (stmt) {
                generate(stmt.get());
            }
        }
    }

public:
    explicit CodeGenerator(const std::string& filename) {
        outFile.open(filename, std::ios::out | std::ios::trunc);
    }

    ~CodeGenerator() {
        if (outFile.is_open()) {
            outFile.close();
        }
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
            outFile << getIndent() << dataTypeToString(varDecl->varType)
                    << " " << varDecl->varName;
            if (varDecl->initExpr) {
                outFile << " = ";
                generate(varDecl->initExpr.get());
            }
            outFile << ";\n";
            return;
        }

        if (auto assign = dynamic_cast<AssignNode*>(node)) {
            outFile << getIndent() << assign->varName << " = ";
            if (assign->expr) {
                generate(assign->expr.get());
            }
            outFile << ";\n";
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
            outFile << getIndent() << "যদি (";
            if (ifNode->condition) {
                generate(ifNode->condition.get());
            }
            outFile << ") {\n";

            ++indentLevel;
            if (ifNode->thenBlock) {
                emitBlock(ifNode->thenBlock->statements);
            }
            --indentLevel;

            if (ifNode->elseBlock) {
                outFile << getIndent() << "} নাহলে {\n";
                ++indentLevel;
                emitBlock(ifNode->elseBlock->statements);
                --indentLevel;
            }

            outFile << getIndent() << "}\n";
            return;
        }

        if (auto whileNode = dynamic_cast<WhileNode*>(node)) {
            outFile << getIndent() << "যতক্ষণ (";
            if (whileNode->condition) {
                generate(whileNode->condition.get());
            }
            outFile << ") {\n";

            ++indentLevel;
            if (whileNode->body) {
                emitBlock(whileNode->body->statements);
            }
            --indentLevel;

            outFile << getIndent() << "}\n";
            return;
        }
    }
};