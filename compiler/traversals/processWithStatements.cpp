#include "processWithStatements.h"
#include "chplalloc.h"
#include "defineSymbols.h"
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
  if (WithStmt* with = dynamic_cast<WithStmt*>(stmt)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (ClassType* ctype = dynamic_cast<ClassType*>(symType->type)) {
	Stmt* defStmt = with->getClass()->definition->copyList();
	with->preinsert(defStmt);
	// SJD: REMOVE STATEMENT "with" NOW
	DefineSymbols* defineSymbols = new DefineSymbols(ctype->scope);
	defStmt->traverseList(defineSymbols);
      }
      else {
	USR_FATAL(stmt, "With statement is not in a class definition");
      }
    }
    else {
      USR_FATAL(stmt, "With statement is not in a class type definition");
    }
  }
}
