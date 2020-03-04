

#include "codegen.h"
#include <string.h>
#include <assert.h>


void do_generate(ASTNode *node, FILE *out) {
    if (node == NULL) return;
    char *type = node->type;
    if (strcmp(type, "PROGRAM_STRUCT") == 0) {
        fprintf(out,"#include <stdio.h>\n");
        do_generate(ast_node_get_attr_node_value(node,"PROGRAM_HEAD"),out);
        do_generate(ast_node_get_attr_node_value(node,"PROGRAM_BODY"),out);
    } else if (strcmp(type, "PROGRAM_HEAD") == 0) {
        fprintf(out,"void %s(",ast_node_get_attr_str_value(node,"ID"));
        node = ast_node_get_attr_node_value(node,"IDLIST");
        while (node != NULL){
            fprintf(out,"FILE * %s",ast_node_get_attr_str_value(node,"ID"));
            node = ast_node_get_attr_node_value(node,"IDLIST");
            if(node != NULL) fprintf(out,",");
        }
        fprintf(out,")");
    } else if (strcmp(type, "PROGRAM_BODY") == 0) {
        printf("TODO:TRANSLATE `PROGRAM_BODY`\n000");
    } else if (strcmp(type, "IDLIST") == 0) {
        printf("TODO:TRANSLATE `IDLIST`\n");
    } else if (strcmp(type, "CONST_DECLARATIONS") == 0) {
        printf("TODO:TRANSLATE `CONST_DECLARATIONS`\n");
    } else if (strcmp(type, "CONST_DECLARATION") == 0) {
        printf("TODO:TRANSLATE `CONST_DECLARATION`\n");
    } else if (strcmp(type, "CONST_VALUE") == 0) {
        printf("TODO:TRANSLATE `CONST_VALUE`\n");
    } else if (strcmp(type, "VAR_DECLARATIONS") == 0) {
        printf("TODO:TRANSLATE `VAR_DECLARATIONS`\n");
    } else if (strcmp(type, "VAR_DECLARATION") == 0) {
        printf("TODO:TRANSLATE `VAR_DECLARATION`\n");
    } else if (strcmp(type, "TYPE") == 0) {
        printf("TODO:TRANSLATE `TYPE`\n");
    } else if (strcmp(type, "SUBPROGRAM_DECLARATIONS") == 0) {
        printf("TODO:TRANSLATE `SUBPROGRAM_DECLARATIONS`\n");
    } else if (strcmp(type, "SUBPROGRAM") == 0) {
        printf("TODO:TRANSLATE `SUBPROGRAM`\n");
    } else if (strcmp(type, "SUBPROGRAM_HEAD") == 0) {
        printf("TODO:TRANSLATE `SUBPROGRAM_HEAD`\n");
    } else if (strcmp(type, "FORMAL_PARAMETER") == 0) {
        printf("TODO:TRANSLATE `FORMAL_PARAMETER`\n");
    } else if (strcmp(type, "PARAMETER_LIST") == 0) {
        printf("TODO:TRANSLATE `PARAMETER_LIST`\n");
    } else if (strcmp(type, "PARAMETER") == 0) {
        printf("TODO:TRANSLATE `PARAMETER`\n");
    } else if (strcmp(type, "VAR_PARAMETER") == 0) {
        printf("TODO:TRANSLATE `VAR_PARAMETER`\n");
    } else if (strcmp(type, "VALUE_PARAMETER") == 0) {
        printf("TODO:TRANSLATE `VALUE_PARAMETER`\n");
    } else if (strcmp(type, "SUBPROGRAM_BODY") == 0) {
        printf("TODO:TRANSLATE `SUBPROGRAM_BODY`\n");
    } else if (strcmp(type, "COMPOUND_STATEMENT") == 0) {
        printf("TODO:TRANSLATE `COMPOUND_STATEMENT`\n");
    } else if (strcmp(type, "STATEMENT_LIST") == 0) {
        printf("TODO:TRANSLATE `STATEMENT_LIST`\n");
    } else if (strcmp(type, "STATEMENT") == 0) {
        printf("TODO:TRANSLATE `STATEMENT`\n");
    } else if (strcmp(type, "VARIABLE_LIST") == 0) {
        printf("TODO:TRANSLATE `VARIABLE_LIST`\n");
    } else if (strcmp(type, "VARIABLE") == 0) {
        printf("TODO:TRANSLATE `VARIABLE`\n");
    } else if (strcmp(type, "PROCEDURE_CALL") == 0) {
        printf("TODO:TRANSLATE `PROCEDURE_CALL`\n");
    } else if (strcmp(type, "ELSE_PART") == 0) {
        printf("TODO:TRANSLATE `ELSE_PART`\n");
    } else if (strcmp(type, "RELOP") == 0) {
        printf("TODO:TRANSLATE `RELOP`\n");
    } else if (strcmp(type, "ADDOP") == 0) {
        printf("TODO:TRANSLATE `ADDOP`\n");
    } else if (strcmp(type, "MULOP") == 0) {
        printf("TODO:TRANSLATE `MULOP`\n");
    } else if (strcmp(type, "EXPRESSION_LIST") == 0) {
        printf("TODO:TRANSLATE `EXPRESSION_LIST`\n");
    } else if (strcmp(type, "EXPRESSION") == 0) {
        printf("TODO:TRANSLATE `EXPRESSION`\n");
    } else if (strcmp(type, "SIMPLE_EXPRESSION") == 0) {
        printf("TODO:TRANSLATE `SIMPLE_EXPRESSION`\n");
    } else if (strcmp(type, "TERM") == 0) {
        printf("TODO:TRANSLATE `TERM`\n");
    } else if (strcmp(type, "FACTOR") == 0) {
        printf("TODO:TRANSLATE `FACTOR`\n");
    } else {
        printf("UNKNOWN TYPE:%s\n", type);
    }
}
void generate_c_code(ASTNode *root, FILE *out) {
    assert(root != NULL && "ROOT SHOULD NOT BE NULL");
    do_generate(root,out);
}
