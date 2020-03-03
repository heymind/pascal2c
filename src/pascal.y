
%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "sds.h"
extern int yylex();
extern ASTNode *root;

%}


%error-verbose

%start program_struct
%union {
    char* text;
    ASTNode *node;
}

%left  T_OR
%left  T_AND
%left  T_CEQ T_CNE
%left  T_CLT T_CGT T_CLE T_CGE
%left  T_PLUS T_MINUS
%left  T_MUL T_DIV T_MOD
%left  T_NOT

//This collection of tokens are related to the keywords reserved by Pascal such as begin, end, etc. More information pascal.l file
%token   T_PROGRAM T_VAR T_PROCEDURE T_FUNCTION T_BEGIN T_END
%token   T_IF T_THEN T_ELSE T_WHILE T_FOR T_TO T_DO
%token   T_ASSIGNOP T_OBRACKET T_CBRACKET T_SEMICOLON T_COLON T_COMMA T_DOT
%token   T_WRITE_INT T_WRITE_CHAR T_WRITE_BOOL T_WRITE_LN
%token   T_CONST T_READ T_WRITE T_EQ

//This tokens has special types because they are related to real information to use and they are not constants like IF or OR, they mean Regular Expressions
%token <text> T_BASIC_TYPE
%token <text> T_ID
%token <text> T_NUM


//This are the keywords that we're gonna use accross the grammar
%type <node> program_struct program_head program_body idlist const_declarations const_declaration const_value var_declarations var_declaration type subprogram_declarations subprogram subprogram_head formal_parameter parameter_list parameter var_parameter value_parameter subprogram_body compound_statement statement_list statement variable_list variable procedure_call else_part relop addop mulop expression_list expression simple_expression term factor


%%
program_struct:program_head T_SEMICOLON program_body T_DOT
    {
        ASTNode *node = ast_node_create_without_pos("PROGRAM_STRUCT");
        ast_node_attr_node_append(node,"PROGRAM_HEAD",$1);

        ast_node_attr_node_append(node,"PROGRAM_BODY",$3);

        root = node;
    }

program_head:T_PROGRAM T_ID T_OBRACKET idlist T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("PROGRAM_HEAD");

        ast_node_set_attr_str(node,"ID",$2);

        ast_node_attr_node_append(node,"IDLIST",$4);

        $$ = node;
    }
    |T_PROGRAM T_ID
    {
        ASTNode *node = ast_node_create_without_pos("PROGRAM_HEAD");

        ast_node_set_attr_str(node,"ID",$2);
        $$ = node;
    }

program_body:const_declarations var_declarations subprogram_declarations compound_statement
    {
        ASTNode *node = ast_node_create_without_pos("PROGRAM_BODY");
        ast_node_attr_node_append(node,"CONST_DECLARATIONS",$1);
        ast_node_attr_node_append(node,"VAR_DECLARATIONS",$2);
        ast_node_attr_node_append(node,"SUBPROGRAM_DECLARATIONS",$3);
        ast_node_attr_node_append(node,"COMPOUND_STATEMENT",$4);
        $$ = node;
    }

idlist:idlist T_COMMA T_ID
    {
        ASTNode *node = ast_node_create_without_pos("IDLIST");
        ast_node_attr_node_append(node,"IDLIST",$1);

        ast_node_set_attr_str(node,"ID",$3);
        $$ = node;
    }
    |T_ID
    {
        ASTNode *node = ast_node_create_without_pos("IDLIST");
        ast_node_set_attr_str(node,"ID",$1);
        $$ = node;
    }

const_declarations:T_CONST const_declaration T_SEMICOLON
    {
        ASTNode *node = ast_node_create_without_pos("CONST_DECLARATIONS");

        ast_node_attr_node_append(node,"CONST_DECLARATION",$2);

        $$ = node;
    }
    |{$$=NULL;}
const_declaration:const_declaration T_SEMICOLON T_ID T_EQ const_value
    {
        ASTNode *node = ast_node_create_without_pos("CONST_DECLARATION");
        ast_node_attr_node_append(node,"CONST_DECLARATION",$1);

        ast_node_set_attr_str(node,"ID",$3);

        ast_node_attr_node_append(node,"CONST_VALUE",$5);
        $$ = node;
    }
    |T_ID T_EQ const_value
    {
        ASTNode *node = ast_node_create_without_pos("CONST_DECLARATION");
        ast_node_set_attr_str(node,"ID",$1);

        ast_node_attr_node_append(node,"CONST_VALUE",$3);
        $$ = node;
    }

