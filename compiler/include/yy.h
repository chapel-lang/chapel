/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _YY_H_
#define _YY_H_

#include "stmt.h"
#include "symbol.h"

extern BlockStmt* yyblock;
extern int yystartlineno;

extern FILE *yyin;

extern char* yytext;
extern int chplLineno;
extern bool chplParseString;
extern const char *chplParseStringMsg;
extern const char* yyfilename;
  
int yyparse(void);
void yyerror(const char* str);
int getNextYYChar();
BlockStmt* parseString(const char* string, const char* filename, 
                       const char* msg);

int yylex(void);
extern int yydebug;

enum ProcIter {
  ProcIter_PROC,
  ProcIter_ITER
};

#endif
