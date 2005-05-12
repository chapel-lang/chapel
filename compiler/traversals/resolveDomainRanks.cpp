#include "resolveDomainRanks.h"
#include "type.h"


ResolveDomainRanks::ResolveDomainRanks(void) {
  whichModules = MODULES_CODEGEN;
}


void ResolveDomainRanks::preProcessType(Type* type) {
  DomainType* domainType = dynamic_cast<DomainType*>(type);
  if (domainType) {
    domainType->computeRank();
  }
}
