%{
#include <stdio.h>
#include "sds.h"
#include "ast.h"
#include "y.tab.h"
YYSTYPE yylval;
%}

%option yylineno
%option noyywrap
%option nounput

%%
[Pp][Rr][Oo][Gg][Rr][Aa][Mm]                                               {yylval.text = sdsnew(yytext); return T_PROGRAM   ; }
[Cc][Oo][Nn][Ss][Tt]                                                       {yylval.text = sdsnew(yytext); return T_CONST   ; }
[Vv][Aa][Rr]                                                               {yylval.text = sdsnew(yytext); return T_VAR       ; }
[Ii][Nn][Tt][Ee][Gg][Ee][Rr]|[Cc][Hh][Aa][Rr]|[Bb][Oo][Oo][Ll][Ee][Aa][Nn] {yylval.text = sdsnew(yytext); return T_BASIC_TYPE; }
[Aa][Rr][Rr][Aa][Yy]                                                       {yylval.text = sdsnew(yytext); return T_ARRAY     ; }
[Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee]                                       {yylval.text = sdsnew(yytext); return T_PROCEDURE ; }
[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]                                           {yylval.text = sdsnew(yytext); return T_FUNCTION  ; }
[Bb][Ee][Gg][Ii][Nn]                                                       {yylval.text = sdsnew(yytext); return T_BEGIN     ; }
[Ee][Nn][Dd]                                                               {yylval.text = sdsnew(yytext); return T_END       ; }
[Oo][Ff]                                                                   {yylval.text = sdsnew(yytext); return T_OF        ; }
[Ii][Ff]                                                                   {yylval.text = sdsnew(yytext); return T_IF        ; }
[Tt][Hh][Ee][Nn]                                                           {yylval.text = sdsnew(yytext); return T_THEN      ; }
[Ee][Ll][Ss][Ee]                                                           {yylval.text = sdsnew(yytext); return T_ELSE      ; }
[Ww][Hh][Ii][Ll][Ee]                                                       {yylval.text = sdsnew(yytext); return T_WHILE     ; }
[Ff][Oo][Rr]                                                               {yylval.text = sdsnew(yytext); return T_FOR       ; }
[Tt][Oo]                                                                   {yylval.text = sdsnew(yytext); return T_TO        ; }
[Dd][Oo]                                                                   {yylval.text = sdsnew(yytext); return T_DO        ; }
":="                  {yylval.text = sdsnew(yytext); return T_ASSIGNOP ; }
"("                   {yylval.text = sdsnew(yytext); return T_OBRACKET ; }
")"                   {yylval.text = sdsnew(yytext); return T_CBRACKET ; }
"["                   {yylval.text = sdsnew(yytext); return T_SOBRACKET; }
"]"                   {yylval.text = sdsnew(yytext); return T_SCBRACKET; }
"+"                   {yylval.text = sdsnew(yytext); return T_PLUS     ; }
"-"                   {yylval.text = sdsnew(yytext); return T_MINUS    ; }
"*"                   {yylval.text = sdsnew(yytext); return T_MUL      ; }
"/"                   {yylval.text = sdsnew(yytext); return T_DIV      ; }
"mod"                 {yylval.text = sdsnew(yytext); return T_MOD      ; }
"and"                 {yylval.text = sdsnew(yytext); return T_AND      ; }
"or"                  {yylval.text = sdsnew(yytext); return T_OR       ; }
"not"                 {yylval.text = sdsnew(yytext); return T_NOT      ; }
">"                   {yylval.text = sdsnew(yytext); return T_CGT      ; }
"<"                   {yylval.text = sdsnew(yytext); return T_CLT      ; }
"="                   {yylval.text = sdsnew(yytext); return T_CEQ      ; }
"<>"                  {yylval.text = sdsnew(yytext); return T_CNE      ; }
">="                  {yylval.text = sdsnew(yytext); return T_CGE      ; }
"<="                  {yylval.text = sdsnew(yytext); return T_CLE      ; }
";"                   {yylval.text = sdsnew(yytext); return T_SEMICOLON; }
":"                   {yylval.text = sdsnew(yytext); return T_COLON    ; }
"."                   {yylval.text = sdsnew(yytext); return T_DOT      ; }
","                   {yylval.text = sdsnew(yytext); return T_COMMA    ; }
[0-9]+                {yylval.text = sdsnew(yytext); return T_NUM      ; }
[A-Za-z][A-Za-z0-9_]* {yylval.text = sdsnew(yytext); return T_ID       ; }
[0-9]+|[0-9]+\.[0-9]+ {yylval.text = sdsnew(yytext); return T_REAL     ; }
[ \t]+                      /* eat all whitespaces */;
.                           { fprintf (stderr, "Syntax Error: unexpected character '%s'\n", yytext); }

%%