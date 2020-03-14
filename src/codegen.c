

#include "codegen.h"
#include <stdlib.h>
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
        do_generate(ast_node_get_attr_node_value(node,"IDLIST"),out);
        fprintf(out,"){\n");
    } else if (strcmp(type, "PROGRAM_BODY") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"CONST_DECLARATIONS"),out);
        do_generate(ast_node_get_attr_node_value(node,"VAR_DECLARATIONS"),out);
        do_generate(ast_node_get_attr_node_value(node,"SUBPROGRAM_DECLARATIONS"),out);
        do_generate(ast_node_get_attr_node_value(node,"COMPOUND_STATEMENT"),out);
        fprintf(out,"\treturn 0;\n}\n");
    } else if (strcmp(type, "IDLIST") == 0) {
        for (ASTNodeAttr *cur = (node->first_attr); cur; (cur) = (cur)->next){
            fprintf(out,"%s",cur->value);
            if(cur != NULL && cur->next != NULL) fprintf(out,", ");
        }
    } else if (strcmp(type, "CONST_DECLARATIONS") == 0) {
        printf("TODO:TRANSLATE `CONST_DECLARATIONS`\n");
    } else if (strcmp(type, "CONST_DECLARATION") == 0) {
        printf("TODO:TRANSLATE `CONST_DECLARATION`\n");
    } else if (strcmp(type, "CONST_VALUE") == 0) {
        printf("TODO:TRANSLATE `CONST_VALUE`\n");
    } else if (strcmp(type, "VAR_DECLARATIONS") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"VAR_DECLARATION"),out);
    } else if (strcmp(type, "VAR_DECLARATION") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"VAR_DECLARATION"),out);
        fprintf(out,"\t");
        ASTNode *type_node = ast_node_get_attr_node_value(node,"TYPE");
        do_generate(type_node,out);
        do_generate(ast_node_get_attr_node_value(node,"IDLIST"),out);
        do_generate(ast_node_get_attr_node_value(type_node,"PERIOD_LIST"),out);
        fprintf(out,";\n");
    } else if (strcmp(type, "TYPE") == 0) {
        char *var_type_second = ast_node_get_attr_str_value(node,"BASIC_TYPE");
        if (!strcmp(var_type_second, "integer") || !strcmp(var_type_second, "boolean")) {
            fprintf(out, "int ");
        }
        if (strcmp(var_type_second, "real") == 0) {
            fprintf(out, "double ");
        }
        if (strcmp(var_type_second, "char") == 0) {
            fprintf(out, "char ");
        }
    } else if (strcmp(type, "ARRAY") == 0) {
        char *var_type_second = ast_node_get_attr_str_value(node,"BASIC_TYPE");
        if (!strcmp(var_type_second, "integer") || !strcmp(var_type_second, "boolean")) {
            fprintf(out, "int ");
        }
        if (strcmp(var_type_second, "real") == 0) {
            fprintf(out, "double ");
        }
        if (strcmp(var_type_second, "char") == 0) {
            fprintf(out, "char ");
        }
    } else if (strcmp(type, "PERIOD_LIST") == 0) {
//        printf(node->type);
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next){
            ASTNode *period_node = (ASTNode *) cur->value;
//            printf(period_node->type);
            char *period_begin = ast_node_get_attr_str_value(period_node,"PERIOD_BEGIN");
            char *period_end = ast_node_get_attr_str_value(period_node,"PERIOD_END");
            fprintf(out,"[%d]",atoi(period_end) - atoi(period_begin)+1);
        }
    } else if (strcmp(type, "SUBPROGRAM_DECLARATIONS") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"SUBPROGRAM_DECLARATIONS"),out);
        do_generate(ast_node_get_attr_node_value(node,"SUBPROGRAM"),out);
    } else if (strcmp(type, "SUBPROGRAM") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"SUBPROGRAM_HEAD"),out);
        do_generate(ast_node_get_attr_node_value(node,"SUBPROGRAM_BODY"),out);
    } else if (strcmp(type, "SUBPROGRAM_HEAD") == 0) {
        fprintf(out, "\t");
        char *func_type = ast_node_get_attr_str_value(node,"BASIC_TYPE");
        if (strcmp(func_type, "integer") == 0) {
            fprintf(out, "int ");
        }
        fprintf(out,"%s(",ast_node_get_attr_str_value(node,"ID"));
        do_generate(ast_node_get_attr_node_value(node,"FORMAL_PARAMETER"),out);
        fprintf(out,"){\n");
//        printf("TODO:ADD NEW TYPE\n");
    } else if (strcmp(type, "FORMAL_PARAMETER") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"PARAMETER_LIST"),out);
    } else if (strcmp(type, "PARAMETER_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_generate(expression_node,out);
        }
    } else if (strcmp(type, "PARAMETER") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"VAL_PARAMETER"),out);
        do_generate(ast_node_get_attr_node_value(node,"VALUE_PARAMETER"),out);
    } else if (strcmp(type, "VAR_PARAMETER") == 0) {
        printf("TODO:TRANSLATE `VAR_PARAMETER`\n");
    } else if (strcmp(type, "VALUE_PARAMETER") == 0) {
        char *var_type = ast_node_get_attr_str_value(node,"BASIC_TYPE");
        if (strcmp(var_type, "integer") == 0) {
            var_type = "int";
        }
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node,"IDLIST")->first_attr); cur; (cur) = (cur)->next){
            fprintf(out,"%s %s",var_type, cur->value);
            if(cur != NULL && cur->next != NULL) fprintf(out,", ");
        }
