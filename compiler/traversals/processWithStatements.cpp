#include "processWithStatements.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


/***
 ***  Copy with symbols into a class
 ***/


void ProcessWithStatements::preProcessStmt(Stmt* stmt) {
  WithStmt* with;

  if (with = dynamic_cast<WithStmt*>(stmt)) {
    

    printf("hi\n");
  }
}
