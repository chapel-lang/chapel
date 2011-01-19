#include <cstdio>
#include <cstdlib>
#include "yy.h"


void yyerror(const char *str) {
  if (!chplParseString) {
    fprintf(stderr, "%s:%d: %s", yyfilename, chplLineno, str);
    if (strlen(yytext) > 0) {
      fprintf(stderr, ": near '%s'", yytext);
    }
  } else {
    fprintf(stderr, "%s: %s", yyfilename, str);
    if (chplParseStringMsg && (strlen(chplParseStringMsg) > 0)) {
      fprintf(stderr, " %s", chplParseStringMsg);
    }
  }
  fprintf(stderr, "\n");
  clean_exit(1);
}
