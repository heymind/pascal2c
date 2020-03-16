

#ifndef PASCAL2C_CODEGEN_H
#define PASCAL2C_CODEGEN_H

#include <stdio.h>
#include "ast.h"

void generate_c_code(ASTNode *root,FILE *out);
char *var_type_change(char *pascal_car_type);
char *func_name_change(char *pascal_func_name);
#endif //PASCAL2C_CODEGEN_H
