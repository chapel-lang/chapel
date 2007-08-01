#ifndef _YY_H_
#define _YY_H_

#include "stmt.h"
#include "symbol.h"

extern Symbol* yypst;
extern AList* yystmtlist;
extern int yystartlineno;

extern FILE *yyin;

extern char* yytext;
extern int chplLineno;
extern const char* yyfilename;
  
int yyparse(void);
void yyerror(const char* str);
int getNextYYChar();

extern int yylex(void);
extern int yydebug;


#endif
