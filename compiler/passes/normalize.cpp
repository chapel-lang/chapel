/*
 * Copyright 2004-2017 Cray Inc.
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
#include "typeSpecifier.h"

#include <cctype>
#include <set>
#include <vector>

bool normalized = false;

static void insertModuleInit();
static FnSymbol* toModuleDeinitFn(ModuleSymbol* mod, Expr* stmt);
static void handleModuleDeinitFn(ModuleSymbol* mod);
static void transformLogicalShortCircuit();
static void lowerReduceAssign();

static void fixup_array_formals(FnSymbol* fn);
static void fixup_query_formals(FnSymbol* fn);


static bool isConstructor(FnSymbol* fn);
static bool isInitMethod (FnSymbol* fn);

static void updateConstructor(FnSymbol* fn);
static void updateInitMethod (FnSymbol* fn);

static void normalizeTheProgram();
static void checkUseBeforeDefs();
static void moveGlobalDeclarationsToModuleScope();
static void insertUseForExplicitModuleCalls(void);

static void hack_resolve_types(ArgSymbol* arg);

static void find_printModuleInit_stuff();

static void processSyntacticDistributions(CallExpr* call);
static void normalize(BaseAST* base);
static void normalizeReturns(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void applyGetterTransform(CallExpr* call);
static void insertCallTemps(CallExpr* call);

static void normalizeTypeAlias(DefExpr* defExpr);
static void normalizeArrayAlias(DefExpr* defExpr);
static void normalizeConfigVariableDefinition(DefExpr* defExpr);
static void normalizeVariableDefinition(DefExpr* defExpr);

static void normRefVar(DefExpr* defExpr);

static void init_untyped_var(VarSymbol* var,
                             Expr*      init,
                             Expr*      insert,
                             VarSymbol* constTemp);

static void init_typed_var(VarSymbol* var,
                           Expr*      type,
                           Expr*      insert,
                           VarSymbol* constTemp);

static void init_typed_var(VarSymbol* var,
                           Expr*      type,
                           Expr*      init,
                           Expr*      insert,
                           VarSymbol* constTemp);

static void init_noinit_var(VarSymbol* var,
                            Expr*      type,
                            Expr*      init,
                            Expr*      insert,
                            VarSymbol* constTemp);

static bool moduleHonorsNoinit(Symbol* var, Expr* init);

static void updateVariableAutoDestroy(DefExpr* defExpr);

static void clone_for_parameterized_primitive_formals(FnSymbol* fn,
                                                      DefExpr*  defExpr,
                                                      int       width);
static void replace_query_uses(ArgSymbol*             formal,
                               DefExpr*               defExpr,
                               CallExpr*              query,
                               std::vector<SymExpr*>& symExprs);

static void add_to_where_clause(ArgSymbol* formal,
                                Expr*      expr,
                                CallExpr*  query);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void normalize() {
  insertModuleInit();

  transformLogicalShortCircuit();

  lowerReduceAssign();

  forv_Vec(AggregateType, at, gAggregateTypes) {
    if (isNonGenericClassWithInitializers(at)  == true ||
        isNonGenericRecordWithInitializers(at) == true) {
      preNormalizeFields(at);
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    SET_LINENO(fn);

    if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)    == false &&
        fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == false) {
      fixup_array_formals(fn);
    }

    fixup_query_formals(fn);

    if (isConstructor(fn) == true) {
      updateConstructor(fn);
    } else if (isInitMethod(fn) == true) {
      updateInitMethod(fn);
    }
  }

  normalizeTheProgram();

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
            (isDirectlyUnderBlockStmt(se) || !isBlockStmt(se->parentExpr))) {
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
    if (arg->defPoint->parentSymbol) {
      hack_resolve_types(arg);
    }
  }

  // perform some checks on destructors
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->formals.length           <  2 ||
          fn->getFormal(1)->typeInfo() != gMethodToken->typeInfo()) {
        USR_FATAL(fn, "destructors must be methods");

      } else if (fn->formals.length > 2) {
        USR_FATAL(fn, "destructors must not have arguments");

      } else {
        DefExpr*       thisDef = toDefExpr(fn->formals.get(2));
        AggregateType* ct      = toAggregateType(thisDef->sym->type);

        INT_ASSERT(thisDef);

        // verify the name of the destructor
        bool notTildeName = (fn->name[0] != '~') ||
                             strcmp(fn->name + 1, ct->symbol->name);
        bool notDeinit = strcmp(fn->name, "deinit");

        if (ct && notDeinit && notTildeName) {
          USR_FATAL(fn,
            "destructor name must match class/record name or deinit()");
        } else {
          fn->name = astr("deinit");
        }
      }
    // make sure methods don't attempt to overload operators
    } else if (isalpha(fn->name[0])         == 0   &&
               fn->name[0]                  != '_' &&
               fn->formals.length           >  1   &&
               fn->getFormal(1)->typeInfo() == gMethodToken->typeInfo()) {
      USR_FATAL(fn, "invalid method name");
    }
  }

  find_printModuleInit_stuff();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void normalize(FnSymbol* fn) {
  normalize((BaseAST*) fn);
}

/************************************* | **************************************
*                                                                             *
* Insert the module initFn in to every module in allModules.  The current     *
* implementation pulls the entire module in to the prototypical initFn and    *
* then lets the rest of normalize sort things out.                            *
* Also stash away the module deinitFn, if the user has written one.           *
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
      if (stmt->isModuleDefinition() == false) {
        if (FnSymbol* deinitFn = toModuleDeinitFn(mod, stmt))
          mod->deinitFn = deinitFn; // the rest is in handleModuleDeinitFn()
        else
          mod->initFn->insertAtTail(stmt->remove());
      }
    }

    mod->block->insertAtHead(new DefExpr(mod->initFn));
    handleModuleDeinitFn(mod);

    //
    // If the module has the EXPORT_INIT flag then
    // propagate it to the module's init function
    //
    if (mod->hasFlag(FLAG_EXPORT_INIT) == true) {
      mod->initFn->addFlag(FLAG_EXPORT);
      mod->initFn->addFlag(FLAG_LOCAL_ARGS);
    }
  }
  USR_STOP();
}

static FnSymbol* toModuleDeinitFn(ModuleSymbol* mod, Expr* stmt) {
  if (DefExpr* def = toDefExpr(stmt))
    if (FnSymbol* fn = toFnSymbol(def->sym))
      // When we retire ~classname naming for deinits,
      // we can replace this strcmp with a check for FLAG_DESTRUCTOR.
      if (!strcmp(fn->name, "deinit"))
        if (fn->numFormals() == 0) {
          if (mod->deinitFn) {
            // Already got one deinit() before.
            // We could allow multiple deinit() fns and merge their contents.
            // If so, beware of possible 'return' stmts in each.
            USR_FATAL_CONT(def,
              "an additional module deinit() function is not allowed");
            USR_PRINT(mod->deinitFn,
              "the first deinit() function is declared here");
            // Let the duplicate play like an ordinary function until USR_STOP.
            return NULL;
          }

        return fn;
        }

  // Not a deinit.
  return NULL;
}

static void handleModuleDeinitFn(ModuleSymbol* mod) {
  FnSymbol* deinitFn = mod->deinitFn;
  if (!deinitFn)
    // We could alternatively create an empty function here.
    return;

  deinitFn->name = astr("chpl__deinit_", mod->name);
  deinitFn->removeFlag(FLAG_DESTRUCTOR);
  // For now leave deinitFn->defPoint wherever the user put it.
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

static void normalizeTheProgram() {
  normalize(theProgram);
}

// the following function is called from multiple places,
// e.g., after generating default or wrapper functions
static void normalize(BaseAST* base) {
  //
  // Phase 1
  //
  std::vector<CallExpr*> calls1;

  collectCallExprs(base, calls1);

  for_vector(CallExpr, call, calls1) {
    processSyntacticDistributions(call);
  }

  //
  // Phase 2
  //
  std::vector<Symbol*> symbols;

  collectSymbols(base, symbols);

  for_vector(Symbol, symbol, symbols) {
    if (FnSymbol* fn = toFnSymbol(symbol)) {
      normalizeReturns(fn);
    }
  }

  //
  // Phase 3
  //
  for_vector(Symbol, symbol, symbols) {
    if (VarSymbol* var = toVarSymbol(symbol)) {
      DefExpr* defExpr = var->defPoint;

      if (FnSymbol* fn = toFnSymbol(defExpr->parentSymbol)) {
        if (fn != stringLiteralModule->initFn) {
          Expr* type = defExpr->exprType;
          Expr* init = defExpr->init;

          if (type != NULL || init != NULL) {
            if (var->isType() == true) {
              normalizeTypeAlias(defExpr);

            } else if (var->hasFlag(FLAG_ARRAY_ALIAS) == true) {
              normalizeArrayAlias(defExpr);

            } else if (var->hasFlag(FLAG_CONFIG) == true) {
              normalizeConfigVariableDefinition(defExpr);

            } else {
              normalizeVariableDefinition(defExpr);
            }

            updateVariableAutoDestroy(defExpr);
          }
        }
      }
    }
  }

  //
  // Phase 4
  //
  std::vector<CallExpr*> calls2;

  collectCallExprs(base, calls2);

  for_vector(CallExpr, call, calls2) {
    applyGetterTransform(call);
    insertCallTemps(call);
  }

  for_vector(CallExpr, call, calls2) {
    call_constructor_for_class(call);
  }
}

/************************************* | **************************************
*                                                                             *
* We can't really do this before resolution, because we need to know if       *
* symbols used as actual arguments are passed by ref, inout, or out           *
* (all of which would be considered definitions).                             *
*                                                                             *
* The workaround for this has been early initialization -- which is redundant *
* with guaranteed initialization, at least with respect to class instances.   *
*                                                                             *
************************************** | *************************************/