//        printf("TODO:ADD NEW TYPE\n");
    } else if (strcmp(type, "SUBPROGRAM_BODY") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"CONST_DECLARATIONS"),out);
        do_generate(ast_node_get_attr_node_value(node,"VAR_DECLARATIONS"),out);
        do_generate(ast_node_get_attr_node_value(node,"COMPOUND_STATEMENT"),out);
        fprintf(out, "\t}\n");
    } else if (strcmp(type, "COMPOUND_STATEMENT") == 0) {
        do_generate(ast_node_get_attr_node_value(node,"STATEMENT_LIST"),out);
    } else if (strcmp(type, "STATEMENT_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_generate(expression_node,out);
        }
    } else if (strcmp(type, "STATEMENT") == 0) {
        if(ast_node_get_attr_node_value(node,"VARIABLE")!=NULL){
            fprintf(out, "\t");
            do_generate(ast_node_get_attr_node_value(node,"VARIABLE"),out);
            fprintf(out, " = ");
            do_generate(ast_node_get_attr_node_value(node,"EXPRESSION"),out);
            fprintf(out, ";\n");
        } else if(ast_node_get_attr_node_value(node,"PROCEDURE_CALL")!=NULL){
            fprintf(out, "\t");
            do_generate(ast_node_get_attr_node_value(node,"PROCEDURE_CALL"),out);
        } else if(ast_node_get_attr_node_value(node,"COMPOUND_STATEMENT")!=NULL){
            fprintf(out, "\t");
            do_generate(ast_node_get_attr_node_value(node,"COMPOUND_STATEMENT"),out);
        } else if(ast_node_get_attr_node_value(node,"EXPRESSION")!=NULL && ast_node_get_attr_node_value(node,"STATEMENT")!=NULL){
            fprintf(out, "\t\tif(");
            do_generate(ast_node_get_attr_node_value(node,"EXPRESSION"),out);
            fprintf(out, "){\n\t\t");
            do_generate(ast_node_get_attr_node_value(node,"STATEMENT"),out);
            fprintf(out, "\t\t}\n");
            if(ast_node_get_attr_node_value(node,"ELSE_PART")!=NULL){
                fprintf(out, "\t\telse{\n");
                do_generate(ast_node_get_attr_node_value(node,"ELSE_PART"),out);
                fprintf(out, "\t\t}\n");
            }
        }
//        printf("TODO:STATEMENT's for read write\n");
    } else if (strcmp(type, "VARIABLE_LIST") == 0) {
        int i=0;
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            i++;
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_generate(expression_node,out);
        }
