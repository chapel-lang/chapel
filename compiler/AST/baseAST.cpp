#include "baseAST.h"

BaseAST::BaseAST(void) {
  INT_FATAL("Base AST must be given type");
}


BaseAST::BaseAST(astType_t type) :
  astType(type)
{}