static Symbol* theDefinedSymbol(BaseAST* ast);

static void checkUseBeforeDefs() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint->parentSymbol) {
      ModuleSymbol*         mod = fn->getModule();

      std::set<Symbol*>     defined;

      std::set<Symbol*>     undefined;
      std::set<const char*> undeclared;

      std::vector<BaseAST*> asts;

      collect_asts_postorder(fn, asts);

      for_vector(BaseAST, ast, asts) {
        if (Symbol* sym = theDefinedSymbol(ast)) {
          defined.insert(sym);

        } else if (SymExpr* se = toSymExpr(ast)) {
          Symbol* sym = se->symbol();

          if (isModuleSymbol(sym)                    == true  &&
              isFnSymbol(fn->defPoint->parentSymbol) == false &&
              isUseStmt(se->parentExpr)              == false) {
            SymExpr* prev = toSymExpr(se->prev);

            if (prev == NULL || prev->symbol() != gModuleToken) {
              USR_FATAL_CONT(se, "illegal use of module '%s'", sym->name);
            }

          } else if (isLcnSymbol(sym) == true) {
            if (sym->defPoint->parentExpr != rootModule->block) {
              Symbol* parent = sym->defPoint->parentSymbol;

              if (parent == fn || (parent == mod && mod->initFn == fn)) {
                if (defined.find(sym)           == defined.end() &&

                    sym->hasFlag(FLAG_ARG_THIS) == false         &&
                    sym->hasFlag(FLAG_EXTERN)   == false         &&
                    sym->hasFlag(FLAG_TEMP)     == false) {

                  // Only complain one time
                  if (undefined.find(sym) == undefined.end()) {
                    USR_FATAL_CONT(se,
                                   "'%s' used before defined (first used here)",
                                   sym->name);

                    undefined.insert(sym);
                  }
                }
              }
            }
          }

        } else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(ast)) {
          CallExpr* call = toCallExpr(use->parentExpr);

          if (call == NULL ||
              (call->baseExpr                              != use   &&
               call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL) == false &&
               call->isPrimitive(PRIM_CAPTURE_FN_FOR_C)    == false)) {
            if (isFnSymbol(fn->defPoint->parentSymbol) == false) {
              const char* name = use->unresolved;

              // Only complain one time
              if (undeclared.find(name) == undeclared.end()) {
                USR_FATAL_CONT(use,
                               "'%s' undeclared (first use this function)",
                               name);

                undeclared.insert(name);
              }
            }
          }
        }
      }
    }
  }
}