const_value:T_PLUS T_NUM
    {
        ASTNode *node = ast_node_create_without_pos("CONST_VALUE");

        ast_node_set_attr_str(node,"NUM",$2);
        $$ = node;
    }
    |T_MINUS T_NUM
    {
        ASTNode *node = ast_node_create_without_pos("CONST_VALUE");

        ast_node_set_attr_str(node,"NUM",$2);
        $$ = node;
    }
    |T_NUM
    {
        ASTNode *node = ast_node_create_without_pos("CONST_VALUE");
        ast_node_set_attr_str(node,"NUM",$1);
        $$ = node;
    }

var_declarations:T_VAR var_declaration T_SEMICOLON
    {
        ASTNode *node = ast_node_create_without_pos("VAR_DECLARATIONS");

        ast_node_attr_node_append(node,"VAR_DECLARATION",$2);

        $$ = node;
    }
    |{$$=NULL;}
var_declaration:var_declaration T_SEMICOLON idlist T_COLON type
    {
        ASTNode *node = ast_node_create_without_pos("VAR_DECLARATION");
        ast_node_attr_node_append(node,"VAR_DECLARATION",$1);

        ast_node_attr_node_append(node,"IDLIST",$3);

        ast_node_attr_node_append(node,"TYPE",$5);
        $$ = node;
    }
    |idlist T_COLON type
    {
        ASTNode *node = ast_node_create_without_pos("VAR_DECLARATION");
        ast_node_attr_node_append(node,"IDLIST",$1);

        ast_node_attr_node_append(node,"TYPE",$3);
        $$ = node;
    }

type:T_BASIC_TYPE
    {
        ASTNode *node = ast_node_create_without_pos("TYPE");
        ast_node_set_attr_str(node,"BASIC_TYPE",$1);
        $$ = node;
    }

subprogram_declarations:subprogram_declarations subprogram T_SEMICOLON
    {
        ASTNode *node = ast_node_create_without_pos("SUBPROGRAM_DECLARATIONS");
        ast_node_attr_node_append(node,"SUBPROGRAM_DECLARATIONS",$1);
        ast_node_attr_node_append(node,"SUBPROGRAM",$2);

        $$ = node;
    }
    |{$$=NULL;}
subprogram:subprogram_head T_SEMICOLON subprogram_body
    {
        ASTNode *node = ast_node_create_without_pos("SUBPROGRAM");
        ast_node_attr_node_append(node,"SUBPROGRAM_HEAD",$1);

        ast_node_attr_node_append(node,"SUBPROGRAM_BODY",$3);
        $$ = node;
    }

subprogram_head:T_PROCEDURE T_ID formal_parameter
    {
        ASTNode *node = ast_node_create_without_pos("SUBPROGRAM_HEAD");

        ast_node_set_attr_str(node,"ID",$2);
        ast_node_attr_node_append(node,"FORMAL_PARAMETER",$3);
        $$ = node;
    }
    |T_FUNCTION T_ID formal_parameter T_COLON T_BASIC_TYPE
    {
        ASTNode *node = ast_node_create_without_pos("SUBPROGRAM_HEAD");

        ast_node_set_attr_str(node,"ID",$2);
        ast_node_attr_node_append(node,"FORMAL_PARAMETER",$3);

        ast_node_set_attr_str(node,"BASIC_TYPE",$5);
        $$ = node;
    }

formal_parameter:T_OBRACKET parameter_list T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("FORMAL_PARAMETER");

        ast_node_attr_node_append(node,"PARAMETER_LIST",$2);

        $$ = node;
    }
    |{$$=NULL;}
parameter_list:parameter_list T_SEMICOLON parameter
    {
        ASTNode *node = ast_node_create_without_pos("PARAMETER_LIST");
        ast_node_attr_node_append(node,"PARAMETER_LIST",$1);

        ast_node_attr_node_append(node,"PARAMETER",$3);
        $$ = node;
    }
    |parameter
    {
        ASTNode *node = ast_node_create_without_pos("PARAMETER_LIST");
        ast_node_attr_node_append(node,"PARAMETER",$1);
        $$ = node;
    }

parameter:var_parameter
    {
        ASTNode *node = ast_node_create_without_pos("PARAMETER");
        ast_node_attr_node_append(node,"VAR_PARAMETER",$1);
        $$ = node;
    }
    |value_parameter
    {
        ASTNode *node = ast_node_create_without_pos("PARAMETER");
        ast_node_attr_node_append(node,"VALUE_PARAMETER",$1);
        $$ = node;
    }

var_parameter:T_VAR value_parameter
    {
        ASTNode *node = ast_node_create_without_pos("VAR_PARAMETER");

        ast_node_attr_node_append(node,"VALUE_PARAMETER",$2);
        $$ = node;
    }

