#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "link.h"
#include "symbol.h"

class Symboltable : public Link {
 public:
  Symboltable(void);

  static void define(Symbol* sym);
  static Symbol* lookup(char* name);
};

#endif

