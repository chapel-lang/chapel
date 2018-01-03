/*
 * Copyright 2004-2018 Cray Inc.
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
#include "driver.h"
#include "ForallStmt.h"
#include "initializerRules.h"
#include "lowerTryExprs.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "TransformLogicalShortCircuit.h"
#include "typeSpecifier.h"
#include "wellknown.h"

#include <cctype>
#include <set>
#include <vector>

bool normalized = false;

static void        insertModuleInit();
static FnSymbol*   toModuleDeinitFn(ModuleSymbol* mod, Expr* stmt);
static void        handleModuleDeinitFn(ModuleSymbol* mod);
static void        transformLogicalShortCircuit();
static void        handleReduceAssign();

static void        fixupArrayFormals(FnSymbol* fn);

static bool        includesParameterizedPrimitive(FnSymbol* fn);
static void        replaceFunctionWithInstantiationsOfPrimitive(FnSymbol* fn);
static void        fixupQueryFormals(FnSymbol* fn);

static bool        isConstructor(FnSymbol* fn);
static bool        isInitMethod (FnSymbol* fn);

static void        updateConstructor(FnSymbol* fn);
static void        updateInitMethod (FnSymbol* fn);

static void        checkUseBeforeDefs();
static void        moveGlobalDeclarationsToModuleScope();
static void        insertUseForExplicitModuleCalls(void);

static void        hack_resolve_types(ArgSymbol* arg);

static void        find_printModuleInit_stuff();

static void        normalizeBase(BaseAST* base);
static void        processSyntacticDistributions(CallExpr* call);
static void        normalizeReturns(FnSymbol* fn);

static bool        isCallToConstructor(CallExpr* call);
static void        normalizeCallToConstructor(CallExpr* call);

static bool        isCallToTypeConstructor(CallExpr* call);
static void        normalizeCallToTypeConstructor(CallExpr* call);

static void        applyGetterTransform(CallExpr* call);
static void        insertCallTemps(CallExpr* call);
static void        insertCallTempsWithStmt(CallExpr* call, Expr* stmt);

static void        normalizeTypeAlias(DefExpr* defExpr);
static void        normalizeConfigVariableDefinition(DefExpr* defExpr);
static void        normalizeVariableDefinition(DefExpr* defExpr);

static void        normRefVar(DefExpr* defExpr);

static void        init_untyped_var(VarSymbol* var,
                                    Expr*      init,
                                    Expr*      insert,
                                    VarSymbol* constTemp);

static void        init_typed_var(VarSymbol* var,
                                  Expr*      type,
                                  Expr*      insert,
                                  VarSymbol* constTemp);

static void        init_typed_var(VarSymbol* var,
                                  Expr*      type,
                                  Expr*      init,
                                  Expr*      insert,
                                  VarSymbol* constTemp);

static void        init_noinit_var(VarSymbol* var,
                                   Expr*      type,
                                   Expr*      init,
                                   Expr*      insert,
                                   VarSymbol* constTemp);

static bool        moduleHonorsNoinit(Symbol* var, Expr* init);

static void        updateVariableAutoDestroy(DefExpr* defExpr);

static TypeSymbol* expandTypeAlias(SymExpr* se);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void normalize() {
  insertModuleInit();

  transformLogicalShortCircuit();

  handleReduceAssign();

  forv_Vec(AggregateType, at, gAggregateTypes) {
    if (isClassWithInitializers(at)  == true ||
        isRecordWithInitializers(at) == true) {
      preNormalizeFields(at);
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    SET_LINENO(fn);

    if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)    == false &&
        fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == false) {
      fixupArrayFormals(fn);
    }

    if (includesParameterizedPrimitive(fn) == true) {
      replaceFunctionWithInstantiationsOfPrimitive(fn);

    } else {
      fixupQueryFormals(fn);

      if (isConstructor(fn) == true) {
        updateConstructor(fn);

      } else if (isInitMethod(fn) == true) {
        updateInitMethod(fn);
      }
    }
  }

  normalizeBase(theProgram);

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
                             strcmp(fn->name + 1, ct->symbol->name) != 0;
        bool notDeinit    = (fn->name != astrDeinit);

        if (ct && notDeinit && notTildeName) {
          USR_FATAL(fn,
                    "destructor name must match class/record name "
                    "or deinit()");

        }

        if (!notDeinit && fn->hasFlag(FLAG_NO_PARENS)) {
          USR_FATAL_CONT(fn, "deinitializers must have parentheses");
        }

        fn->name = astrDeinit;
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
  if (fn->isNormalized() == false) {
    normalizeBase(fn);
    fn->setNormalized(true);
  }
}

void normalize(Expr* expr) {
  normalizeBase(expr);
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
      if (fn->name == astrDeinit)
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
  if (deinitFn->hasFlag(FLAG_NO_PARENS))
    USR_FATAL_CONT(deinitFn, "module deinit() functions must have parentheses");

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

//
// handleReduceAssign(): check+process the reduce= calls
//
static void handleReduceAssign() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_REDUCE_ASSIGN)) {
      SET_LINENO(call);
      INT_ASSERT(call->numActuals() == 2); // comes from the parser
      int rOpIdx;

      // l.h.s. must be a single variable
      if (SymExpr* lhsSE = toSymExpr(call->get(1))) {
        Symbol* lhsVar = lhsSE->symbol();
        // ... which is mentioned in a with clause with a reduce intent
        ForallStmt* enclosingFS = enclosingForallStmt(call);

        if (!enclosingFS)
          USR_FATAL_CONT(call, "The reduce= operator must occur within a forall statement.");
        else if ((rOpIdx = enclosingFS->reduceIntentIdx(lhsVar)) >= 0) {
          call->insertAtHead(new_IntSymbol(rOpIdx, INT_SIZE_64));
        } else
          USR_FATAL(lhsSE, "The l.h.s. of a reduce= operator, '%s', must be passed by a reduce intent into the nearest enclosing forall loop", lhsVar->name);

      } else {
        USR_FATAL(call->get(1), "The l.h.s. of a reduce= operator must be just a variable");
      }
    }
  }
}

//
// handle reduce specs of shadow vars
//
static void insertCallTempsForRiSpecs(BaseAST* base) {
  std::vector<ForallStmt*> forallStmts;

  collectForallStmts(base, forallStmts);

  for_vector(ForallStmt, fs, forallStmts) {
    for_shadow_vars(svar, temp, fs) {
      if (CallExpr* specCall = toCallExpr(svar->reduceOpExpr())) {
        insertCallTempsWithStmt(specCall, fs);
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
static void normalizeBase(BaseAST* base) {

  //
  // Phase 0
  //
  lowerTryExprs(base);


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
      if (fn->isNormalized() == false) {
        normalizeReturns(fn);
      }
    }
  }


  //
  // Phase 3
  //
  for_vector(Symbol, symbol, symbols) {
    if (VarSymbol* var = toVarSymbol(symbol)) {
      DefExpr* defExpr = var->defPoint;

      if (FnSymbol* fn = toFnSymbol(defExpr->parentSymbol)) {
        if (fn                 != stringLiteralModule->initFn &&
            fn->isNormalized() == false) {
          Expr* type = defExpr->exprType;
          Expr* init = defExpr->init;

          if (type != NULL || init != NULL) {
            if (var->isType() == true) {
              normalizeTypeAlias(defExpr);

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

  insertCallTempsForRiSpecs(base);

  // Handle calls to "type" constructor or "value" constructor
  for_vector(CallExpr, call, calls2) {
    if (isAlive(call) == true) {
      if (isCallToConstructor(call) == true) {
        normalizeCallToConstructor(call);

      } else if (isCallToTypeConstructor(call) == true) {
        normalizeCallToTypeConstructor(call);
      }
    }
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

          } else if (isShadowVarSymbol(sym)) {
            // ShadowVarSymbols are always defined.

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
  USR_STOP();
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

    // All arg symbols and loop induction variables are defined.
    if (isArgSymbol(sym) ||
        sym->hasFlag(FLAG_INDEX_VAR)
    ) {
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

        // records with initializers are defined
        } else if (AggregateType* at = toAggregateType(type)) {
          if (isRecordWithInitializers(at) == true) {
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void insertUseForExplicitModuleCalls() {
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->parentSymbol && se->symbol() == gModuleToken) {
      SET_LINENO(se);

      CallExpr*     call  = toCallExpr(se->parentExpr);
      INT_ASSERT(call);

      SymExpr*      mse   = toSymExpr(call->get(2));
      INT_ASSERT(mse);

      ModuleSymbol* mod   = toModuleSymbol(mse->symbol());
      INT_ASSERT(mod);

      Expr*         stmt  = se->getStmtExpr();
      BlockStmt*    block = new BlockStmt();

      stmt->insertBefore(block);

      block->insertAtHead(stmt->remove());
      block->useListAdd(mod);
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Two cases are handled here:                                                 *
*    1. ('new' (dmap arg)) ==> (chpl__buildDistValue arg)                     *
*    2. (chpl__distributed (Dist args)) ==>                                   *
*       (chpl__distributed (chpl__buildDistValue ('new' (Dist args)))),       *
*        where isDistClass(Dist).                                             *
*                                                                             *
*  In 1., the only type that has FLAG_SYNTACTIC_DISTRIBUTION on it is "dmap". *
*  This is a dummy record type that must be replaced.  The call to            *
*  chpl__buildDistValue() performs this task, returning _newDistribution(x),  *
*  where x is a distribution.                                                 *
*                                                                             *
************************************** | *************************************/

