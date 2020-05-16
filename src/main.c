#include <stdio.h>
#include "utlist.h"
#include "ast.h"
#include "codegen.h"
#include "error.h"

extern FILE *yyin;

int yyparse();

ASTNode *root = NULL;

ASTNode *_ast_node_create(const char *type) {
    ASTNodePos *pos = ast_node_pos_create(1, 1, 1, 1);
    return ast_node_create(type, pos);
}

int main(int argc, char *argv[]) {
    yyin = stdin;
    root = ast_node_create_without_pos("PROGRAM");
    if (argc > 1) {
        if ((yyin = fopen(argv[1], "r")) == NULL) {
            printf("Can't open file %s\n", argv[1]);
            return -1;
        }
    } else {
        if ((yyin = fopen("../sample_pas/gcd.pas", "r")) == NULL) {
            printf("Can't open file %s\n", argv[1]);
            return -1;
        }
    }
    yyparse();
    //    printf("%s", root->first_attr->value);
    ast_node_dump_json(root, "step0.json");
    generate_c_code(root, stdout);

}

int yyerror(char *errstr) {
    /* 带行列号的原版报错信息 */
    // printf("(Ln %d, Col %d): ", lex_row_num, lex_column_num);
    // printf("%s\n", errstr);
    print_error(errstr);
}
