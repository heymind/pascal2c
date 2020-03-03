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
"program"                   {yylval.text = sdsnew(yytext);  return T_PROGRAM; }
"var"                       {yylval.text = sdsnew(yytext);  return T_VAR; }
"integer"|"boolean"|"char"  {yylval.text = sdsnew(yytext);  return T_BASIC_TYPE; }
"procedure"                 {yylval.text = sdsnew(yytext);  return T_PROCEDURE; }
"function"                  {yylval.text = sdsnew(yytext);  return T_FUNCTION; }
"begin"                     {yylval.text = sdsnew(yytext);  return T_BEGIN; }
"end"                       {yylval.text = sdsnew(yytext);  return T_END; }
"writeInt"                  {yylval.text = sdsnew(yytext);  return T_WRITE_INT; }
"writeBool"                 {yylval.text = sdsnew(yytext);  return T_WRITE_BOOL; }
"writeChar"                 {yylval.text = sdsnew(yytext);  return T_WRITE_CHAR; }
"writeLn"                   {yylval.text = sdsnew(yytext);  return T_WRITE_LN; }
"if"                        {yylval.text = sdsnew(yytext);  return T_IF; }
"then"                      {yylval.text = sdsnew(yytext);  return T_THEN; }
"else"                      {yylval.text = sdsnew(yytext);  return T_ELSE; }
"while"                     {yylval.text = sdsnew(yytext);  return T_WHILE; }
"for"                       {yylval.text = sdsnew(yytext);  return T_FOR; }
"to"                        {yylval.text = sdsnew(yytext);  return T_TO; }
"do"                        {yylval.text = sdsnew(yytext);  return T_DO; }
":="                        {yylval.text = sdsnew(yytext);  return T_ASSIGNOP; }
"("                         {yylval.text = sdsnew(yytext);  return T_OBRACKET; }
")"                         {yylval.text = sdsnew(yytext);  return T_CBRACKET; }
"+"                         {yylval.text = sdsnew(yytext);   return T_PLUS; }
"-"                         {yylval.text = sdsnew(yytext);   return T_MINUS; }
"*"                         {yylval.text = sdsnew(yytext);   return T_MUL; }
"/"                         {yylval.text = sdsnew(yytext);   return T_DIV; }
"mod"                       {yylval.text = sdsnew(yytext);   return T_MOD; }
"and"                       {yylval.text = sdsnew(yytext);   return T_AND; }
"or"                        {yylval.text = sdsnew(yytext);   return T_OR; }
"not"                       {yylval.text = sdsnew(yytext);   return T_NOT; }
">"                         {yylval.text = sdsnew(yytext);   return T_CGT; }
"<"                         {yylval.text = sdsnew(yytext);   return T_CLT; }
"="                         {yylval.text = sdsnew(yytext);   return T_CEQ; }
"<>"                        {yylval.text = sdsnew(yytext);   return T_CNE; }
">="                        {yylval.text = sdsnew(yytext);   return T_CGE; }
"<="                        {yylval.text = sdsnew(yytext);   return T_CLE; }
";"                         {yylval.text = sdsnew(yytext);  return T_SEMICOLON; }
":"                         {yylval.text = sdsnew(yytext);  return T_COLON; }
"."                         {yylval.text = sdsnew(yytext);  return T_DOT; }
","                         {yylval.text = sdsnew(yytext);  return T_COMMA; }
[0-9]+                      {yylval.text = sdsnew(yytext);  return T_NUM; }
[A-Za-z][A-Za-z0-9_]*       {yylval.text = sdsnew(yytext);   return T_ID; }
[ \t]+                      /* eat all whitespaces */;
.                           { fprintf (stderr, "Syntax Error: unexpected character '%s'\n", yytext); }

%%