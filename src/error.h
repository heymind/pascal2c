#include <stdio.h>
#include "string.h"
#include "utlist.h"
#include "ast.h"

#define TYPELEN 20
#define TYPENUM 10

extern FILE *yyin;

int yyparse();

extern char *yytext;
extern int lex_row_num;
extern int lex_column_num;

int error_string_devider(char unexpected_type[TYPELEN], char expecting_types[TYPENUM][TYPELEN], char *errstr);

void print_error(char *errstr);