#include <stdio.h>
#include "yy.h"


void yyerror(const char *str) {
  fprintf(stderr, "error on line %d: %s\n", yylineno, str);
  fprintf(stderr, "yytext is: %s\n", yytext);
  exit(1);
}

int yywrap() {
  return 1;
}