static void processSyntacticDistributions(CallExpr* call) {
  SET_LINENO(call);

  if (call->isPrimitive(PRIM_NEW) == true) {
    if (CallExpr* type = toCallExpr(call->get(1))) {
      if (SymExpr* base = toSymExpr(type->baseExpr)) {
        if (base->symbol()->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION) == true) {
          const char* name = "chpl__buildDistValue";

          type->baseExpr->replace(new UnresolvedSymExpr(name));

          call->replace(type->remove());
        }
      }
    }
  }

  if (call->isNamed("chpl__distributed")) {
    if (CallExpr* distCall = toCallExpr(call->get(1))) {
      if (SymExpr* distClass = toSymExpr(distCall->baseExpr)) {
        if (TypeSymbol* ts = expandTypeAlias(distClass)) {
          if (isDistClass(ts->type) == true) {
            CallExpr* newExpr = new CallExpr(PRIM_NEW, distCall->remove());

            call->insertAtHead(new CallExpr("chpl__buildDistValue", newExpr));
          }
        }
      }
    }
  }
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
* Transform   new (call C args...) args2...                                   *
*      into   new       C args...  args2...                                   *
*                                                                             *
* Transform   new (call (call (partial) C _mt this) args...)) args2...        *
*      into   new (call       (partial) C _mt this) args...   args2...        *
*                                                                             *
************************************** | *************************************/

