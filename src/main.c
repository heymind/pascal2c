#include <stdio.h>
#include "utlist.h"
#include "ast.h"
#include "codegen.h"
extern FILE* yyin;
int yyparse();
ASTNode *root = NULL;

ASTNode*  _ast_node_create(const char *type){
    ASTNodePos *pos = ast_node_pos_create(1,2,3,4);
    return ast_node_create(type,pos);
}

int main(int argc, char* argv[]) {
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
    generate_c_code(root,stdout);
//        ASTNode *program_node = _ast_node_create("PROGRAM");
//        ast_node_set_attr_str(program_node, "NAME", sdsnew("loop2"));
//
//        ast_node_dump_json(program_node,"step0.json"); // 将现在的语法树输出为 json 文件，便于调试
//
//        ASTNode *var_decl = _ast_node_create("VAR_DECL");
//        ast_node_set_attr_const_str(var_decl, "TYPE", "INTEGER");
//        ast_node_set_attr_str(var_decl, "NAME", sdsnew("i"));
//
////    ASTNodeAttr *var_decls = ast_node_attr_create_node("VAR_DECLS", var_decl);
////    ast_node_attach_attr(program_node, var_decls);
//        ast_node_attr_node_append(program_node,"VAR_DECLS",var_decl);
//        ast_node_dump_json(program_node,"step1.json");
//
//        var_decl = _ast_node_create("VAR_DECL");
//        ast_node_set_attr_const_str(var_decl, "TYPE", "INTEGER");
//        ast_node_set_attr_str(var_decl, "NAME", sdsnew("x"));
//        ast_node_attr_node_append(program_node,"VAR_DECLS",var_decl);
//
//        ast_node_dump_json(program_node,"step3.json");
//
//        ASTNode *assign = _ast_node_create("ASSIGN"),
//        *id_i = _ast_node_create("IDENT"),
//        *id_x = _ast_node_create("IDENT");
//
//        ast_node_set_attr_str(id_i, "NAME", sdsnew("i"));
//        ast_node_set_attr_str(id_x, "NAME", sdsnew("x"));
//
//
//        ASTNodeAttr *assign_left = ast_node_attr_create_node("LEFT", id_i),
//        *assign_right = ast_node_attr_create_node("RIGHT", id_x);
//
//        ast_node_attach_attr(assign, assign_left);
//        ast_node_attach_attr(assign, assign_right);
//
////    ASTNodeAttr *main_stmts = ast_node_attr_create_node("MAIN_STMTS", assign);
////    ast_node_attach_attr(program_node, main_stmts);
//        ast_node_attr_node_append(program_node,"MAIN_STMTS",assign);
//
//        ast_node_dump_json(program_node,"step4.json");
//
//
//        assign = _ast_node_create("ASSIGN");
//        id_i = _ast_node_create("IDENT");
//        id_x = _ast_node_create("IDENT");
//
//        ast_node_set_attr_str(id_i, "NAME", sdsnew("i"));
//        ast_node_set_attr_str(id_x, "NAME", sdsnew("x"));
//
//
//        assign_left = ast_node_attr_create_node("LEFT", id_x);
//        assign_right = ast_node_attr_create_node("RIGHT", id_i);
//
//        ast_node_attach_attr(assign, assign_left);
//        ast_node_attach_attr(assign, assign_right);
//
//        ast_node_attr_node_append(program_node,"MAIN_STMTS",assign);
//
//        ast_node_dump_json(program_node,"step5.json");
}

int yyerror(char *errstr)
{
    printf(" %s\n", errstr);
    return 0;
}