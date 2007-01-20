#ifndef _YY_H_
#define _YY_H_

#ifdef __cplusplus

#include "stmt.h"
#include "symbol.h"

extern Symbol* yypst;
extern AList* yystmtlist;
extern int yystartlineno;

extern "C" {
#endif
  extern FILE *yyin;

  extern char* yytext;
  extern int chplLineno;
  extern char* yyfilename;
  
  int yyparse(void);
  void yyerror(const char* str);
  int yywrap(void);
  int getNextYYChar();

#ifdef __cplusplus
}
#endif

extern int yylex(void);
extern int yydebug;


#endif