static void fixPrimNew(CallExpr* primNewToFix);

static bool isCallToConstructor(CallExpr* call) {
  return call->isPrimitive(PRIM_NEW);
}

static void normalizeCallToConstructor(CallExpr* call) {
  if (CallExpr* arg1 = toCallExpr(call->get(1))) {
    if (isSymExpr(arg1->baseExpr) == true) {
      if (arg1->partialTag == false) {
        fixPrimNew(call);
      }

    } else if (CallExpr* subCall = toCallExpr(arg1->baseExpr)) {
      if (isSymExpr(subCall->baseExpr) == true) {
        if (subCall->partialTag == true) {
          fixPrimNew(call);
        }
      }
    }
  }
}

static void fixPrimNew(CallExpr* primNewToFix) {
  SET_LINENO(primNewToFix);

  CallExpr* callInNew    = toCallExpr(primNewToFix->get(1));
  CallExpr* newNew       = new CallExpr(PRIM_NEW);
  Expr*     exprModToken = NULL;
  Expr*     exprMod      = NULL;

  if (callInNew->numActuals() >= 2) {
    if (SymExpr* se1 = toSymExpr(callInNew->get(1))) {
      if (se1->symbol() == gModuleToken) {
        exprModToken = callInNew->get(1)->remove();
        exprMod      = callInNew->get(1)->remove();
      }
    }
  }

  callInNew->remove();

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

  if (exprModToken != NULL) {
    newNew->insertAtHead(exprMod);
    newNew->insertAtHead(exprModToken);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static SymExpr* callUsedInRiSpec(Expr* call);
static void     restoreReduceIntentSpecCall(SymExpr* riSpec, CallExpr* call);

static bool isCallToTypeConstructor(CallExpr* call) {
  bool retval = false;

  if (SymExpr* se = toSymExpr(call->baseExpr)) {
    if (TypeSymbol* ts = expandTypeAlias(se)) {
      if (isAggregateType(ts->type) == true) {
        // Ensure it is not nested within a new expr
        CallExpr* parent = toCallExpr(call->parentExpr);

        if (parent == NULL) {
          retval = true;

        } else if (parent->isPrimitive(PRIM_NEW) == true) {
          retval = false;

        } else if (CallExpr* parentParent = toCallExpr(parent->parentExpr)) {
          retval = parentParent->isPrimitive(PRIM_NEW) == false;

        } else {
          retval = true;
        }
      }
    }
  }

  return retval;
}

static void normalizeCallToTypeConstructor(CallExpr* call) {
  if (SymExpr* se = toSymExpr(call->baseExpr)) {
    if (TypeSymbol* ts = expandTypeAlias(se)) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        SET_LINENO(call);

        if (at->symbol->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION) == true) {
          // Call chpl__buildDistType for syntactic distributions.
          se->replace(new UnresolvedSymExpr("chpl__buildDistType"));

        } else if (SymExpr* riSpec = callUsedInRiSpec(call)) {
          restoreReduceIntentSpecCall(riSpec, call);

        } else {
          // Transform C ( ... ) into _type_construct_C ( ... )
          const char* name = at->defaultTypeConstructor->name;

          se->replace(new UnresolvedSymExpr(name));
        }
      }
    }
  }
}

//
// These helpers handle RiSpec (Reduce Intent Specification) i.e.:
//
//   forall ... with (<RiSpec> reduce <outer variable>) { ... }
//
// In particular, they implement RiSpecs of the form type(someArg).
// See e.g. test/parallel/forall/vass/3types-*.
// We want to keep these reduce intents in their original form
// until we process reduce intents later.
//
// We do it here to avoid transforming it into _type_construct_C ( ... ).
// That would be incorrect because this is a special syntax for reduce intent.
//
static SymExpr* callUsedInRiSpec(Expr* call) {
  SymExpr* retval = NULL;

  if (CallExpr* parent = toCallExpr(call->parentExpr)) {
    if (parent->isPrimitive(PRIM_MOVE) == true) {
      Symbol*  dest        = toSymExpr(parent->get(1))->symbol();
      SymExpr* riSpecMaybe = dest->firstSymExpr();
      Symbol*  symParent   = riSpecMaybe->parentSymbol;

      if (ShadowVarSymbol* svar = toShadowVarSymbol(symParent)) {
        if (riSpecMaybe == svar->reduceOpExpr()) {
          retval = riSpecMaybe;
        }
      }
    }
  }

  return retval;
}

