//
// Created by Jinyuan Zhang on 2020/5/22.
//

#ifndef PASCAL2C_SYBTABLE_H
#define PASCAL2C_SYBTABLE_H

#include <stdio.h>
#include "ast.h"

//enum symbol_type {
//    T_integer = 1,
//    T_real,
//    T_boolean,
//    T_char,
//    T_array,
//    T_procedure,
//    T_function
//};

struct symbol{
    const char *name;
    const char *type;
    int dimension;
    const char *scope;
    struct symbol *next;
    int arg;//参数
    int ref;//引用方式 1->传值调用  2->引用调用
};


void do_ge_table(ASTNode *node);
void add_symbol(char *name, char *type, int dimension, char *scope, int arg,int ref);
struct symbol* get_symbol(char *name, char *scope);
struct symbol* get_symbol_by_arg(int arg, char *scope);
void print_symbol_table();
void ge_syb_table(ASTNode *root);

#endif //PASCAL2C_SYBTABLE_H
//#endif //PASCAL2C_SYBTABLE_H

