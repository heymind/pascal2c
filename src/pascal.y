
%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "sds.h"
extern int yylex();
extern ASTNodePos *ast_node_pos_create(int start_row, int start_column, int end_row, int end_column);
extern unsigned int lex_column_num;
extern unsigned int lex_row_num;
extern unsigned int yyleng;
extern ASTNode *root;
%}

%error-verbose

%start program_struct
%union {
    char* text;
    ASTNode *node;
    ASTNodeAttr *attr;
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
%token   T_ASSIGNOP T_OBRACKET T_CBRACKET T_SOBRACKET T_SCBRACKET T_SEMICOLON T_COLON T_COMMA T_DOT
%token   T_CONST T_READ T_WRITE T_ARRAY T_OF

//This tokens has special types because they are related to real information to use and they are not constants like IF or OR, they mean Regular Expressions
%token <text> T_BASIC_TYPE
%token <text> T_ID
%token <text> T_NUM
%token <text> T_REAL


//This are the keywords that we're gonna use accross the grammar
%type <node> program_struct program_head program_body idlist const_declarations const_declaration const_value var_declarations var_declaration type subprogram_declarations subprogram subprogram_head formal_parameter parameter_list parameter var_parameter value_parameter subprogram_body compound_statement statement_list statement variable_list variable procedure_call else_part relop addop mulop expression_list expression simple_expression term factor period_list period
%type <attr> idlist_ parameter_list_ statement_list_ variable_list_ expression_list_ period_list_

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
        //T_ID不append ?
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

idlist: T_ID idlist_
    {
        ASTNode *node = ast_node_create_without_pos("IDLIST");
        ast_node_set_attr_str(node,"ID",$1);
        ast_node_attr_append(node->first_attr, $2);
        $$ = node;
    }

idlist_:T_COMMA T_ID idlist_
    {
	ASTNodeAttr *attr = ast_node_attr_create_str("ID", $2);
        ast_node_attr_append(attr, $3);
        $$ = attr;
    }
    |{
        $$ = NULL;
    }

const_declarations:T_CONST const_declaration T_SEMICOLON
    {
        ASTNode *node = ast_node_create_without_pos("CONST_DECLARATIONS");

        ast_node_attr_node_append(node,"CONST_DECLARATION",$2);

        $$ = node;
    }
    |{
        $$=NULL; 
    }
const_declaration:const_declaration T_SEMICOLON T_ID T_CEQ const_value
    {
        ASTNode *node = ast_node_create_without_pos("CONST_DECLARATION");
        ast_node_attr_node_append(node,"CONST_DECLARATION",$1);

        ast_node_set_attr_str(node,"ID",$3);

        ast_node_attr_node_append(node,"CONST_VALUE",$5);
        $$ = node;
    }
    |T_ID T_CEQ const_value
    {
        ASTNode *node = ast_node_create_without_pos("CONST_DECLARATION");
        ast_node_set_attr_str(node,"ID",$1);

        ast_node_attr_node_append(node,"CONST_VALUE",$3);
        $$ = node;
    }

const_value:T_PLUS T_REAL
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("CONST_VALUE",pos);
        ast_node_set_attr_str(node,"REAL",$2);
        $$ = node;
    }
    |T_MINUS T_REAL
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("CONST_VALUE",pos);
        ast_node_set_attr_str(node,"REAL",$2);
        $$ = node;
    }
    |T_REAL
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("CONST_VALUE",pos);
        ast_node_set_attr_str(node,"REAL",$1);
        $$ = node;
    }

var_declarations:T_VAR var_declaration T_SEMICOLON
    {
        ASTNode *node = ast_node_create_without_pos("VAR_DECLARATIONS");

        ast_node_attr_node_append(node,"VAR_DECLARATION",$2);

        $$ = node;
    }
    |{
        $$=NULL; //?
    }
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
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("TYPE",pos);
        ast_node_set_attr_str(node,"BASIC_TYPE",$1);
        $$ = node;
    }
    | T_ARRAY T_SOBRACKET period_list T_SCBRACKET T_OF T_BASIC_TYPE
    {
	ASTNode *node = ast_node_create_without_pos("ARRAY");
	ast_node_set_attr_str(node,"BASIC_TYPE",$6);
        ast_node_attr_node_append(node,"PERIOD_LIST",$3);
        $$ = node;
    }

period_list: period period_list_
    {
	    ASTNode *node = ast_node_create_without_pos("PERIOD_LIST");
        ASTNodeAttr *attr = ast_node_attr_create_node("PERIOD",$1);
        node->first_attr = attr;
        ast_node_attr_append(node->first_attr,$2);
        $$ = node;
    }

