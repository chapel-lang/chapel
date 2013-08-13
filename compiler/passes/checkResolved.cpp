// checkResolved.cpp

#include "passes.h"

#include "stmt.h"
#include "expr.h"
#include "astutil.h"

static int isDefinedAllPaths(Expr* expr, Symbol* ret);
static void checkReturnPaths(FnSymbol* fn);


void
checkResolved(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkReturnPaths(fn);
    if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
        !fn->hasFlag(FLAG_ITERATOR_FN) &&
        fn->retType->initializer->defPoint->parentSymbol == fn)
      USR_FATAL_CONT(fn, "functions cannot return nested iterators or loop expressions");
  }

  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (EnumType* et = toEnumType(type->type)) {
      for_enums(def, et) {
        if (def->init) {
          SymExpr* sym = toSymExpr(def->init);
          if (!sym || (!sym->var->hasFlag(FLAG_PARAM) &&
                       !toVarSymbol(sym->var)->immediate))
            USR_FATAL_CONT(def, "enumerator '%s' is not an integer param value", 
                           def->sym->name);
        }
      }
    }
  }
  checkNoRecordDeletes();
}


static int
isDefinedAllPaths(Expr* expr, Symbol* ret) {
  if (!expr)
    return 0;
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_MOVE) || call->isNamed("="))
      if (SymExpr* lhs = toSymExpr(call->get(1)))
        if (lhs->var == ret)
          return 1 + isDefinedAllPaths(expr->next, ret);
    //
    // should mark functions that exit rather than relying on string
    //
    if (call->isNamed("halt"))
      return 1 + isDefinedAllPaths(expr->next, ret);
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    if (!block->blockInfo ||
        block->blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP))
      if (int result = isDefinedAllPaths(block->body.head, ret))
        return result;
  } else if (isGotoStmt(expr)) {
    return 0;
  } else if (CondStmt* cond = toCondStmt(expr)) {
    if (isDefinedAllPaths(cond->thenStmt, ret) &&
        isDefinedAllPaths(cond->elseStmt, ret))
      return 1;
  }
  return isDefinedAllPaths(expr->next, ret);
}


static void
checkReturnPaths(FnSymbol* fn) {
  // Check to see if the function returns a value.
  if (fn->hasFlag(FLAG_ITERATOR_FN) ||
      !strcmp(fn->name, "=") ||
      !strcmp(fn->name, "chpl__buildArrayRuntimeType") ||
      fn->retType == dtVoid ||
      fn->retTag == RET_TYPE ||
      fn->hasFlag(FLAG_EXTERN) ||
      fn->hasFlag(FLAG_FUNCTION_PROTOTYPE) ||
      fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_AUTO_II))
    return; // No.

  // Check to see if the returned value is initialized.
  Symbol* ret = fn->getReturnSymbol();
  VarSymbol* var = toVarSymbol(ret);
  if (var)
  {
    // If it has an immediate initializer, it is initialized.
    if (var->immediate)
      return;
  }

  if (isEnumSymbol(ret))
    return;

  int result = isDefinedAllPaths(fn->body, ret);

  //
  // Issue a warning if there is a path that has zero definitions or
  // there is a path that has one definition and the function has a
  // specified return type; we care about there being a specified
  // return type because this specified return type is used to
  // initialize the return symbol but we don't want that to count as a
  // definition of a return value.
  //
  if (result == 0 || (result == 1 && fn->hasFlag(FLAG_SPECIFIED_RETURN_TYPE)))
    USR_WARN(fn->body, "control reaches end of function that returns a value");
}


// This test must be run after resolution, since it depends on the knowledge of
// whether the type of delete's operand is a record type.
// But it cannot be run after the compiler adds its own record deletes
// TODO: This violates the "no magic" principle, so the check and associated
// language in the spec should be considered for removal.
// In addition, the ability for user code to explicitly call deletes on fields
// of record type may be necessary for UMM, but this is yet to be demonstrated.
void
checkNoRecordDeletes()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    FnSymbol* fn = call->isResolved();
  
    // Note that fn can (legally) be null if the call is primitive.
    if (fn && fn->hasFlag(FLAG_DESTRUCTOR)) {
      // Statements of the form 'delete x' (PRIM_DELETE) are replaced
      //  during the normalize pass with a call to the destructor
      //  followed by a call to chpl_mem_free(), so here we just check
      //  if the type of the variable being passed to chpl_mem_free()
      //  is a record.
      if (isRecord(call->get(1)->typeInfo()->getValType()))
        USR_FATAL_CONT(call, "delete not allowed on records");
    }
  }
}
