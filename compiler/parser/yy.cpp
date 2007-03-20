#include <stdio.h>
#include <stdlib.h>
#include "yy.h"


void yyerror(const char *str) {
  fprintf(stderr, "%s:%d %s: near '%s'\n", yyfilename, chplLineno, str, yytext);
  exit(1);
}

int yywrap() {
  return 1;
}
