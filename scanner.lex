%{
#include "hw3_output.hpp"
#include "Structs.hpp"
#include "parser.tab.hpp"

%}

%option yylineno
%option noyywrap

%%
void                          return VOID;
int                           return INT;
byte                          return BYTE;
b                             return B;
bool                          return BOOL;
const                         return CONST;
and                           return AND;
or                            return OR;
not                           return NOT;
true                          return TRUE;
false                         return FALSE;
return                        return RETURN;
if                            return IF;
else                          return ELSE;
while                         return WHILE;
break                         return BREAK;
continue                      return CONTINUE;
;                             return SC;
,                             return COMMA;
\(                            return LPAREN;
\)                            return RPAREN;
\{                            return LBRACE;
\}                            return RBRACE;
=                             return ASSIGN;
!=|==                         return RELOP_EQUAL;
\>|\>=|\<|\<=                 return RELOP;
\+|\-                         return BINOP_ADD;
\/|\*                         return BINOP_MULTIPLY;
[a-zA-Z][a-zA-Z0-9]*          {
                                string name(yytext);
                                yylval.id = new Id(name);
                                return ID;
                              }

0|[1-9][0-9]*                 {
                                yylval.val = atoi(yytext);
                                return NUM;
                              }
\"([^\n\r\"\\]|\\[rnt"\\])+\" return STRING;
[\t\r\n ]                     ;
\/\/[^\r\n]*(\r|\n|\r\n)?     ;
.                           {output::errorLex(yylineno); exit(1);}

%%
