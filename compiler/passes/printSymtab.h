#ifndef _PRINTSYMTAB_H_
#define _PRINTSYMTAB_H_

#include "traversal.h"

class PrintSymtab : public Traversal {
 public:
  void run(Stmt* program);
};

#endif
