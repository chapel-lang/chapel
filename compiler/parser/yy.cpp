#include <cstdio>
#include <cstdlib>
#include "yy.h"


void yyerror(const char *str) {
  fprintf(stderr, "%s:%d %s", yyfilename, chplLineno, str);
  if (strlen(yytext) > 0) {
    fprintf(stderr, ": near '%s'", yytext);
  }
  fprintf(stderr, "\n");
  clean_exit(1);
}