//
// This function partially un-does normalization
// so that reduce intents specs (see above) don't get messed up.
//
static void restoreReduceIntentSpecCall(SymExpr* riSpec, CallExpr* call) {
  Symbol* temp = riSpec->symbol();

  // Verify the pattern that occurs if callUsedInRiSpec() returns true.
  // If any of these fail, either the pattern changed or callUsedInRiSpec()
  // returns true when it shouldn't.
  INT_ASSERT(temp->firstSymExpr()      == riSpec);
  INT_ASSERT(temp->lastSymExpr()->next == call);

  // 'temp' has only 2 SymExprs
  INT_ASSERT(riSpec->symbolSymExprsNext == temp->lastSymExpr());

  // Remove 'temp'.
  temp->defPoint->remove();

  call->parentExpr->remove();

  // Put 'call' back into riSpec.
  riSpec->replace(call->remove());
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void applyGetterTransform(CallExpr* call) {
  // Most generally:
  //   x.f(a) --> f(_mt, x)(a)
  // which is the same as
  //   call(call(. x "f") a) --> call(call(f _mt x) a)
  // Also:
  //   x.f --> f(_mt, x)
  // Note:
  //   call(call or )( indicates partial
  if (call->isNamedAstr(astrSdot)) {
    SET_LINENO(call);

    if (SymExpr* symExpr = toSymExpr(call->get(2))) {

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

    } else if (UnresolvedSymExpr* symExpr = toUnresolvedSymExpr(call->get(2))) {
      call->baseExpr->replace(symExpr->remove());
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
    insertCallTempsWithStmt(call, call->getStmtExpr());
  }
}

static void insertCallTempsWithStmt(CallExpr* call, Expr* stmt) {
  SET_LINENO(call);

  CallExpr*  parentCall = toCallExpr(call->parentExpr);
  VarSymbol* tmp        = newTemp("call_tmp");

  stmt->insertBefore(new DefExpr(tmp));

  if (call->isPrimitive(PRIM_NEW)    == true) {
    tmp->addFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);

  } else {
    // Add FLAG_EXPR_TEMP unless this tmp is being used
    // as a sub-expression for a variable initialization.
    // This flag triggers autoCopy/autoDestroy behavior.
    if (parentCall == NULL ||
        (parentCall->isNamed("chpl__initCopy")  == false &&
         parentCall->isPrimitive(PRIM_INIT_VAR) == false)) {
      tmp->addFlag(FLAG_EXPR_TEMP);
    }
  }

  if (call->isPrimitive(PRIM_TYPEOF) == true) {
    tmp->addFlag(FLAG_TYPE_VARIABLE);
  }

  evaluateAutoDestroy(call, tmp);

  tmp->addFlag(FLAG_MAYBE_PARAM);
  tmp->addFlag(FLAG_MAYBE_TYPE);

  if (call->isNamed("super")            == true &&
      parentCall                        != NULL &&
      parentCall->isNamedAstr(astrSdot) == true &&
      parentCall->get(1)                == call) {
    // We've got an access to a method or field on the super type.
    // This means we should preserve that knowledge for when we
    // attempt to access the method on the super type.
    tmp->addFlag(FLAG_SUPER_TEMP);
  }

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

      if (isRecordWithInitializers(type) == true) {
        Expr*     arg1     = initCall->get(1)->remove();
        CallExpr* argExpr  = toCallExpr(arg1);

        SymExpr*  modToken = NULL;
        SymExpr*  modValue = NULL;

        if (argExpr->numActuals() >= 2) {
          if (SymExpr* se = toSymExpr(argExpr->get(1))) {
            if (se->symbol() == gModuleToken) {
              modToken = toSymExpr(argExpr->get(1)->remove());
              modValue = toSymExpr(argExpr->get(1)->remove());
            }
          }
        }

        // Insert the arg portion of the initExpr back into tree
        defExpr->insertAfter(argExpr);

        // Convert it in to a use of the init method
        argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

        // Add _mt and _this (insert at head in reverse order)
        if (isGenericRecord(type) == true) {
          // We need the actual for the "this" argument to be named in the
          // generic record case ...
          argExpr->insertAtHead(new NamedExpr("this", new SymExpr(var)));

          var->addFlag(FLAG_DELAY_GENERIC_EXPANSION);

        } else {
          // ... but not in the non-generic record case
          argExpr->insertAtHead(var);
        }

        argExpr->insertAtHead(gMethodToken);

        if (modToken != NULL) {
          argExpr->insertAtHead(modValue);
          argExpr->insertAtHead(modToken);
        }

      } else {
        defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, initExpr));
      }

      if (type != NULL &&
          (type->isGeneric()                     == false ||
           isGenericRecordWithInitializers(type) == true)) {
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
  Type*   type     = typeForTypeSpecifier(typeExpr, false);

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
  Type*   type     = typeForTypeSpecifier(typeExpr, false);

  // Note: the above line will not obtain a type if the typeExpr is a CallExpr
  // for a generic record or class, as that is a more complicated set of AST.

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
    if        (isSymExpr(initExpr) == true) {
      defExpr->insertAfter(new CallExpr("init", gMethodToken, var, initExpr));

    } else if (isNewExpr(initExpr) == false) {
      defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, initExpr));

    } else {
      Expr*     arg     = toCallExpr(initExpr)->get(1)->remove();
      CallExpr* argExpr = toCallExpr(arg);

      // This call must be in tree before extending argExpr
      defExpr->insertAfter(argExpr);

      // Convert it to a use of the init method
      argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

      // Add _mt and _this (insert at head in reverse order)
      argExpr->insertAtHead(var);
      argExpr->insertAtHead(gMethodToken);
    }

    var->type = type;

  } else if (isNewExpr(initExpr) == true) {
    // This check is necessary because the "typeForTypeSpecifier"
    // call will not obtain a type if the typeExpr is a CallExpr,
    // as it is for generic records and classes

    CallExpr*      origCall = toCallExpr(initExpr);
    AggregateType* rhsType  = typeForNewExpr(origCall);

    if (isGenericRecordWithInitializers(rhsType)) {
      // Create a temporary with the type specified in the lhs declaration
      VarSymbol* typeTemp = newTemp("type_tmp");
      DefExpr*   typeDefn = new DefExpr(typeTemp);
      CallExpr*  initCall = new CallExpr(PRIM_INIT, typeExpr);
      CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp,  initCall);

      defExpr ->insertAfter(typeDefn);
      typeDefn->insertAfter(initMove);

      // Create a temporary to hold the result of the rhs "new" call
      VarSymbol* initExprTemp = newTemp("init_tmp", rhsType);
      DefExpr*   initExprDefn = new DefExpr(initExprTemp);
      Expr*      arg          = origCall->get(1)->remove();
      CallExpr*  argExpr      = toCallExpr(arg);

      initMove    ->insertAfter(initExprDefn);
      initExprDefn->insertAfter(argExpr);

      // Modify the "new" call so that it is in the appropriate form for
      // types with initializers
      argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));
      argExpr->insertAtHead(new NamedExpr("this", new SymExpr(initExprTemp)));
      argExpr->insertAtHead(gMethodToken);

      initExprTemp->addFlag(FLAG_DELAY_GENERIC_EXPANSION);

      // Assign the rhs into the lhs.
      CallExpr*  assign   = new CallExpr("=",       typeTemp,  initExprTemp);

      argExpr->insertAfter(assign);
      // Move the result into the original variable.
      assign ->insertAfter(new CallExpr(PRIM_MOVE, var, typeTemp));

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

