%{
#include <stdio.h>
#include <string.h>
extern int yylex();
//void yyerror(const char *str){
//    fprintf(stderr,"error:%s\n",str);
//}

//main()
//{
//    yyparse();
//}

%}

%token BEGIN END PROGRAM VAR INTEGER REAL WHILE DO IF THEN IDENTIFIER
%token ELSE AND OR NOT ARRAY OF REALNUM INTNUM LE GE NE ASSIGN DOTDOT