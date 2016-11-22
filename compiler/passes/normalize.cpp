/*
 * Copyright 2004-2016 Cray Inc.
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

/*** normalize
 ***
 *** This pass and function normalizes parsed and scope-resolved AST.
 ***/

#include "passes.h"

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "initializerRules.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TransformLogicalShortCircuit.h"

#include <cctype>
#include <set>
#include <vector>

bool normalized = false;

//
// Static functions: forward declaration
//
static void insertModuleInit();
static void transformLogicalShortCircuit();
static void lowerReduceAssign();
static void checkUseBeforeDefs();
static void moveGlobalDeclarationsToModuleScope();
static void insertUseForExplicitModuleCalls(void);
static void processSyntacticDistributions(CallExpr* call);
static bool is_void_return(CallExpr* call);
static void normalize_returns(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void applyGetterTransform(CallExpr* call);
static void insert_call_temps(CallExpr* call);
static void fix_def_expr(VarSymbol* var);
static void hack_resolve_types(ArgSymbol* arg);
static void fixup_array_formals(FnSymbol* fn);
static void clone_for_parameterized_primitive_formals(FnSymbol* fn,
                                                      DefExpr* def,
                                                      int width);
static void replace_query_uses(ArgSymbol* formal,
                               DefExpr*   def,
                               CallExpr*  query,
                               std::vector<SymExpr*>& symExprs);
static void add_to_where_clause(ArgSymbol* formal,
                                Expr*      expr,
                                CallExpr*  query);
static void fixup_query_formals(FnSymbol* fn);
static void change_method_into_constructor(FnSymbol* fn);
static void find_printModuleInit_stuff();

void normalize() {
  insertModuleInit();
  transformLogicalShortCircuit();
  lowerReduceAssign();

  // tag iterators and replace delete statements with calls to ~chpl_destroy
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_YIELD)) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);
      // violations should have caused USR_FATAL in semanticChecks.cpp
      INT_ASSERT(fn && fn->isIterator());
    }
    if (call->isPrimitive(PRIM_DELETE)) {
      SET_LINENO(call);
      VarSymbol* tmp = newTemp("delete_tmp");
      call->insertBefore(new DefExpr(tmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(1)->remove()));
      call->insertBefore(new CallExpr("~chpl_destroy", gMethodToken, tmp));
      CallExpr* freeExpr = callChplHereFree(tmp);
      if (fLocal) {
        call->insertBefore(freeExpr);
      } else {
        //
        // if compiling for multiple locales, we need to be sure that the
        // delete is executed on the locale on which the object lives for
        // correctness sake.
        //
        BlockStmt* onStmt = buildOnStmt(new SymExpr(tmp), freeExpr);
        call->insertBefore(onStmt);
      }
      call->remove();
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    SET_LINENO(fn);
    if (!fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) &&
        !fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR))
      fixup_array_formals(fn);
    fixup_query_formals(fn);
    change_method_into_constructor(fn);
  }

  normalize(theProgram);
  normalized = true;
  checkUseBeforeDefs();
  moveGlobalDeclarationsToModuleScope();
  insertUseForExplicitModuleCalls();

  if (!fMinimalModules) {
    // Calls to _statementLevelSymbol() are inserted here and in
    // function resolution to ensure that sync vars are in the correct
    // state (empty) if they are used but not assigned to anything.
    forv_Vec(SymExpr, se, gSymExprs) {
      if (FnSymbol* parentFn = toFnSymbol(se->parentSymbol)) {
        if (se == se->getStmtExpr() &&
            // avoid exprs under ForallIntents
            (isDirectlyUnderBlockStmt(se) || !isBlockStmt(se->parentExpr)))
        {
          // Don't add these calls for the return type, since
          // _statementLevelSymbol would do nothing in that case
          // anyway, and it contributes to order-of-resolution issues for
          // extern functions with declared return type.
          if (parentFn->retExprType != se->parentExpr) {
            SET_LINENO(se);
            CallExpr* call = new CallExpr("_statementLevelSymbol");
            se->insertBefore(call);
            call->insertAtTail(se->remove());
          }
        }
      }
    }
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->defPoint->parentSymbol)
      hack_resolve_types(arg);
  }

  // perform some checks on destructors
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->formals.length < 2
          || fn->getFormal(1)->typeInfo() != gMethodToken->typeInfo()) {
        USR_FATAL(fn, "destructors must be methods");
      } else if (fn->formals.length > 2) {
        USR_FATAL(fn, "destructors must not have arguments");
      } else {
        DefExpr* thisDef = toDefExpr(fn->formals.get(2));
        INT_ASSERT(fn->name[0] == '~' && thisDef);
        AggregateType* ct = toAggregateType(thisDef->sym->type);
        // make sure the name of the destructor matches the name of the class
        if (ct && strcmp(fn->name + 1, ct->symbol->name)) {
          USR_FATAL(fn, "destructor name must match class name");
        } else {
          fn->name = astr("~chpl_destroy");
        }
      }
    }
    // make sure methods don't attempt to overload operators
    else if (!isalpha(*fn->name) && *fn->name != '_'
             && fn->formals.length > 1
             && fn->getFormal(1)->typeInfo() == gMethodToken->typeInfo()) {
      USR_FATAL(fn, "invalid method name");
    }
  }

  find_printModuleInit_stuff();
}

/************************************* | **************************************
*                                                                             *
* Insert the module initFn in to every module in allModules.  The current     *
* implementation pulls the entire module in to the prototypical initFn and    *
* then lets the reset of normalize sort things out.  The module looks         *
* reasonable by the end of the pass but odd in the middle.                    *
*                                                                             *
* MDN 2014/07/25 At some point this transformation should be reworked to be   *
* more delicate e.g. insert an empty init function and then carefully         *
* populate it so that the AST is well-behaved at all points.                  *
*                                                                             *
************************************** | *************************************/

static void insertModuleInit() {
  // Insert an init function into every module
  forv_Vec(ModuleSymbol, mod, allModules) {
    SET_LINENO(mod);

    mod->initFn          = new FnSymbol(astr("chpl__init_", mod->name));
    mod->initFn->retType = dtVoid;

    mod->initFn->addFlag(FLAG_MODULE_INIT);
    mod->initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);

    //
    // move module-level statements into module's init function
    //
    for_alist(stmt, mod->block->body) {
      if (stmt->isModuleDefinition() == false)
        mod->initFn->insertAtTail(stmt->remove());
    }

    mod->block->insertAtHead(new DefExpr(mod->initFn));

    //
    // If the module has the EXPORT_INIT flag then
    // propagate it to the module's init function
    //
    if (mod->hasFlag(FLAG_EXPORT_INIT) == true) {
      mod->initFn->addFlag(FLAG_EXPORT);
      mod->initFn->addFlag(FLAG_LOCAL_ARGS);
    }
  }
}



/************************************* | **************************************
*                                                                             *
* Historically, parser/build converted                                        *
*                                                                             *
*    <expr1> && <expr2>                                                       *
*    <expr1> || <expr2>                                                       *
*                                                                             *
* into an IfExpr (which itself currently has a complex implementation).       *
*                                                                             *
* Now we allow the parser to generate a simple unresolvable call to either    *
* && or || and then replace it with the original IF/THEN/ELSE expansion.      *
*                                                                             *
************************************** | *************************************/

static void transformLogicalShortCircuit()
{
  std::set<Expr*>           stmts;
  std::set<Expr*>::iterator iter;

  // Collect the distinct stmts that contain logical AND/OR expressions
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (call->primitive == 0)
    {
      if (UnresolvedSymExpr* expr = toUnresolvedSymExpr(call->baseExpr))
      {
        if (strcmp(expr->unresolved, "&&") == 0 ||
            strcmp(expr->unresolved, "||") == 0)
        {
          stmts.insert(call->getStmtExpr());
        }
      }
    }
  }

  // Transform each expression.
  //
  // In general this will insert new IF-expressions immediately before the
  // current statement.  This approach interacts with Chapel's scoping
  // rule for do-while stmts.  We need to ensure that the additional
  // scope has been wrapped around the do-while before we perform this
  // transform.
  //
  for (iter = stmts.begin(); iter != stmts.end(); iter++)
  {
    Expr*                        stmt = *iter;
    TransformLogicalShortCircuit transform(stmt);

    stmt->accept(&transform);
  }
}


