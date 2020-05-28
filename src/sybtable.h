//
// Created by Jinyuan Zhang on 2020/5/10.
//

#ifndef PASCAL2C_SYBTABLE_H
#define PASCAL2C_SYBTABLE_H

#include <stdio.h>
#include "ast.h"

enum symbol_type {
    T_integer = 1,
    T_real,
    T_boolean,
    T_char,
    T_array,
    T_procedure,
    T_function
};

struct symbol{
    const char *name;
    enum symbol_type type;
    int dimension;
    const char *scope;
    struct symbol *next;
};

<<<<<<< Updated upstream
void do_ge_table(ASTNode *node, FILE *out);
void add_symbol(char *name,enum symbol_type type,int dimension,char *scope,struct symbol *table);
struct symbol* get_symbol(char *name, char *scope,struct symbol *table);
void ge_syb_table(ASTNode *root, FILE *out);
=======

void do_ge_table(ASTNode *node);
void add_symbol(char *name, char *type, int dimension, char *scope, int is_arg,int ref);
struct symbol* get_symbol(char *name, char *scope);
void print_symbol_table();
void ge_syb_table(ASTNode *root);
void destroy_syb_table();
>>>>>>> Stashed changes

#endif //PASCAL2C_SYBTABLE_H