/************************************* | **************************************
*                                                                             *
* The parser represents formals with an array type specifier as a formal with *
* a typeExpr that use chpl__buildArrayRuntimeType e.g.                        *
*                                                                             *
*   : []            -> buildArrayRuntimeType(symExpr(nil))                    *
*   : [D]           -> buildArrayRuntimeType(symExpr('D'))                    *
*   : [1..3]        -> buildArrayRuntimeType(buildRange(1, 3));               *
*   : [?D]          -> buildArrayRuntimeType(defExpr('D'))                    *
*                                                                             *
*   : []     string -> buildArrayRuntimeType(symExpr(nil), symExpr(string))   *
*   : [D]    int    -> buildArrayRuntimeType(symExpr('D'), symExpr(int)       *
*   : [D]    ?t     -> buildArrayRuntimeType(symExpr('D'), defExpr('t'))      *
*                                                                             *
* Replace these with uses of the generic _array type and make other changes   *
* as necessary.                                                               *
*                                                                             *
************************************** | *************************************/

static void fixupArrayFormal(FnSymbol* fn, ArgSymbol* formal);

static void fixupArrayDomainExpr(FnSymbol*                    fn,
                                 ArgSymbol*                   formal,
                                 Expr*                        domExpr,
                                 const std::vector<SymExpr*>& symExprs);

static void fixupArrayElementExpr(FnSymbol*                    fn,
                                  ArgSymbol*                   formal,
                                  Expr*                        eltExpr,
                                  const std::vector<SymExpr*>& symExprs);

static void fixupArrayFormals(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (BlockStmt* typeExpr = formal->typeExpr) {
      //
      // The body is usually a single callExpr.  However there are rare
      // cases in which normalization generates one or more call_temps
      // i.e. a sequence of defExpr/primMove pairs.
      //
      // In either case the desired callExpr is the tail of the body.
      //

      if (CallExpr* call = toCallExpr(typeExpr->body.tail)) {
        if (call->isNamed("chpl__buildArrayRuntimeType") == true) {
          fixupArrayFormal(fn, formal);
        }
      }
    }
  }
}

// Preliminary validation is performed within the caller
static void fixupArrayFormal(FnSymbol* fn, ArgSymbol* formal) {
  BlockStmt*            typeExpr = formal->typeExpr;

  CallExpr*             call     = toCallExpr(typeExpr->body.tail);
  int                   nArgs    = call->numActuals();
  Expr*                 domExpr  = call->get(1);
  Expr*                 eltExpr  = nArgs == 2 ? call->get(2) : NULL;

  std::vector<SymExpr*> symExprs;

  // Replace the type expression with "_array" to make it generic.
  typeExpr->replace(new BlockStmt(new SymExpr(dtArray->symbol), BLOCK_TYPE));

  if (isDefExpr(domExpr) == true || isDefExpr(eltExpr) == true) {
    collectSymExprs(fn, symExprs);
  }

  fixupArrayDomainExpr(fn, formal, domExpr, symExprs);

  if (eltExpr != NULL) {
    fixupArrayElementExpr(fn, formal, eltExpr, symExprs);
  }
}

static void fixupArrayDomainExpr(FnSymbol*                    fn,
                                 ArgSymbol*                   formal,
                                 Expr*                        domExpr,
                                 const std::vector<SymExpr*>& symExprs) {
  // : [?D]   -> defExpr('D')
  if (DefExpr* queryDomain = toDefExpr(domExpr)) {
    // Walk the body of 'fn' and replace uses of 'D' with 'D'._dom
    for_vector(SymExpr, se, symExprs) {
      if (se->symbol() == queryDomain->sym) {
        SET_LINENO(se);

        se->replace(new CallExpr(".", formal, new_CStringSymbol("_dom")));
      }
    }

  // : []     -> symExpr('nil')
  // : [D]    -> symExpr('D')
  // : [1..3] -> callExpr('buildRange', 1, 3)
  } else {
    bool insertCheck = true;

    if (SymExpr* dom = toSymExpr(domExpr)) {
      if (dom->symbol() == gNil) {
        insertCheck = false;
      }
    }

    if (insertCheck == true) {
      Symbol* checkDoms = new_CStringSymbol("chpl_checkArrArgDoms");

      fn->insertAtHead(new CallExpr(new CallExpr(".", formal, checkDoms),
                                    domExpr->copy(),
                                    fNoFormalDomainChecks ? gFalse : gTrue));
    }
  }
}

