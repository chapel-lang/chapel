/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// checkResolved.cpp

#include "passes.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stlUtil.h"

#include <set>

// We use RefVector to store a list of symbols that are aliases for the return
// symbol being sought in isDefinedAllPaths.
// We assume that if the ref is being used then it is valid (resolution should
// ensure this).
typedef std::set<Symbol*> RefSet;

static void checkConstLoops();
static int isDefinedAllPaths(Expr* expr, Symbol* ret, RefSet& refs);
static void checkReturnPaths(FnSymbol* fn);
static void checkNoRecordDeletes();
static void checkExternProcs();


static void
checkConstLoops() {
  if (fWarnConstLoops == true) {
    forv_Vec(BlockStmt, block, gBlockStmts) {
      block->checkConstLoops();
    }
  }
}

void
checkResolved() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkReturnPaths(fn);
    if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
        !fn->isIterator() &&
        fn->retType->defaultInitializer &&
        fn->retType->defaultInitializer->defPoint->parentSymbol == fn)
      USR_FATAL_CONT(fn, "functions cannot return nested iterators or loop expressions");
    if (fn->hasFlag(FLAG_ASSIGNOP) && fn->retType != dtVoid)
      USR_FATAL(fn, "The return value of an assignment operator must be 'void'.");
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
  checkConstLoops();
  checkExternProcs();
}


// Returns the smallest number of definitions of ret on any path through the
// given expression.
static int
isDefinedAllPaths(Expr* expr, Symbol* ret, RefSet& refs)
{
  if (!expr)
    return 0;

  if (isDefExpr(expr))
    return 0;

  if (isSymExpr(expr))
    return 0;

  if (CallExpr* call = toCallExpr(expr))
  {
    // Maybe add a "no return" pragma and use that instead.
    if (call->isNamed("halt"))
      return 1;

    if (call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr* lhs = toSymExpr(call->get(1));

      if (lhs->var == ret)
        return 1;

      if (refs.find(lhs->var) != refs.end())
        return 1;

      if (CallExpr* rhs = toCallExpr(call->get(2)))
        if (rhs->isPrimitive(PRIM_ADDR_OF))
        {
          // We expect only a SymExpr as the operand of 'addr of'.
          SymExpr* se = toSymExpr(rhs->get(1));
          if (se->var == ret)
            // lhs <- ('addr of' ret)
            refs.insert(lhs->var);
        }
    }

    if (call->isResolved())
    {
      for_alist(e, call->argList)
      {
        if (SymExpr* se = toSymExpr(e))
        {
          ArgSymbol* arg = actual_to_formal(se);
          // If ret is passed as an out or inout argument, that's a definition.
          if (se->var == ret &&
              (arg->intent == INTENT_OUT ||
               arg->intent == INTENT_INOUT ||
               arg->intent == INTENT_REF))
            return 1;

          // Treat all (non-const) refs as definitions, until we know better.
          // TODO: This may not be needed after moving insertReferenceTemps()
          // after this pass.

          // Commenting out debugging output
          //for (RefSet::iterator i = refs.begin();
          //     i != refs.end(); ++i)
          //  printf("%d\n", (*i)->id);

          if (refs.find(se->var) != refs.end() &&
              arg->intent == INTENT_REF)
            return 1;
        }
      }
    }

    return 0;
  }

  if (CondStmt* cond = toCondStmt(expr))
  {
    return std::min(isDefinedAllPaths(cond->thenStmt, ret, refs),
                    isDefinedAllPaths(cond->elseStmt, ret, refs));
  }

  if (isGotoStmt(expr))
    return 0;

  if (BlockStmt* block = toBlockStmt(expr))
  {
    // NOAKES 2014/11/25 Transitional.  Ensure we don't call blockInfoGet()
    if (block->isWhileDoStmt()  == true ||
        block->isForLoop()      == true ||
        block->isCForLoop()     == true ||
        block->isParamForLoop() == true)
    {
      return 0;
    }

    else if (block->isDoWhileStmt() == true ||
             block->blockInfoGet()  == NULL ||
             block->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL))
    {
      int result = 0;

      for_alist(e, block->body)
        result += isDefinedAllPaths(e, ret, refs);

      return result;
    }

    else
    {
      return 0;
    }
  }

  if (isExternBlockStmt(expr))
    return 0;

  INT_FATAL("isDefinedAllPaths: Unhandled case.");
  return 0;
}


static void
checkReturnPaths(FnSymbol* fn) {
  // Check to see if the function returns a value.
  if (fn->isIterator() ||
      !strcmp(fn->name, "=") || // TODO: Remove this to enforce new signature.
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

  RefSet refs;
  int result = isDefinedAllPaths(fn->body, ret, refs);

  //
  // Issue a warning if there is a path that has zero definitions or
  // there is a path that has one definition and the function has a
  // specified return type of a type we still default initialize; we care
  // about there being a specified return type because this specified
  // return type is used to initialize the return symbol but we don't want
  // that to count as a definition of a return value.
  //
  // The only types we still expect initialization code for are those marked
  // with FLAG_IGNORE_NOINIT, so those are the only cases where a single
  // definition means that the function writer neglected to return a value.
  if (result == 0 || (result == 1 && fn->hasFlag(FLAG_SPECIFIED_RETURN_TYPE) &&
                      fn->retType->symbol->hasFlag(FLAG_IGNORE_NOINIT)))
    USR_FATAL_CONT(fn->body, "control reaches end of function that returns a value");
}


// This test must be run after resolution, since it depends on the knowledge of
// whether the type of delete's operand is a record type.
// But it cannot be run after the compiler adds its own record deletes
// TODO: This violates the "no magic" principle, so the check and associated
// language in the spec should be considered for removal.
// In addition, the ability for user code to explicitly call deletes on fields
// of record type may be necessary for UMM, but this is yet to be demonstrated.
static void
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


static void checkExternProcs() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->hasFlag(FLAG_EXTERN))
      continue;

    for_formals(formal, fn) {
      if (formal->typeInfo()->getValType() == dtString) {
        if (fn->instantiatedFrom == NULL) {
          USR_FATAL_CONT(fn, "extern procedures should not take arguments of "
                             "type string, use c_string instead");
        } else {
          // This is a generic instantiation of an extern proc that is using
          // string, so we want to report the call sites causing this
          USR_FATAL_CONT(fn, "extern procedure has arguments of type string");
          forv_Vec(CallExpr, call, *fn->calledBy) {
            USR_PRINT(call, "when instantiated from here");
          }
          USR_PRINT(fn, "use c_string instead");
        }
        break;
      }
    }
  }
}


