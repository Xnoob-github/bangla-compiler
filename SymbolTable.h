#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "ASTNodes.h"

struct Symbol {
    std::string name;
    DataType type;
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;

public:
    SymbolTable() {
        enterScope(); // Global scope
    }

    void enterScope() {
        scopes.push_back({});
    }

    void exitScope() {
        if (scopes.size() > 1) {
            scopes.pop_back();
        }
    }

    void reset() {
        scopes.clear();
        enterScope();
    }

    bool declareVariable(const std::string& name, DataType type) {
        auto& currentScope = scopes.back();
        if (currentScope.find(name) != currentScope.end()) {
            return false; // Already declared in current scope
        }
        currentScope[name] = Symbol{name, type};
        return true;
    }

    Symbol* lookup(const std::string& name) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return &(found->second);
            }
        }
        return nullptr;
    }
};

#endif // SYMBOL_TABLE_H