static void fixupArrayElementExpr(FnSymbol*                    fn,
                                  ArgSymbol*                   formal,
                                  Expr*                        eltExpr,
                                  const std::vector<SymExpr*>& symExprs) {
  // e.g. : [1..3] ?t
  if (DefExpr* queryEltType = toDefExpr(eltExpr)) {
    // Walk the body of 'fn' and replace uses of 't' with 't'.eltType
    for_vector(SymExpr, se, symExprs) {
      if (se->symbol() == queryEltType->sym) {
        SET_LINENO(se);

        se->replace(new CallExpr(".", formal, new_CStringSymbol("eltType")));
      }
    }

  } else if (eltExpr != NULL) {
    if (fn->where == NULL) {
      fn->where = new BlockStmt(new SymExpr(gTrue));

      insert_help(fn->where, NULL, fn);

      fn->addFlag(FLAG_COMPILER_ADDED_WHERE);
    }

    formal->addFlag(FLAG_NOT_FULLY_GENERIC);

    Expr*     oldWhere   = fn->where->body.tail;
    CallExpr* newWhere   = new CallExpr("&");
    Symbol*   eltType    = new_CStringSymbol("eltType");
    CallExpr* getEltType = new CallExpr(".", formal, eltType);

    oldWhere->replace(newWhere);

    newWhere->insertAtTail(oldWhere);
    newWhere->insertAtTail(new CallExpr("==", eltExpr->remove(), getEltType));
  }
}

/************************************* | **************************************
*                                                                             *
* Consider a function of the form                                             *
*                                                                             *
*   proc +(a: int(?w), b: int(w)) return __primitive("+", a, b);              *
*                                                                             *
* This function is *replaced* with 4 instantiations :-                        *
*                                                                             *
*   proc +(a: int( 8), b: int( 8)) return __primitive("+", a, b);             *
*   proc +(a: int(16), b: int(16)) return __primitive("+", a, b);             *
*   proc +(a: int(32), b: int(32)) return __primitive("+", a, b);             *
*   proc +(a: int(64), b: int(64)) return __primitive("+", a, b);             *
*                                                                             *
* If a function has multiple formals with parameterized primitives this       *
* process will be repeated iteratively with one set of expansions per formal. *
* This iteration relies on the fact that the first set of expansions are      *
* appended to gFnSymbols while it is being processed.                         *
*                                                                             *
************************************** | *************************************/

static bool isParameterizedPrimitive(CallExpr* typeSpecifier);

static void cloneParameterizedPrimitive(FnSymbol* fn, ArgSymbol* formal);

static void cloneParameterizedPrimitive(FnSymbol* fn,
                                        DefExpr*  def,
                                        int       width);

static bool includesParameterizedPrimitive(FnSymbol* fn) {
  bool retval = false;

  for_formals(formal, fn) {
    if (BlockStmt* typeExpr = formal->typeExpr) {
      if (CallExpr* typeSpecifier = toCallExpr(typeExpr->body.tail)) {
        if (isParameterizedPrimitive(typeSpecifier) == true) {
          retval = true;
          break;
        }
      }
    }
  }

  return retval;
}

static void replaceFunctionWithInstantiationsOfPrimitive(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (BlockStmt* typeExpr = formal->typeExpr) {
      if (CallExpr* typeSpecifier = toCallExpr(typeExpr->body.tail)) {
        if (isParameterizedPrimitive(typeSpecifier) == true) {
          cloneParameterizedPrimitive(fn, formal);

          break;
        }
      }
    }
  }
}

// e.g. x : int(?w)
static bool isParameterizedPrimitive(CallExpr* typeSpecifier) {
  bool retval = false;

  if (SymExpr* callFnSymExpr = toSymExpr(typeSpecifier->baseExpr)) {
    if (typeSpecifier->numActuals()      ==    1 &&
        isDefExpr(typeSpecifier->get(1)) == true) {
      Symbol* callFnSym = callFnSymExpr->symbol();

      if (callFnSym == dtBools[BOOL_SIZE_DEFAULT]->symbol ||
          callFnSym == dtInt[INT_SIZE_DEFAULT]->symbol    ||
          callFnSym == dtUInt[INT_SIZE_DEFAULT]->symbol   ||
          callFnSym == dtReal[FLOAT_SIZE_DEFAULT]->symbol ||
          callFnSym == dtImag[FLOAT_SIZE_DEFAULT]->symbol ||
          callFnSym == dtComplex[COMPLEX_SIZE_DEFAULT]->symbol) {
        retval = true;
      }
    }
  }

  return retval;
}

