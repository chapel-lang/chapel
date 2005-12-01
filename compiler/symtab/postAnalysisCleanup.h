#ifndef _POST_ANALYSIS_CLEANUP_H_
#define _POST_ANALYSIS_CLEANUP_H_

#include "symtabTraversal.h"

class PostAnalysisCleanup : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

void postAnalysisCleanup(void);

#endif