static Symbol* reduceIntentOp(ForallIntents* fi, Symbol* reVar) {
  int nv = fi->numVars();
  for (int i = 0; i < nv; i++)
    if (fi->isReduce(i))
      if (SymExpr* varSE = toSymExpr(fi->fiVars[i]))
        if (varSE->symbol() == reVar) {
          SymExpr* ri = toSymExpr(fi->riSpecs[i]);
          INT_ASSERT(ri);
          return ri->symbol();
        }

  // Did not see 'reVar' with a reduce intent.
  return NULL;
}

//
// lowerReduceAssign(): lower the reduce= calls, verifying their correct use.
//
// Parser represents "x reduce= y" in source code as PRIM_REDUCE_ASSIGN(x,y).
// lowerReduceAssign() converts it into globalOp.accumulateOntoState(x,y)
// where globalOp comes from the reduce intent for x.
// This includes checking that there is, indeed, a reduce intent for x:
// without it, there is no globalOp.
//
static void lowerReduceAssign() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_REDUCE_ASSIGN)) {

      // l.h.s. must be a single variable
      if (SymExpr* lhsSE = toSymExpr(call->get(1))) {
        Symbol* lhsVar = lhsSE->symbol();
        // ... which is mentioned in a with clause with a reduce intent
        Expr* curr = call->parentExpr;
        ForallIntents* enclosingFI = NULL;
        while (curr) {
          if (BlockStmt* block = toBlockStmt(curr))
            if (ForallIntents* fi = block->forallIntents) {
              enclosingFI = fi;
              break;
            }
          curr = curr->parentExpr;
        }

        // I'd love these to be USR_FATAL_CONT, however currently
        // the forall loop body is replicated 3 times, so we would
        // report the same error 3 times. We could have a hashset of
        // astlocs to avoid that with USR_FATAL_CONT, if we wanted.
        if (!enclosingFI)
          USR_FATAL_CONT(call, "The reduce= operator must occur within a forall loop.");
        else if (Symbol* globalOp = reduceIntentOp(enclosingFI, lhsVar))
          {
            SET_LINENO(call);
            Expr* rhs = call->get(2)->remove(); // do it before lhsSE->remove()
            Expr* repl = new_Expr(".(%S, 'accumulateOntoState')(%E,%E)",
                           globalOp, lhsSE->remove(), rhs);
            call->replace(repl);
          }
        else
          USR_FATAL(lhsSE, "The l.h.s. of a reduce= operator, '%s', must be passed by a reduce intent into the nearest enclosing forall loop", lhsVar->name);

      } else {
        USR_FATAL(call->get(1), "The l.h.s. of a reduce= operator must be just a variable");
      }
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

// the following function is called from multiple places,
// e.g., after generating default or wrapper functions
void normalize(BaseAST* base) {
  std::vector<CallExpr*> calls;
  collectCallExprs(base, calls);
  for_vector(CallExpr, call, calls) {
    processSyntacticDistributions(call);
  }

  std::vector<Symbol*> symbols;
  collectSymbols(base, symbols);
  for_vector(Symbol, symbol, symbols) {
    if (FnSymbol* fn = toFnSymbol(symbol))
      normalize_returns(fn);
  }

  for_vector(Symbol, symbol2, symbols) {
    if (VarSymbol* var = toVarSymbol(symbol2))
      if (FnSymbol* fn = toFnSymbol(var->defPoint->parentSymbol))
        if (fn != stringLiteralModule->initFn)
          fix_def_expr(var);
  }

  calls.clear();
  collectCallExprs(base, calls);
  for_vector(CallExpr, call1, calls) {
    applyGetterTransform(call1);
    insert_call_temps(call1);
  }
  for_vector(CallExpr, call2, calls) {
    call_constructor_for_class(call2);
  }
}

// We can't really do this before resolution, because we need to know
// if symbols used as actual arguments are passed by ref, inout, or out
// (all of which would be considered definitions).
// The workaround for this has been early initialization --
// which is redundant with guaranteed initialization, at least with respect
// to class instances.
// Given that it is not completely correct, and it forces unnecessary
// initializations to be added to the AST, I recommend that the check be
// removed from this pass (and perhaps reinserted in a later pass).
static void
checkUseBeforeDefs() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint->parentSymbol)
    {
      ModuleSymbol* mod = fn->getModule();
      Vec<const char*> undeclared;
      Vec<Symbol*> undefined;
      std::vector<BaseAST*> asts;
      Vec<Symbol*> defined;

      // Walk the asts in this function.
      collect_asts_postorder(fn, asts);
      for_vector(BaseAST, ast, asts) {
        // Adds definitions (this portion could probably be made into a
        // separate function - see loopInvariantCodeMotion and copyPropagation)
        if (CallExpr* call = toCallExpr(ast)) {
          // A symbol gets defined when it appears on the LHS of a move or
          // assignment.
          if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
            if (SymExpr* se = toSymExpr(call->get(1)))
              defined.set_add(se->symbol());
        } else if (DefExpr* def = toDefExpr(ast)) {

          // All arg symbols are defined.
          if (isArgSymbol(def->sym))
            defined.set_add(def->sym);

          if (VarSymbol* vs = toVarSymbol(def->sym))
          {
            // All type aliases are taken as defined.
            if (vs->isType())
              defined.set_add(def->sym);
            // All variables of type 'void' are treated as defined.
            if (vs->typeInfo() == dtVoid)
              defined.set_add(vs);
          }

        } else {
          // The AST in question is not one of our methods of declaration so now
          // we check if it is a (resolved/unresolved) symbol and make sure
          // that symbol is not defined/declared before use
          if (SymExpr* sym = toSymExpr(ast)) {
            CallExpr* call = toCallExpr(sym->parentExpr);
            if (call &&
                (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) &&
                call->get(1) == sym)
              continue; // We already handled this case above.

            if (toModuleSymbol(sym->symbol())) {
              if (!toFnSymbol(fn->defPoint->parentSymbol)) {
                UseStmt* use = toUseStmt(sym->parentExpr);
                if (!use) {
                  SymExpr* prev = toSymExpr(sym->prev);
                  if (!prev || prev->symbol() != gModuleToken)
                    USR_FATAL_CONT(sym, "illegal use of module '%s'", sym->symbol()->name);
                }
              }
            } else if (isLcnSymbol(sym->symbol())) {
              if (sym->symbol()->defPoint->parentExpr != rootModule->block &&
                  (sym->symbol()->defPoint->parentSymbol == fn ||
                   (sym->symbol()->defPoint->parentSymbol == mod && mod->initFn == fn))) {
                if (!defined.set_in(sym->symbol()) && !undefined.set_in(sym->symbol())) {
                  if (!sym->symbol()->hasEitherFlag(FLAG_ARG_THIS,FLAG_EXTERN) &&
                      !sym->symbol()->hasFlag(FLAG_TEMP)) {
                    USR_FATAL_CONT(sym, "'%s' used before defined (first used here)", sym->symbol()->name);
                    undefined.set_add(sym->symbol());
                  }
                }
              }
            }
          } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(ast)) {
            CallExpr* call = toCallExpr(sym->parentExpr);
            if (call &&
                (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) &&
                call->get(1) == sym)
              continue; // We already handled this case above.
            if ((!call || 
                 (call->baseExpr != sym && 
                  !call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL) &&
                  !call->isPrimitive(PRIM_CAPTURE_FN_FOR_C))) && 
                sym->unresolved) {
              if (!undeclared.set_in(sym->unresolved)) {
                if (!toFnSymbol(fn->defPoint->parentSymbol)) {
                  USR_FATAL_CONT(sym, "'%s' undeclared (first use this function)",
                                 sym->unresolved);
                  undeclared.set_add(sym->unresolved);
                }
              }
            }
          }
        }
      }
    }
  }
}