// 'formal' is certain to be a parameterized primitive e.g int(?w)
static void cloneParameterizedPrimitive(FnSymbol* fn, ArgSymbol* formal) {
  BlockStmt* typeExpr      = formal->typeExpr;
  CallExpr*  typeSpecifier = toCallExpr(typeExpr->body.tail);
  Symbol*    callFnSym     = toSymExpr(typeSpecifier->baseExpr)->symbol();
  DefExpr*   def           = toDefExpr(typeSpecifier->get(1));

  if (callFnSym == dtBools[BOOL_SIZE_DEFAULT]->symbol) {
    for (int i = BOOL_SIZE_8; i < BOOL_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, def, get_width(dtBools[i]));
    }

  } else if (callFnSym == dtInt [INT_SIZE_DEFAULT]->symbol ||
             callFnSym == dtUInt[INT_SIZE_DEFAULT]->symbol) {
    for (int i = INT_SIZE_8; i < INT_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, def, get_width(dtInt[i]));
    }

  } else if (callFnSym == dtReal[FLOAT_SIZE_DEFAULT]->symbol ||
             callFnSym == dtImag[FLOAT_SIZE_DEFAULT]->symbol) {
    for (int i = FLOAT_SIZE_32; i < FLOAT_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, def, get_width(dtReal[i]));
    }

  } else if (callFnSym == dtComplex[COMPLEX_SIZE_DEFAULT]->symbol) {
    for (int i = COMPLEX_SIZE_64; i < COMPLEX_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, def, get_width(dtComplex[i]));
    }
  }

  fn->defPoint->remove();
}

static void cloneParameterizedPrimitive(FnSymbol* fn,
                                        DefExpr*  def,
                                        int       width) {
  SymbolMap             map;
  FnSymbol*             newFn  = fn->copy(&map);
  Symbol*               newSym = map.get(def->sym);
  std::vector<SymExpr*> symExprs;

  newSym->defPoint->replace(new SymExpr(new_IntSymbol(width)));

  collectSymExprs(newFn, symExprs);

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == newSym) {
      se->setSymbol(new_IntSymbol(width));
    }
  }

  fn->defPoint->insertAfter(new DefExpr(newFn));
}

/************************************* | **************************************
*                                                                             *
* Query formals appear in two/four forms                                      *
*                                                                             *
*   1)  proc chpl__autoDestroy(x: ?t) ...                                     *
*       proc foo(x: ?t, y : t) ...                                            *
*                                                                             *
*       The identifier 't' may appear in the formals list, in a where clause, *
*       or in the body of the function.                                       *
*                                                                             *
*       The parser represents the definition of the query variable as a       *
*       DefExpr in the BlockStmt that is created for any formal type-expr.    *
*                                                                             *
*       This case is handled by replacing every use of 't' with x.type.       *
*                                                                             *
*                                                                             *
*                                                                             *
*                                                                             *
*   2)  proc =(ref a: _ddata(?t), b: _ddata(t)) ...                           *
*                                                                             *
*       The important difference is that 't' is a type field for a generic    *
*       type; in this example the generic type _ddata.                        *
*                                                                             *
*       The BlockStmt for the type-expr of the formal will include a          *
*       CallExpr with an actual that is a DefExpr.                            *
*                                                                             *
*                                                                             *
*   2a) proc _getView(r:range(?))                                             *
*                                                                             *
*       This generic function handles any actual that is a specialization of  *
*       the generic type range.  The compiler uses the same representation    *
*       as for (2) and supplies a unique name for the query variable.         *
*                                                                             *
*       The current implementation does not attempt to distinguish this case  *
*       from (2).                                                             *
*                                                                             *
*                                                                             *
*   2b) proc +(a: int(?w), b: int(w)) return __primitive("+", a, b);          *
*                                                                             *
*       This appears to be another example of (2) but it is handled           *
*       completely differently.  An earlier sub-phase notices these queries   *
*       for a generic primitive type, instantiates new versions for all       *
*       allowable values of 'w', and then deletes the original AST.           *
*       Hence these functions are never observed by the following code.       *
*                                                                             *
*   NB: This code does not handle the count for variadic functions e.g.       *
*                                                                             *
*       proc min(x, y, z...?k) ...                                            *
*                                                                             *
*       The identifier 'k' is not part of the type of 'z'.  The current AST   *
*       stores the DefExpr for 'k' in the variableExpr of 'z'.                *
*                                                                             *
************************************** | *************************************/

static void replaceUsesWithPrimTypeof(FnSymbol* fn, ArgSymbol* formal);

static bool isQueryForGenericTypeSpecifier(ArgSymbol* formal);

static void expandQueryForGenericTypeSpecifier(FnSymbol*  fn,
                                               ArgSymbol* formal);

static void replaceQueryUses(ArgSymbol*             formal,
                             DefExpr*               def,
                             CallExpr*              query,
                             std::vector<SymExpr*>& symExprs);

static void addToWhereClause(ArgSymbol* formal,
                             Expr*      expr,
                             CallExpr*  query);

static void fixupQueryFormals(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (BlockStmt* typeExpr = formal->typeExpr) {
      Expr* tail = typeExpr->body.tail;

      if  (isDefExpr(tail) == true) {
        replaceUsesWithPrimTypeof(fn, formal);

      } else if (isQueryForGenericTypeSpecifier(formal) == true) {
        expandQueryForGenericTypeSpecifier(fn, formal);
      }
    }
  }
}

// The type-expr is known to be a simple DefExpr
static void replaceUsesWithPrimTypeof(FnSymbol* fn, ArgSymbol* formal) {
  BlockStmt*            typeExpr = formal->typeExpr;
  DefExpr*              def      = toDefExpr(typeExpr->body.tail);
  std::vector<SymExpr*> symExprs;

  collectSymExprs(fn, symExprs);

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == def->sym) {
      se->replace(new CallExpr(PRIM_TYPEOF, formal));
    }
  }

  formal->typeExpr->remove();

  formal->type = dtAny;
}

