
#ifndef COMPI__HW3_STRUCTS_HPP
#define COMPI__HW3_STRUCTS_HPP


#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "bp.hpp"



struct Argument {
    std::string name;
    std::string type;
    std::string annotation;
    Argument(const std::string& annotation, const std::string& name, const std::string& type) : name(name), type(type) ,annotation(annotation){};
};

typedef std::vector<Argument> ArgVector;
typedef std::vector<Argument> TypeVector;
typedef std::vector<std::string> StringVector;

struct Exp {
    std::string name;
    std::string type;
    std::string size;
    std::string reg;
    string val;
    bool print_val = false;
    size_t string_len;
    vector<pair<int,BranchLabelIndex>> true_list;
    vector<pair<int,BranchLabelIndex>> false_list;
    std::string annotation;

    explicit Exp(const std::string& type, const string& reg_name = "") {
        this->type = /*string(type)*/type;
        this->reg = string(reg_name);
    }
    Exp(const Exp& exp, const std::string& type) {
        this->name = exp.name;
        this->type = type;
        this->val = exp.val;
    }
};

struct Id {
    std::string name;
    explicit Id(const std::string& name): name(name) {};
};



struct ArgumentList {
    vector<Exp*> exp;
    ArgVector arguments;
    ArgumentList() = default;

    friend ostream& operator<<(ostream& os, const ArgVector & sl) {
        for (auto & it : sl) {
            os << it.type << ", ";
        }
        os << endl;
        return os;
    }
};

struct CodeGenLabel {
    string label;
    explicit CodeGenLabel(const string& label_name) {
        this->label = string(label_name);
    }
};

struct CodeGenBlock {
    vector<pair<int,BranchLabelIndex>> continueList;
    vector<pair<int,BranchLabelIndex>> breakList;
    vector<pair<int,BranchLabelIndex>> nextList;
    CodeGenBlock() {
        this->continueList = vector<pair<int,BranchLabelIndex>>();
        this->nextList = vector<pair<int,BranchLabelIndex>>();
        this->breakList = vector<pair<int,BranchLabelIndex>>();
    }
};

struct CodeGenScope {
    Exp* exp;
    string label;
    CodeGenBlock* block;

    CodeGenScope (Exp* exp, const string& label_name, const vector<pair<int,BranchLabelIndex>>& next_list, const vector<pair<int,BranchLabelIndex>>& break_list, const vector<pair<int,BranchLabelIndex>>& continue_list) {
        this->exp = nullptr;
        if (exp) {
            this->exp = new Exp(*exp);
        }
        this->block = new CodeGenBlock();
        this->block->continueList = vector<pair<int,BranchLabelIndex>>(continue_list);
        this->block->breakList = vector<pair<int,BranchLabelIndex>>(break_list);
        this->block->nextList = vector<pair<int,BranchLabelIndex>>(next_list);
        this->label = string(label_name);
    }
};

typedef union Node
{
    ArgumentList* argument_list;
	Argument* argument;
	Exp* exp;
    Id* id;
    int val;
    CodeGenLabel* label;
    CodeGenBlock* block;
    CodeGenScope* scope;
} STYPE;




#define YYSTYPE STYPE


#endif //COMPI__HW3_STRUCTS_HPP