static void
moveGlobalDeclarationsToModuleScope() {
  bool move = false;
  forv_Vec(ModuleSymbol, mod, allModules) {
    for_alist(expr, mod->initFn->body->body) {
      // If the last iteration set "move" to true, move this block to the end
      // of the module (see below).
      if (move)
      {
        INT_ASSERT(isBlockStmt(expr));
        mod->block->insertAtTail(expr->remove());
        move = false;
        continue;
      }

      if (DefExpr* def = toDefExpr(expr)) {

        // Non-temporary variable declarations are moved out to module scope.
        if (VarSymbol* vs = toVarSymbol(def->sym))
        {
          // Ignore compiler-inserted temporaries.
          // Only non-compiler-generated variables in the module init
          // function are moved out to module scope.
          if (vs->hasFlag(FLAG_TEMP))
            continue;

          // If the var declaration is an extern, we want to move its
          // initializer block with it.
          if (vs->hasFlag(FLAG_EXTERN))
          {
            BlockStmt* block = toBlockStmt(def->next);
            if (block)
            {
              // Mark this as a type block, so it is removed later.
              // Casts are because C++ is lame.
              (unsigned&)(block->blockTag) |= (unsigned) BLOCK_TYPE_ONLY;
              // Set the flag, so we move it out to module scope.
              move = true;
            }
          }

          mod->block->insertAtTail(def->remove());
        }

        // All type and function symbols are moved out to module scope.
        if (isTypeSymbol(def->sym) || isFnSymbol(def->sym))
        {
          mod->block->insertAtTail(def->remove());
        }
      }
    }
  }
}

static void
insertUseForExplicitModuleCalls(void) {
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->parentSymbol && se->symbol() == gModuleToken) {
      SET_LINENO(se);
      CallExpr* call = toCallExpr(se->parentExpr);
      INT_ASSERT(call);
      SymExpr* mse = toSymExpr(call->get(2));
      INT_ASSERT(mse);
      ModuleSymbol* mod = toModuleSymbol(mse->symbol());
      INT_ASSERT(mod);
      Expr* stmt = se->getStmtExpr();
      BlockStmt* block = new BlockStmt();
      stmt->insertBefore(block);
      block->insertAtHead(stmt->remove());
      block->moduleUseAdd(mod);
    }
  }
}

// Two cases are handled here:
// 1. ('new' (dmap arg)) ==> (chpl__buildDistValue arg)
// 2. (chpl__distributed (Dist args)) ==>
//     (chpl__distributed (chpl__buildDistValue ('new' (Dist args)))),
//     where isDistClass(Dist).
// In 1., the only type that has FLAG_SYNTACTIC_DISTRIBUTION on it is "dmap".
// This is a dummy record type that must be replaced.  The call to
// chpl__buildDistValue() performs this task, returning _newDistribution(x),
// where x is a distribution.
static void
processSyntacticDistributions(CallExpr* call) {
  SET_LINENO(call);
  if (call->isPrimitive(PRIM_NEW))
    if (CallExpr* type = toCallExpr(call->get(1)))
      if (SymExpr* base = toSymExpr(type->baseExpr))
        if (base->symbol()->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
          type->baseExpr->replace(new UnresolvedSymExpr("chpl__buildDistValue"));
          call->replace(type->remove());
        }
  if (call->isNamed("chpl__distributed"))
    if (CallExpr* distCall = toCallExpr(call->get(1)))
      if (SymExpr* distClass = toSymExpr(distCall->baseExpr))
        if (TypeSymbol* ts = toTypeSymbol(distClass->symbol()))
          if (isDistClass(ts->type))
            call->insertAtHead(
              new CallExpr("chpl__buildDistValue",
                new CallExpr(PRIM_NEW, distCall->remove())));
}

static bool is_void_return(CallExpr* call) {
  if (call->isPrimitive(PRIM_RETURN)) {
    SymExpr* arg = toSymExpr(call->argList.first());
    if (arg)
      // NB false for 'return void' in type functions, as it should be
      if (arg->symbol() == gVoid)
        return true;
  }
  return false;
}

static void insertRetMove(FnSymbol* fn, VarSymbol* retval, CallExpr* ret) {
  Expr* ret_expr = ret->get(1);
  ret_expr->remove();
  if (fn->returnsRefOrConstRef())
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, new CallExpr(PRIM_ADDR_OF, ret_expr)));
  else if (fn->retExprType)
  {
    // This is the case for a declared return type.
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval,
                      new CallExpr(PRIM_COERCE, ret_expr,
                        fn->retExprType->body.tail->copy())));
  }
  else if (fn->hasFlag(FLAG_IF_EXPR_FN))
  {
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, ret_expr));
  }
  else if (!fn->hasFlag(FLAG_WRAPPER) &&
           strcmp(fn->name, "iteratorIndex") &&
           strcmp(fn->name, "iteratorIndexHelp"))
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, new CallExpr(PRIM_DEREF, ret_expr)));
  else
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, ret_expr));
}

// Following normalization, each function contains only one return statement
// preceded by a label.  The first half of the function counts the
// total number of returns and the number of void returns.
// The big IF beginning with if (rets.n == 1) determines if the function
// is already normal.
// The last half of the function performs the normalization steps.
static void normalize_returns(FnSymbol* fn) {
  SET_LINENO(fn);

  CallExpr* theRet = NULL; // Contains the return if it is unique.
  Vec<CallExpr*> rets;
  std::vector<CallExpr*> calls;
  int numVoidReturns = 0;
  int numYields = 0;
  bool isIterator = fn->isIterator();

  collectMyCallExprs(fn, calls, fn); // calls not in a nested function

  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_RETURN)) {
      rets.add(call);

      theRet = call;

      if (is_void_return(call))
          numVoidReturns++;
    }
    else if (call->isPrimitive(PRIM_YIELD)) {
      rets.add(call);
      ++numYields;
    }
  }

  // If an iterator, then there is at least one nonvoid return-or-yield.
  INT_ASSERT(!isIterator || rets.n > numVoidReturns);

  // Check if this function's returns are already normal.
  if (rets.n - numYields == 1) {
    if (theRet == fn->body->body.last()) {
      if (SymExpr* se = toSymExpr(theRet->get(1))) {
        if (fn->hasFlag(FLAG_CONSTRUCTOR) ||
            fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) ||
            !strncmp("_if_fn", fn->name, 6) ||
            !strcmp("=", fn->name) ||
            !strcmp("_init", fn->name) ||
            !strcmp("_ret", se->symbol()->name)) {
          return;
        }
      }
    }
  }

  // Add a void return if needed.
  if (rets.n == 0) {
    if (fn->retExprType == NULL) {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
      return;
    }
  }

  LabelSymbol* label  = new LabelSymbol(astr("_end_", fn->name));
  VarSymbol*   retval = NULL;

  fn->insertAtTail(new DefExpr(label));

  // If a proc has a void return, do not return any values ever.
  // (Types are not resolved yet, so we judge by presence of "void returns"
  // i.e. returns with no expr. See also a related check in semanticChecks.)
  if (!isIterator && (numVoidReturns != 0)) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  } else {
    // Handle declared return type.
    retval = newTemp("ret", fn->retType);

    retval->addFlag(FLAG_RVV);

    if (fn->retTag == RET_PARAM)
      retval->addFlag(FLAG_PARAM);

    if (fn->retTag == RET_TYPE)
      retval->addFlag(FLAG_TYPE_VARIABLE);

    if (fn->hasFlag(FLAG_MAYBE_TYPE))
      retval->addFlag(FLAG_MAYBE_TYPE);

    // If the function has a specified return type (and is not a var function),
    // declare and initialize the return value up front,
    // and set the specified_return_type flag.
    if (fn->retExprType && fn->retTag != RET_REF) {
      BlockStmt* retExprType = fn->retExprType->copy();

      if (isIterator)
        if (SymExpr* lastRTE = toSymExpr(retExprType->body.tail))
          if (TypeSymbol* retSym = toTypeSymbol(lastRTE->symbol()))
            if (retSym->type == dtVoid)
              USR_FATAL_CONT(fn,
                             "an iterator's return type cannot be 'void'; "
                             "if specified, it must be the type of the "
                             "expressions the iterator yields");

      fn->addFlag(FLAG_SPECIFIED_RETURN_TYPE);
    }

    fn->insertAtHead(new DefExpr(retval));
    fn->insertAtTail(new CallExpr(PRIM_RETURN, retval));
  }

  // Now, for each return statement appearing in the function body,
  // move the value of its body into the declared return value.
  bool label_is_used = false;

  forv_Vec(CallExpr, ret, rets) {
    SET_LINENO(ret);

    if (isIterator) {
      INT_ASSERT(!!retval);

      // Three cases:
      // (1) yield expr; => mov _ret expr; yield _ret;
      // (2) return; => goto end_label;
      // (3) return expr; -> mov _ret expr; yield _ret; goto end_label;
      // Notice how (3) is the composition of (1) and (2).
      if (!is_void_return(ret)) { // Cases 1 and 3
        // insert MOVE(retval,ret_expr)
        insertRetMove(fn, retval, ret);

        // insert YIELD(retval)
        ret->insertBefore(new CallExpr(PRIM_YIELD, retval));
      }

      if (ret->isPrimitive(PRIM_YIELD)) // Case 1 only.
          // it's a yield => no goto; need to remove the original node
          ret->remove();
      else {    // Cases 2 and 3.
        if (ret->next != label->defPoint) {
          ret->replace(new GotoStmt(GOTO_RETURN, label));
          label_is_used = true;
        } else {
          ret->remove();
        }
      }
    } else {
      // Not an iterator
      if (retval) {
        // insert MOVE(retval,ret_expr)
        insertRetMove(fn, retval, ret);
      }

      // replace with GOTO(label)
      if (ret->next != label->defPoint) {
        ret->replace(new GotoStmt(GOTO_RETURN, label));
        label_is_used = true;
      } else {
        ret->remove();
      }
    }
  }

  if (!label_is_used)
    label->defPoint->remove();
}


