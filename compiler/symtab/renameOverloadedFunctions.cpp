#include "renameOverloadedFunctions.h"
#include "stringutil.h"

static char* munger = "_user_overload_";

static bool shouldRenameOverload(Symbol* fn) {
  if (strstr(fn->cname, munger)) {
    return false;
  }
  if (fn == FnSymbol::mainFn) {
    return false;
  }
  return true;
}


void RenameOverloadedFunctions::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->overload) {
      Symbol* tmp = fn;

      while (tmp) {
        if (dynamic_cast<FnSymbol*>(tmp) && shouldRenameOverload(tmp)) {
          tmp->cname = glomstrings(3, tmp->cname, munger, intstring(tmp->id));
        }
        tmp = tmp->overload;
      }
    }
  }
}
