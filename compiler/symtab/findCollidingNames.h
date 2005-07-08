#ifndef _FIND_COLLIDING_NAMES_H_
#define _FIND_COLLIDING_NAMES_H_
#include "symtabTraversal.h"

extern bool _adhoc_to_uniform_mangling;

class FindCollidingNames : public SymtabTraversal {
public:
  void processSymbol(Symbol* sym);
};

#endif 
