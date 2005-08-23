#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"

void reportAnalysisErrors(Vec<AError*>* analysis_errors) {
  USR_WARNING("Analysis errors detected, scroll down for analysis messages");
  forv_Vec(AError, error, *analysis_errors) {
    USR_FATAL_CONT("High-level error message");
  }
  USR_WARNING("Analysis messages follow");
}