// If the AST node defines a symbol, then extract that symbol
static Symbol* theDefinedSymbol(BaseAST* ast) {
  Symbol* retval = NULL;

  // A symbol is "defined" if it is the LHS of a move, an assign,
  // or a variable initialization.
  //
  // The caller performs a post-order traversal and so we find the
  // symExpr before we see the callExpr
  if (SymExpr* se = toSymExpr(ast)) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE)     == true  ||
          call->isPrimitive(PRIM_ASSIGN)   == true  ||
          call->isPrimitive(PRIM_INIT_VAR) == true)  {
        if (call->get(1) == se) {
          retval = se->symbol();
        }
      }
    }

  } else if (DefExpr* def = toDefExpr(ast)) {
    Symbol* sym = def->sym;

    // All arg symbols are defined.
    if (isArgSymbol(sym)) {
      retval = sym;

    } else if (VarSymbol* var = toVarSymbol(sym)) {
      // All type aliases are taken as defined.
      if (var->isType() == true) {
        retval = var;
      } else {
        Type* type = var->typeInfo();

        // All variables of type 'void' are treated as defined.
        if (type == dtVoid) {
          retval = var;

        // The primitive scalars are treated as defined
        } else if (isPrimitiveScalar(type) == true) {
          retval = var;

        // non generic records with initializers are defined
        } else if (AggregateType* at = toAggregateType(type)) {
          if (isNonGenericRecordWithInitializers(at) == true) {
            retval = var;
          }
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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
          //
          // Make an exception for references to array slices.
          if (vs->hasFlag(FLAG_TEMP)) {
            // is this a call_tmp that is later stored in a ref variable?
            // if so, move the call_tmp to global scope as well. E.g.
            //   var MyArray:[1..20] int;
            //   ref MySlice = MyArray[1..10];

            // Look for global = PRIM_ADDR_OF var
            //          global with flag FLAG_REF_VAR.
            bool refToTempInGlobal = false;
            for_SymbolSymExprs(se, vs) {
              if (CallExpr* addrOf = toCallExpr(se->parentExpr))
                if (addrOf->isPrimitive(PRIM_ADDR_OF))
                  if (CallExpr* move = toCallExpr(addrOf->parentExpr))
                    if (move->isPrimitive(PRIM_MOVE)) {
                      SymExpr* lhs = toSymExpr(move->get(1));
                      if (lhs->symbol()->hasFlag(FLAG_REF_VAR))
                        refToTempInGlobal = true;
                    }
            }

            if (refToTempInGlobal == false)
              continue;
          }

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

/************************************* | **************************************
*                                                                             *
* Following normalization, each function contains only one return statement   *
* preceded by a label.  The first half of the function counts the total       *
* number of returns and the number of void returns.                           *
*                                                                             *
* The big IF beginning with if (rets.n == 1) determines if the function is    *
* already normal.                                                             *
*                                                                             *
* The last half of the function performs the normalization steps.             *
*                                                                             *
************************************** | *************************************/

static bool isVoidReturn(CallExpr* call);
static void insertRetMove(FnSymbol* fn, VarSymbol* retval, CallExpr* ret);

static void normalizeReturns(FnSymbol* fn) {
  SET_LINENO(fn);

  std::vector<CallExpr*> rets;
  std::vector<CallExpr*> calls;
  size_t                 numVoidReturns = 0;
  size_t                 numYields      = 0;
  CallExpr*              theRet         = NULL;
  bool                   isIterator     = fn->isIterator();

  collectMyCallExprs(fn, calls, fn);

  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_RETURN) == true) {
      rets.push_back(call);

      theRet = call;

      if (isVoidReturn(call) == true) {
        numVoidReturns++;
      }

    } else if (call->isPrimitive(PRIM_YIELD)) {
      rets.push_back(call);

      numYields++;
    }
  }

  // Check if this function's returns are already normal.
  if (rets.size() == numYields + 1 && theRet == fn->body->body.last()) {
    if (SymExpr* se = toSymExpr(theRet->get(1))) {
      if (fn->hasFlag(FLAG_CONSTRUCTOR)         == true ||
          fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)    == true ||
          strncmp("_if_fn", fn->name, 6)        ==    0 ||
          strcmp ("=",      fn->name)           ==    0 ||
          strcmp ("_init",  fn->name)           ==    0||
          strcmp ("_ret",   se->symbol()->name) ==    0) {
        return;
      }
    }
  }

  // Add a void return if needed.
  if (rets.size() == 0 && fn->retExprType == NULL) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    return;
  }

  LabelSymbol* label       = new LabelSymbol(astr("_end_", fn->name));
  bool         labelIsUsed = false;
  VarSymbol*   retval      = NULL;

  label->addFlag(FLAG_EPILOGUE_LABEL);

  fn->insertAtTail(new DefExpr(label));

  // If a proc has a void return, do not return any values ever.
  // (Types are not resolved yet, so we judge by presence of "void returns"
  // i.e. returns with no expr. See also a related check in semanticChecks.)
  if (isIterator == false && numVoidReturns != 0) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  } else {
    // Handle declared return type.
    retval = newTemp("ret", fn->retType);

    retval->addFlag(FLAG_RVV);

    if (fn->retTag == RET_PARAM) {
      retval->addFlag(FLAG_PARAM);
    }

    if (fn->retTag == RET_TYPE) {
      retval->addFlag(FLAG_TYPE_VARIABLE);
    }

    if (fn->hasFlag(FLAG_MAYBE_TYPE)) {
      retval->addFlag(FLAG_MAYBE_TYPE);
    }

    if (isIterator == true) {
      if (fn->retExprType != NULL && fn->retTag != RET_REF) {
        if (SymExpr* lastRTE = toSymExpr(fn->retExprType->body.tail)) {
          if (TypeSymbol* retSym = toTypeSymbol(lastRTE->symbol())) {
            if (retSym->type == dtVoid) {
              USR_FATAL_CONT(fn,
                             "an iterator's return type cannot be 'void'; "
                             "if specified, it must be the type of the "
                             "expressions the iterator yields");
            }
          }
        }
      }
    }

    fn->insertAtHead(new DefExpr(retval));
    fn->insertAtTail(new CallExpr(PRIM_RETURN, retval));
  }

  // Now, for each return statement appearing in the function body,
  // move the value of its body into the declared return value.
  for_vector(CallExpr, ret, rets) {
    SET_LINENO(ret);

    if (isIterator == true) {
      INT_ASSERT(retval != NULL);

      // Three cases:
      // (1) yield expr; => mov _ret expr; yield _ret;
      // (2) return; => goto end_label;
      // (3) return expr; -> mov _ret expr; yield _ret; goto end_label;
      // Notice how (3) is the composition of (1) and (2).
      if (isVoidReturn(ret) == false) { // Cases 1 and 3
        insertRetMove(fn, retval, ret);
        ret->insertBefore(new CallExpr(PRIM_YIELD, retval));
      }

      if (ret->isPrimitive(PRIM_YIELD) == true) { // Case 1 only.
        // it's a yield => no goto; need to remove the original node
        ret->remove();
      } else {    // Cases 2 and 3.
        if (ret->next != label->defPoint) {
          ret->replace(new GotoStmt(GOTO_RETURN, label));

          labelIsUsed = true;
        } else {
          ret->remove();
        }
      }


    // Not an iterator
    } else {
      if (retval != NULL) {
        insertRetMove(fn, retval, ret);
      }

      // replace with GOTO(label)
      if (ret->next != label->defPoint) {
        ret->replace(new GotoStmt(GOTO_RETURN, label));

        labelIsUsed = true;
      } else {
        ret->remove();
      }
    }
  }

  if (labelIsUsed == false) {
    label->defPoint->remove();
  }
}

static bool isVoidReturn(CallExpr* call) {
  bool retval = false;

  if (call->isPrimitive(PRIM_RETURN) == true) {
    if (SymExpr* arg = toSymExpr(call->get(1))) {
      retval = (arg->symbol() == gVoid) ? true : false;
    }
  }

  return retval;
}

