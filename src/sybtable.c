//
// Created by Jinyuan Zhang on 2020/5/22.
//


#include "sybtable.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stdio.h"

#define LEN sizeof(struct symbol)

struct symbol* head= NULL;
struct symbol* end = NULL;
char *scope = "null";
int is_arg = 0;
int ref = 0;

void do_ge_table(ASTNode *node){
    if (node == NULL) return;
    //create symbolList

    char *type = node->type;

    if (strcmp(type, "PROGRAM_STRUCT") == 0) {
//        printf("program_struct\n");
        char *temp_scope = scope;
        do_ge_table(ast_node_get_attr_node_value(node, "PROGRAM_HEAD"));
        do_ge_table(ast_node_get_attr_node_value(node, "CONST_DECLARATIONS"));
        do_ge_table(ast_node_get_attr_node_value(node, "TYPE_DECLARATIONS"));
        do_ge_table(ast_node_get_attr_node_value(node, "VAR_DECLARATIONS"));
        do_ge_table(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS_LIST"));
        scope = temp_scope;
    } else if (strcmp(type, "PROGRAM_HEAD") == 0) {
//        printf("PROGRAM_HEAD\n");
        char *program_name = ast_node_get_attr_str_value(node,"ID");
//        printf("name:%s\n",program_name);
        add_symbol(program_name,"program",0,"null",0,0);
        scope = program_name;
    } else if (strcmp(type, "IDLIST") == 0) {
//        printf("IDLIST\n");
        for (ASTNodeAttr *cur = (node->first_attr); cur; (cur) = (cur)->next) {
//            printf("%s", cur->value);
//            if (cur != NULL && cur->next != NULL) printf(", ");
        }
    } else if (strcmp(type, "CONST_DECLARATIONS") == 0) {
        do_ge_table(ast_node_get_attr_node_value(node, "CONST_DECLARATION_LIST"));
    } else if (strcmp(type, "CONST_DECLARATION_LIST") == 0) {
//        printf("CONST_DECLARATION_LIST\n");
        for (ASTNodeAttr *cur = (node->first_attr); cur; (cur) = (cur)->next) {
            do_ge_table(cur->value);
        }
    } else if (strcmp(type, "CONST_DECLARATION") == 0) {
//        printf("CONST_DECLARATION\n");
//        printf("%s\n",ast_node_get_attr_str_value(node, "ID"));
        do_ge_table(ast_node_get_attr_node_value(node, "CONST_VALUE"));
        add_symbol(ast_node_get_attr_str_value(node, "ID"),"const",0,scope,0,0);
    } else if (strcmp(type, "CONST_VALUE") == 0){
//        printf("CONST_VALUE\n");
        if (ast_node_get_attr(node, "T_NUM") != NULL) {
//        printf("%s", ast_node_get_attr_str_value(node, "T_NUM"));
        } else if (ast_node_get_attr(node, "T_MINUS") != NULL) {
    //        printf("-%s", ast_node_get_attr_str_value(node, "T_MINUS"));
        } else if (ast_node_get_attr(node, "T_LETTER") != NULL) {
    //        printf("\"%s\"", ast_node_get_attr_str_value(node, "T_LETTER"));
        }
    }else if (strcmp(type, "TYPE_DECLARATIONS") == 0) {
        do_ge_table(ast_node_get_attr_node_value(node, "TYPE_DECLARATION_LIST"));
    } else if (strcmp(type, "TYPE_DECLARATION_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_ge_table(type_list_node);
        }
    } else if (strcmp(type, "VAR_DECLARATIONS") == 0) {
//        printf("VAR_DECLARATIONS\n");
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_ge_table(type_list_node);
        }
    } else if (strcmp(type, "VAR_DECLARATION_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_ge_table(type_list_node);
        }
    } else if (strcmp(type, "VAR_DECLARATION") == 0) {
//        printf("VAR_DECLARATION\n");
        ASTNode *type_node = ast_node_get_attr_node_value(node, "TYPE");
//        printf("TYPE\n");
        char *var_type_second = ast_node_get_attr_str_value(type_node, "BASIC_TYPE");
//        printf("%s\n", var_type_second);
        do_ge_table(type_node);
//        printf("IDLIST\n");
        ASTNode *idlist_node = ast_node_get_attr_node_value(node, "IDLIST");
        for (ASTNodeAttr *cur = (idlist_node->first_attr); cur; (cur) = (cur)->next) {
//            printf("%s\n", cur->value);
//            if (cur != NULL && cur->next != NULL) printf(", ");
            add_symbol(cur->value,var_type_second,0,scope,0,0);
        }
        do_ge_table(ast_node_get_attr_node_value(node, "IDLIST"));

    } else if (strcmp(type, "TYPE") == 0) {
//        printf("TYPE\n");
    } else if (strcmp(type, "ARRAY") == 0) {
//        printf("ARRAY\n");
        char *name = ast_node_get_attr_str_value(node, "ID");
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        int period_count = 0;
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "PERIOD_LIST"))->first_attr; cur; (cur) = (cur)->next) {
            period_count += 1;
            ASTNode *period = cur->value;
            int period_end = atoi(ast_node_get_attr_str_value(period, "PERIOD_END"));
            char *id = malloc(sizeof(char) * 11);
            strcpy(id, "dimension_");
            id[strlen(id) - 1] = period_count + '0';
            add_symbol(id, var_type, period_end, name, 0, 0);
        }
        add_symbol(name, "array", period_count, scope, 0, 0);
    } else if (strcmp(type, "RECORD") == 0) {
        char *name = ast_node_get_attr_str_value(node, "ID");
        char *var_type = "record";
        ASTNode *member_list = ast_node_get_attr_node_value(node, "TYPE_RECORD_DECLARATION");
        char *pre_scope = scope;
        scope = name;
        int member_count = 0;
        for (ASTNodeAttr *cur = member_list->first_attr; cur; (cur) = (cur)->next){
            do_ge_table(cur->value);
            member_count += 1;
        }
        scope = pre_scope;
        add_symbol(name, var_type, member_count, scope, 0, 0);
    } else if (strcmp(type, "TYPE_RECORD_DECLARATION") == 0) {
//        printf("TYPE_RECORD_DECLARATION\n");
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_ge_table(type_list_node);
        }
    } else if (strcmp(type, "PERIOD_LIST") == 0) {
//        printf("PERIOD_LIST");
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *period_node = (ASTNode *) cur->value;
            char *period_begin = ast_node_get_attr_str_value(period_node, "PERIOD_BEGIN");
            char *period_end = ast_node_get_attr_str_value(period_node, "PERIOD_END");
//            printf("[%d]", atoi(period_end) - atoi(period_begin) + 1);
        }
    } else if (strcmp(type, "SUBPROGRAM_DECLARATIONS_LIST") == 0) {
//        printf( "SUBPROGRAM_DECLARATIONS_LIST\n");
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            do_ge_table((ASTNode *) cur->value);
        }
    } else if (strcmp(type, "SUBPROGRAM") == 0) {
//        printf("SUBPROGRAM\n");
        char *temp_sub_program_name = scope;
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_HEAD"));
        do_ge_table(ast_node_get_attr_node_value(node,"SUBPROGRAM_BODY"));
        scope = temp_sub_program_name;
    } else if (strcmp(type, "SUBPROGRAM_HEAD") == 0) {
//        printf("SUBPROGRAM_HEAD\n");
//        printf("%s\n",ast_node_get_attr_str_value(node, "ID"));
        scope = ast_node_get_attr_str_value(node, "ID");
        if (ast_node_get_attr(node, "BASIC_TYPE") != NULL) {
            char *func_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
//            printf("%s ", func_type);
            add_symbol(ast_node_get_attr_str_value(node, "ID"),func_type,0,scope,0,0);
        } else {
//            printf("void ");
            add_symbol(ast_node_get_attr_str_value(node, "ID"),"program",0,scope,0,0);
        }

//        printf("%s(", ast_node_get_attr_str_value(node, "ID"));
        do_ge_table(ast_node_get_attr_node_value(node, "FORMAL_PARAMETER"));
//        fprintf(out, ")");
    } else if (strcmp(type, "FORMAL_PARAMETER") == 0) {
//        printf( "FORMAL_PARAMETER\n");
        do_ge_table(ast_node_get_attr_node_value(node,"PARAMETER_LIST"));
    } else if (strcmp(type, "PARAMETER_LIST") == 0) {
//        printf("PARAMETER_LIST\n");
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_ge_table(expression_node);
        }
    } else if (strcmp(type, "PARAMETER") == 0) {
//        printf("PARAMETER\n");
        is_arg = 1;
        do_ge_table(ast_node_get_attr_node_value(node,"VAR_PARAMETER"));
        do_ge_table(ast_node_get_attr_node_value(node,"VALUE_PARAMETER"));
        is_arg = 0;
    } else if (strcmp(type, "VAR_PARAMETER") == 0) {
//        printf( "VAR_PARAMETER\n");
        ref = 2;
        node = ast_node_get_attr_node_value(node, "VALUE_PARAMETER");
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
//        var_type = var_type;
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
//            printf("%s *%s", var_type, cur->value);
//            if (cur != NULL && cur->next != NULL) printf(", ");
            add_symbol(cur->value,var_type,0,scope,is_arg,ref);
        }
    } else if (strcmp(type, "VALUE_PARAMETER") == 0) {
//        printf("VALUE_PARAMETER\n");
        ref = 1;
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
//            printf("%s %s", var_type, cur->value);
//            if (cur != NULL && cur->next != NULL) printf(", ");
            add_symbol(cur->value,var_type,0,scope,is_arg,ref);
        }
    } else {
//        printf("else:%s\n", type);
    }
}