value_parameter:idlist T_COLON T_BASIC_TYPE
    {
        ASTNode *node = ast_node_create_without_pos("VALUE_PARAMETER");
        ast_node_attr_node_append(node,"IDLIST",$1);

        ast_node_set_attr_str(node,"BASIC_TYPE",$3);
        $$ = node;
    }

subprogram_body:const_declarations var_declarations compound_statement
    {
        ASTNode *node = ast_node_create_without_pos("SUBPROGRAM_BODY");
        ast_node_attr_node_append(node,"CONST_DECLARATIONS",$1);
        ast_node_attr_node_append(node,"VAR_DECLARATIONS",$2);
        ast_node_attr_node_append(node,"COMPOUND_STATEMENT",$3);
        $$ = node;
    }

compound_statement:T_BEGIN statement_list T_END
    {
        ASTNode *node = ast_node_create_without_pos("COMPOUND_STATEMENT");

        ast_node_attr_node_append(node,"STATEMENT_LIST",$2);

        $$ = node;
    }

statement_list:statement_list T_SEMICOLON statement
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT_LIST");
        ast_node_attr_node_append(node,"STATEMENT_LIST",$1);

        ast_node_attr_node_append(node,"STATEMENT",$3);
        $$ = node;
    }
    |statement
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT_LIST");
        ast_node_attr_node_append(node,"STATEMENT",$1);
        $$ = node;
    }

statement:variable T_ASSIGNOP expression
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");
        ast_node_attr_node_append(node,"VARIABLE",$1);

        ast_node_attr_node_append(node,"EXPRESSION",$3);
        $$ = node;
    }
    |procedure_call
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");
        ast_node_attr_node_append(node,"PROCEDURE_CALL",$1);
        $$ = node;
    }
    |compound_statement
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");
        ast_node_attr_node_append(node,"COMPOUND_STATEMENT",$1);
        $$ = node;
    }
    |T_IF expression T_THEN statement else_part
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");

        ast_node_attr_node_append(node,"EXPRESSION",$2);

        ast_node_attr_node_append(node,"STATEMENT",$4);
        ast_node_attr_node_append(node,"ELSE_PART",$5);
        $$ = node;
    }
    |T_FOR T_ID T_ASSIGNOP expression T_TO expression T_DO statement
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");

        ast_node_set_attr_str(node,"ID",$2);

        ast_node_attr_node_append(node,"EXPRESSION",$4);

        ast_node_attr_node_append(node,"EXPRESSION",$6);

        ast_node_attr_node_append(node,"STATEMENT",$8);
        $$ = node;
    }
    |T_READ T_OBRACKET variable_list T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");


        ast_node_attr_node_append(node,"VARIABLE_LIST",$3);

        $$ = node;
    }
    |T_WRITE T_OBRACKET expression_list T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT");


        ast_node_attr_node_append(node,"EXPRESSION_LIST",$3);

        $$ = node;
    }
    |{$$=NULL;}
variable_list:variable_list T_COMMA variable
    {
        ASTNode *node = ast_node_create_without_pos("VARIABLE_LIST");
        ast_node_attr_node_append(node,"VARIABLE_LIST",$1);

        ast_node_attr_node_append(node,"VARIABLE",$3);
        $$ = node;
    }
    |variable
    {
        ASTNode *node = ast_node_create_without_pos("VARIABLE_LIST");
        ast_node_attr_node_append(node,"VARIABLE",$1);
        $$ = node;
    }

variable:T_ID
    {
        ASTNode *node = ast_node_create_without_pos("VARIABLE");
        ast_node_set_attr_str(node,"ID",$1);
        $$ = node;
    }

procedure_call:T_ID
    {
        ASTNode *node = ast_node_create_without_pos("PROCEDURE_CALL");
        ast_node_set_attr_str(node,"ID",$1);
        $$ = node;
    }
    |T_ID T_OBRACKET expression_list T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("PROCEDURE_CALL");
        ast_node_set_attr_str(node,"ID",$1);

        ast_node_attr_node_append(node,"EXPRESSION_LIST",$3);

        $$ = node;
    }

else_part:T_ELSE statement
    {
        ASTNode *node = ast_node_create_without_pos("ELSE_PART");

        ast_node_attr_node_append(node,"STATEMENT",$2);
        $$ = node;
    }
    |{$$=NULL;}
