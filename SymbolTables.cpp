
#include "SymbolTables.hpp"
#include "hw3_output.hpp"
#include "Structs.hpp"

extern int yylineno;



//======================================================================================
Variable::Variable(string name, int offset, string type, string type_annotation) : Symbol(name, offset, type), type_annotation(type_annotation) {}
void Variable::print() {output::printID(name, offset, type);}

//======================================================================================
Function::Function(string name, string return_type, ArgVector& args) : Symbol(name, 0, "FUNCTION"), return_type(return_type), args(args){}

Function::~Function() {}

void Function::print() {output::printID(name, offset, output::makeFunctionType(this->return_type, this->args));}

Symbol::Symbol(string name, int offset, string type) : name(name), offset(offset), type(type) {};
void Symbol::print() {}
//======================================================================================

TablesList::TablesList() {
    this->offsets = stack<int>();
    this->tables = TableVector();
    this->OpenGlobal();
	ArgVector print_args = ArgVector();
	print_args.push_back(Argument("","arguments", "STRING"));
    AddSymbol("print", "VOID", print_args);
    ArgVector printi_args = ArgVector();
    printi_args.push_back(Argument("","arguments", "INT"));
    AddSymbol("printi", "VOID", printi_args);
}



Symbol* TablesList::GetSymbol(const string& name, bool is_func) {
    for (TableVector::reverse_iterator table_it = this->tables.rbegin(); table_it != this->tables.rend(); table_it++) {
        for (SymbolsVector::reverse_iterator symbol_it = (*table_it)->symbols.rbegin(); symbol_it != (*table_it)->symbols.rend(); symbol_it++) {
			if(is_func) {
                if ((*symbol_it)->name == name && (*symbol_it)->type == "FUNCTION") {
					try {
						Function* f = static_cast<Function*> (*symbol_it);
						return (f);
					}
					catch(...) {
					}
                }
            }
            else {
                if ((*symbol_it)->name == name && (*symbol_it)->type != "FUNCTION") {
                    return (*(symbol_it));
                }
            }
        }
    }
    return nullptr;
}



void TablesList::OpenGlobal() {
    Symbol_Table* first_table = new Symbol_Table(nullptr);
    this->tables.push_back(first_table);
    this->offsets.push(0);
	OpenScope();
}

void TablesList::OpenScope() {
    Symbol_Table* new_table = new Symbol_Table(this->tables.back());
    this->tables.push_back(new_table);
    this->offsets.push(this->offsets.top());
}

void TablesList::CloseScope() {
    output::endScope();
    for (Symbol* symbol : this->tables.back()->symbols) {
        symbol->print();
    }
    delete this->tables.back();
    this->tables.pop_back();
    this->offsets.pop();
}

void TablesList::CloseGlobal() {
    Symbol* main_symbol = GetSymbol("main", true);
    Function* main_function_symbol = (Function*) main_symbol;

    if(main_symbol == nullptr || main_function_symbol->return_type != "VOID"
       || !main_function_symbol->args.empty()) {
        output::errorMainMissing();
        exit(1);
    }
    CloseScope();
}



void TablesList::AddSymbol(const string& annotation, const string& name, const string& type) {
    IsNameExists(name, false);
    Variable* variable_symbol = new Variable(name, this->offsets.top(), type, annotation );
    this->tables.back()->symbols.push_back(variable_symbol);
    this->offsets.top() += 1;
}

void TablesList::AddSymbol(const string& name, const string& return_type, ArgVector& args) {
    IsNameExists(name, true);

    ArgVector args_to_push = ArgVector();

    for(ArgVector::reverse_iterator arg_it = args.rbegin(); arg_it != args.rend(); arg_it++) {
        IsNameExists(arg_it->name, false);
        args_to_push.push_back(*arg_it);
    }
    Function* function_symbol = new Function(name, return_type, args_to_push);
    this->tables.back()->symbols.push_back(function_symbol);
}

void TablesList::AddArgsSymbols(ArgVector& args) {
    int args_offset = -1;

    for(ArgVector::reverse_iterator arg_it = args.rbegin(); arg_it != args.rend(); arg_it++) {
        IsNameExists(arg_it->name, false);
        Variable* symbol = new Variable(arg_it->name, args_offset, arg_it->type, "");
        this->tables.back()->symbols.push_back(symbol);
        args_offset -= 1;
    }
	
}

void TablesList::IsNameExists(const string& name, bool is_func) {
    if(GetSymbol(name, !is_func) != nullptr) {
        output::errorDef(yylineno ,name);
        exit(1);
    }
	if(GetSymbol(name, is_func) != nullptr) {
        output::errorDef(yylineno ,name);
        exit(1);
    }
}