static void insertRetMove(FnSymbol* fn, VarSymbol* retval, CallExpr* ret) {
  Expr* retExpr = ret->get(1)->remove();

  if (fn->returnsRefOrConstRef() == true) {
    CallExpr* addrOf = new CallExpr(PRIM_ADDR_OF, retExpr);

    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, addrOf));

  } else if (fn->retExprType != NULL) {
    Expr*     tail   = fn->retExprType->body.tail;
    CallExpr* coerce = new CallExpr(PRIM_COERCE, retExpr, tail->copy());

    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, coerce));

  } else if (fn->hasFlag(FLAG_MAYBE_REF) == true) {
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, retExpr));

  } else if (fn->hasFlag(FLAG_WRAPPER)             == false &&
             strcmp(fn->name, "iteratorIndex")     !=     0 &&
             strcmp(fn->name, "iteratorIndexHelp") !=     0) {
    CallExpr* deref = new CallExpr(PRIM_DEREF, retExpr);

    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, deref));

  } else {
    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, retExpr));
  }
}

/************************************* | **************************************
*                                                                             *
* If se is a type alias, resolves it recursively, or fails and returns NULL.  *
*                                                                             *
************************************** | *************************************/

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
      if (ct->symbol->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
        // Call chpl__buildDistType for syntactic distributions.
        se->replace(new UnresolvedSymExpr("chpl__buildDistType"));
      } else {
        if (ct->initializerStyle == DEFINES_INITIALIZER && ct->isGeneric()) {
          USR_FATAL_CONT(se, "Type constructors are not yet supported for generic types that define initializers.  As a workaround, try relying on type inference");
        }

        // Transform C ( ... ) into _type_construct_C ( ... ) .
        se->replace(new UnresolvedSymExpr(ct->defaultTypeConstructor->name));
      }
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

    symExpr->remove();

    if (VarSymbol* var = toVarSymbol(symExpr->symbol())) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        const char* str = var->immediate->v_string;

        call->baseExpr->replace(new UnresolvedSymExpr(str));

        call->insertAtHead(gMethodToken);

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

    if (CallExpr* parent = toCallExpr(call->parentExpr)) {
      if (parent->baseExpr == call) {
        call->partialTag = true;
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool  shouldInsertCallTemps(CallExpr* call);
static void  evaluateAutoDestroy(CallExpr* call, VarSymbol* tmp);
static bool  moveMakesTypeAlias(CallExpr* call);
static Type* typeForNewNonGenericRecord(CallExpr* call);

static void insertCallTemps(CallExpr* call) {
  if (shouldInsertCallTemps(call) == true) {
    SET_LINENO(call);

    CallExpr*  parentCall = toCallExpr(call->parentExpr);
    Expr*      stmt       = call->getStmtExpr();
    VarSymbol* tmp        = newTemp("call_tmp");

    // Add FLAG_EXPR_TEMP unless this tmp is being used
    // as a sub-expression for a variable initialization.
    // This flag triggers autoCopy/autoDestroy behavior.
    if (parentCall == NULL ||
        (parentCall->isNamed("chpl__initCopy")  == false &&
         parentCall->isPrimitive(PRIM_INIT_VAR) == false)) {
      tmp->addFlag(FLAG_EXPR_TEMP);
    }

    if (call->isPrimitive(PRIM_NEW)    == true) {
      tmp->addFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);
    }

    if (call->isPrimitive(PRIM_TYPEOF) == true) {
      tmp->addFlag(FLAG_TYPE_VARIABLE);
    }

    evaluateAutoDestroy(call, tmp);

    tmp->addFlag(FLAG_MAYBE_PARAM);
    tmp->addFlag(FLAG_MAYBE_TYPE);

    if (call->isNamed("super")   == true &&

        parentCall               != NULL &&
        parentCall->isNamed(".") == true &&
        parentCall->get(1)       == call) {
      // We've got an access to a method or field on the super type.
      // This means we should preserve that knowledge for when we
      // attempt to access the method on the super type.
      tmp->addFlag(FLAG_SUPER_TEMP);
    }

    // Define the tmp
    stmt->insertBefore(new DefExpr(tmp));

    // Is this a new-expression for a record with an initializer?
    if (Type* type = typeForNewNonGenericRecord(call)) {
      // Define the type for the tmp
      tmp->type = type;

      // 2017/03/14: call has the form prim_new(MyRec(a, b, c))
      // Extract the argument to the new expression
      CallExpr* newArg = toCallExpr(call->get(1));

      // Convert the argument for the new-expression into an init call
      newArg->setUnresolvedFunction("init");

      // Add _mt and _this (insert at head in reverse order)
      newArg->insertAtHead(tmp);
      newArg->insertAtHead(gMethodToken);

      // Move the tmp.init(args) expression to before the call
      stmt->insertBefore(newArg->remove());

      // Replace the degenerate new-expression with a use of the tmp variable
      call->replace(new SymExpr(tmp));


    // No.  The simple case
    } else {
      call->replace(new SymExpr(tmp));

      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, call));
    }
  }
}

static bool shouldInsertCallTemps(CallExpr* call) {
  Expr*     parentExpr = call->parentExpr;
  CallExpr* parentCall = toCallExpr(parentExpr);
  Expr*     stmt       = call->getStmtExpr();
  bool      retval     = false;

  if        (parentExpr                               == NULL) {
    retval = false;

  } else if (isDefExpr(parentExpr)                    == true) {
    retval = false;

  } else if (stmt                                     == NULL) {
    retval = false;

  } else if (call                                     == stmt) {
    retval = false;

  } else if (call->partialTag                         == true) {
    retval = false;

  } else if (call->isPrimitive(PRIM_TUPLE_EXPAND)     == true) {
    retval = false;

  } else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) == true) {
    retval = false;

  } else if (parentCall && parentCall->isPrimitive(PRIM_MOVE)) {
    retval = false;

  } else if (parentCall && parentCall->isPrimitive(PRIM_NEW))  {
    retval = false;

  } else {
    retval =  true;
  }

  return retval;
}

static void evaluateAutoDestroy(CallExpr* call, VarSymbol* tmp) {
  Expr*     parentExpr = call->parentExpr;
  CallExpr* parentCall = toCallExpr(parentExpr);
  FnSymbol* fn         = call->getFunction();

  // Noakes 2015/11/02
  //   The expansion of _build_tuple() creates temps that need to be
  //   autoDestroyed.  This is a short-cut to arrange for that to occur.
  //   A better long term solution would be preferred
  if (call->isNamed("chpl__initCopy")     == true &&
      parentCall                          != NULL &&
      parentCall->isNamed("_build_tuple") == true) {
    tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  // MPF 2016-10-20
  // This is a workaround for a problem in
  //   types/typedefs/bradc/arrayTypedef
  //
  // I'm sure that there is a better way to handle this either in the
  // module init function or in a sequence of parloopexpr functions
  // computing an array type that are in a module init fn

  while (fn->hasFlag(FLAG_MAYBE_ARRAY_TYPE) == true) {
    fn = fn->defPoint->getFunction();
  }

  if (fn == fn->getModule()->initFn) {
    CallExpr* cur = parentCall;
    CallExpr* sub = call;

    // Look for a parent call that is either:
    //  making an array type alias, or
    //  passing the result into the 2nd argument of buildArrayRuntimeType.
    while (cur != NULL) {
      if (moveMakesTypeAlias(cur) == true) {
        break;

      } else if (cur->isNamed("chpl__buildArrayRuntimeType") == true &&
                 cur->get(2)                                 == sub) {
        break;

      } else {
        sub = cur;
        cur = toCallExpr(cur->parentExpr);
      }
    }

    if (cur) {
      tmp->addFlag(FLAG_NO_AUTO_DESTROY);
    }
  }
}

static bool moveMakesTypeAlias(CallExpr* call) {
  bool retval = false;

  if (call->isPrimitive(PRIM_MOVE)) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (VarSymbol* var = toVarSymbol(se->symbol())) {
        retval = var->isType();
      }
    }
  }

  return retval;
}