// If se is a type alias, resolves it recursively, or fails and returns NULL.
static TypeSymbol* resolveTypeAlias(SymExpr* se)
{
  while (se)
  {
    Symbol* sym = se->symbol();

    if (TypeSymbol* ts = toTypeSymbol(sym))
      return ts;

    // By default, we break out of the loop
    se = NULL;

    // Unless we can find a new se to check.
    if (VarSymbol* vs = toVarSymbol(sym))
    {
      if (vs->isType())
      {
        // We expect to find its definition in the init field of its declaration.
        DefExpr* def = vs->defPoint;

        // Only bother with simple sym expressions.
        se = toSymExpr(def->init);
      }
    }
  }
  return NULL;
}


/*
   Normalize constructor/type constructor calls.
    * a call whose base expression is a symbol referring to an aggregate
      type is converted to a call to the default type constructor for
      that class, unless it's in a 'new' expression
    * calls to such aggregate types in a 'new' expression are transformed
      to put the call arguments directly into the PRIM_NEW in order to
      improve function resolution's ability to handle them.
    * if the type is "dmap" (syntactic distribution), it is replaced by a
      call to chpl_buildDistType().

 */
static void call_constructor_for_class(CallExpr* call) {
  if (SymExpr* se = toSymExpr(call->baseExpr)) {

    CallExpr* parent = toCallExpr(call->parentExpr);
    CallExpr* parentParent = NULL;
    if (parent)
      parentParent = toCallExpr(parent->parentExpr);

    AggregateType* ct = NULL;
    if (TypeSymbol* ts = resolveTypeAlias(se))
      ct = toAggregateType(ts->type);

    CallExpr* primNewToFix = NULL;

    // Select symExprs of class (or record) type.

    SET_LINENO(call);

    if (parent && parent->isPrimitive(PRIM_NEW) ) {
      if (!call->partialTag) { // avoid normalizing PRIM_NEW twice
        primNewToFix = parent;
        INT_ASSERT(primNewToFix->get(1) == call);
      }
    } else if (parentParent && parentParent->isPrimitive(PRIM_NEW) &&
             call->partialTag) {
      primNewToFix = parentParent;
      INT_ASSERT(primNewToFix->get(1) == parent);
    } else if (ct) {
      if (ct->symbol->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION))
        // Call chpl__buildDistType for syntactic distributions.
        se->replace(new UnresolvedSymExpr("chpl__buildDistType"));
      else
        // Transform C ( ... ) into _type_construct_C ( ... ) .
        se->replace(new UnresolvedSymExpr(ct->defaultTypeConstructor->name));
    }

    if (primNewToFix) {
      // Transform   new (call C args...) args2...
      //      into   new C args... args2...

      // Transform   new (call (call (partial) C _mt this) args...)) args2...
      //      into   new (call (partial) C _mt this) args... args2...

      // The resulting AST will be handled in function resolution
      // where the PRIM_NEW will be removed. It is transformed
      // to no longer be a call with a type baseExpr in order
      // to make better sense to function resolution.

      CallExpr* callInNew = toCallExpr(primNewToFix->get(1));
      callInNew->remove();
      CallExpr *newNew = new CallExpr(PRIM_NEW);
      primNewToFix->replace(newNew);

      newNew->insertAtHead(callInNew->baseExpr);
      // Move the actuals from the call to the new PRIM_NEW
      for_actuals(actual, callInNew) {
        newNew->insertAtTail(actual->remove());
      }
      // Move actual from the PRIM_NEW as well
      // This is not the expected AST form, but keeping this
      // code here adds some resiliency.
      for_actuals(actual, primNewToFix) {
        newNew->insertAtTail(actual->remove());
      }
    }
  }
}

static void applyGetterTransform(CallExpr* call) {
  // Most generally:
  //   x.f(a) --> f(_mt, x)(a)
  // which is the same as
  //   call(call(. x "f") a) --> call(call(f _mt x) a)
  // Also:
  //   x.f --> f(_mt, x)
  // Note:
  //   call(call or )( indicates partial
  if (call->isNamed(".")) {
    SET_LINENO(call);
    SymExpr* symExpr = toSymExpr(call->get(2));
    INT_ASSERT(symExpr);
    symExpr->remove();
    if (VarSymbol* var = toVarSymbol(symExpr->symbol())) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        call->baseExpr->replace(new UnresolvedSymExpr(var->immediate->v_string));
        if (!strcmp(var->immediate->v_string, "init")) {
          // Transform:
          //   call(call(. x "init") args)
          // into:
          //   call(call(init meme=x) args)
          // because initializers are handled differently from other methods
          Expr* firstArg = call->get(1)->remove();
          call->insertAtHead(new NamedExpr("meme", firstArg));
        } else {
          call->insertAtHead(gMethodToken);
        }
      } else {
        INT_FATAL(call, "unexpected case");
      }
    } else if (TypeSymbol* type = toTypeSymbol(symExpr->symbol())) {
      call->baseExpr->replace(new SymExpr(type));
      call->insertAtHead(gMethodToken);
    } else {
      INT_FATAL(call, "unexpected case");
    }
    call->methodTag = true;
    if (CallExpr* parent = toCallExpr(call->parentExpr))
      if (parent->baseExpr == call)
        call->partialTag = true;
  }
}

