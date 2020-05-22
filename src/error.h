#include <stdio.h>
#include "string.h"
#include "utlist.h"
#include "ast.h"

#define TYPELEN 20
#define TYPENUM 10
extern FILE *yyin;

int yyparse();

extern char *yytext;
extern unsigned int lex_row_num;
extern unsigned int lex_column_num;

void error_pos_print();
void error_with_pos(char * error_str);
int error_string_devider(char unexpected_type[TYPELEN], char expecting_types[TYPENUM][TYPELEN], char *errstr);

void print_error(char *errstr);