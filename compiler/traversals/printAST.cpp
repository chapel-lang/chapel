#include "printAST.h"
#include "stmt.h"

PrintAST::PrintAST(void) :
  Traversal(true, false)
{}


void PrintAST::preProcessStmt(Stmt* &stmt) {
  stmt->print(stdout);
}
