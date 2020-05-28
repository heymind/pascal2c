

#include "codegen.h"
#include "sybtable.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <utlist.h>

int level = 0;
char *scope_ = "null";
#define BEGIN_BLOCK(o) {level++;fprintf((o),"{");}
#define NEWLINE(o)  newline((o))
#define END_BLOCK(o) {level--;newline((o));fprintf((o),"}");}

void newline(FILE *out) {
    fprintf(out, "\n");
    for (int i = 0; i < level; i++) fprintf(out, "\t");
}

void do_generate(ASTNode *node, FILE *out) {
    if (node == NULL) return;
    char *type = node->type;
    if (strcmp(type, "PROGRAM_STRUCT") == 0) {
        fprintf(out, "#include <stdio.h>");
        do_generate(ast_node_get_attr_node_value(node, "PROGRAM_HEAD"), out);
        do_generate(ast_node_get_attr_node_value(node, "CONST_DECLARATIONS"), out);
        do_generate(ast_node_get_attr_node_value(node, "TYPE_DECLARATIONS"), out);
        do_generate(ast_node_get_attr_node_value(node, "VAR_DECLARATIONS"), out);
        generate_subprogram_defs(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS_LIST"), out);
        do_generate(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS_LIST"), out);
        NEWLINE(out);
        fprintf(out, "int main()");
        BEGIN_BLOCK(out);
        do_generate(ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT"), out);
        NEWLINE(out);
        fprintf(out, "return 0;");
        END_BLOCK(out);
        do_generate(ast_node_get_attr_node_value(node, "SUBPROGRAM_DECLARATIONS"), out);
    } else if (strcmp(type, "PROGRAM_HEAD") == 0) {
        char *program_id = ast_node_get_attr_str_value(node, "ID");
//        fprintf(out,"int main(){\n\t%s();\n\treturn 0;\n}\n",program_id);
//        NEWLINE(out);
//        fprintf(out, "int main()");
        scope_ = program_id;
    } else if (strcmp(type, "IDLIST") == 0) {
        int period_count = 1;
        char *id_varpart = malloc(sizeof(char) * 20);
        strcpy(id_varpart, "");
        while(1){
            char *id = malloc(sizeof(char) * 11);
            strcpy(id, "dimension_");
            id[strlen(id) - 1] = period_count + '0';
            struct symbol *sym = get_symbol(id, scope_);
            free(id);
            if(sym == NULL)
                break;
            sprintf(id_varpart + strlen(id_varpart), "[%d]", sym->dimension);
            period_count += 1;
        }
        for (ASTNodeAttr *cur = (node->first_attr); cur; (cur) = (cur)->next) {
            fprintf(out, "%s%s", cur->value, id_varpart);
            if (cur != NULL && cur->next != NULL) fprintf(out, ", ");
        }
        free(id_varpart);
    } else if (strcmp(type, "CONST_DECLARATIONS") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "CONST_DECLARATION_LIST"), out);
    } else if (strcmp(type, "CONST_DECLARATION_LIST") == 0) {
        for (ASTNodeAttr *cur = (node->first_attr); cur; (cur) = (cur)->next) {
            do_generate(cur->value, out);
        }
    } else if (strcmp(type, "CONST_DECLARATION") == 0) {
        NEWLINE(out);
        fprintf(out, "#define %s (",
                ast_node_get_attr_str_value(node, "ID"));
        do_generate(ast_node_get_attr_node_value(node, "CONST_VALUE"), out);
        fprintf(out, ")\n");
    } else if (strcmp(type, "CONST_VALUE") == 0) {
        if (ast_node_get_attr(node, "T_NUM") != NULL) {
            fprintf(out, "%s", ast_node_get_attr_str_value(node, "T_NUM"));
        } else if (ast_node_get_attr(node, "T_MINUS") != NULL) {
            fprintf(out, "-%s", ast_node_get_attr_str_value(node, "T_MINUS"));
        } else if (ast_node_get_attr(node, "T_LETTER") != NULL) {
            fprintf(out, "%s", ast_node_get_attr_str_value(node, "T_LETTER"));
        }
    } else if (strcmp(type, "TYPE_DECLARATIONS") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "TYPE_DECLARATION_LIST"), out);
    } else if (strcmp(type, "TYPE_DECLARATION_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            NEWLINE(out);
            ASTNode *type_list_node = (ASTNode *) cur->value;
            if(strcmp(type_list_node->type, "RECORD") == 0)
                do_generate(type_list_node, out);
        }
    } else if (strcmp(type, "VAR_DECLARATION_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_generate(type_list_node, out);
        }
    } else if (strcmp(type, "VAR_DECLARATIONS") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            NEWLINE(out);
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_generate(type_list_node, out);
        }
    } else if (strcmp(type, "VAR_DECLARATION") == 0) {
        NEWLINE(out);
        ASTNode *type_node = ast_node_get_attr_node_value(node, "TYPE");
        do_generate(type_node, out);
        char *pre_scope = scope_;
        scope_ = ast_node_get_attr_str_value(type_node, "BASIC_TYPE");
        do_generate(ast_node_get_attr_node_value(node, "IDLIST"), out);
        scope_ = pre_scope;
        fprintf(out, ";");
    } else if (strcmp(type, "TYPE") == 0) {
        char *var_type_second = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        if(is_basic_type(var_type_second))
            fprintf(out, "%s ", var_type_change(var_type_second));
        else{
            struct symbol *sym = get_symbol(var_type_second, scope_);
            if(sym == NULL){
                //error no this type
            } else if(strcmp(sym->type, "array") == 0) {
                char *name = sym->name;
                char *id = "dimension1";
                struct symbol *sub_sym = get_symbol(id, name);
                fprintf(out, "%s ", var_type_change(sub_sym->type));
            } else if(strcmp(sym->type, "record") == 0){
                char *name = sym->name;
                fprintf(out, "struct %s ", name);
            }
        }
    } else if (strcmp(type, "ARRAY") == 0) {
        NEWLINE(out);
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        fprintf(out, "%s ", var_type_change(var_type));
        fprintf(out, "%s", ast_node_get_attr_str_value(node, "ID"));
        do_generate(ast_node_get_attr_node_value(node, "PERIOD_LIST"), out);
        fprintf(out, ";");
    } else if (strcmp(type, "RECORD") == 0) {
        NEWLINE(out);
        fprintf(out, "struct %s", ast_node_get_attr_str_value(node, "ID"));
        BEGIN_BLOCK(out)
        ASTNode *member_list = ast_node_get_attr_node_value(node, "TYPE_RECORD_DECLARATION");
        for (ASTNodeAttr *cur = member_list->first_attr; cur; (cur) = (cur)->next){
            do_generate(cur->value, out);
        }
        END_BLOCK(out);
        fprintf(out, ";");
    } else if (strcmp(type, "TYPE_RECORD_DECLARATION") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            NEWLINE(out);
            ASTNode *type_list_node = (ASTNode *) cur->value;
            do_generate(type_list_node, out);
        }
    } else if (strcmp(type, "PERIOD_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *period_node = (ASTNode *) cur->value;
            char *period_begin = ast_node_get_attr_str_value(period_node, "PERIOD_BEGIN");
            char *period_end = ast_node_get_attr_str_value(period_node, "PERIOD_END");
            fprintf(out, "[%d]", atoi(period_end) - atoi(period_begin) + 1);
        }
    } else if (strcmp(type, "SUBPROGRAM_DECLARATIONS_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
//            fprintf(out, "%s", cur->key);
            do_generate((ASTNode *) cur->value, out);
            fprintf(out, ";");
        }

    } else if (strcmp(type, "SUBPROGRAM") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "SUBPROGRAM_HEAD"), out);
        do_generate(ast_node_get_attr_node_value(node, "SUBPROGRAM_BODY"), out);
    } else if (strcmp(type, "SUBPROGRAM_HEAD") == 0) {
        NEWLINE(out);
        if (ast_node_get_attr(node, "BASIC_TYPE") != NULL) {
            char *func_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
            fprintf(out, "%s ", var_type_change(func_type));
        } else {
            fprintf(out, "void ");
        }

        fprintf(out, "%s(", ast_node_get_attr_str_value(node, "ID"));
        do_generate(ast_node_get_attr_node_value(node, "FORMAL_PARAMETER"), out);
        fprintf(out, ")");
    } else if (strcmp(type, "FORMAL_PARAMETER") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "PARAMETER_LIST"), out);
    } else if (strcmp(type, "PARAMETER_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_generate(expression_node, out);
        }
    } else if (strcmp(type, "PARAMETER") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "VAR_PARAMETER"), out);
        do_generate(ast_node_get_attr_node_value(node, "VALUE_PARAMETER"), out);
    } else if (strcmp(type, "VAR_PARAMETER") == 0) {
        node = ast_node_get_attr_node_value(node, "VALUE_PARAMETER");
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        var_type = var_type_change(var_type);
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
            fprintf(out, "%s *%s", var_type, cur->value);
            if (cur != NULL && cur->next != NULL) fprintf(out, ", ");
        }
    } else if (strcmp(type, "VALUE_PARAMETER") == 0) {
        char *var_type = ast_node_get_attr_str_value(node, "BASIC_TYPE");
        var_type = var_type_change(var_type);
        for (ASTNodeAttr *cur = (ast_node_get_attr_node_value(node, "IDLIST")->first_attr); cur; (cur) = (cur)->next) {
            fprintf(out, "%s %s", var_type, cur->value);
            if (cur != NULL && cur->next != NULL) fprintf(out, ", ");
        }
    } else if (strcmp(type, "SUBPROGRAM_BODY") == 0) {
        BEGIN_BLOCK(out);
        do_generate(ast_node_get_attr_node_value(node, "CONST_DECLARATIONS"), out);
        do_generate(ast_node_get_attr_node_value(node, "VAR_DECLARATIONS"), out);
        do_generate(ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT"), out);
        END_BLOCK(out);
    } else if (strcmp(type, "COMPOUND_STATEMENT") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "STATEMENT_LIST"), out);
    } else if (strcmp(type, "STATEMENT_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_generate(expression_node, out);
        }
    } else if (strcmp(type, "STATEMENT") == 0) {
//        printf("%s", node->first_attr->value);
        //这里必须保证TYPE节点是STATEMENT的第一个属性
        if (strcmp(node->first_attr->value, "WHILE") == 0) {
            fprintf(out, "while(");
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION"), out);
            fprintf(out, ")");
            BEGIN_BLOCK(out);
            do_generate(ast_node_get_attr_node_value(node, "STATEMENT"), out);
            END_BLOCK(out);
        } else if (ast_node_get_attr_node_value(node, "VARIABLE") != NULL) {
            NEWLINE(out);
            do_generate(ast_node_get_attr_node_value(node, "VARIABLE"), out);
            fprintf(out, " = ");
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION"), out);
            fprintf(out, ";");
        } else if (ast_node_get_attr_node_value(node, "PROCEDURE_CALL") != NULL) {
            NEWLINE(out);
            do_generate(ast_node_get_attr_node_value(node, "PROCEDURE_CALL"), out);
        } else if (ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT") != NULL) {
            NEWLINE(out);
            do_generate(ast_node_get_attr_node_value(node, "COMPOUND_STATEMENT"), out);
        } else if (strcmp(node->first_attr->value, "IF") == 0 &&
                   ast_node_get_attr_node_value(node, "EXPRESSION") != NULL &&
                   ast_node_get_attr_node_value(node, "STATEMENT") != NULL) {
            NEWLINE(out);
            fprintf(out, "if(");
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION"), out);
            fprintf(out, ")");
            BEGIN_BLOCK(out);
            do_generate(ast_node_get_attr_node_value(node, "STATEMENT"), out);
            END_BLOCK(out);
            if (ast_node_get_attr_node_value(node, "ELSE_PART") != NULL) {
                fprintf(out, "else");
                BEGIN_BLOCK(out);
                do_generate(ast_node_get_attr_node_value(node, "ELSE_PART"), out);
                END_BLOCK(out);
            }
        } else if (strcmp(node->first_attr->value, "FOR") == 0 &&
                   ast_node_get_attr_node_value(node, "EXPRESSION_BEGIN") != NULL &&
                   ast_node_get_attr_node_value(node, "EXPRESSION_END") != NULL &&
                   ast_node_get_attr_node_value(node, "STATEMENT") != NULL
                ) {
            char para[50];
            strcpy(para,node->first_attr->next->value);
            NEWLINE(out);
            fprintf(out, "for(%s=",para);
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION_BEGIN"), out);
            fprintf(out, ";%s<=",para);
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION_END"), out);
            fprintf(out, ";%s++)",para);
            BEGIN_BLOCK(out);
            do_generate(ast_node_get_attr_node_value(node, "STATEMENT"), out);
            END_BLOCK(out);
        }
    } else if (strcmp(type, "VARIABLE_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            ASTNode *expression_node = (ASTNode *) cur->value;
            do_generate(expression_node, out);
        }
    } else if (strcmp(type, "VARIABLE") == 0) {
        char *name = ast_node_get_attr_str_value(node, "ID");
        fprintf(out, "%s", name);
        if(ast_node_get_attr_str_value(node, "MEMBER")){
            char *member = ast_node_get_attr_str_value(node, "MEMBER");
            struct symbol *sym = get_symbol(name, scope_);
            if(sym != NULL){
                fprintf(out, ".%s", member);
            } else{
                //error
            }
        } else{
            struct symbol *sym = get_symbol(name, scope_);
            char *pre_scope = scope_;
            if(sym != NULL) //当variable的生成符号表代码补全后，如果sym为空，则报错，这里是为了测试通过
                scope_ = sym->type;
            else{
                //error
            }
            do_generate(ast_node_get_attr_node_value(node, "ID_VARPART"), out);
            scope_ = pre_scope;
        }
    } else if (strcmp(type, "ID_VARPART") == 0) {
        ASTNode *list_node = node->first_attr->value;
        ASTNodeAttr *start = list_node->first_attr;
        int period_count = 1;
        for (ASTNodeAttr *cur = start; cur; (cur) = (cur)->next) {
            fprintf(out, "[");
            do_generate(cur->value, out);
            fprintf(out, "]");
        }
    } else if (strcmp(type, "PROCEDURE_CALL") == 0) {
        char *func_name = ast_node_get_attr_str_value(node, "ID");
        if (!strcmp(func_name, "read")) {
            generate_read(node, out);
            return;
        } else if (!strcmp(func_name, "write")) {
            generate_write(node, out);
        } else {
            fprintf(out, "%s(", func_name);
        }
        if (ast_node_get_attr_node_value(node, "EXPRESSION_LIST") != NULL) {
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION_LIST"), out);
        }
        fprintf(out, ");");


    } else if (strcmp(type, "ELSE_PART") == 0) {
        do_generate(ast_node_get_attr_node_value(node, "STATEMENT"), out);
    } else if (strcmp(type, "RELOP_CEQ") == 0) {
        fprintf(out, " == ");
    } else if (strcmp(type, "RELOP_CNE") == 0) {
        fprintf(out, " != ");
    } else if (strcmp(type, "RELOP_CGE") == 0) {
        fprintf(out, " >= ");
    } else if (strcmp(type, "RELOP_CLE") == 0) {
        fprintf(out, " <= ");
    } else if (strcmp(type, "RELOP_CGT") == 0) {
        fprintf(out, " > ");
    } else if (strcmp(type, "RELOP_CLT") == 0) {
        fprintf(out, " < ");
    } else if (strcmp(type, "ADDOP_PLUS") == 0) {
        fprintf(out, " + ");
    } else if (strcmp(type, "ADDOP_MINUS") == 0) {
        fprintf(out, " - ");
    } else if (strcmp(type, "ADDOP_OR") == 0) {
        fprintf(out, " || ");
    } else if (strcmp(type, "MULOP_MUL") == 0) {
        fprintf(out, " * ");
    } else if (strcmp(type, "MULOP_DIV") == 0) {
        fprintf(out, " / ");
    } else if (strcmp(type, "MULOP_AND") == 0) {
        fprintf(out, " && ");
    } else if (strcmp(type, "MULOP_MOD") == 0) {
        fprintf(out, " %% ");
    } else if (strcmp(type, "MULOP") == 0) {
        printf("TODO:TRANSLATE `MULOP`\n");
    } else if (strcmp(type, "EXPRESSION_LIST") == 0) {
        ASTNode *expression_node;
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            expression_node = (ASTNode *) cur->value;
            do_generate(expression_node, out);
            if (cur != NULL && cur->next != NULL) {
                fprintf(out, ", ");
            }
        }
    } else if (strcmp(type, "EXPRESSION") == 0) {
        if (ast_node_get_attr_node_value(node, "RELOP") != NULL) {
//            for (ASTNode *cur = ast_node_get_attr_node_value(node, "SIMPLE_EXPRESSION_HEAD"); cur; (cur) = (cur)->next) {
//                fprintf(out, "%s", cur->first_attr->key);
//                if (ast_node_get_attr_node_value(cur, "ADDOP") != NULL) {
//                    do_generate(ast_node_get_attr_node_value(cur, "SIMPLE_EXPRESSION"), out);
//                    do_generate(ast_node_get_attr_node_value(cur, "ADDOP"), out);
//                    do_generate(ast_node_get_attr_node_value(cur, "TERM"), out);
//                } else {
//                    do_generate(ast_node_get_attr_node_value(cur, "TERM"), out);
//                }
//                if (cur != NULL && cur->next != NULL) {
//                    do_generate(ast_node_get_attr_node_value(node, "RELOP"), out);
//                }
//            }
            for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
                if (strcmp(cur->key, "SIMPLE_EXPRESSION_HEAD") == 0) {
                    do_generate(cur->value, out);
                } else if (strcmp(cur->key, "RELOP") == 0) {
                    do_generate(cur->value, out);
                } else if (strcmp(cur->key, "SIMPLE_EXPRESSION_TAIL") == 0) {
                    do_generate(cur->value, out);
                }
            }
        } else {
            do_generate(ast_node_get_attr_node_value(node, "SIMPLE_EXPRESSION"), out);
        }
    } else if (strcmp(type, "SIMPLE_EXPRESSION") == 0) {
//        for (ASTNode *cur = node; cur; (cur) = (cur)->next) {
//            if (ast_node_get_attr_node_value(cur, "ADDOP") != NULL) {
//                do_generate(ast_node_get_attr_node_value(cur, "SIMPLE_EXPRESSION"), out);
//                do_generate(ast_node_get_attr_node_value(cur, "ADDOP"), out);
//                do_generate(ast_node_get_attr_node_value(cur, "TERM"), out);
//            } else {
//                do_generate(ast_node_get_attr_node_value(cur, "TERM"), out);
//            }
//        }
        if (ast_node_get_attr_node_value(node, "ADDOP") != NULL) {
            for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
                if (strcmp(cur->key, "SIMPLE_EXPRESSION") == 0) {
                    do_generate(cur->value, out);
                } else if (strcmp(cur->key, "ADDOP") == 0) {
                    do_generate(cur->value, out);
                } else if (strcmp(cur->key, "TERM") == 0) {
                    do_generate(cur->value, out);
                }
            }
        } else {
            do_generate(ast_node_get_attr_node_value(node, "TERM"), out);
        }
    } else if (strcmp(type, "TERM") == 0) {
        if (ast_node_get_attr_node_value(node, "MULOP") != NULL) {
            do_generate(ast_node_get_attr_node_value(node, "TERM"), out);
            do_generate(ast_node_get_attr_node_value(node, "MULOP"), out);
            do_generate(ast_node_get_attr_node_value(node, "FACTOR"), out);
        } else {
            do_generate(ast_node_get_attr_node_value(node, "FACTOR"), out);
        }
    } else if (strcmp(type, "FACTOR") == 0) {
        if (strcmp(node->first_attr->key, "NUM") == 0) {
            fprintf(out, "%s", ast_node_get_attr_str_value(node, "NUM"));
        } else if (strcmp(node->first_attr->key, "ID") == 0) {
            fprintf(out, "%s", ast_node_get_attr_str_value(node, "ID"));
            if (ast_node_get_attr_node_value(node, "EXPRESSION_LIST") != NULL) {
                fprintf(out, "(");
                do_generate(ast_node_get_attr_node_value(node, "EXPRESSION_LIST"), out);
                fprintf(out, ")");
            }
        } else if (strcmp(node->first_attr->key, "VARIABLE") == 0) {
            do_generate(ast_node_get_attr_node_value(node, "VARIABLE"), out);
        } else if (strcmp(node->first_attr->key, "EXPRESSION") == 0) {
            fprintf(out, "(");
            do_generate(ast_node_get_attr_node_value(node, "EXPRESSION"), out);
            fprintf(out, ")");
        } else if (strcmp(node->first_attr->key, "TYPE") == 0) {
            char *value = node->first_attr->value;
            if (strcmp(value, "T_NOT") == 0) {
                fprintf(out, "!");
                do_generate(ast_node_get_attr_node_value(node, "FACTOR"), out);
            } else if (strcmp(value, "T_MINUS") == 0) {
                fprintf(out, "-");
                do_generate(ast_node_get_attr_node_value(node, "FACTOR"), out);
            }
        }
    } else {
        printf("UNKNOWN TYPE:%s\n", type);
    }
}

