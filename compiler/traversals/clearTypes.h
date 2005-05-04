#ifndef _CLEAR_TYPES_H_
#define _CLEAR_TYPES_H_

#include "traversal.h"
#include "baseAST.h"
#include "vec.h"

#include "map.h"
#include "baseAST.h"

/***
 *** This traversal is called by clone to make the types of defined
 *** symbols unknown. They will be changed (the whole point of
 *** cloning, and need to be resolved after analysis is finished).
 ***/
class ClearTypes : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

#endif