void add_symbol(char *name, char *type, int dimension, char *scope, int is_arg,int ref)
{
    struct symbol* temp = (struct symbol*)malloc(LEN);

    temp->name = name;
    temp->type = type;
    temp->dimension = dimension;
    temp->scope = scope;
    temp->is_arg = is_arg;
    temp->ref = ref;
    temp->next = NULL;

    if (head == NULL)
    {
        head = temp;
        end = temp;
    } else
    {
        end->next = temp;
    }
    end = temp;
}

struct symbol* get_symbol(char *name, char *scope)
{
    struct symbol *temp = head;
    while (temp != NULL)
    {
        if(strcmp(temp->name, name) == 0 && strcmp(temp->scope, scope) == 0) {
//            printf("%s\t%s\t%d\t%s\t%d\t%d\n", temp->name, temp->type, temp->dimension, temp->scope, temp->is_arg,
//                   temp->ref);
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void print_symbol_table()
{
    struct symbol *temp = head;
    printf("%20s\t%10s\t%10s\t\t%20s\t\t%10s\t\t%10s\n", "name", "type", "dimension", "scope", "is_arg", "ref");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    while (temp != NULL)
    {
        printf("%20s\t%10s\t%10d\t\t%20s\t\t%10d\t\t%10d\n",temp->name,temp->type,temp->dimension,temp->scope,temp->is_arg,temp->ref);
        temp = temp->next;
    }
}

void ge_syb_table(ASTNode *root) {
    assert(root != NULL && "ROOT SHOULD NOT BE NULL");
    do_ge_table(root);
//    printf("\nsymbol table:\n");
//    print_symbol_table();
//    printf("\n");
}