#ifndef _YY_H_
#define _YY_H_

#ifdef __cplusplus
#include "stmt.h"
extern Stmt* program;

extern "C" {
#endif
  extern FILE *yyin;

  extern char* yytext;
  extern char* yyinfile;
  extern int yylineno;
  
  int yyparse(void);
  void yyerror(const char* str);
  int yywrap(void);
#ifdef __cplusplus
}
#endif

extern int yylex(void);
extern int yydebug;

#endif