period_list_: T_COMMA period period_list_
    {
	    ASTNodeAttr *attr = ast_node_attr_create_node("PERIOD",$2);
        ast_node_attr_append(attr, $3);
        $$ = attr;
    }
    |{$$=NULL;}

period: T_NUM T_DOT T_DOT T_NUM
    {
    	ASTNodeAttr *attr1 = ast_node_attr_create_str("PERIOD_BEGIN",$1);
    	ASTNodeAttr *attr2 = ast_node_attr_create_str("PERIOD_END",$4);
    	ASTNode *node = ast_node_create_without_pos("PERIOD");
    	node->first_attr = attr1;
    	ast_node_attr_append(node->first_attr, attr2);
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

parameter_list:parameter parameter_list_
    {
        ASTNode *node = ast_node_create_without_pos("PARAMETER_LIST");
        ASTNodeAttr *attr = ast_node_attr_create_node("PARAMETER",$1);
        node->first_attr = attr;
        ast_node_attr_append(node->first_attr,$2);
        $$ = node;
    }

parameter_list_:T_SEMICOLON parameter parameter_list_
    {
	ASTNodeAttr *attr = ast_node_attr_create_node("PARAMETER",$2);
        ast_node_attr_append(attr, $3);
        $$ = attr;
    }
    |{$$=NULL;}

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

statement_list:statement statement_list_
    {
        ASTNode *node = ast_node_create_without_pos("STATEMENT_LIST");
        ASTNodeAttr *attr = ast_node_attr_create_node("STATEMENT",$1);
        node->first_attr = attr;
        ast_node_attr_append(node->first_attr,$2);
        $$ = node;
    }

statement_list_:T_SEMICOLON statement statement_list_
    {
        ASTNodeAttr *attr = ast_node_attr_create_node("STATEMENT",$2);
        ast_node_attr_append(attr, $3);
        $$ = attr;
    }
    |{$$=NULL;}

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

variable_list:variable variable_list_
    {
        ASTNode *node = ast_node_create_without_pos("VARIABLE_LIST");
        ASTNodeAttr *attr = ast_node_attr_create_node("VARIABLE",$1);
        node->first_attr = attr;
        ast_node_attr_append(node->first_attr,$2);
        $$ = node;
    }

variable_list_:T_COMMA variable variable_list_
    {
        ASTNodeAttr *attr = ast_node_attr_create_node("VARIABLE",$2);
        ast_node_attr_append(attr, $3);
        $$ = attr;
    }
    |{$$=NULL;}


variable:T_ID
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("VARIABLE",pos);
        ast_node_set_attr_str(node,"ID",$1);
        $$ = node;
    }

procedure_call:T_ID
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("PROCEDURE_CALL",pos);
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
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("RELOP",pos);
        
        $$ = node;
    }
    |T_CLT
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("RELOP",pos);
        
        $$ = node;
    }
    |T_CEQ
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("RELOP",pos);

        $$ = node;
    }
    |T_CNE
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("RELOP",pos);

        $$ = node;
    }
    |T_CGE
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("RELOP",pos);

        $$ = node;
    }
    |T_CLE
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("RELOP",pos);

        $$ = node;
    }

addop:T_PLUS
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("ADDOP",pos);

        $$ = node;
    }
    |T_MINUS
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("ADDOP",pos);

        $$ = node;
    }
    |T_OR
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("ADDOP",pos);

        $$ = node;
    }

mulop:T_AND
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("MULOP",pos);

        $$ = node;
    }
    |T_MOD
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("MULOP",pos);

        $$ = node;
    }
    |T_DIV
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("MULOP",pos);

        $$ = node;
    }
    |T_MUL
    {
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("MULOP",pos);

        $$ = node;
    }
    |{$$=NULL;}

expression_list:expression expression_list_
    {
        ASTNode *node = ast_node_create_without_pos("EXPRESSION_LIST");
        ASTNodeAttr *attr = ast_node_attr_create_node("EXPRESSION",$1);
        node->first_attr = attr;
        ast_node_attr_append(node->first_attr,$2);
        $$ = node;
    }

expression_list_:T_COMMA expression expression_list_
    {
        ASTNodeAttr *attr = ast_node_attr_create_node("EXPRESSION",$2);
        ast_node_attr_append(attr, $3);
        $$ = attr;
    }
    |{$$=NULL;}


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
        ASTNodePos * pos = ast_node_pos_create(lex_row_num, lex_column_num-yyleng, lex_row_num, lex_column_num);
        ASTNode *node = ast_node_create("FACTOR",pos);
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