static void insert_call_temps(CallExpr* call)
{
  Expr* stmt = call->getStmtExpr();

  // Ignore call if it is not in the tree.
  if (call->parentExpr == NULL || stmt == NULL)
    return;

  // Call is already at statement level, so no need to flatten.
  if (call == stmt)
    return;

  if (toDefExpr(call->parentExpr))
    return;

  if (call->partialTag)
    return;

  if (call->isPrimitive(PRIM_TUPLE_EXPAND) ||
      call->isPrimitive(PRIM_GET_MEMBER_VALUE))
    return;

  // TODO: Check if we need a call temp for PRIM_ASSIGN.
  CallExpr* parentCall = toCallExpr(call->parentExpr);

  if (parentCall && (parentCall->isPrimitive(PRIM_MOVE) ||
                     parentCall->isPrimitive(PRIM_NEW)))
    return;

  SET_LINENO(call);

  VarSymbol* tmp = newTemp("call_tmp");

  if (!parentCall || !parentCall->isNamed("chpl__initCopy"))
    tmp->addFlag(FLAG_EXPR_TEMP);

  if (call->isPrimitive(PRIM_NEW))
    tmp->addFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);

  if (call->isPrimitive(PRIM_TYPEOF))
    tmp->addFlag(FLAG_TYPE_VARIABLE);

  // NOAKES 2015/11/02
  //   The expansion of _build_tuple() creates temps that need to be
  //   autoDestroyed.  This is a short-cut to arrange for that to occur.
  //   A better long term solution would be preferred
  if (call->isNamed("chpl__initCopy")       == true &&
      parentCall                            != NULL &&
      parentCall->isNamed("_build_tuple")   == true)
    tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);

  // MPF 2016-10-20
  //   This is a workaround for a problem in
  //     types/typedefs/bradc/arrayTypedef
  //   I'm sure that there is a better way to handle this
  {
    // either in module init fn or in a sequence of parloopexpr fns
    // computing an array type than are in a module init fn
    FnSymbol* fn = call->getFunction();
    while( fn->hasFlag(FLAG_MAYBE_ARRAY_TYPE) ) {
      fn = fn->defPoint->getFunction();
    }
    if (fn == fn->getModule()->initFn) {
      CallExpr* cur = parentCall;
      CallExpr* sub = call;
      // Look for a parent call that is either:
      //  * making an array type alias, or
      //  * passing the result into the 2nd argument of buildArrayRuntimeType.
      while (cur != NULL) {
        if (cur->isNamed("chpl__typeAliasInit") ||
            (cur->isNamed("chpl__buildArrayRuntimeType") && cur->get(2) == sub))
          break;
        sub = cur;
        cur = toCallExpr(cur->parentExpr);
      }
      if (cur) {
        tmp->addFlag(FLAG_NO_AUTO_DESTROY);
      }
    }
  }

  tmp->addFlag(FLAG_MAYBE_PARAM);
  tmp->addFlag(FLAG_MAYBE_TYPE);

  if (call->isNamed("super") && parentCall && parentCall->isNamed(".") &&
      parentCall->get(1) == call) {
    // We've got an access to a method or field on the super type.  This means
    // we should preserve that knowledge for when we attempt to access the
    // method on the super type.
    tmp->addFlag(FLAG_SUPER_TEMP);
  }

  call->replace(new SymExpr(tmp));

  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, call));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void init_array_alias(VarSymbol* var,
                             Expr*      type,
                             Expr*      init,
                             Expr*      stmt);

static void init_ref_var(VarSymbol*     var,
                         Expr*          init,
                         Expr*          stmt);

static void init_config_var(VarSymbol* var,
                            Expr*&     stmt,
                            VarSymbol* constTemp);

static void init_typed_var(VarSymbol* var,
                           Expr*      type,
                           Expr*      init,
                           Expr*      stmt,
                           VarSymbol* constTemp);

static void init_untyped_var(VarSymbol* var,
                             Expr*      init,
                             Expr*      stmt,
                             VarSymbol* constTemp);

