#include "baseAST.h"

BaseAST::BaseAST(void) {
  INT_FATAL("Base AST must be given type");
}


BaseAST::BaseAST(astType_t type) :
  astType(type)
{}

int
BaseAST::getBaseASTs(Vec<BaseAST *> &asts) {
  int r = getSymbolTypes(asts);
  return r + getStmtExprs(asts);
}

int
BaseAST::getStmtExprs(Vec<BaseAST *> &asts) {
  int r = getStmts(asts);
  return r + getExprs(asts);
}

int
BaseAST::getSymbolTypes(Vec<BaseAST *> &asts) {
  int r = getSymbols(asts);
  return r + getTypes(asts);
}

int
BaseAST::getSymbols(Vec<BaseAST *> &asts) {
  return 0;
}

int
BaseAST::getTypes(Vec<BaseAST *> &asts) {
  return 0;
}

int
BaseAST::getStmts(Vec<BaseAST *> &asts) {
  return 0;
}

int
BaseAST::getExprs(Vec<BaseAST *> &asts) {
  return 0;
}
