#ifndef _CLEANUP_H_
#define _CLEANUP_H_

#include "traversal.h"
#include "vec.h"
#include "baseAST.h"

class Cleanup : public Traversal {
 public:
  void run(ModuleSymbol* moduleList);
};

void call_cleanup(BaseAST* ast);
void call_cleanup_ls(BaseAST* ast);

#endif
