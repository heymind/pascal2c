//
// Created by Jinyuan Zhang on 2020/5/10.
//

#include "sybtable.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stdio.h"

#define LEN sizeof(struct symbol)
struct symbol* head= NULL;
struct symbol* end = NULL;

void do_ge_table(ASTNode *node, FILE *out){
    printf("get table\n");
    if (node == NULL) return;

    //create symbolList

    char *type = node->type;

    if (strcmp(type, "PROGRAM_STRUCT") == 0) {
        printf("program_struct\n");
        do_ge_table(ast_node_get_attr_node_value(node, "VAR_DECLARATIONS"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "CONST_DECLARATIONS"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "PROGRAM_HEAD"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT"), out);
        do_ge_table(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS"), out);
    }
    else if (strcmp(type, "CONST_DECLARATIONS") == 0) {
        printf("CONST_DECLARATIONS\n");
    } else if (strcmp(type, "CONST_DECLARATION") == 0) {
        printf("CONST_DECLARATION\n");
    } else if (strcmp(type, "CONST_VALUE") == 0) {
        printf("CONST_VALUE\n");
    }else if (strcmp(type, "VAR_DECLARATIONS") == 0) {
        printf("VAR_DECLARATIONS\n");
        do_ge_table(ast_node_get_attr_node_value(node,"VAR_DECLARATION"),out);
    } else if (strcmp(type, "VAR_DECLARATION") == 0) {
        printf("VAR_DECLARATION\n");
        do_ge_table(ast_node_get_attr_node_value(node, "VAR_DECLARATION"), out);
        ASTNode *type_node = ast_node_get_attr_node_value(node, "TYPE");
        char *var_type = ast_node_get_attr_str_value(type_node,"BASIC_TYPE");
        int dimension = 0;
//        fprintf(out, "%s\n",var_type);
        do_ge_table(type_node, out);
        do_ge_table(ast_node_get_attr_node_value(type_node, "PERIOD_LIST"), out);
        ASTNode *p_node = ast_node_get_attr_node_value(node, "PERIOD_LIST");
        if (p_node !=  NULL) {
            printf("PERIOD_LIST\n");

            for (ASTNodeAttr *cur = p_node->first_attr; cur; (cur) = (cur)->next) {
//            ASTNode *period_node = (ASTNode *) cur->value;
//            dimension++;
//            char *period_begin = ast_node_get_attr_str_value(period_node,"PERIOD_BEGIN");
//            char *period_end = ast_node_get_attr_str_value(period_node,"PERIOD_END");
//            fprintf(out,"[%d]",atoi(period_end) - atoi(period_begin)+1);
            }
        }
        printf("%d\n", dimension);
        do_ge_table(ast_node_get_attr_node_value(node, "IDLIST"), out);
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
            printf("%s %s %d\n", var_type, cur->value,dimension);
//            addSymbol(var_type,cur->value,dimension);
//            if (cur != NULL && cur->next != NULL) fprintf(out, ", ");
        }

    }

    else if (strcmp(type, "SUBPROGRAM_DECLARATIONS") == 0) {
        fprintf(out, "SUBPROGRAM_DECLARATIONS\n");
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_DECLARATIONS"),out);
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM"),out);
    } else if (strcmp(type, "SUBPROGRAM") == 0) {
        fprintf(out, "SUBPROGRAM\n");
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_HEAD"),out);
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_BODY"),out);
    } else if (strcmp(type, "SUBPROGRAM_HEAD") == 0) {
        fprintf(out, "SUBPROGRAM_HEAD\n");
        char *func_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        fprintf(out, "%s(", ast_node_get_attr_str_value(node, "ID"));
        do_ge_table(ast_node_get_attr_node_value(node, "FORMAL_PARAMETER"), out);
        fprintf(out, ")");
    } else if (strcmp(type, "FORMAL_PARAMETER") == 0) {
        fprintf(out, "FORMAL_PARAMETER\n");
        do_ge_table(ast_node_get_attr_node_value(node,"PARAMETER_LIST"),out);
    } else if (strcmp(type, "PARAMETER_LIST") == 0) {
        fprintf(out, "PARAMETER_LIST\n");
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_ge_table(expression_node,out);
        }
    } else if (strcmp(type, "PARAMETER") == 0) {
        printf("PARAMETER\n");
        do_ge_table(ast_node_get_attr_node_value(node,"VAL_PARAMETER"),out);
        do_ge_table(ast_node_get_attr_node_value(node,"VALUE_PARAMETER"),out);
    } else if (strcmp(type, "VAR_PARAMETER") == 0) {
        printf( "VAR_PARAMETER\n");
        printf("TODO:TRANSLATE `VAR_PARAMETER`\n");
    } else if (strcmp(type, "VALUE_PARAMETER") == 0) {
        printf("VALUE_PARAMETER\n");
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
            printf("%s %s", var_type, cur->value);
            if (cur != NULL && cur->next != NULL) printf(", ");
        }
<<<<<<< Updated upstream
=======
    } else if (strcmp(type, "SUBPROGRAM_BODY") == 0) {
        do_ge_table(ast_node_get_attr_node_value(node, "CONST_DECLARATIONS"));
        do_ge_table(ast_node_get_attr_node_value(node, "VAR_DECLARATIONS"));
        do_ge_table(ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT"));
    }
    else {
//        printf("else:%s\n", type);
>>>>>>> Stashed changes
    }
}

void add_symbol(char *name,enum symbol_type type,int dimension,char *scope,struct symbol *table)
{
    struct symbol* temp = (struct symbol*)malloc(LEN);

    temp->name = name;
    temp->type = type;
    temp->dimension = dimension;
    temp->scope = scope;
    temp->next = NULL;

    if (head == NULL)
    {
        head = temp;
    } else
    {
        end->next = temp;
    }
    end = temp;
}
;
struct symbol* get_symbol(char *name, char *scope,struct symbol *table)
{
    struct symbol *temp = head;
    while (temp != NULL)
    {
        if(temp->name == name && temp->scope == scope)
        {
            printf("%s\t%d\t%d\t%s\n",temp);
            return temp;
        }
        return NULL;
    }
}

void ge_syb_table(ASTNode *root, FILE *out) {
    assert(root != NULL && "ROOT SHOULD NOT BE NULL");
<<<<<<< Updated upstream
    do_ge_table(root,out);
=======
    do_ge_table(root);
//    printf("\nsymbol table:\n");
//    print_symbol_table();
//    printf("\n");
}

void destroy_syb_table(){
    struct symbol* temp;
    while (head != NULL)
    {
        temp = head;
        head = temp->next;
        free(temp);
    }
>>>>>>> Stashed changes
}