//参考自 https://blog.csdn.net/farmerjohnofzs/article/details/52268572
char *var_type_change(char *pascal_var_type) {
    char *c_var_type;
    if (!strcmp(pascal_var_type, "shortint")) {
        c_var_type = "short";
    } else if (!strcmp(pascal_var_type, "integer") || !strcmp(pascal_var_type, "Integer") ||
               !strcmp(pascal_var_type, "boolean") || !strcmp(pascal_var_type, "Boolean")) {
        c_var_type = "int";
    } else if (!strcmp(pascal_var_type, "longint")) {
        c_var_type = "long";
    } else if (!strcmp(pascal_var_type, "int64")) {
        c_var_type = "long long";
    } else if (!strcmp(pascal_var_type, "byte")) {
        c_var_type = "unsigned short";
    } else if (!strcmp(pascal_var_type, "word")) {
        c_var_type = "unsigned int";
    } else if (!strcmp(pascal_var_type, "dword")) {
        c_var_type = "unsigned long";
    } else if (!strcmp(pascal_var_type, "qword")) {
        c_var_type = "unsigned long long";
    } else if (!strcmp(pascal_var_type, "real") || !strcmp(pascal_var_type, "Real")) {
        c_var_type = "float";
    } else if (!strcmp(pascal_var_type, "double")) {
        c_var_type = "double";
    } else if (!strcmp(pascal_var_type, "char") || !strcmp(pascal_var_type, "Char")) {
        c_var_type = "char";
    } else {
        c_var_type = pascal_var_type;
    }
    return c_var_type;
}

