%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
extern int yylex();
extern ASTNode *root;
extern char str[200];

//main()
//{
//    yyparse();
//}

%}

%token START END PROGRAM VAR CONST INTEGER REAL BOOLEAN CHAR WHILE DO IF IDENTIFIER
%token THEN ELSE AND OR NOT ARRAY OF REALNUM INTNUM LE GE NE ASSIGN DOTDOT

%%
program_struct:     program_head ';' program_body '.'
                    {;};
program_head :      PROGRAM IDENTIFIER {ast_node_set_attr_str(program_node, "NAME", sdsnew(str));} ( idlist ) |
                    PROGRAM IDENTIFIER {ast_node_set_attr_str(program_node, "NAME", sdsnew(str));}
                    ;
idlist:             islist ',' IDENTIFIER | IDENTIFIER ;
program_body:       const_declarations |
                    var_declarations   |
                    subprogram_declarations |
                    compound_statement;
const_declarations: | CONST const_declaration ';' ;
const_declaration:  const_declaration ';' IDENTIFIER '=' const_value |
                    IDENTIFIER '=' const_value;
const_value:        '+'REALNUM | '-'REALNUM | REALNUM | '\'' letter'\'' // letter是啥。。。怎么还用单引号引起来了;
var_declarations:   | VAR var_declaration ';' ;
var_declaration:    var_declaration ';' idlist ':' type |
                    idlist ':' type;
type:               basic_type |
                    ARRAY [ period ] OF basic_type;
basic_type:         INTEGER | REAL | BOOLEAN | CHAR;
period:             period ',' INTNUM DOTDOT INTNUM |
                    INTNUM DOTDOT INTNUM;

//这部分先没看懂啥含义，注释符号可能有错误
//subprogram_declarations: subprogram_declarations subprogram ';' |
//subprogram: subprogram_head ';' subprogram_body
//subprogram_head: procedure id formal_parameter
//| function id formal_parameter : basic_type
//formal_parameter: ( parameter_list ) |
//parameter_list:parameter_list ';' parameter | parameter