//
// If this is a new-expression for a non-generic record with an initializer
// then return the type for the initializer
//
// 2017/03/14 This currently runs before new expressions have been
// normalized.
//
// Before normalization, a new expression is usually
//
//    prim_new(MyRec(a, b, c))
//
// and this is the form that is currently recognized
//
//
// After normalization, it will generally be
//
//    prim_new(MyRec, a, b, c);

static Type* typeForNewNonGenericRecord(CallExpr* call) {
  Type* retval = NULL;

  if (call->isPrimitive(PRIM_NEW) == true && call->numActuals() == 1) {
    if (CallExpr* arg1 = toCallExpr(call->get(1))) {
      if (SymExpr* base = toSymExpr(arg1->baseExpr)) {
        if (TypeSymbol* sym = toTypeSymbol(base->symbol())) {
          if (isNonGenericRecordWithInitializers(sym->type) == true) {
            retval = sym->type;
          }
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void normalizeTypeAlias(DefExpr* defExpr) {
  SET_LINENO(defExpr);

  Symbol* var  = defExpr->sym;
  Expr*   type = defExpr->exprType;
  Expr*   init = defExpr->init;

  INT_ASSERT(type == NULL);
  INT_ASSERT(init != NULL);

  defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, init->copy()));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void normalizeArrayAlias(DefExpr* defExpr) {
  SET_LINENO(defExpr);

  Symbol* var  = defExpr->sym;
  Expr*   init = defExpr->init->remove();

  if (defExpr->exprType == NULL) {
    CallExpr* newAlias = new CallExpr("newAlias", gMethodToken, init);

    defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, newAlias));

  } else {
    Expr*     type    = defExpr->exprType->remove();
    CallExpr* reindex = new CallExpr("reindex",  gMethodToken, init);
    CallExpr* partial = new CallExpr(reindex,    type);

    reindex->partialTag = true;
    reindex->methodTag  = true;

    defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, partial));
  }
}

/************************************* | **************************************
*                                                                             *
* Config variables are fundamentally different form non-configs especially    *
* for multi-locale programs. Non-param config variables e.g.                  *
*                                                                             *
*   config var x : int = 10;                                                  *
*                                                                             *
* should be "initialized" in a manner that is approximately                   *
*                                                                             *
*   var x : int = no-init;                                                    *
*                                                                             *
*   if (!chpl_config_has_value("x", <module-name>)) then                      *
*     x = 10;                                                                 *
*   else                                                                      *
*     x = chpl_config_get_value("x", <module-name>);                          *
*                                                                             *
* and such that the conditional arms of the if-stmt implement initialization  *
* rather than assignment.  This requires additional care for config const and *
* multi-locale in order to enable privatization to be implemented correctly.  *
*                                                                             *
* Noakes Feb 17, 2017:                                                        *
*   The compiler has weaknesses with variable initialization which are a      *
* little more evident for config variables.  Configs have been split from     *
* non-configs to enable them to evolve independently in the nearer term.      *
*                                                                             *
* Additionally the current implementation has, undocumented and confusing,    *
* support for config ref and config const ref.  There has been discussion     *
* on whether to turn this in to a compile-time error or to continue the       *
* current support.                                                            *
*                                                                             *
************************************** | *************************************/

static CondStmt* assignConfig(VarSymbol* var,
                              VarSymbol* varTmp,
                              Expr*      noop);

static Symbol*   varModuleName(VarSymbol* var);

static void normalizeConfigVariableDefinition(DefExpr* defExpr) {
  SET_LINENO(defExpr);

  VarSymbol* var  = toVarSymbol(defExpr->sym);
  Expr*      type = defExpr->exprType;
  Expr*      init = defExpr->init;

  // Noakes: Feb 17, 2017
  //   config ref / const ref can be overridden at compile time.
  //   There is a proposal to convert this to a compile time error.
  if (var->hasFlag(FLAG_REF_VAR)) {
    normRefVar(defExpr);

  } else {
    VarSymbol* varTmp = var;
    Expr*      insert = defExpr;

    // insert code to initialize a config var/const
    // config param is evaluated at compile time
    if (var->hasFlag(FLAG_PARAM) == false) {
      if (var->hasFlag(FLAG_CONST)  ==  true &&
          var->hasFlag(FLAG_EXTERN) == false) {
        varTmp = newTemp("tmp");

        defExpr->insertBefore(new DefExpr(varTmp));
        defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, varTmp));
      }

      insert = new CallExpr(PRIM_NOOP);
      defExpr->insertAfter(assignConfig(var, varTmp, insert));
    }

    if (type == NULL) {
      init_untyped_var(var, init, insert, varTmp);

    } else if (init == NULL) {
      init_typed_var(var, type, insert, varTmp);

    } else if (var->hasFlag(FLAG_PARAM) == true) {
      CallExpr* cast = createCast(init->remove(), type->remove());

      insert->insertAfter(new CallExpr(PRIM_MOVE, var, cast));

    } else if (init->isNoInitExpr() == true) {
      init_noinit_var(var, type, init, insert, varTmp);

    } else {
      init_typed_var(var, type, init, insert, varTmp);
    }
  }
}

static CondStmt* assignConfig(VarSymbol* var, VarSymbol* varTmp, Expr* noop) {
  Symbol*    modName  = varModuleName(var);

  //
  // A fragment for the conditional test
  //
  SymExpr*   name0    = new SymExpr(new_CStringSymbol(var->name));
  CallExpr*  hasValue = new CallExpr("chpl_config_has_value", name0, modName);
  CallExpr*  test     = new CallExpr("!", hasValue);

  //
  // An "empty" block stmt for the consequent
  //
  BlockStmt* cons     = new BlockStmt(noop);

  //
  // The alternative sets the config from the command line
  //
  SymExpr*   name1    = new SymExpr(new_CStringSymbol(var->name));
  CallExpr*  typeOf   = new CallExpr(PRIM_TYPEOF, varTmp);

  SymExpr*   name2    = new SymExpr(new_CStringSymbol(var->name));
  CallExpr*  getValue = new CallExpr("chpl_config_get_value", name2, modName);

  CallExpr*  strToVal = new CallExpr("_command_line_cast",
                                     name1,
                                     typeOf,
                                     getValue);

  CallExpr*  moveTmp  = new CallExpr(PRIM_MOVE, varTmp, strToVal);
  BlockStmt* alt      = new BlockStmt(moveTmp);

  return new CondStmt(test, cons, alt);
}

