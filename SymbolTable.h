#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <variant>
#include "ASTNodes.h"

using Value = std::variant<int, double, std::string, bool>;

struct Symbol {
    std::string name;
    DataType type;
    Value value = 0;
};

class Scope {
public:
    std::unordered_map<std::string, Symbol> table;
    std::shared_ptr<Scope> parent;

    Scope(std::shared_ptr<Scope> p = nullptr) : parent(p) {}

    bool insert(const std::string& name, DataType type, const Value& value = Value(0)) {
        if (table.find(name) != table.end()) return false;
        table[name] = {name, type, value};
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

    bool declareVariable(const std::string& name, DataType type, const Value& value = Value(0)) {
        return currentScope->insert(name, type, value);
    }

    bool declare(const std::string& name, DataType type, const Value& value = Value(0)) {
        return declareVariable(name, type, value);
    }

    bool assign(const std::string& name, const Value& value) {
        Symbol* sym = lookup(name);
        if (!sym) return false;
        sym->value = value;
        return true;
    }

    Symbol* lookupVariable(const std::string& name) { return currentScope->lookup(name); }
    Symbol* lookup(const std::string& name) { return currentScope->lookup(name); }
};

#endif
