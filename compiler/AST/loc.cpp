#include "loc.h"
#include "yy.h"

Loc::Loc(void) :
  filename(yyfilename), 
  lineno(yylineno)
{}

/*
void user_error(Loc* loc, char* control, ...) {
  char linenostr[32];
  char* locstr;

  if (loc) {
    sprintf(linenostr, "%d", loc->lineno);
    locstr = glomstrings(5, loc->filename, ":", linenostr, ": %%s");
  } else {
    locstr = "???:???: %%s";
  }

  
}
*/
