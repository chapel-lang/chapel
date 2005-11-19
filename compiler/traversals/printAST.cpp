#include "printAST.h"
#include "stmt.h"
#include "symtab.h"

PrintAST::PrintAST(void) :
  Traversal(true, false)
{}


void PrintAST::preProcessStmt(Stmt* stmt) {
  stmt->print(stdout);
}


void printAST(void) {
  Pass* pass = new PrintAST();
  pass->run(Symboltable::getModules(pass->whichModules));
}
