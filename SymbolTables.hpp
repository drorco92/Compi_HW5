#ifndef COMPI__HW3_SYMBOLTABLES_HPP
#define COMPI__HW3_SYMBOLTABLES_HPP


#include <vector>
#include <string>
#include <stack>
#include "Structs.hpp"
#include "hw3_output.hpp"

extern int yylineno;


class Symbol {
public:
    std::string name;
    int offset;
    std::string type;
    Symbol(std::string name, int offset, std::string type);
    virtual void print();
};

typedef std::vector<Symbol*> SymbolsVector;

class Symbol_Table {
public:
    Symbol_Table() = default;
    ~Symbol_Table() = default;
    Symbol_Table* parent = nullptr;
    SymbolsVector symbols ;
    explicit Symbol_Table(Symbol_Table* parent) : parent(parent) {}
};


typedef std::vector<Argument> ArgVector;
typedef std::vector<Symbol_Table*> TableVector;
typedef std::stack<int> OffsetStack;




class Variable : public Symbol {
public:
    Variable(std::string name, int offset, std::string type, std::string type_annotation);
    std::string type_annotation;
    void print() override;
};

class Function : public Symbol {
public:
    Function(std::string name, std::string return_type, ArgVector& args);
    ~Function();
    ArgVector args;
    std::string return_type;
    void print() override;
};





class TablesList {
public:
    TablesList();
    OffsetStack offsets;
    TableVector tables;
    Symbol_Table sym_table;
    Symbol* GetSymbol(const std::string& name, bool is_func);
    void OpenScope();
    void OpenGlobal();
    void CloseScope();
    void CloseGlobal();
    void AddSymbol(const std::string& annotation, const std::string& name, const std::string& type);                                     //variable symbol
    void AddSymbol(const std::string& name, const std::string& return_type, ArgVector& args);       //function symbol
    void AddArgsSymbols(ArgVector& args);
    void IsNameExists(const std::string& name, bool is_func);
};




#endif //COMPI__HW3_SYMBOLTABLES_HPP
