

#ifndef PASCAL2C_CODEGEN_H
#define PASCAL2C_CODEGEN_H

#include <stdio.h>
#include "ast.h"

void generate_c_code(ASTNode *root,FILE *out);

#endif //PASCAL2C_CODEGEN_H
