#include "renameOverloadedFunctions.h"
#include "stringutil.h"

static char* munger = "_user_overload_";

static bool shouldRenameOverload(FnSymbol* fnsym) {
  if (strstr(fnsym->cname, munger)) {
    return false;
  }
  if (fnsym == FnSymbol::mainFn) {
    return false;
  }
  return true;
}


void RenameOverloadedFunctions::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->overload) {
      FnSymbol* tmp = fn;

      while (tmp) {
	if (shouldRenameOverload(tmp)) {
	  tmp->cname = glomstrings(3, tmp->cname, munger, intstring(tmp->id));
	}
	tmp = tmp->overload;
      }
    }
  }
}