//
// fix_def_expr removes DefExpr::exprType and DefExpr::init from a
//   variable's def expression, normalizing the AST with primitive
//   moves, calls to chpl__initCopy, _init, and _cast, and assignments.
//
static void fix_def_expr(VarSymbol* var) {
  SET_LINENO(var);

  Expr*      type      = var->defPoint->exprType;
  Expr*      init      = var->defPoint->init;
  Expr*      stmt      = var->defPoint; // insertion point
  VarSymbol* constTemp = var;           // temp for constants

  if (!type && !init)
    return; // already fixed

  //
  // add "insert auto destroy" pragma to user variables that should be
  // auto destroyed
  //
  FnSymbol* fn = toFnSymbol(var->defPoint->parentSymbol);

  INT_ASSERT(fn);

  if (!var->hasFlag(FLAG_NO_AUTO_DESTROY) &&
      !var->hasFlag(FLAG_PARAM)           && // Note 1.
      !var->hasFlag(FLAG_REF_VAR)         &&
      fn->_this != var                    && // Note 2.
      !fn->hasFlag(FLAG_INIT_COPY_FN)     && // Note 3.
      !fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {

    // Variables in a module initializer need special attention
    if (var->defPoint->parentExpr == fn->getModule()->initFn->body) {

      // MDN 2016/04/27
      //
      // Most variables in a module init function will become
      // global and should not be auto destroyed.  The challenging
      // case is injected by
      //
      // var (a1, a2) = fnReturnTuple();
      //
      // The parser expands this as
      //
      // var tmp = fnReturnTuple();
      // var a1  = tmp.x1;
      // var a2  = tmp.x2;
      //
      // This pseudo-tuple must be auto-destroyed to ensure the components
      // are managed correctly. However the AST doesn't provide us with a
      // strong/easy way to determine that we're dealing with this case.
      // In practice is appears to be sufficient to flag any TMP
      if (var->hasFlag(FLAG_TEMP)) {
        var->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }

    } else {
      var->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }

  //
  // handle "no copy" variables
  //
  if (var->hasFlag(FLAG_NO_COPY)) {
    INT_ASSERT(init);
    // If a type expression is set, normalize would normally
    // use defaultOf/assignment anyway. As of 9-21-2016
    // setting FLAG_NO_COPY and having a type leads to some
    // unresolved type expression hanging around in the AST.
    INT_ASSERT(!type);
    stmt->insertAfter(new CallExpr(PRIM_MOVE, var, init->remove()));
    return;
  }

  //
  // handle type aliases
  //
  if (var->isType()) {
    INT_ASSERT(init);
    INT_ASSERT(!type);
    stmt->insertAfter(new CallExpr(PRIM_MOVE,
                                   var,
                                   new CallExpr("chpl__typeAliasInit",
                                                init->copy())));
    // note: insert_call_temps adjusts auto-destroy in this case
    // by checking for chpl__typeAliasInit

    return;
  }

  //
  // handle var ... : ... => ...;
  //
  if (var->hasFlag(FLAG_ARRAY_ALIAS)) {
    init_array_alias(var, type, init, stmt);
    return;
  }

  //
  // insert temporary for constants to assist constant checking
  //
  if ( var->hasFlag(FLAG_CONST) &&
      !var->hasEitherFlag(FLAG_EXTERN, FLAG_REF_VAR)) {
    constTemp = newTemp("const_tmp");

    stmt->insertBefore(new DefExpr(constTemp));
    stmt->insertAfter(new CallExpr(PRIM_MOVE, var, constTemp));
  }

  //
  // handle ref variables
  //
  if (var->hasFlag(FLAG_REF_VAR)) {
    init_ref_var(var, init, stmt);
    return;
  }

  //
  // insert code to initialize config variable from the command line
  //
  if (var->hasFlag(FLAG_CONFIG)) {
    if (!var->hasFlag(FLAG_PARAM)) {
      init_config_var(var, stmt, constTemp);
    }
  }

  if (type) {
    init_typed_var(var, type, init, stmt, constTemp);
  } else {
    init_untyped_var(var, init, stmt, constTemp);
  }
}


static void init_array_alias(VarSymbol* var,
                             Expr*      type,
                             Expr*      init,
                             Expr*      stmt) {
  CallExpr* partial  = NULL;

  if (!type) {
    partial = new CallExpr("newAlias", gMethodToken, init->remove());

    // newAlias is not a method, so we don't set the methodTag

  } else {
    CallExpr* reindex = new CallExpr("reindex", gMethodToken, init->remove());

    reindex->partialTag = true;
    reindex->methodTag  = true;

    partial = new CallExpr(reindex, type->remove());
  }

  stmt->insertAfter(new CallExpr(PRIM_MOVE, var, partial));
}


static void init_ref_var(VarSymbol* var, Expr* init, Expr* stmt) {
  if (!init) {
    USR_FATAL_CONT(var,
                   "References must be initialized when they are defined.");
  }

  Expr* varLocation = NULL;

  // If this is a const reference to an immediate, we need to insert a temp
  // variable so we can take the address of it, non-const references to an
  // immediate are not allowed.
  if (var->hasFlag(FLAG_CONST)) {
    if (SymExpr* initSym = toSymExpr(init)) {
      if (initSym->symbol()->isImmediate()) {
        VarSymbol* constRefTemp  = newTemp("const_ref_immediate_tmp");

        stmt->insertBefore(new DefExpr(constRefTemp));
        stmt->insertBefore(new CallExpr(PRIM_MOVE,
                                        constRefTemp,
                                        init->remove()));

        varLocation = new SymExpr(constRefTemp);
      }
    }
  }

  if (varLocation == NULL && init != NULL) {
    varLocation = init->remove();
  }

  if (SymExpr* sym = toSymExpr(varLocation)) {
    if (!var->hasFlag(FLAG_CONST) && sym->symbol()->isConstant()) {
      USR_FATAL_CONT(sym,
                     "Cannot set a non-const reference to a const variable.");
    }
  }

  stmt->insertAfter(new CallExpr(PRIM_MOVE,
                                 var,
                                 new CallExpr(PRIM_ADDR_OF, varLocation)));
}


static void init_config_var(VarSymbol* var,
                            Expr*&     stmt,
                            VarSymbol* constTemp) {
  Expr*   noop        = new CallExpr(PRIM_NOOP);
  Symbol* module_name = (var->getModule()->modTag != MOD_INTERNAL ?
                         new_CStringSymbol(var->getModule()->name) :
                         new_CStringSymbol("Built-in"));

  CallExpr* strToValExpr =
    new CallExpr("_command_line_cast",
                 new SymExpr(new_CStringSymbol(var->name)),
                 new CallExpr(PRIM_TYPEOF, constTemp),
                 new CallExpr("chpl_config_get_value",
                              new_CStringSymbol(var->name),
                              module_name));

  stmt->insertAfter(
        new CondStmt(new CallExpr("!",
                                  new CallExpr("chpl_config_has_value",
                                               new_CStringSymbol(var->name),
                                               module_name)),
                     noop,
                     new CallExpr(PRIM_MOVE, constTemp, strToValExpr)));

  stmt = noop; // insert regular definition code in then block
}


static void init_typed_var(VarSymbol* var,
                           Expr*      type,
                           Expr*      init,
                           Expr*      stmt,
                           VarSymbol* constTemp) {
  //
  // use cast for parameters to avoid multiple parameter assignments
  //
  if (init && var->hasFlag(FLAG_PARAM)) {
    stmt->insertAfter(new CallExpr(PRIM_MOVE,
                                   var,
                                   new CallExpr("_cast",
                                                type->remove(),
                                                init->remove())));

    return;
  }

  //
  // initialize variable based on specified type and then assign it
  // the initialization expression if it exists
  //
  bool isNoinit     = init && init->isNoInitExpr();
  bool moduleNoinit = false;

  if (isNoinit && !fUseNoinit) {
    // In the case where --no-use-noinit is thrown, we want to still use
    // noinit in the module code (as the correct operation of complexes
    // depends on it).

    // Lydia note: The requirement for complexes is expected to go away when
    // we transition to constructors for all types instead of the _defaultOf
    // function
    Symbol* moduleSource = var;

    while (!isModuleSymbol(moduleSource)  &&
           moduleSource           != NULL &&
           moduleSource->defPoint != NULL) {
      // This will go up the definition tree until it reaches a module symbol
      // Or until it encounters a null field.
      moduleSource = moduleSource->defPoint->parentSymbol;
    }

    ModuleSymbol* mod = toModuleSymbol(moduleSource);

    if (mod != NULL && moduleSource->defPoint != NULL) {
      // As these are the only other cases that would have caused the prior
      // while loop to exit, the moduleSource must be a module
      moduleNoinit = mod->modTag == MOD_INTERNAL ||
                     mod->modTag == MOD_STANDARD;

      // Check if the parent module of this variable is a standard or
      // internal module, and store the result of this check in moduleNoinit
    }
  }

  // Lydia note:  I'm adding fUseNoinit here because utilizing noinit with
  // return temps is necessary, so the only instances that should be
  // controlled by the flag are generated here
  if (isNoinit && (fUseNoinit || moduleNoinit)) {
    // Only perform this action if noinit has been specified and the flag
    // --no-use-noinit has not been thrown (or if the noinit is found in
    // module code)
    var->defPoint->init->remove();

    CallExpr* initCall = new CallExpr(PRIM_MOVE,
                                      var,
                                      new CallExpr(PRIM_NO_INIT,
                                                   type->remove()));

    // Since the variable won't have been defined just yet (stmt is
    // its def expression after all), insert the move after the defPoint
    stmt->insertAfter(initCall);

  } else {
    if (!fUseNoinit && isNoinit) {
      // The instance would be initialized to noinit, but we aren't allowing
      // noinit, so remove the initialization expression, allowing  it to
      // default initialize.
      init->remove();
      init = NULL;
    }

    //
    // MDN 2016/02/02
    // The code for resolving the type of an extern variable
    //
    //   functionResolution.cpp : resolveExternVarSymbols()
    //
    // expects to find the init code inside a block stmt.
    //
    // However the remaining cases do not need it.
    //
    BlockStmt* block = NULL;

    if (var->hasFlag(FLAG_EXTERN) == true)
      block = new BlockStmt(NULL, BLOCK_SCOPELESS);

    VarSymbol* typeTemp = newTemp("type_tmp");
    DefExpr*   typeDefn = new DefExpr(typeTemp);
    CallExpr*  initCall = NULL;

    initCall = new CallExpr(PRIM_MOVE,
                            typeTemp,
                            new CallExpr(PRIM_INIT, type->remove()));

    if (block != NULL)
      block->insertAtTail(typeDefn);
    else
      stmt->insertAfter(typeDefn);

    typeDefn->insertAfter(initCall);

    if (init) {
      CallExpr* assign = new CallExpr("=",       typeTemp,  init->remove());
      CallExpr* move   = new CallExpr(PRIM_MOVE, constTemp, typeTemp);

      // This should be copy-initialization, not assignment.
      initCall->insertAfter(assign);
      assign->insertAfter(move);

    } else if (constTemp->isType()) {
      initCall->insertAfter(new CallExpr(PRIM_MOVE,
                                         constTemp,
                                         new CallExpr(PRIM_TYPEOF, typeTemp)));

    } else {
      initCall->insertAfter(new CallExpr(PRIM_MOVE, constTemp, typeTemp));

      if (constTemp->hasFlag(FLAG_EXTERN))
        (unsigned&) block->blockTag |= BLOCK_EXTERN | BLOCK_TYPE;
    }

    if (block != NULL)
      stmt->insertAfter(block);
  }
}


static void init_untyped_var(VarSymbol* var,
                             Expr*      init,
                             Expr*      stmt,
                             VarSymbol* constTemp)
{
  // See Note 4.
  //
  // initialize untyped variable with initialization expression
  //
  // sjd: this new specialization of PRIM_NEW addresses the test
  //         test/classes/diten/test_destructor.chpl
  //      in which we call an explicit record destructor and avoid
  //      calling the default constructor.  However, if written with
  //      an explicit type, this would happen.  The record in this
  //      test is an issue since its destructor deletes field c, but
  //      the default constructor does not 'new' it.  Thus if we
  //      pass the record to a function and it is copied, we have an
  //      issue since we will do a double free.
  //
  CallExpr* initCall = toCallExpr(init);
  Expr*     rhs      = NULL;

  if (initCall && initCall->isPrimitive(PRIM_NEW)) {
    rhs = init->remove();
  } else {
    rhs = new CallExpr("chpl__initCopy", init->remove());
  }

  stmt->insertAfter(new CallExpr(PRIM_MOVE, constTemp, rhs));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void hack_resolve_types(ArgSymbol* arg) {
  // Look only at unknown or arbitrary types.
  if (arg->type == dtUnknown || arg->type == dtAny) {
    if (!arg->typeExpr) {
      if (!arg->hasFlag(FLAG_TYPE_VARIABLE) && arg->defaultExpr) {
        SymExpr* se = NULL;
        if (arg->defaultExpr->body.length == 1)
          se = toSymExpr(arg->defaultExpr->body.tail);
        if (!se || se->symbol() != gTypeDefaultToken) {
          SET_LINENO(arg->defaultExpr);
          arg->typeExpr = arg->defaultExpr->copy();
          insert_help(arg->typeExpr, NULL, arg);
        }
      }
    } else {
      INT_ASSERT(arg->typeExpr);

      // If there is a simple type expression, and its type is something more specific than
      // dtUnknown or dtAny, then replace the type expression with that type.
      // hilde sez: don't we lose information here?
      if (arg->typeExpr->body.length == 1) {
        Type* type = arg->typeExpr->body.only()->typeInfo();
        if (type != dtUnknown && type != dtAny) {
          // This test ensures that we are making progress.
          arg->type = type;
          arg->typeExpr->remove();
        }
      }
    }
  }
}

// Replaces formals whose type is computed by chpl__buildArrayRuntimeType
// with the generic _array type.
// I think this prepares the function to be instantiated with various argument types.
// That is, it reaches through one level in the type hierarchy -- treating all
// arrays equally and then resolving using the element type.
// But this is something of a kludge.  The expansion of arrays
// w.r.t. generic argument types should be done during expansion and resolution,
// not up front like this. <hilde>
static void fixup_array_formals(FnSymbol* fn) {
  for_formals(arg, fn) {
    INT_ASSERT(toArgSymbol(arg));
    if (arg->typeExpr) {
      // The argument has a type expression
      CallExpr* call = toCallExpr(arg->typeExpr->body.tail);
      // Not sure why we select the tail here....

      if (call && call->isNamed("chpl__buildArrayRuntimeType")) {
        // We are building an array type.
        bool noDomain = (isSymExpr(call->get(1))) ?  toSymExpr(call->get(1))->symbol() == gNil : false;
        DefExpr* queryDomain = toDefExpr(call->get(1));
        bool noEltType = (call->numActuals() == 1);
        DefExpr* queryEltType = (!noEltType) ? toDefExpr(call->get(2)) : NULL;

        // Replace the type expression with "_array" to make it generic.
        arg->typeExpr->replace(new BlockStmt(new SymExpr(dtArray->symbol), BLOCK_TYPE));

        std::vector<SymExpr*> symExprs;
        collectSymExprs(fn, symExprs);

        // If we have an element type, replace reference to its symbol with
        // "arg.eltType", so we use the instantiated element type.
        if (queryEltType) {
          for_vector(SymExpr, se, symExprs) {
            if (se->symbol() == queryEltType->sym) {
              SET_LINENO(se);
              se->replace(new CallExpr(".", arg, new_CStringSymbol("eltType")));
            }
          }
        } else if (!noEltType) {
          // The element type is supplied, but it is null.
          // Add a new where clause "eltType == arg.eltType".
          INT_ASSERT(queryEltType == NULL);
          if (!fn->where) {
            fn->where = new BlockStmt(new SymExpr(gTrue));
            insert_help(fn->where, NULL, fn);
          }
          Expr* oldWhere = fn->where->body.tail;
          CallExpr* newWhere = new CallExpr("&");
          oldWhere->replace(newWhere);
          newWhere->insertAtTail(oldWhere);
          newWhere->insertAtTail(
            new CallExpr("==", call->get(2)->remove(),
                         new CallExpr(".", arg, new_CStringSymbol("eltType"))));
        }

        if (queryDomain) {
          // Array type is built using a domain.
          // If we match the domain symbol, replace it with arg._dom.
          for_vector(SymExpr, se, symExprs) {
            if (se->symbol() == queryDomain->sym) {
              SET_LINENO(se);
              se->replace(new CallExpr(".", arg, new_CStringSymbol("_dom")));
            }
          }
        } else if (!noDomain) {
          // The domain argument is supplied but NULL.
          INT_ASSERT(queryDomain == NULL);

          // actualArg.chpl_checkArrArgDoms(arg->typeExpr)
          fn->insertAtHead(new CallExpr(new CallExpr(".", arg,
                                                     new_CStringSymbol("chpl_checkArrArgDoms")
                                                     ),
                                        call->get(1)->copy(),
                                        (fNoFormalDomainChecks ? gFalse : gTrue)));
        }
      }
    }
  }
}

static void
clone_for_parameterized_primitive_formals(FnSymbol* fn,
                                          DefExpr* def,
                                          int width) {
  SymbolMap map;
  FnSymbol* newfn = fn->copy(&map);
  Symbol* newsym = map.get(def->sym);
  newsym->defPoint->replace(new SymExpr(new_IntSymbol(width)));
  std::vector<SymExpr*> symExprs;
  collectSymExprs(newfn, symExprs);
  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == newsym)
      se->setSymbol(new_IntSymbol(width));
  }
  fn->defPoint->insertAfter(new DefExpr(newfn));
}

static void
replace_query_uses(ArgSymbol* formal, DefExpr* def, CallExpr* query,
                   std::vector<SymExpr*>& symExprs) {
  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == def->sym) {
      if (formal->variableExpr) {
        CallExpr* parent = toCallExpr(se->parentExpr);
        if (!parent || parent->numActuals() != 1)
          USR_FATAL(se, "illegal access to query type or parameter");
        se->replace(new SymExpr(formal));
        parent->replace(se);
        CallExpr* myQuery = query->copy();
        myQuery->insertAtHead(parent);
        se->replace(myQuery);
      } else {
        CallExpr* myQuery = query->copy();
        myQuery->insertAtHead(formal);
        se->replace(myQuery);
      }
    }
  }
}

