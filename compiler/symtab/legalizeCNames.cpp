#include <typeinfo>
#include "legalizeCNames.h"
#include "stringutil.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"


struct charPair {
  char origChar;
  char newChar;
};

static charPair replacement[] = {
  {'?', 'Q'},
  {'-', '_'}, 

  {'\0', '\0'}  // This is a sentinel to mark the end of the array. 
};


static void symbolSub(Symbol* sym, int index) {
  if (sym->name == sym->cname) {
    sym->cname = copystring(sym->name);
  }
  int i = 0;
  for (i = index; replacement[i].origChar != '\0'; i++) {
    char* illegalChar = strchr(sym->cname, replacement[i].origChar);
    while (illegalChar) {
      *illegalChar = replacement[i].newChar;
      illegalChar = strchr(sym->cname, replacement[i].origChar);
    }
  }
}


static void applyRenamePragma(Symbol* sym) {
  if (sym->defPoint && sym->defPoint->parentStmt) {
    for (Pragma* pragma = sym->defPoint->parentStmt->pragmas;
         pragma;
         pragma = dynamic_cast<Pragma*>(pragma->next)) {
      if (!strncmp(pragma->str, "rename ", 7)) {
        sym->cname = copystring(pragma->str+7);
      }
    }
  }
}


void LegalizeCNames::processSymbol(Symbol* sym) {
  applyRenamePragma(sym);
  if (sym->parentScope->type != SCOPE_INTRINSIC) {
    int i = 0;
    for (i = 0; replacement[i].origChar != '\0'; i++) {
      char* illegalChar = strchr(sym->cname, replacement[i].origChar);
      if (illegalChar) {   
        // This function processes all illegal chars.
        symbolSub(sym, i);
        break;
      }
    }
  }
}

