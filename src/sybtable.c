//
// Created by Jinyuan Zhang on 2020/5/10.
//

#include "sybtable.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LEN sizeof(struct symbol)
struct symbol* head= NULL;
struct symbol* end = NULL;

void do_ge_table(ASTNode *node, FILE *out){
    if (node == NULL) return;

    //create symbolList
    struct symbol *table = NULL;

    char *type = node->type;

    if (strcmp(type, "PROGRAM_STRUCT") == 0) {
        fprintf(out, "program_struct\n");
        do_ge_table(ast_node_get_attr_node_value(node, "VAR_DECLARATIONS"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "CONST_DECLARATIONS"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "PROGRAM_HEAD"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS"), out);
    }
    else if (strcmp(type, "VAR_DECLARATION") == 0) {
        do_ge_table(ast_node_get_attr_node_value(node, "VAR_DECLARATION"), out);
        ASTNode *type_node = ast_node_get_attr_node_value(node, "TYPE");
        char *var_type = ast_node_get_attr_str_value(type_node,"BASIC_TYPE");
        int dimension = 0;
        do_ge_table(type_node, out);
        do_ge_table(ast_node_get_attr_node_value(type_node, "PERIOD_LIST"), out);
        ASTNode *p_node = ast_node_get_attr_node_value(node, "PERIOD_LIST");
        if (p_node !=  NULL) {
            for (ASTNodeAttr *cur = p_node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *period_node = (ASTNode *) cur->value;
            dimension++;
//            char *period_begin = ast_node_get_attr_str_value(period_node,"PERIOD_BEGIN");
//            char *period_end = ast_node_get_attr_str_value(period_node,"PERIOD_END");
//            fprintf(out,"[%d]",atoi(period_end) - atoi(period_begin)+1);
            }
        }
        fprintf(out, "%d\n", dimension);
        do_ge_table(ast_node_get_attr_node_value(node, "IDLIST"), out);
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
            fprintf(out, "%s %s %d\n", var_type, cur->value,dimension,table);
            addSymbol(var_type,cur->value,dimension,table);
//            if (cur != NULL && cur->next != NULL) fprintf(out, ", ");
        }

    }

    else if (strcmp(type, "SUBPROGRAM_DECLARATIONS") == 0) {
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_DECLARATIONS"),out);
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM"),out);
    } else if (strcmp(type, "SUBPROGRAM") == 0) {
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_HEAD"),out);
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_BODY"),out);
    }
}

void addSymbol(char *name,enum table_type Ttype,int dimension, struct symbol *table)
{

}

void getSymbol(char *name, struct symbol *table)
{

}

void ge_syb_table(ASTNode *root, FILE *out) {
    assert(root != NULL && "ROOT SHOULD NOT BE NULL");
    do_ge_table(root,out);
}