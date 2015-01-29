#ifndef _TRANSFORM_LOGICAL_SHORT_CIRCUIT_H_
#define _TRANSFORM_LOGICAL_SHORT_CIRCUIT_H_

/************************************ | *************************************
*                                                                           *
* This transformer is responsible for converting any uses of binary logical *
* AND or binary logical OR into an equivalent form using IF statements that *
* implements the required short-circuit semantics.                          *
*                                                                           *
* These binary operators cannot be expressed using Chapel primitives as     *
* these evaluate both arguments before invoking the operator.  Hence this   *
* transformation must be applied before function resolution.  In the        *
* current implementation this Visitor is leveraged early in normalize().    *
*                                                                           *
* This transfom inserts Chapel if-expressions that must be in the same      *
* scope as the boolean operators.  Hence the transformer requires that a    *
* suitable insertion point be provided to the constructor.                  *
*                                                                           *
************************************* | ************************************/

#include "AstVisitorTraverse.h"

class Expr;

class TransformLogicalShortCircuit : public AstVisitorTraverse
{
public:
                 TransformLogicalShortCircuit(Expr* insertionPoint);
  virtual       ~TransformLogicalShortCircuit();

  // Transform performed post-order
  virtual void   exitCallExpr (CallExpr* node);

private:
                 TransformLogicalShortCircuit();

  Expr*          mInsertionPoint;
};

#endif
