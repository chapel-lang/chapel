#include <stdlib.h>
#include "expr.h"
#include "getstuff.h"
#include "stmt.h"
#include "testGetStuff.h"

void TestGetStuff::preProcessStmt(Stmt* stmt) {
  int i;
  GetStuff* asts = new GetStuff();

  asts->clear();
  stmt->traverse(asts);
  fprintf(stderr, "\n\n\nstmt at line %d:\n", stmt->lineno);
  for (i = 0; i<asts->stmts.n; i++) {
    fprintf(stderr, "stmt %d ----------------------------------------\n", i);
    asts->stmts.v[i]->print(stderr);
    fprintf(stderr, "\n-----------------------------------------------\n");
  }
  for (i = 0; i<asts->exprs.n; i++) {
    fprintf(stderr, "expr %d ----------------------------------------\n", i);
    asts->exprs.v[i]->print(stderr);
    fprintf(stderr, "\n-----------------------------------------------\n", i);
  }
  for (i = 0; i<asts->symbols.n; i++) {
    fprintf(stderr, "symbol %d ----------------------------------------\n", i);
    asts->symbols.v[i]->print(stderr);
    fprintf(stderr, "\n-----------------------------------------------\n");
  }
  for (i = 0; i<asts->types.n; i++) {
    fprintf(stderr, "type %d ----------------------------------------\n", i);
    asts->types.v[i]->print(stderr);
    fprintf(stderr, "\n-----------------------------------------------\n");
  }
}


void testGetStuff(Stmt* program) {
  program->traverseList(new TestGetStuff());
  exit(0);
}