static Symbol* varModuleName(VarSymbol* var) {
  ModuleSymbol* module     = var->getModule();
  bool          isInternal = module->modTag == MOD_INTERNAL;

  return new_CStringSymbol(isInternal ? "Built-in" : module->name);
}

static void init_untyped_var(VarSymbol* var,
                             Expr*      init,
                             Expr*      insert,
                             VarSymbol* constTemp) {
  if (var->hasFlag(FLAG_NO_COPY)) {
    insert->insertAfter(new CallExpr(PRIM_MOVE, var, init->remove()));

  } else {
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

    insert->insertAfter(new CallExpr(PRIM_MOVE, constTemp, rhs));
  }
}

static void init_typed_var(VarSymbol* var,
                           Expr*      type,
                           Expr*      insert,
                           VarSymbol* constTemp) {
  VarSymbol* typeTemp = newTemp("type_tmp");
  DefExpr*   typeDefn = new DefExpr(typeTemp);
  CallExpr*  initCall = new CallExpr(PRIM_INIT, type->remove());
  CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp, initCall);

  //
  // Noakes 2016/02/02
  // The code for resolving the type of an extern variable
  //
  //   functionResolution.cpp : resolveExternVarSymbols()
  //
  // expects to find the init code inside a block stmt.
  //
  // However the remaining cases do not need it.
  //
  if (var->hasFlag(FLAG_EXTERN) == true) {
    INT_ASSERT(var->hasFlag(FLAG_PARAM) == false);

    BlockStmt* block = new BlockStmt(NULL, BLOCK_EXTERN_TYPE);

    block->insertAtTail(typeDefn);
    block->insertAtTail(initMove);
    block->insertAtTail(new CallExpr(PRIM_MOVE, constTemp, typeTemp));

    insert->insertAfter(block);

  } else {
    if (var->hasFlag(FLAG_PARAM) == true) {
      typeTemp->addFlag(FLAG_PARAM);
    }

    insert->insertAfter(typeDefn);
    typeDefn->insertAfter(initMove);
    initMove->insertAfter(new CallExpr(PRIM_MOVE, constTemp, typeTemp));
  }
}

static void init_typed_var(VarSymbol* var,
                           Expr*      type,
                           Expr*      init,
                           Expr*      insert,
                           VarSymbol* constTemp) {
  VarSymbol* typeTemp = newTemp("type_tmp");
  DefExpr*   typeDefn = new DefExpr(typeTemp);
  CallExpr*  initCall = new CallExpr(PRIM_INIT, type->remove());
  CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp,  initCall);
  CallExpr*  assign   = new CallExpr("=",       typeTemp,  init->remove());
  CallExpr*  varMove  = new CallExpr(PRIM_MOVE, constTemp, typeTemp);

  insert->insertAfter(typeDefn);
  typeDefn->insertAfter(initMove);
  initMove->insertAfter(assign);
  assign->insertAfter(varMove);
}

static void init_noinit_var(VarSymbol* var,
                            Expr*      type,
                            Expr*      init,
                            Expr*      insert,
                            VarSymbol* constTemp) {
  init->remove();

  if (fUseNoinit == true || moduleHonorsNoinit(var, init) == true) {
    CallExpr* noinitCall = new CallExpr(PRIM_NO_INIT, type->remove());

    insert->insertAfter(new CallExpr(PRIM_MOVE, var, noinitCall));

  } else {
    // Ignore no-init expression and fall back on default init
    init_typed_var(var, type, insert, constTemp);
  }
}

/************************************* | **************************************
*                                                                             *
* normalizeVariableDefinition removes DefExpr::exprType and DefExpr::init     *
* from a variable's def expression, normalizing the AST with primitive        *
* moves, calls to chpl__initCopy, _init, and _cast, and assignments.          *
*                                                                             *
************************************** | *************************************/

static void           normVarTypeInference(DefExpr* expr);
static void           normVarTypeWoutInit(DefExpr* expr);
static void           normVarTypeWithInit(DefExpr* expr);
static void           normVarNoinit(DefExpr* defExpr);

static bool           isNewExpr(Expr* expr);
static AggregateType* typeForNewExpr(CallExpr* expr);

static void normalizeVariableDefinition(DefExpr* defExpr) {
  SET_LINENO(defExpr);

  VarSymbol* var  = toVarSymbol(defExpr->sym);
  Expr*      type = defExpr->exprType;
  Expr*      init = defExpr->init;

  // handle ref variables
  if (var->hasFlag(FLAG_REF_VAR)) {
    normRefVar(defExpr);

  } else if (type == NULL && init != NULL) {
    normVarTypeInference(defExpr);

  } else if (type != NULL && init == NULL) {
    normVarTypeWoutInit(defExpr);

  } else if (type != NULL && init != NULL) {
    if (var->hasFlag(FLAG_PARAM) == true) {
      CallExpr* cast = createCast(init->remove(), type->remove());

      defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, cast));

    } else if (init->isNoInitExpr() == true) {
      normVarNoinit(defExpr);

    } else {
      normVarTypeWithInit(defExpr);
    }

  } else {
    INT_ASSERT(false);
  }
}

