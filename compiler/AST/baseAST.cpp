#include "baseAST.h"

static long uid = 1;

long BaseAST::getNumIDs(void) {
  return uid;
}

BaseAST::BaseAST(void) {
  INT_FATAL("Base AST must be given a type");
}


BaseAST::BaseAST(astType_t type) :
  astType(type),
  id(uid++)
{}
