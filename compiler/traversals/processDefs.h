#ifndef _PROCESS_DEFS_H_
#define _PROCESS_DEFS_H_

#include "traversal.h"

class ProcessDefs : public Traversal {
 public:
  ProcessDefs::ProcessDefs();
  void postProcessStmt(Stmt* stmt);
};

#endif
