%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    //初始化变量最大长度
    #define MAXSTR 200
    //用于记录行号
    int LINE_NUM = 1;
    //用于传递变量值
    char str[MAXSTR];
%}

/*
    词法分析，匹配规则
    注： pascal是大小写不敏感的语言
*/
%%

[Bb][Ee][Gg][iI][Nn]		    {return START;}
[Ee][Nn][Dd]					{return END;}
[Pp][Rr][oO][Gg][Rr][Aa][Mm]	{return PROGRAM;}
[Cc][Oo][Nn][Ss][Tt]            {return CONST;}
[Vv][Aa][Rr]					{return VAR;}
[Ii][nN][Tt][Ee][Gg][Ee][Rr]	{return INTEGER;}
[Rr][Ee][Aa][lL]			    {return REAL;}
[Bb][Oo][Oo][Ll][Ee][Aa][Nn]	{return BOOLEAN;}
[Cc][Hh][Aa][Rr]                {return CHAR;}
[Rr][Ee][Aa][lL]			    {return REAL;}
[Ww][Hh][Ii][Ll][Ee]		    {return WHILE;}
[Dd][oO]					    {return DO;}
[iI][Ff]					    {return IF;}
[Tt][Hh][Ee][Nn]	            {return THEN;}
[Ee][Ll][Ss][Ee]	            {return ELSE;}
[Aa][Nn][Dd]			        {return AND;}
[Oo][Rr]					    {return OR;}
[Nn][Oo][Tt]			        {return NOT;}
[Aa][Rr][Rr][Aa][Yy]	        {return ARRAY;}
[Oo][Ff]				        {return OF;}
([0-9]+(\.[0-9]*)?[Ee]([+-])?[0-9]+)|([0-9]*\.[0-9]+[Ee]([+-])?[0-9]+)	{;strcpy(str,yytext);return REALNUM;}
[0-9]+\.[0-9]*|[0-9]*\.[0-9]+	{strcpy(str,yytext);return REALNUM;}
[0-9]+							{strcpy(str,yytext);return INTNUM;}
"("			                    {return '(';}
")"		                    	{return ')';}
"["		                    	{return '[';}
"]"		                    	{return ']';}
">"		                    	{return '>';}
"<"		                    	{return '<';}
"="		                    	{return '=';}
"<="	                    	{return LE;}
">="	                    	{return GE;}
"<>"	                    	{return NE;}
":"		                    	{return ':';}
":="                            {return ASSIGN;}
";"		                    	{return ';';}
"\."                            {return '.';}
"\.\."		                    {return DOTDOT;}
"*"			                    {return '*';}
"+"		                    	{return '+';}
"-"		                    	{return '-';}
"/"		                    	{return '/';}
"%"		                    	{return '%';}
","		                    	{return ',';}
\n                              {LINE_NUM++;}
[\t\f\040]                          {;}
[a-zA-Z]([a-zA-Z0-9])+ 	        {strcpy(str,yytext);return IDENTIFIER;}
"(*"|"{"                        {
                                    int c;
                                    while((c = input())) {
                                        if (c == '}') break;
                                        else if (c == '*') {
                                           if ((c = input()) == ')') break;
                                           else unput(c);
                                        }
                                        else if (c == '\n') LINE_NUM++;
                                        else if (c == 0){
                                          printf("unexpected EOF at line: %d\n", LINE_NUM);exit(1);
                                        }
                                    }
                                }
"//"                            {
                                    int c;
                                    while((c = input())){
                                        if(c == '\n') break;
                                    }
                                    LINE_NUM++;
                                }
.						        {printf("Invalid char %s at line: %d\n", yytext, LINE_NUM);exit(1); }

%%

#ifndef yywrap
    int yywrap()
    {
        return 1;
    }
#endif
