#include "error.h"

void error_pos_print() {
    char pos_str[200] = "";
    int column = lex_column_num;
    int row = lex_row_num;
    char str[50];
    strcat(pos_str, "(Line ");
    sprintf(str, "%d", row);
    strcat(pos_str, str);
    strcat(pos_str, ", Column ");
    sprintf(str, "%d", column);
    strcat(pos_str, str);
    strcat(pos_str, ") ");
    printf("%s",pos_str);
}

void error_with_pos(char * error_str) {
    error_pos_print();
    printf("%s\n",error_str);
}

void print_error(char *errstr) {
    char unexpected_type[TYPELEN];
    char expecting_types[TYPENUM][TYPELEN];
    error_string_devider(unexpected_type, expecting_types, errstr);

    printf("(Ln %d, Col %d): ", lex_row_num, lex_column_num);
    printf("syntax error, string \"%s\" (%s) unexpected\n", yytext, unexpected_type);

    printf("(Ln %d, Col %d): ", lex_row_num, lex_column_num);
    char *tmp = strstr(errstr, "expecting ");
    if (tmp == NULL) {
        printf("String \"arrstr\" has fetal error.\n");
    }
    printf("%s\n", tmp);
}

int error_string_devider(char unexpected_type[TYPELEN], char expecting_types[TYPENUM][TYPELEN], char *errstr) {
    char *tmp_start;
    char *tmp_end;
    char tmp[200];

    tmp_start = strstr(errstr, "unexpected ");
    if (tmp_start == NULL) {
        printf("String \"arrstr\" has fetal error.\n");
        return 0;
    }
    tmp_start += strlen("unexpected ");
    strcpy(tmp, tmp_start);
    tmp_end = strchr(tmp, ',');
    *tmp_end = '\0';
    strcpy(unexpected_type, tmp);

    return 0;
}