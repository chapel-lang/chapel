#include "renameCSymbols.h"
#include "symbol.h"


void RenameCSymbols::run(ModuleList* moduleList) {
  // BLC: we could/should rename other things here as well -- types,
  // overloaded function names?  reserved C words?

  // BLC: or perhaps every symbol should have two names, a chapel name
  // and a C name, in which case they could just be created correctly
  // in the first place for most cases, and others could be munged here
  // or wherever is appropriate.
}