static void normRefVar(DefExpr* defExpr) {
  VarSymbol* var         = toVarSymbol(defExpr->sym);
  Expr*      init        = defExpr->init;
  Expr*      varLocation = NULL;

  if (init == NULL) {
    USR_FATAL_CONT(var,
                   "References must be initialized when they are defined.");
  }

  // If this is a const reference to an immediate, we need to insert a temp
  // variable so we can take the address of it, non-const references to an
  // immediate are not allowed.
  if (var->hasFlag(FLAG_CONST)) {
    if (SymExpr* initSym = toSymExpr(init)) {
      if (initSym->symbol()->isImmediate()) {
        VarSymbol* constRefTemp  = newTemp("const_ref_immediate_tmp");

        defExpr->insertBefore(new DefExpr(constRefTemp));
        defExpr->insertBefore(new CallExpr(PRIM_MOVE,
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
    Symbol* symbol = sym->symbol();
    bool    error  = var->hasFlag(FLAG_CONST) == false &&
                     symbol->isConstant()     == true;

    // This is a workaround for the fact that isConstant for an
    // ArgSymbol with blank intent and type dtUnknown returns true,
    // but blank intent isn't necessarily const.
    if (ArgSymbol* arg = toArgSymbol(symbol)) {
      if (arg->intent == INTENT_BLANK && arg->type == dtUnknown) {
        error = false;
      }
    }

    if (error == true) {
      USR_FATAL_CONT(sym,
                     "Cannot set a non-const reference to a const variable.");
    }
  }

  defExpr->insertAfter(new CallExpr(PRIM_MOVE,
                                    var,
                                    new CallExpr(PRIM_ADDR_OF, varLocation)));
}

//
// const <name> = <value>;
// param <name> = <value>;
// var   <name> = <value>;
//
// The type of <name> will be inferred from the type of <value>
//
static void normVarTypeInference(DefExpr* defExpr) {
  Symbol* var      = defExpr->sym;
  Expr*   initExpr = defExpr->init->remove();

  // Do not complain here.  Put this stub in to the AST and let
  // checkUseBeforeDefs() generate a consistent error message.
  if (isUnresolvedSymExpr(initExpr) == true) {
    defExpr->insertAfter(new CallExpr(PRIM_INIT_VAR, var, initExpr));

  // e.g.
  //   var x = <immediate>;
  //   var y = <identifier>;
  } else if (SymExpr* initSym = toSymExpr(initExpr)) {
    Type* type = initSym->symbol()->type;

    if (isPrimitiveScalar(type) == true) {
      defExpr->insertAfter(new CallExpr(PRIM_MOVE,     var, initExpr));

      var->type = type;
    } else {
      defExpr->insertAfter(new CallExpr(PRIM_INIT_VAR, var, initExpr));
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (CallExpr* initCall = toCallExpr(initExpr)) {
    if (initCall->isPrimitive(PRIM_NEW) == true) {
      AggregateType* type = typeForNewExpr(initCall);

      if (isNonGenericRecordWithInitializers(type) == true) {
        Expr*     arg1    = initCall->get(1)->remove();
        CallExpr* argExpr = toCallExpr(arg1);

        // Insert the arg portion of the initExpr back into tree
        defExpr->insertAfter(argExpr);

        // Convert it in to a use of the init method
        argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

        // Add _mt and _this (insert at head in reverse order)
        argExpr->insertAtHead(var);
        argExpr->insertAtHead(gMethodToken);

      } else {
        defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, initExpr));
      }

      if (type != NULL && type->isGeneric() == false) {
        var->type = type;
      }

    } else {
      defExpr->insertAfter(new CallExpr(PRIM_INIT_VAR, var, initExpr));
    }

  } else {
    INT_ASSERT(false);
  }
}

//
// const <name> : <type>;
// param <name> : <type>;
// var   <name> : <type>;
//
// The type is explicit and the initial value is implied by the type
//

static void normVarTypeWoutInit(DefExpr* defExpr) {
  Symbol* var      = defExpr->sym;
  Expr*   typeExpr = defExpr->exprType->remove();
  Type*   type     = typeForTypeSpecifier(typeExpr);

  // Noakes 2016/02/02
  // The code for resolving the type of an extern variable
  //
  //   functionResolution.cpp : resolveExternVarSymbols()
  //
  // expects to find the init code inside a block stmt.
  if (var->hasFlag(FLAG_EXTERN) == true) {
    BlockStmt* block    = new BlockStmt(NULL, BLOCK_EXTERN_TYPE);

    VarSymbol* typeTemp = newTemp("type_tmp");
    DefExpr*   typeDefn = new DefExpr(typeTemp);
    CallExpr*  initCall = new CallExpr(PRIM_INIT, typeExpr);
    CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp, initCall);

    block->insertAtTail(typeDefn);
    block->insertAtTail(initMove);
    block->insertAtTail(new CallExpr(PRIM_MOVE, var, typeTemp));

    defExpr->insertAfter(block);

  } else if (isPrimitiveScalar(type) == true) {
    CallExpr* defVal = new CallExpr("_defaultOf", type->symbol);

    defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, defVal));

    var->type = type;

  } else if (isNonGenericClass(type) == true) {
    CallExpr* defVal = new CallExpr("_defaultOf", type->symbol);

    defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, defVal));

    var->type = type;

  } else if (isNonGenericRecordWithInitializers(type) == true) {
    defExpr->insertAfter(new CallExpr("init", gMethodToken, var));

    var->type = type;

  } else {
    VarSymbol* typeTemp = newTemp("type_tmp");
    DefExpr*   typeDefn = new DefExpr(typeTemp);
    CallExpr*  initCall = new CallExpr(PRIM_INIT, typeExpr);
    CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp, initCall);

    if (var->hasFlag(FLAG_PARAM) == true) {
      typeTemp->addFlag(FLAG_PARAM);
    }

    defExpr ->insertAfter(typeDefn);
    typeDefn->insertAfter(initMove);
    initMove->insertAfter(new CallExpr(PRIM_MOVE, var, typeTemp));
  }
}

static void normVarTypeWithInit(DefExpr* defExpr) {
  Symbol* var      = defExpr->sym;
  Expr*   typeExpr = defExpr->exprType->remove();
  Expr*   initExpr = defExpr->init->remove();
  Type*   type     = typeForTypeSpecifier(typeExpr);

  //
  // e.g. const x : int     = 10;
  //      var   y : int(32) = 20;
  //
  //      var   x : MyCls   = new MyCls(1, 2);
  //
  // Noakes 2017/02/25
  //    Use a temp to compute the value for the init-expression and
  //    use PRIM_MOVE to initialize x.  This simplifies const checking
  //    for the first case and supports a current limitation for RVF
  //
  if (isPrimitiveScalar(type) == true ||
      isNonGenericClass(type) == true) {
    VarSymbol* tmp = newTemp("tmp", type);

    defExpr->insertBefore(new DefExpr(tmp));
    defExpr->insertBefore(new CallExpr("=",      tmp, initExpr));

    defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, tmp));

    var->type = type;

  } else if (isNonGenericRecordWithInitializers(type) == true) {
    if (isNewExpr(initExpr) == true) {
      Expr*     arg     = toCallExpr(initExpr)->get(1)->remove();
      CallExpr* argExpr = toCallExpr(arg);

      // Insert the arg portion of the initExpr back into tree
      defExpr->insertAfter(argExpr);

      // Convert it in to a use of the init method
      argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

      // Add _mt and _this (insert at head in reverse order)
      argExpr->insertAtHead(var);
      argExpr->insertAtHead(gMethodToken);

    } else {
      CallExpr* init   = new CallExpr("init", gMethodToken, var);
      CallExpr* assign = new CallExpr("=",    var,          initExpr);

      defExpr->insertAfter(init);
      init->insertAfter(assign);
    }

    var->type = type;

  } else {
    VarSymbol* typeTemp = newTemp("type_tmp");
    DefExpr*   typeDefn = new DefExpr(typeTemp);
    CallExpr*  initCall = new CallExpr(PRIM_INIT, typeExpr);
    CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp,  initCall);
    CallExpr*  assign   = new CallExpr("=",       typeTemp,  initExpr);

    defExpr ->insertAfter(typeDefn);
    typeDefn->insertAfter(initMove);
    initMove->insertAfter(assign);
    assign  ->insertAfter(new CallExpr(PRIM_MOVE, var, typeTemp));
  }
}

static bool isNewExpr(Expr* expr) {
  bool retval = false;

  if (CallExpr* callExpr = toCallExpr(expr)) {
    retval = callExpr->isPrimitive(PRIM_NEW);
  }

  return retval;
}

