#include <typeinfo>
#include "legalizeCNames.h"
#include "stringutil.h"
#include "symbol.h"



static void symbolSub(Symbol* sym, char illegalChar) {

  if (sym->name == sym->cname) {
    sym->cname = copystring(sym->name);
  }

  switch (illegalChar) {
  case '?':
    char* questionMark = strchr(sym->cname, '?');
    while (questionMark) {
      *questionMark = 'Q';
      questionMark = strchr(sym->cname, '?');
    }
    break;

  case '-':
    char* dash = strchr(sym->cname, '-');
    while (dash) {
      *dash = '_';
      dash = strchr(sym->cname, '-');
    }
    break;
  }
}


void LegalizeCNames::processSymbol(Symbol* sym) {
  char* questionMark = strchr(sym->cname, '?');
  char* dash = strchr(sym->cname, '-');
  
  if (questionMark) {
    symbolSub(sym, *questionMark);
  } else if (dash) {
    symbolSub(sym, *dash);
  }
}

