/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "analysis.h"
#include "expr.h"
#include "files.h"
#include "link.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

ASymbol::ASymbol() : symbol(0) {
}

AInfo::AInfo() : code(0) {
}

void
analyze_new_ast(Stmt* s) {
}
