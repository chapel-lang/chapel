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

extern int yylex(void);
extern int yydebug;

enum ProcIter {
  ProcIter_DEF, // ProcIter: remove after transition
  ProcIter_PROC,
  ProcIter_ITER
};

#endif