static AggregateType* typeForNewExpr(CallExpr* newExpr) {
  AggregateType* retval = NULL;

  if (CallExpr* constructor = toCallExpr(newExpr->get(1))) {
    if (SymExpr* baseExpr = toSymExpr(constructor->baseExpr)) {
      if (TypeSymbol* sym = toTypeSymbol(baseExpr->symbol())) {
        if (AggregateType* type = toAggregateType(sym->type)) {
          if (isClass(type) == true || isRecord(type) == true) {
            retval = type;
          }
        }
      }
    }
  }

  return retval;
}

// Internal and Standard modules always honor no-init
//
// As a minimum, the complex type appears to rely on this
static bool moduleHonorsNoinit(Symbol* var, Expr* init) {
  bool isNoinit = init->isNoInitExpr();
  bool retval   = false;

  if (isNoinit == true && fUseNoinit == false) {
    Symbol* moduleSource = var;

    while (isModuleSymbol(moduleSource)  == false &&
           moduleSource                  != NULL &&
           moduleSource->defPoint        != NULL) {
      moduleSource = moduleSource->defPoint->parentSymbol;
    }

    if (ModuleSymbol* mod = toModuleSymbol(moduleSource)) {
      if (moduleSource->defPoint != NULL) {
        retval = mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD;
      }
    }
  }

  return retval;
}

static void normVarNoinit(DefExpr* defExpr) {
  Symbol* var  = defExpr->sym;
  Expr*   init = defExpr->init;

  init->remove();

  if (fUseNoinit == true || moduleHonorsNoinit(var, init) == true) {
    Expr*     type   = defExpr->exprType;
    CallExpr* noinit = new CallExpr(PRIM_NO_INIT, type->remove());

    defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, noinit));
  } else {
    // Ignore no-init expression and fall back on default init
    normVarTypeWoutInit(defExpr);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void updateVariableAutoDestroy(DefExpr* defExpr) {
  VarSymbol* var = toVarSymbol(defExpr->sym);
  FnSymbol*  fn  = toFnSymbol(defExpr->parentSymbol);

  if (var->hasFlag(FLAG_NO_AUTO_DESTROY) == false &&
      var->hasFlag(FLAG_PARAM)           == false && // Note 1.
      var->hasFlag(FLAG_REF_VAR)         == false &&

      fn->_this                          != var   && // Note 2.
      fn->hasFlag(FLAG_INIT_COPY_FN)     == false && // Note 3.
      fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == false) {

    // Variables in a module initializer need special attention
    if (defExpr->parentExpr == fn->getModule()->initFn->body) {

      // Noakes 2016/04/27
      //
      // Most variables in a module init function will become global and
      // should not be auto destroyed.  The challenging case is
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
      // In practice it appears to be sufficient to flag any TMP
      if (var->hasFlag(FLAG_TEMP)) {
        var->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }

    } else {
      var->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }
}

// Note 1: Since param variables can only be of primitive or enumerated type,
// their destructors are trivial.  Allowing this case to proceed could result
// in a regularization (reduction in # of conditionals == reduction in code
// complexity).

// Note 2: "this" should be passed by reference.  Then, no constructor call
// is made, and therefore no autodestroy call is needed.

// Note 3: If a record arg to an init copy function is passed by value,
// infinite recursion would ensue.  This is an unreachable case (assuming that
// magic conversions from R -> ref R are removed and all existing
// implementations of chpl__initCopy are rewritten using "ref" or "const ref"
// intent on the record argument).


// Note 4: These two cases should be regularized.  Either the copy constructor
// should *always* be called (and the corresponding destructor always called),
// or we should ensure that the destructor is called only if a constructor is
// called on the same variable.  The latter case is an optimization, so the
// simplest implementation calls the copy-constructor in both cases.

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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isConstructor(FnSymbol* fn) {
  bool retval = false;

  if (fn->numFormals()       >= 2 &&
      fn->getFormal(1)->type == dtMethodToken) {

    retval = strcmp(fn->name, fn->getFormal(2)->type->symbol->name) == 0;
  }

  return retval;
}

static bool isInitMethod(FnSymbol* fn) {
  bool retval = false;

  if (fn->numFormals()       >= 2 &&
      fn->getFormal(1)->type == dtMethodToken) {

    retval = strcmp(fn->name, "init") == 0;
  }

  return retval;
}

static void updateConstructor(FnSymbol* fn) {
  SymbolMap      map;
  Type*          type = fn->getFormal(2)->type;
  AggregateType* ct   = toAggregateType(type);

  if (ct == NULL) {
    if (type == dtUnknown) {
      INT_FATAL(fn, "'this' argument has unknown type");
    } else {
      INT_FATAL(fn, "initializer on non-class type");
    }
  }

  if (fn->hasFlag(FLAG_NO_PARENS)) {
    USR_FATAL(fn, "a constructor cannot be declared without parentheses");
  }

  // Call the constructor, passing in just the generic arguments.
  // This call ensures that the object is default-initialized before the
  // user's constructor body is called.
  // NOTE: This operation is not necessary for initializers, as Phase 1 of
  // the initializer body is intended to perform this operation on its own.
  CallExpr* call = new CallExpr(ct->defaultInitializer);

  for_formals(defaultTypeConstructorArg, ct->defaultTypeConstructor) {
    ArgSymbol* arg = NULL;

    for_formals(methodArg, fn) {
      if (defaultTypeConstructorArg->name == methodArg->name) {
        arg = methodArg;
      }
    }

    if (arg == NULL) {
      if (defaultTypeConstructorArg->defaultExpr == NULL) {
        USR_FATAL_CONT(fn,
                       "constructor for class '%s' requires a generic "
                       "argument called '%s'",
                       ct->symbol->name,
                       defaultTypeConstructorArg->name);
      }
    } else {
      call->insertAtTail(new NamedExpr(arg->name, new SymExpr(arg)));
    }
  }

  fn->_this = new VarSymbol("this");
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->insertAtHead(new CallExpr(PRIM_MOVE, fn->_this, call));

  fn->insertAtHead(new DefExpr(fn->_this));
  fn->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(fn->_this)));

  map.put(fn->getFormal(2), fn->_this);

  fn->formals.get(2)->remove();
  fn->formals.get(1)->remove();

  update_symbols(fn, &map);

  // The constructor's name is the name of the type.
  // Replace it with _construct_typename
  fn->name = ct->defaultInitializer->name;

  fn->addFlag(FLAG_CONSTRUCTOR);
}

static void updateInitMethod(FnSymbol* fn) {
  if (isAggregateType(fn->_this->type) == true) {
    preNormalizeInitMethod(fn);

  } else if (fn->_this->type == dtUnknown) {
    INT_FATAL(fn, "'this' argument has unknown type");

  } else {
    INT_FATAL(fn, "initializer on non-class type");
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void find_printModuleInit_stuff() {
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
