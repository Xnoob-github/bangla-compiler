#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include "ASTNodes.h"

struct Symbol {
    std::string name;
    DataType type;
};

class Scope {
public:
    std::unordered_map<std::string, Symbol> table;
    std::shared_ptr<Scope> parent;

    Scope(std::shared_ptr<Scope> p = nullptr) : parent(p) {}

    bool insert(const std::string& name, DataType type) {
        if (table.find(name) != table.end()) return false;
        table[name] = {name, type};
        return true;
    }

    Symbol* lookup(const std::string& name) {
        if (table.find(name) != table.end()) return &table[name];
        if (parent != nullptr) return parent->lookup(name);
        return nullptr;
    }
};

class SymbolTable {
private:
    std::shared_ptr<Scope> currentScope;

public:
    SymbolTable() { currentScope = std::make_shared<Scope>(); }
    void enterScope() { currentScope = std::make_shared<Scope>(currentScope); }
    void exitScope() { if (currentScope->parent != nullptr) currentScope = currentScope->parent; }
    bool declareVariable(const std::string& name, DataType type) { return currentScope->insert(name, type); }
    Symbol* lookupVariable(const std::string& name) { return currentScope->lookup(name); }
};

#endif