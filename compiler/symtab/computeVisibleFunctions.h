#ifndef _COMPUTE_VISIBLE_FUNCTIONS_H_
#define _COMPUTE_VISIBLE_FUNCTIONS_H_

#include "symtabTraversal.h"

class ComputeVisibleFunctions : public SymtabTraversal {
 public:
  Vec<FnSymbol*> classMethods;
  void run(void);
  void preProcessScope(SymScope*);
};

#endif
