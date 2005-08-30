#include "computeVisibleFunctions.h"
#include "stmt.h"
#include "expr.h"


void ComputeVisibleFunctions::preProcessScope(SymScope* scope) {
  scope->setVisibleFunctions(NULL);
}
