#include <stdio.h>
#include "yy.h"


void yyerror(const char *str) {
  fprintf(stderr, "%s:%d ERROR: %s\n", yyfilename, yylineno, str);
  fprintf(stderr, "yytext is: %s\n", yytext);
  exit(1);
}

int yywrap() {
  return 1;
}