void generate_read(ASTNode *node, FILE *out) {
    fprintf(out, "scanf(\"");
    ASTNode *lower_node = node->first_attr->next->value;
    ASTNodeAttr *start = lower_node->first_attr;
    for (ASTNodeAttr *cur = start; cur; (cur) = (cur)->next) {
        // 从符号表里获取类型信息，目前没有符号表，全部默认为 整数型
        if (cur == start) {
            fprintf(out, "%%d");
        } else {
            fprintf(out, " %%d");
        }
    }
    fprintf(out, "\",");
    ASTNode *expr_list = ast_node_get_attr_node_value(node, "EXPRESSION_LIST");
    if (expr_list) {
        fprintf(out, "&");
        for (ASTNodeAttr *cur = expr_list->first_attr; cur; (cur) = (cur)->next) {
            do_generate(cur->value, out);
            if (cur != NULL && cur->next != NULL) {
                fprintf(out, ", &");
            }
        }
    }

    fprintf(out, ");");


}

void generate_write(ASTNode *node, FILE *out) {
    fprintf(out, "printf(\"");
    ASTNodeAttr *first = ast_node_get_attr_node_value(node, "EXPRESSION_LIST")->first_attr;
    for (ASTNodeAttr *cur = first; cur; (cur) = (cur)->next) {
        // 从符号表里获取类型信息，目前没有符号表，全部默认为 整数型
        if (cur == first) {
            fprintf(out, "%%d");
        } else {
            fprintf(out, " %%d");
        }
    }
    fprintf(out, "\",");

}