static bool isQueryForGenericTypeSpecifier(ArgSymbol* formal) {
  bool retval = false;

  if (CallExpr* call = toCallExpr(formal->typeExpr->body.tail)) {
    for_actuals(actual, call) {
      if (isDefExpr(actual) == true) {
        retval = true;
        break;

      } else if (NamedExpr* named = toNamedExpr(actual)) {
        if (isDefExpr(named->actual) == true) {
          retval = true;
          break;
        }
      }
    }
  }

  return retval;
}

// The type-expr is known to be a CallExpr with a query definition
static void expandQueryForGenericTypeSpecifier(FnSymbol*  fn,
                                               ArgSymbol* formal) {
  BlockStmt*            typeExpr  = formal->typeExpr;
  Expr*                 tail      = typeExpr->body.tail;
  CallExpr*             call      = toCallExpr(tail);
  int                   position  = 1;
  CallExpr*             queryCall = new CallExpr(PRIM_QUERY);
  std::vector<SymExpr*> symExprs;

  collectSymExprs(fn, symExprs);

  if (call->isNamed("_build_tuple") == true) {
    Expr*     actual = new SymExpr(new_IntSymbol(call->numActuals()));
    CallExpr* query  = new CallExpr(PRIM_QUERY, new_CStringSymbol("size"));

    addToWhereClause(formal, actual, query);

    call->baseExpr->replace(new SymExpr(dtTuple->symbol));

    position = position + 1;
  }

  for_actuals(actual, call) {
    if (NamedExpr* named = toNamedExpr(actual)) {
      Symbol*   name1 = new_CStringSymbol(named->name);
      Symbol*   name2 = new_CStringSymbol(named->name);
      CallExpr* query = new CallExpr(PRIM_QUERY, name1);

      queryCall->insertAtTail(name2);

      if (DefExpr* def = toDefExpr(named->actual)) {
        replaceQueryUses(formal, def, query, symExprs);
      } else {
        addToWhereClause(formal, named->actual, query);
      }
    }
  }

  for_actuals(actual, call) {
    if (isNamedExpr(actual) == false) {
      CallExpr* query = queryCall->copy();

      query->insertAtTail(new_IntSymbol(position));

      if (DefExpr* def = toDefExpr(actual)) {
        replaceQueryUses(formal, def, query, symExprs);
      } else {
        addToWhereClause(formal, actual, query);
      }

      position = position + 1;
    }
  }

  formal->typeExpr->replace(new BlockStmt(call->baseExpr->remove()));

  formal->addFlag(FLAG_MARKED_GENERIC);
}

static void replaceQueryUses(ArgSymbol*             formal,
                             DefExpr*               def,
                             CallExpr*              query,
                             std::vector<SymExpr*>& symExprs) {
  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == def->sym) {
      if (formal->variableExpr != NULL) {
        CallExpr* parent  = toCallExpr(se->parentExpr);
        CallExpr* myQuery = query->copy();

        if (parent == NULL || parent->numActuals() != 1) {
          USR_FATAL(se, "illegal access to query type or parameter");
        }

        se->replace(new SymExpr(formal));

        parent->replace(se);

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

static void addToWhereClause(ArgSymbol* formal,
                             Expr*      expr,
                             CallExpr*  query) {
  FnSymbol* fn     = formal->defPoint->getFunction();
  Expr*     where  = NULL;
  CallExpr* clause = NULL;

  if (fn->where == NULL) {
    where = new SymExpr(gTrue);

    fn->where = new BlockStmt(where);

    insert_help(fn->where, NULL, fn);

    fn->addFlag(FLAG_COMPILER_ADDED_WHERE);

  } else {
    where = fn->where->body.tail;
  }

  formal->addFlag(FLAG_NOT_FULLY_GENERIC);

  query->insertAtHead(formal);

  if (formal->variableExpr != NULL) {
    clause = new CallExpr(PRIM_TUPLE_AND_EXPAND);

    while (query->numActuals() > 0) {
      clause->insertAtTail(query->get(1)->remove());
    }

    clause->insertAtTail(expr->copy());

  } else {
    clause = new CallExpr("==", expr->copy(), query);
  }

  where->replace(new CallExpr("&", where->copy(), clause));
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

  if (fNoUserConstructors) {
    ModuleSymbol* mod = fn->getModule();
    if (mod && mod->modTag != MOD_INTERNAL && mod->modTag != MOD_STANDARD) {
      USR_FATAL_CONT(fn, "Type '%s' defined a constructor here",
                     ct->symbol->name);
    }
  }

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
* If se is a type alias, resolves it recursively, or fails and returns NULL.  *
*                                                                             *
************************************** | *************************************/

static TypeSymbol* expandTypeAlias(SymExpr* se) {
  TypeSymbol* retval = NULL;

  while (se != NULL && retval == NULL) {
    Symbol* sym = se->symbol();

    if (TypeSymbol* ts = toTypeSymbol(sym)) {
      retval = ts;

    } else if (VarSymbol* vs = toVarSymbol(sym)) {
      if (vs->isType() == true) {
        // The definition in the init field of its declaration.
        DefExpr* def = vs->defPoint;

        se = toSymExpr(def->init);

      } else {
        se = NULL;
      }

    } else {
      se = NULL;
    }
  }

  return retval;
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

    // TODO -- move this logic to wellknown.cpp
    if (symbol->hasFlag(FLAG_PRINT_MODULE_INIT_INDENT_LEVEL)) {
      gModuleInitIndentLevel = toVarSymbol(symbol);
      INT_ASSERT(gModuleInitIndentLevel);
    }
  }
}