relop:T_CGT
    {
        ASTNode *node = ast_node_create_without_pos("RELOP");

        $$ = node;
    }
    |T_CLT
    {
        ASTNode *node = ast_node_create_without_pos("RELOP");

        $$ = node;
    }
    |T_CEQ
    {
        ASTNode *node = ast_node_create_without_pos("RELOP");

        $$ = node;
    }
    |T_CNE
    {
        ASTNode *node = ast_node_create_without_pos("RELOP");

        $$ = node;
    }
    |T_CGE
    {
        ASTNode *node = ast_node_create_without_pos("RELOP");

        $$ = node;
    }
    |T_CLE
    {
        ASTNode *node = ast_node_create_without_pos("RELOP");

        $$ = node;
    }

addop:T_PLUS
    {
        ASTNode *node = ast_node_create_without_pos("ADDOP");

        $$ = node;
    }
    |T_MINUS
    {
        ASTNode *node = ast_node_create_without_pos("ADDOP");

        $$ = node;
    }
    |T_OR
    {
        ASTNode *node = ast_node_create_without_pos("ADDOP");

        $$ = node;
    }

mulop:T_AND
    {
        ASTNode *node = ast_node_create_without_pos("MULOP");

        $$ = node;
    }
    |T_MOD
    {
        ASTNode *node = ast_node_create_without_pos("MULOP");

        $$ = node;
    }
    |T_DIV
    {
        ASTNode *node = ast_node_create_without_pos("MULOP");

        $$ = node;
    }
    |T_MUL
    {
        ASTNode *node = ast_node_create_without_pos("MULOP");

        $$ = node;
    }
    |{$$=NULL;}
expression_list:expression_list T_COMMA expression
    {
        ASTNode *node = ast_node_create_without_pos("EXPRESSION_LIST");
        ast_node_attr_node_append(node,"EXPRESSION_LIST",$1);

        ast_node_attr_node_append(node,"EXPRESSION",$3);
        $$ = node;
    }
    |expression
    {
        ASTNode *node = ast_node_create_without_pos("EXPRESSION_LIST");
        ast_node_attr_node_append(node,"EXPRESSION",$1);
        $$ = node;
    }

expression:simple_expression relop simple_expression
    {
        ASTNode *node = ast_node_create_without_pos("EXPRESSION");
        ast_node_attr_node_append(node,"SIMPLE_EXPRESSION",$1);
        ast_node_attr_node_append(node,"RELOP",$2);
        ast_node_attr_node_append(node,"SIMPLE_EXPRESSION",$3);
        $$ = node;
    }
    |simple_expression
    {
        ASTNode *node = ast_node_create_without_pos("EXPRESSION");
        ast_node_attr_node_append(node,"SIMPLE_EXPRESSION",$1);
        $$ = node;
    }

simple_expression:simple_expression addop term
    {
        ASTNode *node = ast_node_create_without_pos("SIMPLE_EXPRESSION");
        ast_node_attr_node_append(node,"SIMPLE_EXPRESSION",$1);
        ast_node_attr_node_append(node,"ADDOP",$2);
        ast_node_attr_node_append(node,"TERM",$3);
        $$ = node;
    }
    |term
    {
        ASTNode *node = ast_node_create_without_pos("SIMPLE_EXPRESSION");
        ast_node_attr_node_append(node,"TERM",$1);
        $$ = node;
    }

term:term mulop factor
    {
        ASTNode *node = ast_node_create_without_pos("TERM");
        ast_node_attr_node_append(node,"TERM",$1);
        ast_node_attr_node_append(node,"MULOP",$2);
        ast_node_attr_node_append(node,"FACTOR",$3);
        $$ = node;
    }
    |factor
    {
        ASTNode *node = ast_node_create_without_pos("TERM");
        ast_node_attr_node_append(node,"FACTOR",$1);
        $$ = node;
    }

factor:T_NUM
    {
        ASTNode *node = ast_node_create_without_pos("FACTOR");
        ast_node_set_attr_str(node,"NUM",$1);
        $$ = node;
    }
    |variable
    {
        ASTNode *node = ast_node_create_without_pos("FACTOR");
        ast_node_attr_node_append(node,"VARIABLE",$1);
        $$ = node;
    }
    |T_ID T_OBRACKET expression_list T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("FACTOR");
        ast_node_set_attr_str(node,"ID",$1);

        ast_node_attr_node_append(node,"EXPRESSION_LIST",$3);

        $$ = node;
    }
    |T_OBRACKET expression T_CBRACKET
    {
        ASTNode *node = ast_node_create_without_pos("FACTOR");

        ast_node_attr_node_append(node,"EXPRESSION",$2);

        $$ = node;
    }
    |T_NOT factor
    {
        ASTNode *node = ast_node_create_without_pos("FACTOR");

        ast_node_attr_node_append(node,"FACTOR",$2);
        $$ = node;
    }



%%