#include "loc.h"
#include "yy.h"

Loc::Loc(void) {
  filename = yyfilename;
  lineno = yylineno;
}
