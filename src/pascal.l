%{
#include <stdio.h>
#include "sds.h"
#include "ast.h"
#include "y.tab.h"
YYSTYPE yylval;
unsigned int lex_column_num = 0;
unsigned int lex_row_num = 1;
%}

%option yylineno
%option noyywrap
%option nounput

%%
[Pp][Rr][Oo][Gg][Rr][Aa][Mm]                                                    {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_PROGRAM   ; }
[Cc][Oo][Nn][Ss][Tt]                                                            {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_CONST   ; }
[Vv][Aa][Rr]                                                                    {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_VAR       ; }
[Ii][Nn][Tt][Ee][Gg][Ee][Rr]|[Bb][Oo][Oo][Ll][Ee][Aa][Nn]|[Cc][Hh][Aa][Rr]      {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_BASIC_TYPE; }
[Aa][Rr][Rr][Aa][Yy]                                                            {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_ARRAY     ; }
[Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee]                                            {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_PROCEDURE ; }
[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]                                                {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_FUNCTION  ; }
[Bb][Ee][Gg][Ii][Nn]                                                            {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_BEGIN     ; }
[Ee][Nn][Dd]                                                                    {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_END       ; }
[Oo][Ff]                                                                        {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_OF        ; }
[Ii][Ff]                                                                   {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_IF        ; }
[Tt][Hh][Ee][Nn]                                                           {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_THEN      ; }
[Ee][Ll][Ss][Ee]                                                           {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_ELSE      ; }
[Ww][Hh][Ii][Ll][Ee]                                                       {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_WHILE     ; }
[Ff][Oo][Rr]                                                               {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_FOR       ; }
[Tt][Oo]                                                                   {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_TO        ; }
[Dd][Oo]                                                                   {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_DO        ; }
":="                  {yylval.text = sdsnew(yytext); lex_column_num += 2; return T_ASSIGNOP ; }
"("                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_OBRACKET ; }
")"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_CBRACKET ; }
"["                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_SOBRACKET; }
"]"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_SCBRACKET; }
"+"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_PLUS     ; }
"-"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_MINUS    ; }
"*"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_MUL      ; }
"/"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_DIV      ; }
"mod"                 {yylval.text = sdsnew(yytext); lex_column_num += 3; return T_MOD      ; }
"and"                 {yylval.text = sdsnew(yytext); lex_column_num += 3; return T_AND      ; }
"or"                  {yylval.text = sdsnew(yytext); lex_column_num += 2; return T_OR       ; }
"not"                 {yylval.text = sdsnew(yytext); lex_column_num += 3; return T_NOT      ; }
">"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_CGT      ; }
"<"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_CLT      ; }
"="                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_CEQ      ; }
"<>"                  {yylval.text = sdsnew(yytext); lex_column_num += 2; return T_CNE      ; }
">="                  {yylval.text = sdsnew(yytext); lex_column_num += 2; return T_CGE      ; }
"<="                  {yylval.text = sdsnew(yytext); lex_column_num += 2; return T_CLE      ; }
";"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_SEMICOLON; }
":"                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_COLON    ; }
"."                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_DOT      ; }
","                   {yylval.text = sdsnew(yytext); lex_column_num += 1; return T_COMMA    ; }
[0-9]+                {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_NUM      ; }
[A-Za-z][A-Za-z0-9_]* {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_ID       ; }
[0-9]+|[0-9]+\.[0-9]+ {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_REAL     ; }
" "|"\t"              {lex_column_num++;}
"\n"|"\r"|[\r][\n]|[\n][\r]       {lex_column_num = 0; lex_row_num = yylineno;}
.                           { fprintf (stderr, "Lexical Error: unexpected character '%c' at line: %d colum: %d \n", *yytext, yylineno, lex_column_num); }

%%