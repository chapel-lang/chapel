#ifndef _CLEANUP_H_
#define _CLEANUP_H_

#include "traversal.h"
#include "vec.h"

class Cleanup : public Traversal {
 public:
  void run(ModuleSymbol* moduleList);
};

#endif
