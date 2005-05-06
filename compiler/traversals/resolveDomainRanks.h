#ifndef _RESOLVE_DOMAIN_RANKS_H_
#define _RESOLVE_DOMAIN_RANKS_H_

#include "traversal.h"

class ResolveDomainRanks : public Traversal {
 public:
  ResolveDomainRanks(void);
  void preProcessType(Type* type);
};

#endif