static void
add_to_where_clause(ArgSymbol* formal, Expr* expr, CallExpr* query) {
  FnSymbol* fn = formal->defPoint->getFunction();
  if (!fn->where) {
    fn->where = new BlockStmt(new SymExpr(gTrue));
    insert_help(fn->where, NULL, fn);
  }
  Expr* where = fn->where->body.tail;
  CallExpr* clause;
  query->insertAtHead(formal);
  if (formal->variableExpr) {
    clause = new CallExpr(PRIM_TUPLE_AND_EXPAND);
    while (query->numActuals())
      clause->insertAtTail(query->get(1)->remove());
    clause->insertAtTail(expr->copy());
  } else {
    clause = new CallExpr("==", expr->copy(), query);
  }
  where->replace(new CallExpr("&", where->copy(), clause));
}

static void
fixup_query_formals(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (!formal->typeExpr)
      continue;
    if (DefExpr* def = toDefExpr(formal->typeExpr->body.tail)) {
      std::vector<SymExpr*> symExprs;
      collectSymExprs(fn, symExprs);
      for_vector(SymExpr, se, symExprs) {
        if (se->symbol() == def->sym)
          se->replace(new CallExpr(PRIM_TYPEOF, formal));
      }
      // Consider saving as origTypeExpr instead?
      formal->typeExpr->remove();
      formal->type = dtAny;
    } else if (CallExpr* call = toCallExpr(formal->typeExpr->body.tail)) {
      // clone query primitive types
      SymExpr* callFnSymExpr = toSymExpr(call->baseExpr);
      if (callFnSymExpr && call->numActuals() == 1) {
        Symbol* callFnSym = callFnSymExpr->symbol();
        if (DefExpr* def = toDefExpr(call->get(1))) {
          if (callFnSym == dtBools[BOOL_SIZE_DEFAULT]->symbol) {
            for (int i=BOOL_SIZE_8; i<BOOL_SIZE_NUM; i++)
              if (dtBools[i]) {
                clone_for_parameterized_primitive_formals(fn, def,
                                                          get_width(dtBools[i]));
              }
            fn->defPoint->remove();
            return;
          } else if (callFnSym == dtInt[INT_SIZE_DEFAULT]->symbol ||
                     callFnSym == dtUInt[INT_SIZE_DEFAULT]->symbol) {
            for( int i=INT_SIZE_8; i<INT_SIZE_NUM; i++)
              if (dtInt[i])
                clone_for_parameterized_primitive_formals(fn, def,
                                                          get_width(dtInt[i]));
            fn->defPoint->remove();
            return;
          } else if (callFnSym == dtReal[FLOAT_SIZE_DEFAULT]->symbol ||
                     callFnSym == dtImag[FLOAT_SIZE_DEFAULT]->symbol) {
            for( int i=FLOAT_SIZE_16; i<FLOAT_SIZE_NUM; i++)
              if (dtReal[i])
                clone_for_parameterized_primitive_formals(fn, def,
                                                          get_width(dtReal[i]));
            fn->defPoint->remove();
            return;
          } else if (callFnSym == dtComplex[COMPLEX_SIZE_DEFAULT]->symbol) {
            for( int i=COMPLEX_SIZE_32; i<COMPLEX_SIZE_NUM; i++)
              if (dtComplex[i])
                clone_for_parameterized_primitive_formals(fn, def,
                                                          get_width(dtComplex[i]));
            fn->defPoint->remove();
            return;
          }
        }
      }
      bool queried = false;
      for_actuals(actual, call) {
        if (toDefExpr(actual))
          queried = true;
        if (NamedExpr* named = toNamedExpr(actual))
          if (toDefExpr(named->actual))
            queried = true;
      }
      if (queried) {
        bool isTupleType = false;
        std::vector<SymExpr*> symExprs;
        collectSymExprs(fn, symExprs);
        if (call->isNamed("_build_tuple")) {
          add_to_where_clause(formal, new SymExpr(new_IntSymbol(call->numActuals())), new CallExpr(PRIM_QUERY, new_CStringSymbol("size")));
          call->baseExpr->replace(new SymExpr(dtTuple->symbol));
          isTupleType = true;
        }
        CallExpr* positionQueryTemplate = new CallExpr(PRIM_QUERY);
        for_actuals(actual, call) {
          if (NamedExpr* named = toNamedExpr(actual)) {
            positionQueryTemplate->insertAtTail(new_CStringSymbol(named->name));
            CallExpr* nameQuery = new CallExpr(PRIM_QUERY, new_CStringSymbol(named->name));
            if (DefExpr* def = toDefExpr(named->actual)) {
              replace_query_uses(formal, def, nameQuery, symExprs);
            } else {
              add_to_where_clause(formal, named->actual, nameQuery);
            }
          }
        }
        int position = (isTupleType) ? 2 : 1; // size is first for tuples
        for_actuals(actual, call) {
          if (!toNamedExpr(actual)) {
            CallExpr* positionQuery = positionQueryTemplate->copy();
            positionQuery->insertAtTail(new_IntSymbol(position));
            if (DefExpr* def = toDefExpr(actual)) {
              replace_query_uses(formal, def, positionQuery, symExprs);
            } else {
              add_to_where_clause(formal, actual, positionQuery);
            }
            position++;
          }
        }
        formal->typeExpr->replace(new BlockStmt(call->baseExpr->remove()));
        formal->addFlag(FLAG_MARKED_GENERIC);
      }
    }
  }
}