char *func_name_change(char *pascal_func_name) {
    char *c_func_name;
    if (!strcmp(pascal_func_name, "read")) {
        c_func_name = "scanf";
    } else if (!strcmp(pascal_func_name, "write")) {
        c_func_name = "printf";
    }
    return c_func_name;
}

void generate_c_code(ASTNode *root, FILE *out) {
    assert(root != NULL && "ROOT SHOULD NOT BE NULL");
    do_generate(root, out);
}

void generate_subprogram_defs(ASTNode *node, FILE *out) {
    if (node == NULL) return;
    char *type = node->type;
    if (strcmp(type, "SUBPROGRAM_DECLARATIONS_LIST") == 0) {
        for (ASTNodeAttr *cur = node->first_attr; cur; (cur) = (cur)->next) {
            do_generate(ast_node_get_attr_node_value((ASTNode *) cur->value, "SUBPROGRAM_HEAD"), out);
            fprintf(out, ";");
        }
    } else {
        assert(0 && "unreachable");
    }
}

int is_basic_type(char *type) {
    if(strcmp(type, "integer") == 0 || strcmp(type, "Integer") == 0 ||
       strcmp(type, "real") == 0 || strcmp(type, "Real") == 0 ||
       strcmp(type, "boolean") == 0 || strcmp(type, "Boolean") == 0 ||
       strcmp(type, "char") == 0 || strcmp(type, "Char") == 0)
        return 1;
    else
        return 0;
}
