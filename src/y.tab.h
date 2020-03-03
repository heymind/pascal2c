/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START = 258,
     END = 259,
     PROGRAM = 260,
     VAR = 261,
     INTEGER = 262,
     REAL = 263,
     WHILE = 264,
     DO = 265,
     IF = 266,
     THEN = 267,
     IDENTIFIER = 268,
     ELSE = 269,
     AND = 270,
     OR = 271,
     NOT = 272,
     ARRAY = 273,
     OF = 274,
     REALNUM = 275,
     INTNUM = 276,
     LE = 277,
     GE = 278,
     NE = 279,
     ASSIGN = 280,
     DOTDOT = 281
   };
#endif
/* Tokens.  */
#define START 258
#define END 259
#define PROGRAM 260
#define VAR 261
#define INTEGER 262
#define REAL 263
#define WHILE 264
#define DO 265
#define IF 266
#define THEN 267
#define IDENTIFIER 268
#define ELSE 269
#define AND 270
#define OR 271
#define NOT 272
#define ARRAY 273
#define OF 274
#define REALNUM 275
#define INTNUM 276
#define LE 277
#define GE 278
#define NE 279
#define ASSIGN 280
#define DOTDOT 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

