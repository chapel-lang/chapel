#include <stdio.h>
#include "yy.h"


char* yyinfile = "foo.chpl";
int yylineno = 1;


void yyerror(const char *str) {
  fprintf(stderr, "error on line %d: %s\n", yylineno, str);
  fprintf(stderr, "yytext is: %s\n", yytext);
  exit(1);
}

int yywrap() {
  return 1;
}
