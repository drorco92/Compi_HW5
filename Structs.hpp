
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
    int val;
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
        this->type = /*string(type)*/type;
        this->val = exp.val;
    }
};

struct Id {
    std::string name;
    explicit Id(const std::string& name): name(name) {};
};



struct ArgumentList {
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

//struct TypeList {
//    TypeVector type_vector;
//    TypeList() = default;
//};

struct ExpList {
    vector<Exp*> exp;
    vector<string> typeList;

    ExpList() {
        this->m_exp = vector<Exp*>();
        this->m_typeList = vector<string>();
    }

    friend ostream& operator<<(ostream& os, const ExpList& sl) {
        for (auto & it : sl.m_typeList) {
            os << it << ", ";
        }
        os << endl;
        return os;
    }
};



struct Label {
    string label;
    explicit Label(const string& label_name) {
        this->label = string(label_name);
    }
};

struct Block {
    vector<pair<int,BranchLabelIndex>> nextList;
    vector<pair<int,BranchLabelIndex>> breakList;
    vector<pair<int,BranchLabelIndex>> continueList;
    Block() {
        this->nextList = vector<pair<int,BranchLabelIndex>>();
        this->breakList = vector<pair<int,BranchLabelIndex>>();
        this->continueList = vector<pair<int,BranchLabelIndex>>();
    }
};

struct Scope {
    Exp* exp;
    string label;
    Block* block;

    Scope (Exp* exp, const string& label_name, const vector<pair<int,BranchLabelIndex>>& next_list, const vector<pair<int,BranchLabelIndex>>& break_list, const vector<pair<int,BranchLabelIndex>>& continue_list) {
        if (exp) {
            this->exp = new Exp(*exp);
        } else {
            this->exp = nullptr;
        }
        this->label = string(label_name);
        this->block = new Block();
        this->block->nextList = vector<pair<int,BranchLabelIndex>>(next_list);
        this->block->breakList = vector<pair<int,BranchLabelIndex>>(break_list);
        this->block->continueList = vector<pair<int,BranchLabelIndex>>(continue_list);
    }
};





typedef union Node
{
    ArgumentList* argument_list;
	Argument* argument;
    ExpList* expList;
	Exp* exp;
    Id* id;
    int val;
    Label* label;
    Block* block;
    Scope* scope;
} STYPE;




#define YYSTYPE STYPE


#endif //COMPI__HW3_STRUCTS_HPP