//        printf("TODO:TRANSLATE `VARIABLE_LIST`\n");
    } else if (strcmp(type, "VARIABLE") == 0) {
        fprintf(out,"%s",ast_node_get_attr_str_value(node,"ID"));
        do_generate(ast_node_get_attr_node_value(node,"ID_VARPART"),out);
//        printf("TODO:TRANSLATE `VARIABLE`\n");
    } else if (strcmp(type, "ID_VARPART") == 0) {
        printf("TODO:TRANSLATE `ID_VARPART`\n");
    } else if (strcmp(type, "PROCEDURE_CALL") == 0) {
        char *func_name = ast_node_get_attr_str_value(node,"ID");
        if(strcmp(func_name,"read") == 0){
            fprintf(out,"scanf");
        } else if (strcmp(func_name,"write") == 0){
            fprintf(out,"printf");
        }
        if(ast_node_get_attr_node_value(node,"EXPRESSION_LIST")!=NULL){
            fprintf(out, "(");
            do_generate(ast_node_get_attr_node_value(node,"EXPRESSION_LIST"),out);
            fprintf(out, ");\n");
        }
    } else if (strcmp(type, "ELSE_PART") == 0) {
        fprintf(out, "\t\t");
        do_generate(ast_node_get_attr_node_value(node,"STATEMENT"),out);
//        printf("TODO:TRANSLATE `ELSE_PART`\n");
    } else if (strcmp(type, "RELOP") == 0) {
        printf("TODO:TRANSLATE `RELOP`\n");
    } else if (strcmp(type, "ADDOP") == 0) {
        printf("TODO:TRANSLATE `ADDOP`\n");
    } else if (strcmp(type, "MULOP") == 0) {
        printf("TODO:TRANSLATE `MULOP`\n");
    } else if (strcmp(type, "EXPRESSION_LIST") == 0) {
        ASTNode *expression_node;
        int i = 0;
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next){
            i++;
            expression_node = (ASTNode *) cur->value;
            do_generate(expression_node,out);
            if(cur!=NULL && cur->next!=NULL){
                fprintf(out, ", ");
            }
        }
    } else if (strcmp(type, "EXPRESSION") == 0) {
        if(ast_node_get_attr_node_value(node,"RELOP") != NULL) {
            for (ASTNode *cur = ast_node_get_attr_node_value(node,"SIMPLE_EXPRESSION"); cur; (cur) = (cur)->next){
                if (ast_node_get_attr_node_value(cur,"ADDOP") != NULL){
                    do_generate(ast_node_get_attr_node_value(cur,"SIMPLE_EXPRESSION"),out);
                    do_generate(ast_node_get_attr_node_value(cur,"ADDOP"),out);
                    do_generate(ast_node_get_attr_node_value(cur,"TERM"),out);
                }
                else{
                    do_generate(ast_node_get_attr_node_value(cur,"TERM"),out);
                }
                if(cur!=NULL && cur->next!=NULL){
//                    do_generate(ast_node_get_attr_node_value(node,"RELOP"),out);
                    fprintf(out, " == ");
                }
            }
        } else{
            do_generate(ast_node_get_attr_node_value(node,"SIMPLE_EXPRESSION"),out);
        }
    } else if (strcmp(type, "SIMPLE_EXPRESSION") == 0) {
        for (ASTNode *cur = node; cur; (cur) = (cur)->next){
            if (ast_node_get_attr_node_value(cur,"ADDOP") != NULL){
                do_generate(ast_node_get_attr_node_value(cur,"SIMPLE_EXPRESSION"),out);
                do_generate(ast_node_get_attr_node_value(cur,"ADDOP"),out);
                do_generate(ast_node_get_attr_node_value(cur,"TERM"),out);
            }
            else{
                do_generate(ast_node_get_attr_node_value(cur,"TERM"),out);
            }
        }
//        printf("TODO:TRANSLATE `SIMPLE_EXPRESSION`\n");
    } else if (strcmp(type, "TERM") == 0) {
        if (ast_node_get_attr_node_value(node,"MULOP") != NULL){
            do_generate(ast_node_get_attr_node_value(node,"TERM"),out);
//            do_generate(ast_node_get_attr_node_value(node,"MULOP"),out);
            fprintf(out, " %% ");
            do_generate(ast_node_get_attr_node_value(node,"FACTOR"),out);
        }
        else{
            do_generate(ast_node_get_attr_node_value(node,"FACTOR"),out);
        }
    } else if (strcmp(type, "FACTOR") == 0) {
        if (strcmp(node->first_attr->key, "NUM")==0){
            fprintf(out, "%s", ast_node_get_attr_str_value(node,"NUM"));
        } else if (strcmp(node->first_attr->key, "ID")==0){
            fprintf(out, "%s", ast_node_get_attr_str_value(node,"ID"));
            if(ast_node_get_attr_node_value(node,"EXPRESSION_LIST")!=NULL){
                fprintf(out, "(");
                do_generate(ast_node_get_attr_node_value(node,"EXPRESSION_LIST"),out);
                fprintf(out, ")");
            }
        } else if (strcmp(node->first_attr->key, "VARIABLE")==0){
            do_generate(ast_node_get_attr_node_value(node,"VARIABLE"),out);
        }
    } else {
        printf("UNKNOWN TYPE:%s\n", type);
    }
}
void generate_c_code(ASTNode *root, FILE *out) {
    assert(root != NULL && "ROOT SHOULD NOT BE NULL");
    do_generate(root,out);
}
