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
[Pp][Rr][Oo][Gg][Rr][Aa][Mm]                              {lex_column_num += yyleng; return T_PROGRAM    ; }
[Cc][Oo][Nn][Ss][Tt]                                      {lex_column_num += yyleng; return T_CONST      ; }
[Vv][Aa][Rr]                                              {lex_column_num += yyleng; return T_VAR        ; }
[Ii][Nn][Tt][Ee][Gg][Ee][Rr]|[Bb][Oo][Oo][Ll][Ee][Aa][Nn] {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_BASIC_TYPE ; }
[Rr][Ee][Aa][Ll]|[Cc][Hh][Aa][Rr]                         {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_BASIC_TYPE ; }
[Aa][Rr][Rr][Aa][Yy]                                      {lex_column_num += yyleng; return T_ARRAY      ; }
[Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee]                      {lex_column_num += yyleng; return T_PROCEDURE  ; }
[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]                          {lex_column_num += yyleng; return T_FUNCTION   ; }
[Bb][Ee][Gg][Ii][Nn]                                      {lex_column_num += yyleng; return T_BEGIN      ; }
[Ee][Nn][Dd]                                              {lex_column_num += yyleng; return T_END        ; }
[Oo][Ff]                                                  {lex_column_num += yyleng; return T_OF         ; }
[Ii][Ff]                                                  {lex_column_num += yyleng; return T_IF         ; }
[Tt][Hh][Ee][Nn]                                          {lex_column_num += yyleng; return T_THEN       ; }
[Ee][Ll][Ss][Ee]                                          {lex_column_num += yyleng; return T_ELSE       ; }
[Ww][Hh][Ii][Ll][Ee]                                      {lex_column_num += yyleng; return T_WHILE      ; }
[Ff][Oo][Rr]                                              {lex_column_num += yyleng; return T_FOR        ; }
[Tt][Oo]                                                  {lex_column_num += yyleng; return T_TO         ; }
[Dd][Oo]                                                  {lex_column_num += yyleng; return T_DO         ; }
":="                  {lex_column_num += 2; return T_ASSIGNOP ; }
"("                   {lex_column_num += 1; return T_OBRACKET ; }
")"                   {lex_column_num += 1; return T_CBRACKET ; }
"["                   {lex_column_num += 1; return T_SOBRACKET; }
"]"                   {lex_column_num += 1; return T_SCBRACKET; }
"+"                   {lex_column_num += 1; return T_PLUS     ; }
"-"                   {lex_column_num += 1; return T_MINUS    ; }
"*"                   {lex_column_num += 1; return T_MUL      ; }
"/"|[Dd][Ii][Vv]      {lex_column_num += 1; return T_DIV      ; }
"mod"                 {lex_column_num += 3; return T_MOD      ; }
"and"                 {lex_column_num += 3; return T_AND      ; }
"or"                  {lex_column_num += 2; return T_OR       ; }
"not"                 {lex_column_num += 3; return T_NOT      ; }
">"                   {lex_column_num += 1; return T_CGT      ; }
"<"                   {lex_column_num += 1; return T_CLT      ; }
"="                   {lex_column_num += 1; return T_CEQ      ; }
"<>"                  {lex_column_num += 2; return T_CNE      ; }
">="                  {lex_column_num += 2; return T_CGE      ; }
"<="                  {lex_column_num += 2; return T_CLE      ; }
";"                   {lex_column_num += 1; return T_SEMICOLON; }
":"                   {lex_column_num += 1; return T_COLON    ; }
"."                   {lex_column_num += 1; return T_DOT      ; }
","                   {lex_column_num += 1; return T_COMMA    ; }
[0-9]+                {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_NUM      ; }
[A-Za-z][A-Za-z0-9_]* {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_ID       ; }
[0-9]+|[0-9]+\.[0-9]+ {yylval.text = sdsnew(yytext); lex_column_num += yyleng; return T_REAL     ; }
" "|"\t"              {lex_column_num++;}
"\n"|"\r"|[\r][\n]|[\n][\r]       {lex_column_num = 0; lex_row_num = yylineno;}
.                           { fprintf (stderr, "Lexical Error: unexpected character '%c' at line: %d colum: %d \n", *yytext, yylineno, lex_column_num); }

%%