static void
find_printModuleInit_stuff() {
  std::vector<Symbol*> symbols;

  collectSymbols(printModuleInitModule, symbols);

  for_vector(Symbol, symbol, symbols) {
    if (symbol->hasFlag(FLAG_PRINT_MODULE_INIT_INDENT_LEVEL)) {
      gModuleInitIndentLevel = toVarSymbol(symbol);
      INT_ASSERT(gModuleInitIndentLevel);

    } else if (symbol->hasFlag(FLAG_PRINT_MODULE_INIT_FN)) {
      gPrintModuleInitFn = toFnSymbol(symbol);
      INT_ASSERT(gPrintModuleInitFn);
    }
  }
}


static void change_method_into_constructor(FnSymbol* fn) {
  if (fn->numFormals() <= 1)
    return;

  // This function must be a method.
  if (fn->getFormal(1)->type != dtMethodToken)
    return;

  // The second argument is 'this'.
  // For starters, it needs a known type.
  if (fn->getFormal(2)->type == dtUnknown)
    INT_FATAL(fn, "'this' argument has unknown type");

  // Now check that the function name matches the name of the type
  // attached to 'this' or matches 'init'.
  bool isCtor = (0 == strcmp(fn->getFormal(2)->type->symbol->name, fn->name));
  bool isInit = (0 == strcmp(fn->name, "init"));
  if (!isCtor && !isInit)
    return;

  // The type must be a class type.
  // No constructors for records? <hilde>
  AggregateType* ct = toAggregateType(fn->getFormal(2)->type);
  if (!ct)
    INT_FATAL(fn, "initializer on non-class type");

  if (fn->hasFlag(FLAG_NO_PARENS)) {
    USR_FATAL(fn, "a%s cannot be declared without parentheses", isCtor ? " constructor" : "n initializer");
  }

  if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
    // We hadn't previously seen a constructor or initializer definition.
    // Update the field on the type appropriately.
    if (isInit) {
      ct->initializerStyle = DEFINES_INITIALIZER;
    } else if (isCtor) {
      ct->initializerStyle = DEFINES_CONSTRUCTOR;
    } else {
      // Should never reach here, but just in case...
      INT_FATAL(fn, "Function was neither a constructor nor an initializer");
    }
  } else if ((ct->initializerStyle == DEFINES_CONSTRUCTOR && !isCtor) ||
             (ct->initializerStyle == DEFINES_INITIALIZER && !isInit)) {
    // We've previously seen a constructor but this new method is an initializer
    // or we've previously seen an initializer but this new method is a
    // constructor.  We don't allow both to be defined on a type.
    USR_FATAL_CONT(fn, "Definition of both constructor '%s' and initializer 'init'.  Please choose one.", ct->symbol->name);
  }

  // Call the initializer, passing in just the generic arguments.
  // This call ensures that the object is default-initialized before the user's
  // constructor body is called.
  CallExpr* call = new CallExpr(ct->defaultInitializer);
  for_formals(defaultTypeConstructorArg, ct->defaultTypeConstructor) {
    ArgSymbol* arg = NULL;
    for_formals(methodArg, fn) {
      if (defaultTypeConstructorArg->name == methodArg->name) {
        arg = methodArg;
      }
    }
    if (!arg) {
      if (!defaultTypeConstructorArg->defaultExpr)
        USR_FATAL_CONT(fn, "initializer for class '%s' requires a generic argument called '%s'", ct->symbol->name, defaultTypeConstructorArg->name);
    } else {
      call->insertAtTail(new NamedExpr(arg->name, new SymExpr(arg)));
    }
  }

  if (ct->initializerStyle == DEFINES_INITIALIZER) {
    ArgSymbol* meme = new ArgSymbol(INTENT_BLANK, "meme", ct, NULL,
                                    new SymExpr(gTypeDefaultToken));
    meme->addFlag(FLAG_IS_MEME);
    fn->insertFormalAtTail(meme);
    call->insertAtTail(new NamedExpr ("meme", new SymExpr(meme)));

    handleInitializerRules(fn, ct);
  }

  fn->_this = new VarSymbol("this");
  fn->_this->addFlag(FLAG_ARG_THIS);
  fn->insertAtHead(new CallExpr(PRIM_MOVE, fn->_this, call));
  fn->insertAtHead(new DefExpr(fn->_this));
  fn->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(fn->_this)));

  SymbolMap map;
  map.put(fn->getFormal(2), fn->_this);
  fn->formals.get(2)->remove();
  fn->formals.get(1)->remove();
  update_symbols(fn, &map);

  if (isCtor) {
    // The constructor's name is the name of the type.  Replace it with
    // _construct_typename
    fn->name = ct->defaultInitializer->name;
  }
  fn->addFlag(FLAG_CONSTRUCTOR);
}

// Note 1: Since param variables can only be of primitive or enumerated type,
// their destructors are trivial.  Allowing this case to proceed could result in
// a regularization (reduction in # of conditionals == reduction in code
// complexity).
// Note 2: "this" should be passed by reference.  Then, no constructor call is
// made, and therefore no autodestroy call is needed.
// Note 3: If a record arg to an init copy function is passed by value, infinite
// recursion would ensue.  This is an unreachable case (assuming that magic
// conversions from R -> ref R are removed and all existing implementations of
// chpl__initCopy are rewritten using "ref" or "const ref" intent on the record
// argument).
// Note 4: These two cases should be regularized.  Either the copy constructor
// should *always* be called (and the corresponding destructor always called),
// or we should ensure that the destructor is called only if a constructor is
// called on the same variable.  The latter case is an optimization, so the
// simplest implementation calls the copy-constructor in both cases.
