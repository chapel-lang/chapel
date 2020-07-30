/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "DecoratedClassType.h"
#include "driver.h"
#include "errorHandling.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "initializerRules.h"
#include "library.h"
#include "LoopExpr.h"
#include "preNormalizeOptimizations.h"
#include "scopeResolve.h"
#include "splitInit.h"
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
static void        checkReduceAssign();

static bool        isArrayFormal(ArgSymbol* arg);

static bool        returnsArray(FnSymbol* fn);
static void        makeExportWrapper(FnSymbol* fn);

static void        fixupArrayFormals(FnSymbol* fn);

static void        fixupInoutFormals(FnSymbol* fn);

static bool        includesParameterizedPrimitive(FnSymbol* fn);
static void        replaceFunctionWithInstantiationsOfPrimitive(FnSymbol* fn);
static void        fixupQueryFormals(FnSymbol* fn);

static bool        isConstructor(FnSymbol* fn);

static void        updateInitMethod (FnSymbol* fn);

static void        checkUseBeforeDefs();
static void        moveGlobalDeclarationsToModuleScope();

static void        lowerIfExprs(BaseAST* base);

static void        hack_resolve_types(ArgSymbol* arg);

static void        find_printModuleInit_stuff();

static void        normalizeBase(BaseAST* base, bool addEndOfStatements);
static void        processSyntacticDistributions(CallExpr* call);
static void        processManagedNew(CallExpr* call);
static void        processSyntacticTupleAssignment(CallExpr* call);
static void        addEndOfStatementMarkers(BaseAST* base);
static Expr*       getCallTempInsertPoint(Expr* expr);
static void        addTypeBlocksForParentTypeOf(CallExpr* call);
static void        normalizeReturns(FnSymbol* fn);
static void        normalizeYields(FnSymbol* fn);

static bool        isCallToConstructor(CallExpr* call);
static void        normalizeCallToConstructor(CallExpr* call);
static void        fixStringLiteralInit(FnSymbol* fn);

static bool        isCallToTypeConstructor(CallExpr* call);
static void        normalizeCallToTypeConstructor(CallExpr* call);

static void        applyGetterTransform(CallExpr* call);
static void        insertCallTemps(CallExpr* call);
static void        insertCallTempsWithStmt(CallExpr* call, Expr* stmt);

static void errorIfSplitInitializationRequired(DefExpr* def, Expr* cur);

static void        normalizeTypeAlias(DefExpr* defExpr);
static void        normalizeConfigVariableDefinition(DefExpr* defExpr);
static void        normalizeVariableDefinition(DefExpr* defExpr);

static void        emitRefVarInit(Expr* after, Symbol* var, Expr* init);
static void        normRefVar(DefExpr* defExpr);

static void        updateVariableAutoDestroy(DefExpr* defExpr);

static TypeSymbol* expandTypeAlias(SymExpr* se);

static bool        firstConstructorWarning = true;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void normalize() {

  doPreNormalizeArrayOptimizations();

  insertModuleInit();

  transformLogicalShortCircuit();

  checkReduceAssign();

  forv_Vec(AggregateType, at, gAggregateTypes) {
    if (isClassWithInitializers(at)  == true ||
        isRecordOrUnionWithInitializers(at) == true) {
      preNormalizeFields(at);
    }

    preNormalizePostInit(at);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    SET_LINENO(fn);

    if (fn->hasFlag(FLAG_EXPORT) &&
        fn->hasFlag(FLAG_COMPILER_GENERATED)  == false) {
      makeExportWrapper(fn);
    }

    fixupArrayFormals(fn);

    if (includesParameterizedPrimitive(fn) == true) {
      replaceFunctionWithInstantiationsOfPrimitive(fn);

    } else {
      fixupQueryFormals(fn);

      if (isConstructor(fn) == true) {
        Type* ct = fn->_this->getValType();
        if (firstConstructorWarning) {
          USR_PRINT(fn, "Constructors have been deprecated as of Chapel 1.18. Please use initializers instead.");
          firstConstructorWarning = false;
        }
        USR_FATAL_CONT(fn, "Type '%s' defines a constructor here", ct->symbol->name);

      } else if (fn->isInitializer() || fn->isCopyInit()) {
        updateInitMethod(fn);
      }
    }

    fixupInoutFormals(fn);
  }

  normalizeBase(theProgram, true);

  normalized = true;

  checkUseBeforeDefs();

  moveGlobalDeclarationsToModuleScope();

  if (!fMinimalModules) {
    // Calls to chpl_statementLevelSymbol() are inserted here and in
    // function resolution to ensure that sync vars are in the correct
    // state (empty) if they are used but not assigned to anything.
    forv_Vec(SymExpr, se, gSymExprs) {
      if (FnSymbol* parentFn = toFnSymbol(se->parentSymbol)) {
        if (se == se->getStmtExpr()) {
          // Don't add these calls for the return type, since
          // chpl_statementLevelSymbol would do nothing in that case
          // anyway, and it contributes to order-of-resolution issues for
          // extern functions with declared return type.
          if (parentFn->retExprType != se->parentExpr) {
            SET_LINENO(se);

            CallExpr* call = new CallExpr(astr_chpl_statementLevelSymbol);

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

        if (ct == NULL)
          USR_FATAL_CONT(fn, "deinit may not be defined for types other than record, union, or class");
        else if (ct->symbol->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "deinit may not be currently defined for extern types");

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
    normalizeBase(fn, true);
    fn->setNormalized(true);
  }
}

void normalize(Expr* expr) {
  normalizeBase(expr, false);
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
        if (FnSymbol* deinitFn = toModuleDeinitFn(mod, stmt)) {
          mod->deinitFn = deinitFn; // the rest is in handleModuleDeinitFn()

        } else {
          mod->initFn->insertAtTail(stmt->remove());
        }
      }
    }

    mod->block->insertAtHead(new DefExpr(mod->initFn));
    handleModuleDeinitFn(mod);

    //
    // If the module has the EXPORT_INIT flag then
    // propagate it to the module's init function
    //
    if (mod->hasFlag(FLAG_EXPORT_INIT) == true ||
        (fLibraryCompile == true && mod->modTag == MOD_USER)) {
      mod->initFn->addFlag(FLAG_EXPORT);
      mod->initFn->addFlag(FLAG_LOCAL_ARGS);
    }
  }

  USR_STOP();
}

static FnSymbol* toModuleDeinitFn(ModuleSymbol* mod, Expr* stmt) {
  FnSymbol* retval = NULL;

  if (DefExpr* def = toDefExpr(stmt)) {
    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (fn->name == astrDeinit) {
        if (fn->numFormals() == 0) {
          if (mod->deinitFn == NULL) {
            retval = fn;

          } else {
            // Already got one deinit() before.
            // We could allow multiple deinit() fns and merge their contents.
            // If so, beware of possible 'return' stmts in each.
            USR_FATAL_CONT(def,
                           "an additional module deinit() "
                           "function is not allowed");

            USR_PRINT(mod->deinitFn,
                      "the first deinit() function is declared here");
          }
        }
      }
    }
  }

  return retval;
}

static void handleModuleDeinitFn(ModuleSymbol* mod) {
  if (FnSymbol* deinitFn = mod->deinitFn) {
    if (deinitFn->hasFlag(FLAG_NO_PARENS) == true) {
      USR_FATAL_CONT(deinitFn,
                     "module deinit() functions must have parentheses");
    }

    deinitFn->name = astr("chpl__deinit_", mod->name);
    deinitFn->removeFlag(FLAG_DESTRUCTOR);
  }
}

static bool isInLifetimeClause(CallExpr* call) {
  // Go up until we reach a BlockStmt
  Expr* parent = call->parentExpr;
  BlockStmt* parentBlock = NULL;
  while (parent != NULL) {
    if (BlockStmt* block = toBlockStmt(parent))
      parentBlock = block;

    parent = parent->parentExpr;
  }

  FnSymbol* fn = call->getFunction();
  return (fn && parentBlock && fn->lifetimeConstraints == parentBlock);
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

static void transformLogicalShortCircuit() {
  std::set<Expr*>           stmts;
  std::set<Expr*>::iterator iter;

  // Collect the distinct stmts that contain logical AND/OR expressions
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->primitive == 0) {
      if (UnresolvedSymExpr* expr = toUnresolvedSymExpr(call->baseExpr)) {
        if (strcmp(expr->unresolved, "&&") == 0 ||
            strcmp(expr->unresolved, "||") == 0) {
          // Don't normalize lifetime constraint clauses
          if (isInLifetimeClause(call) == false)
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
  for (iter = stmts.begin(); iter != stmts.end(); iter++) {
    Expr* stmt = *iter;
    TransformLogicalShortCircuit transform;

    if (isAlive(stmt)) {
      stmt->accept(&transform);
    }
  }
}

//
// checkReduceAssign(): check correctness of the reduce= calls
//
static void checkReduceAssign() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_REDUCE_ASSIGN)) {
      INT_ASSERT(call->numActuals() == 2); // comes from the parser

      SET_LINENO(call);

      // l.h.s. must be a single variable
      if (SymExpr* lhsSE = toSymExpr(call->get(1))) {
        Symbol*     lhsVar      = lhsSE->symbol();
        ForallStmt* enclosingFS = enclosingForallStmt(call);

        if (enclosingFS == NULL) {
          USR_FATAL_CONT(call,
                         "The reduce= operator must occur within "
                         "a forall statement.");

        } else if (enclosingFS->isReduceIntent(lhsVar)) {
          // Great.

        } else {
          USR_FATAL(lhsSE,
                    "The l.h.s. of a reduce= operator, '%s', "
                    "must be passed by a reduce intent into the "
                    "nearest enclosing forall loop",
                    lhsVar->name);
        }

      } else {
        USR_FATAL(call->get(1),
                  "The l.h.s. of a reduce= operator must be just a variable");
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
*                                                                             *
************************************** | *************************************/

static void normalizeBase(BaseAST* base, bool addEndOfStatements) {
  //
  // Phase 0
  //
  normalizeErrorHandling(base);
  if (addEndOfStatements)
    addEndOfStatementMarkers(base);

  //
  // Phase 1
  //
  {
    std::vector<CallExpr*> calls1;

    collectCallExprs(base, calls1);

    for_vector(CallExpr, call, calls1) {
      processSyntacticDistributions(call);
      processManagedNew(call);
      processSyntacticTupleAssignment(call);
      if (call->isPrimitive(PRIM_TYPEOF))
        addTypeBlocksForParentTypeOf(call);
    }
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

        if (fn->isIterator() == true) {
          normalizeYields(fn);
        }
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
        if (fn == stringLiteralModule->initFn) {
          fixStringLiteralInit(fn);
        } else if (fn->isNormalized() == false) {
          if (defExpr->exprType != NULL || defExpr->init != NULL) {
            // MPF 2019-11 - the above condition allows code generated
            // by the compiler to rely on type inference upon a PRIM_MOVE.

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

  lowerIfExprs(base);

  lowerLoopExprs(base);


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

void checkUseBeforeDefs(FnSymbol* fn) {
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
            isUseStmt(se->parentExpr)              == false &&
            isImportStmt(se->parentExpr)           == false) {

          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            if (call->isPrimitive(PRIM_REFERENCED_MODULES_LIST)) {
              continue;
            }
          }
          SymExpr* prev = toSymExpr(se->prev);

          if (prev == NULL || prev->symbol() != gModuleToken) {
            USR_FATAL_CONT(se, "modules (like '%s' here) cannot be called like procedures", sym->name);
          }

        } else if (isLcnSymbol(sym) == true) {
          if (sym->defPoint && sym->defPoint->parentExpr != rootModule->block) {
            Symbol* parent = sym->defPoint->parentSymbol;

            if (parent == fn || (parent == mod && mod->initFn == fn)) {
              if (defined.find(sym)           == defined.end() &&

                  sym->hasFlag(FLAG_ARG_THIS) == false         &&
                  sym->hasFlag(FLAG_EXTERN)   == false         &&
                  sym->hasFlag(FLAG_TEMP)     == false) {

                // Only complain one time
                if (undefined.find(sym) == undefined.end()) {
                  USR_FATAL_CONT(se, "'%s' used before defined", sym->name);
                  USR_PRINT(sym->defPoint, "defined here");
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

static void checkUseBeforeDefs() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkUseBeforeDefs(fn);
  }
  USR_STOP();
}

// guard against "var a:int = a;"
static void checkSelfDef(CallExpr* call, Symbol* sym) {
  if (call->numActuals() >= 2)
    if (SymExpr* se2 = toSymExpr(call->get(2)))
      if (se2->symbol() == sym)
        USR_FATAL_CONT(se2, "'%s' is used to define itself", sym->name);
}

// If the AST node defines a symbol, then return that symbol.
// Otherwise return NULL. Also check for self-defs.
static Symbol* theDefinedSymbol(BaseAST* ast) {
  Symbol* retval = NULL;

  // A symbol is "defined" if it is the LHS of a move, an assign,
  // or a variable initialization.
  //
  // The caller performs a post-order traversal and so we find the
  // symExpr before we see the callExpr.
  // In particular, given a move(symexpr1,symexpr2), if we consider
  // the move itself as defining the symbol, symexpr1 will raise
  // the "use before defined" error.

  if (SymExpr* se = toSymExpr(ast)) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE)      ||
          call->isPrimitive(PRIM_ASSIGN)    ||
          call->isPrimitive(PRIM_INIT_VAR)  ||
          call->isPrimitive(PRIM_INIT_VAR_SPLIT_INIT)  ||
          call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
          call->isPrimitive(PRIM_NOINIT_INIT_VAR) ||
          call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
        if (call->get(1) == se) {
          retval = se->symbol();
          checkSelfDef(call, se->symbol());
        }
      }
      // Allow for init() for a task-private variable, which occurs in
      // ShadowVarSymbol::initBlock(), which does not include its DefExpr.
      else if (ShadowVarSymbol* svar = toShadowVarSymbol(se->symbol())) {
        if (svar->isTaskPrivate()  &&
            call->isNamed("init")  &&
            // the first argument is gMethodToken
            call->get(2) == se)
          retval = svar;
      }
    }

  } else if (DefExpr* def = toDefExpr(ast)) {
    Symbol* sym = def->sym;

    // All arg symbols and loop induction variables are defined.
    // All shadow variables are defined.
    if (isArgSymbol(sym) ||
        isShadowVarSymbol(sym) ||
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
        if (type == dtNothing) {
          retval = var;

        // records with initializers are defined
        } else if (AggregateType* at = toAggregateType(type)) {
          if (isRecordOrUnionWithInitializers(at) == true) {
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

static std::set<VarSymbol*> globalTemps;

static void moveGlobalDeclarationsToModuleScope() {

  forv_Vec(ModuleSymbol, mod, allModules) {
    for_alist(expr, mod->initFn->body->body) {
      if (DefExpr* def = toDefExpr(expr)) {
        if (VarSymbol* vs = toVarSymbol(def->sym)) {
          // Don't move end counts to module scope
          // (just so that parallel.cpp can find them)
          if (vs->hasFlag(FLAG_END_COUNT))
            continue;

          // Move temps to module scope later
          // (after it is determined if they are last-mention or not)
          if (vs->hasFlag(FLAG_TEMP))
            continue;

          // move the DefExpr
          mod->block->insertAtTail(def->remove());
        } else if (isTypeSymbol(def->sym) || isFnSymbol(def->sym)) {
          // All type and function symbols are moved out to module scope.
          mod->block->insertAtTail(def->remove());
        }
      }
    }
  }

  // TODO: Can this transformation be done earlier without the aid of the
  // 'globalTemps' set?
  //
  // Note: The temporary variable might not be in the module init function.
  // This can happen if the temporary is in a loopexpr wrapper function that
  // was hoisted out of the module init function.
  for_set(VarSymbol, tmp, globalTemps) {
    ModuleSymbol* mod = tmp->getModule();
    if (tmp->defPoint->getFunction())
      mod->block->insertAtTail(tmp->defPoint->remove());
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


//
// Inserts a temporary for the result if the last statement is a call.
//
// Inserts PRIM_LOGICAL_FOLDER from the local branch result into the argument
// 'result' based on the argument 'cond'. PRIM_LOGICAL_FOLDER will either turn
// into an addr-of during resolution or be replaced by its second actual.
//
// BHARSH INIT TODO: Do we really need PRIM_LOGICAL_FOLDER anymore?
//
static void normalizeIfExprBranch(VarSymbol* cond, VarSymbol* result, BlockStmt* stmt) {
  Expr* last = stmt->body.tail->remove();
  Symbol* localResult = NULL;

  if (SymExpr* se = toSymExpr(last)) {
    localResult = se->symbol();
  } else {
    localResult = newTemp();
    localResult->addFlag(FLAG_MAYBE_TYPE);
    localResult->addFlag(FLAG_MAYBE_PARAM);
    localResult->addFlag(FLAG_EXPR_TEMP);
    localResult->addFlag(FLAG_NO_AUTO_DESTROY);

    stmt->body.insertAtTail(new DefExpr(localResult));
    stmt->body.insertAtTail(new CallExpr(PRIM_MOVE, localResult, last));
  }

  stmt->body.insertAtTail(new CallExpr(PRIM_MOVE, result, new CallExpr(PRIM_LOGICAL_FOLDER, cond, localResult)));
}

//
// Transforms an IfExpr into a CondStmt.
//
// The expression at the end of each branch is moved into a temporary. This
// temporary will take the place of the IfExpr, and the CondStmt will be
// inserted before the IfExpr's parent statement.
//
class LowerIfExprVisitor : public AstVisitorTraverse
{
  public:
    LowerIfExprVisitor() { }
    virtual ~LowerIfExprVisitor() { }

    virtual void exitIfExpr(IfExpr* node);
};

static bool isInsideDefExpr(Expr* expr) {
  bool ret = false;

  Expr* cur = expr->parentExpr;
  while (cur != NULL) {
    if (isDefExpr(cur)) {
      ret = true;
      break;
    } else if (isBlockStmt(cur)) {
      // OK to lower inside a BlockStmt because unlike a DefExpr we can insert
      // temporaries.
      break;
    } else {
      cur = cur->parentExpr;
    }
  }

  return ret;
}

void LowerIfExprVisitor::exitIfExpr(IfExpr* ife) {
  if (isAlive(ife) == false) return;
  if (isInsideDefExpr(ife)) return;
  if (isLoopExpr(ife->parentExpr)) return;

  SET_LINENO(ife);

  VarSymbol* result = newTemp();
  result->addFlag(FLAG_MAYBE_TYPE);
  result->addFlag(FLAG_EXPR_TEMP);
  result->addFlag(FLAG_IF_EXPR_RESULT);

  // Don't auto-destroy local result if returning from a branch of a parent
  // if-expression.
  const bool parentIsIfExpr = isBlockStmt(ife->parentExpr) &&
                              isIfExpr(ife->parentExpr->parentExpr);
  if (parentIsIfExpr == false) {
    result->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  VarSymbol* cond = newTemp();
  cond->addFlag(FLAG_MAYBE_PARAM);

  Expr* anchor = ife->getStmtExpr();
  anchor->insertBefore(new DefExpr(result));
  anchor->insertBefore(new DefExpr(cond));

  CallExpr* condTest = new CallExpr("_cond_test", ife->getCondition()->remove());
  anchor->insertBefore(new CallExpr(PRIM_MOVE, cond, condTest));

  normalizeIfExprBranch(cond, result, ife->getThenStmt());
  normalizeIfExprBranch(cond, result, ife->getElseStmt());

  CondStmt* cs = new CondStmt(new SymExpr(cond),
                              ife->getThenStmt()->remove(),
                              ife->getElseStmt()->remove(),
                              true /* isIfExpr */);


  // Remove nested BlockStmts
  toBlockStmt(cs->thenStmt->body.tail)->flattenAndRemove();
  toBlockStmt(cs->elseStmt->body.tail)->flattenAndRemove();

  anchor->insertBefore(cs);

  INT_ASSERT(cs->isIfExpr());

  ife->replace(new SymExpr(result));
}

static void lowerIfExprs(BaseAST* base) {
  LowerIfExprVisitor vis;
  base->accept(&vis);
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
*    1. supports e.g.  var x = new dmap(new Block(...));                      *
*    2. supports e.g.  var y = space dmapped Block (...);                     *
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
          if (isDistClass(canonicalClassType(ts->type)) == true) {
            CallExpr* newExpr = new CallExpr(PRIM_NEW,
                new NamedExpr(astr_chpl_manager,
                              new SymExpr(dtUnmanaged->symbol)),
                distCall->remove());

            call->insertAtHead(new CallExpr("chpl__buildDistValue", newExpr));

            processManagedNew(newExpr);
          }
        }
      }
    }
  }
}

/* Find patterns like
     (new _chpl_manager=<manager> (call ClassType <init-args>))
     ... where <manager> might be dtOwned, dtBorrowed, etc

   and replace them with
     (new (call ClassType <init-args> _chpl_manager=<manager>)))

   Also handles some patterns involving ? / PRIM_TO_NILABLE_CLASS.

   Here the "manager" indicates to function resolution whether
   the new pointer should be:
    * unmanaged
    * owned
    * shared

   This happens before call-tmps are added because they
   would obscure the situation.
 */
static void processManagedNew(CallExpr* newCall) {
  SET_LINENO(newCall);
  bool argListError = false;

  bool makeNilable = false;
  NamedExpr* managerArg = NULL;

  if (newCall->inTree() && newCall->isPrimitive(PRIM_NEW)) {
    // Transform (new _chpl_manager=_ (call _)) into
    //           (new (call _ _chpl_manager=_))
    // This also asserts that the call expr exists in this case.
    if (NamedExpr* ne = toNamedExpr(newCall->get(1))) {
      if (ne->name == astr_chpl_manager) {
        managerArg = ne;

        CallExpr* subCall = toCallExpr(newCall->get(2));
        // Find inner call in the event of a to-nilable e.g. for
        //   new owned C()?
        if (subCall != NULL) {
          CallExpr* callClass = NULL;
          if (subCall->numActuals() >= 1)
            callClass = toCallExpr(subCall->get(1));
          if (subCall->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
              subCall->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED)) {
            makeNilable = true;
            subCall->replace(callClass->remove());
            subCall = callClass;
          }
        }

        if (subCall != NULL) {
          subCall->insertAtTail(ne->remove());
        } else {
          USR_FATAL_CONT(newCall,
                         "type in 'new' expression is missing its argument list");
        }
      }
    }

    // Handle the case of e.g. new C(int)? aka (new (call to-nilable (call C)))
    // In that event, we need to indicate to resolution to create
    // a nilable class.
    if (managerArg == NULL) {
      if (CallExpr* callPrim = toCallExpr(newCall->get(1))) {
        if (callPrim->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
            callPrim->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED)) {
          if (CallExpr* sub = toCallExpr(callPrim->get(1))) {
            makeNilable = true;
            sub->remove();
            TypeSymbol* ts = dtAnyManagementNilable->symbol;
            managerArg = new NamedExpr(astr_chpl_manager, new SymExpr(ts));
            callPrim->replace(sub);
            sub->insertAtTail(managerArg);
          }
        }
      }
    }

    if (makeNilable) {
      INT_ASSERT(managerArg != NULL);
      // Adjust the manager to the nilable variant
      SymExpr* managerSe = toSymExpr(managerArg->actual);
      INT_ASSERT(managerSe);
      TypeSymbol* ts = toTypeSymbol(managerSe->symbol());
      INT_ASSERT(ts);
      Type* t = ts->type;
      if (isManagedPtrType(t))
        t = getDecoratedClass(t, CLASS_TYPE_MANAGED_NILABLE);
      else if (t == dtBorrowed)
        t = dtBorrowedNilable;
      else if (t == dtUnmanaged)
        t = dtUnmanagedNilable;
      managerSe->setSymbol(t->symbol);
    }

    if (newCall->numActuals() == 1 && !isCallExpr(newCall->get(1))) {
      argListError = true;
    }
    if (newCall->numActuals() >= 1) {
      if (CallExpr* subCall = toCallExpr(newCall->get(1))) {
        if (subCall->isNamedAstr(astrSdot)) {
          USR_FATAL_CONT(newCall,
                         "Please use parentheses to disambiguate "
                         "dot expression after new");
        } else {
          UnresolvedSymExpr* baseSe = toUnresolvedSymExpr(subCall->baseExpr);
          if (baseSe != NULL) {
            // We should have SymExprs for types by now
            // A call that is not resolved, as in new (t())(),
            //  would appear as a nested CallExpr.
            USR_FATAL_CONT(newCall,
                           "Attempt to 'new' a function or undefined symbol");
          }
        }
      }
    }
  }

  if (argListError) {
    FnSymbol* parent = newCall->getFunction();

    //
    // Later in normalization valid AST like "('new' (call R))" will turn into
    // "('new' R)" during fixPrimNew. If those expressions were run through
    // normalization again, they would encounter this error.
    //
    // This conditional avoids known cases where those expressions are
    // re-normalized.
    //
    bool resolvingField = normalized && isTypeSymbol(newCall->parentSymbol);
    if (parent->hasFlag(FLAG_NEW_WRAPPER) == false &&
        parent->hasFlag(FLAG_COMPILER_GENERATED) == false &&
        resolvingField == false) {
      USR_FATAL_CONT(newCall, "type in 'new' expression is missing its argument list");
    }
  }
}

/************************************* | **************************************
*                                                                             *
* processSyntacticTupleAssignment / destructureTupleAssignment                *
*                                                                             *
*    (i,j) = expr;    ==>    i = expr(1);                                     *
*                            j = expr(2);                                     *
*                                                                             *
* note: handles recursive tuple destructuring, (i,(j,k)) = ...                *
*                                                                             *
************************************** | *************************************/

static void      insertDestructureStatements(Expr*     S1,
                                             Expr*     S2,
                                             CallExpr* lhs,
                                             Expr*     rhs);
static CallExpr* destructureChk(CallExpr* lhs, Expr* rhs);
static CallExpr* destructureErr();
static void destructureTupleAssignment(CallExpr* assign, CallExpr* lhsCall);

static void processSyntacticTupleAssignment(CallExpr* call) {
  if (call->isNamedAstr(astrSassign)) {
    if (CallExpr* lhsCall = toCallExpr(call->get(1))) {
      if (lhsCall->isNamed("_build_tuple")) {
        destructureTupleAssignment(call, lhsCall);
      }
    }
  }
}

static void destructureTupleAssignment(CallExpr* assign, CallExpr* lhsCall) {
  SET_LINENO(assign);

  VarSymbol* rtmp = newTemp();
  Expr*      S1   = new CallExpr(PRIM_MOVE, rtmp, assign->get(2)->remove());
  Expr*      S2   = new CallExpr(PRIM_NOOP);

  rtmp->addFlag(FLAG_EXPR_TEMP);
  rtmp->addFlag(FLAG_MAYBE_TYPE);
  rtmp->addFlag(FLAG_MAYBE_PARAM);

  assign->replace(S1);

  S1->insertBefore(new DefExpr(rtmp));
  S1->insertAfter(S2);

  insertDestructureStatements(S1, S2, lhsCall, new SymExpr(rtmp));

  S2->remove();
}

static void insertDestructureStatements(Expr*     S1,
                                        Expr*     S2,
                                        CallExpr* lhs,
                                        Expr*     rhs) {
  int       index = 0;
  CallExpr* test  = destructureChk(lhs, rhs);
  CallExpr* err   = destructureErr();

  S1->getStmtExpr()->insertAfter(buildIfStmt(test, err));

  for_actuals(expr, lhs) {
    UnresolvedSymExpr* se = toUnresolvedSymExpr(expr->remove());

    if (se == NULL || strcmp(se->unresolved, "chpl__tuple_blank") != 0) {
      CallExpr* nextLHS = toCallExpr(expr);
      Expr*     nextRHS = new CallExpr(rhs->copy(), new_IntSymbol(index));

      if (nextLHS != NULL && nextLHS->isNamed("_build_tuple") == true) {
        insertDestructureStatements(S1, S2, nextLHS, nextRHS);

      } else {
        VarSymbol* lhsTmp = newTemp();
        CallExpr*  addrOf = new CallExpr(PRIM_ADDR_OF, expr);

        lhsTmp->addFlag(FLAG_MAYBE_PARAM);

        S1->insertBefore(new DefExpr(lhsTmp));
        S1->insertBefore(new CallExpr(PRIM_MOVE, lhsTmp, addrOf));

        S2->insertBefore(new CallExpr("=", lhsTmp, nextRHS));
      }
    }
    index = index + 1;
  }
}

static CallExpr* destructureChk(CallExpr* lhs, Expr* rhs) {
  CallExpr* dot  = new CallExpr(".", rhs->copy(), new_CStringSymbol("size"));

  return new CallExpr("!=", new_IntSymbol(lhs->numActuals()), dot);
}

static CallExpr* destructureErr() {
  const char* msg  = NULL;
  Symbol*     zero = new_IntSymbol(0);

  msg = "tuple size must match the number of grouped variables";

  return new CallExpr("compilerError", new_StringSymbol(msg), zero);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/



class AddEndOfStatementMarkers : public AstVisitorTraverse
{
  public:
    virtual bool enterCallExpr(CallExpr* node);
    virtual bool enterDefExpr(DefExpr* node);
    virtual bool enterIfExpr(IfExpr* node);
    virtual bool enterLoopExpr(LoopExpr* node);
};

// Note, this might be called also during resolution
// Adds a PRIM_END_OF_STATEMENT if the second argument is NULL
void addMentionToEndOfStatement(Expr* node, CallExpr* existingEndOfStatement) {

  // Rule out several cases that shouldn't get end-of-statement markers
  if (node->list == NULL)
    return;

  FnSymbol* fn = toFnSymbol(node->parentSymbol);
  if (fn == NULL)
    return;

  BlockStmt* firstBlock = NULL;
  BlockStmt* fnBlock = fn->body;
  bool foundInBody = false;

  // Check that the call is contained a FnSymbol's Block
  // While traversing, also compute the first parent Block.
  for (Expr* cur = node->parentExpr;
       cur != NULL;
       cur = cur->parentExpr) {

    if (firstBlock == NULL)
      if (BlockStmt* block = toBlockStmt(cur))
        firstBlock = block;

    if (cur == fnBlock) {
      foundInBody = true;
      break;
    }
  }

  // not in a FnSymbol's body
  //  e.g. in where clause, lifetime clause, return type expression, arg, etc
  if (foundInBody == false)
    return;

  // Otherwise, check that the call is a statement-level call, and
  // in that event, add the end-of-statement marker.
  if (firstBlock != node->parentExpr)
    return;

  // Gather symexprs used in the statement
  std::vector<SymExpr*> mentions;
  collectSymExprs(node, mentions);

  SET_LINENO(node);
  CallExpr* call = existingEndOfStatement;
  bool insertCall = false;

  if (call == NULL) {
    // Don't add duplicate PRIM_END_OF_STATEMENT calls, but do
    // add mentions to an existing one.
    if (CallExpr* next = toCallExpr(node->next))
      if (next->isPrimitive(PRIM_END_OF_STATEMENT))
        call = next;
  }

  if (call == NULL) {
    call = new CallExpr(PRIM_END_OF_STATEMENT);
    insertCall = true;
  }

  // Add SymExprs for any user variables mentioned in the statement
  // That way, if later passes remove them, e.g. for .type,
  // the variable lifetime still matches the user's view of the code.
  // A reasonable alternative would be for transformations such as
  // the removal of .type blocks to add such SymExprs.
  for_vector(SymExpr, se, mentions) {
    if (VarSymbol* var = toVarSymbol(se->symbol())) {
      if (!var->hasFlag(FLAG_TEMP) &&
          !var->isParameter() &&
          // exclude global variables, e.g. gMethodToken
          var->defPoint->getFunction() == node->getFunction()) {

        // check that the variable is defined outside of the
        // node (to avoid adding mentions of removed variables
        // in param-if folding)
        bool definedOutsideOfNode = true;
        if (DefExpr* def = var->defPoint) {
          for (Expr* cur = def; cur != NULL; cur = cur->parentExpr) {
            if (cur == node) {
              definedOutsideOfNode = false;
              break;
            }
          }
        }
        if (definedOutsideOfNode)
          call->insertAtTail(new SymExpr(se->symbol()));
      }
    }
  }

  // Don't add if already at the end of the block and no mentions are stored
  if (insertCall && (call->numActuals() > 0 || node->next != NULL))
    node->insertAfter(call);
}

bool AddEndOfStatementMarkers::enterIfExpr(IfExpr* node) {
  addMentionToEndOfStatement(node, NULL);
  return false;
}

bool AddEndOfStatementMarkers::enterLoopExpr(LoopExpr* node) {
  addMentionToEndOfStatement(node, NULL);
  return false;
}

bool AddEndOfStatementMarkers::enterCallExpr(CallExpr* node) {
  // Don't add more markers after an end of statement marker.
  if (node->isPrimitive(PRIM_END_OF_STATEMENT))
    return false;

  // Don't add markers after a move setting a temp
  if (node->isPrimitive(PRIM_MOVE) ||
      node->isPrimitive(PRIM_ASSIGN))
    if (SymExpr* lhs = toSymExpr(node->get(1)))
      if (lhs->symbol()->hasFlag(FLAG_TEMP))
        return false;

  // If the next statement is a PRIM_INIT_FIELD, it's a compound
  // thing from initializer pre-normalization, so don't add an end-of-statement
  // in-between. Instead, add an end-of-statement after the PRIM_INIT_FIELD.
  if (CallExpr* nextCall = toCallExpr(node->next)) {
    if (nextCall->isPrimitive(PRIM_INIT_FIELD) ||
        nextCall->isPrimitive(PRIM_SET_MEMBER)) {
      CallExpr* endOfStatement = NULL;
      if (CallExpr* nextNextCall = toCallExpr(nextCall->next))
        if (nextNextCall->isPrimitive(PRIM_END_OF_STATEMENT))
          endOfStatement = nextNextCall;

      if (endOfStatement == NULL) {
        SET_LINENO(nextCall);
        endOfStatement = new CallExpr(PRIM_END_OF_STATEMENT);
        nextCall->insertAfter(endOfStatement);
      }

      addMentionToEndOfStatement(node, endOfStatement);
      return false;
    }
  }

  addMentionToEndOfStatement(node, NULL);
  return false;
}

bool AddEndOfStatementMarkers::enterDefExpr(DefExpr* node) {
  VarSymbol* var = toVarSymbol(node->sym);

  if (var != NULL && !var->hasFlag(FLAG_TEMP)) {
    // Scroll forward to find a PRIM_END_OF_STATEMENT
    // (these are added in the parser along with DefExprs)
    CallExpr* endOfStatement = NULL;
    for (Expr* cur = node->next; cur != NULL; cur = cur->next)
      if (CallExpr* call = toCallExpr(cur))
        if (call->isPrimitive(PRIM_END_OF_STATEMENT))
          endOfStatement = call;
    addMentionToEndOfStatement(node, endOfStatement);
    return false;
  }

  if (isModuleSymbol(node->sym))
    return true; // look for functions in modules

  if (isFnSymbol(node->sym))
    if (!node->sym->hasFlag(FLAG_COMPILER_GENERATED))
      return true;  // visit non-compiler-generated functions

  return false; // other symbols, e.g. ArgSymbols
}


static void addEndOfStatementMarkers(BaseAST* base) {
  AddEndOfStatementMarkers visitor;
  base->accept(&visitor);
}


static void addTypeBlocksForParentTypeOf(CallExpr* call) {
  Expr* stmt = getCallTempInsertPoint(call);
  if (stmt == NULL)
    return;

  // Is the argument to PRIM_TYPEOF already just a symbol?
  // Then there's no possibility for side effects and nothing to do.
  if (isSymExpr(call->get(1)))
    return;

  // Look for a parent PRIM_TYPEOF
  CallExpr* typeOf = NULL;
  for (CallExpr* cur = call; cur != NULL; cur = toCallExpr(cur->parentExpr)) {
    if (cur->isPrimitive(PRIM_TYPEOF))
      typeOf = cur;
  }
  // No PRIM_TYPEOF in parent exprs? Then nothing to do.
  if (typeOf == NULL)
    return;

  SET_LINENO(call);

  // Look for a type block -- did we already add one?
  bool inTypeBlock = false;
  CallExpr* noop = NULL;
  IfExpr* parentIfExpr = NULL;
  //ForallStmt* parentForallStmt = NULL;
  ShadowVarSymbol* parentShadowVarSymbol = NULL;

  for (Expr* cur = typeOf->parentExpr; cur != NULL; cur = cur->parentExpr) {
    if (IfExpr* ifExpr = toIfExpr(cur))
      if (parentIfExpr == NULL)
        parentIfExpr = ifExpr;

    //if (ForallStmt* forallStmt = toForallStmt(cur))
    //  if (parentForallStmt == NULL)
    //    parentForallStmt = forallStmt;

    if (DefExpr* def = toDefExpr(cur))
      if (ShadowVarSymbol* sv = toShadowVarSymbol(def->sym))
        if (parentShadowVarSymbol == NULL)
          parentShadowVarSymbol = sv;

    if (BlockStmt* block = toBlockStmt(cur))
      if ((block->blockTag & BLOCK_TYPE_ONLY))
        inTypeBlock = true;
  }

  // Found type block? then nothing to do.
  if (inTypeBlock)
    return;

  // If it's in a shadow var symbol's def expr,
  // use the shadow var stmt init block as the stmt.
  if (parentShadowVarSymbol != NULL) {
    noop = new CallExpr(PRIM_NOOP);
    parentShadowVarSymbol->initBlock()->insertAtTail(noop);
    stmt = noop;
  }

  // Add a type block and put the contents of typeOf into it.
  INT_ASSERT(typeOf && typeOf->isPrimitive(PRIM_TYPEOF));

  VarSymbol* tmp = newTemp("call_type_tmp");
  tmp->addFlag(FLAG_TYPE_VARIABLE);
  tmp->addFlag(FLAG_MAYBE_TYPE);
  tmp->addFlag(FLAG_EXPR_TEMP);

  SymExpr* tmpSe = new SymExpr(tmp);
  typeOf->replace(tmpSe);
  if (stmt == typeOf)
    stmt = tmpSe;
  BlockStmt* typeBlock = new BlockStmt(BLOCK_TYPE);
  typeBlock->insertAtTail(new DefExpr(tmp));
  typeBlock->insertAtTail(new CallExpr(PRIM_MOVE, tmp, typeOf));
  stmt->insertBefore(typeBlock);

  if (noop)
    noop->remove();
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

static void fixupExportedArrayReturns(FnSymbol* fn);
static bool isVoidReturn(CallExpr* call);
static bool hasGenericArrayReturn(FnSymbol* fn);
static void insertRetMove(FnSymbol* fn, VarSymbol* retval, CallExpr* ret,
                          bool genericArrayRet);

static void normalizeReturns(FnSymbol* fn) {
  SET_LINENO(fn);

  fixupExportedArrayReturns(fn);

  std::vector<CallExpr*> rets;
  std::vector<CallExpr*> calls;
  size_t                 numVoidReturns = 0;
  CallExpr*              theRet         = NULL;
  bool                   isIterator     = fn->isIterator();

  collectMyCallExprs(fn, calls, fn);

  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_RETURN) && !isInLifetimeClause(call)) {
      rets.push_back(call);

      theRet = call;

      if (isVoidReturn(call)) {
        numVoidReturns++;
      }
    }
  }

  // Check if this function's returns are already normal.
  if (rets.size() == 1 && theRet == fn->body->body.last()) {
    if (SymExpr* se = toSymExpr(theRet->get(1))) {
      if (strcmp ("=",      fn->name)           ==    0 ||
          strcmp ("_init",  fn->name)           ==    0||
          strcmp ("_ret",   se->symbol()->name) ==    0) {
        return;
      }
    }
  }

  bool retExprIsVoid = false;
  if (fn->retExprType) {
    if (SymExpr* retExpr = toSymExpr(fn->retExprType->body.only())) {
      if (retExpr->symbol() == gVoid) {
        retExprIsVoid = true;
      }
    }
  }

  // Add a void return if needed.
  if (isIterator == false && rets.size() == 0 &&
      (fn->retExprType == NULL || retExprIsVoid)) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    return;
  }

  LabelSymbol* label       = new LabelSymbol(astr("_end_", fn->name));
  bool         labelIsUsed = false;
  VarSymbol*   retval      = NULL;

  label->addFlag(FLAG_EPILOGUE_LABEL);

  fn->insertAtTail(new DefExpr(label));

  // Check that iterators do not return 'void'
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

  // If a proc has a void return, do not return any values ever.
  // (Types are not resolved yet, so we judge by presence of "void returns"
  // i.e. returns with no expr. See also a related check in semanticChecks.)
  // (Note iterators always need an RVV so resolution knows to resolve the
  //  return/yield type)
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

    fn->insertAtHead(new DefExpr(retval));
    fn->insertAtTail(new CallExpr(PRIM_RETURN, retval));
  }

  bool genericArrayRet = hasGenericArrayReturn(fn);

  // Now, for each return statement appearing in the function body,
  // move the value of its body into the declared return value.
  for_vector(CallExpr, ret, rets) {
    SET_LINENO(ret);

    if (isIterator == false && retval != NULL) {
      insertRetMove(fn, retval, ret, genericArrayRet);
    }

    // replace with GOTO(label)
    if (ret->next != label->defPoint) {
      ret->replace(new GotoStmt(GOTO_RETURN, label));

      labelIsUsed = true;
    } else {
      ret->remove();
    }
  }

  if (genericArrayRet) {
    fn->retExprType->remove();
  }

  if (labelIsUsed == false) {
    label->defPoint->remove();
  }
}

// Expected to run after we make the wrapper, since the wrapper is generated
// prior to any normalization occurring on the function, and this gets called
// during normalize called on a specific AST node.
static void fixupExportedArrayReturns(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_EXPORT) &&
      fn->hasFlag(FLAG_COMPILER_GENERATED) &&
      returnsArray(fn)) {
    // Save the element type for use at code generation, specifically for Python
    // modules.  Eventually, these operations will probably want to be moved
    // after type resolution in order to handle more complicated types.
    CallExpr* call = toCallExpr(fn->retExprType->body.tail);
    int nArgs = call->numActuals();
    Expr* eltExpr = nArgs == 2 ? call->get(2) : NULL;
    if (SymExpr* eltSym = toSymExpr(eltExpr)) {
      if (TypeSymbol* eltType = toTypeSymbol(eltSym->symbol())) {
        exportedArrayElementType[fn] = eltType;
      }
    }
    if (exportedArrayElementType[fn] == NULL && fLibraryPython) {
      USR_FATAL_CONT(fn,
                     "Array return types must include an explicit element type"
                     " for Python compilation");
    }

    // Let the return type get determined by the result of the conversion call
    fn->retExprType->remove();
    CallExpr* retCall = toCallExpr(fn->body->body.tail);
    INT_ASSERT(retCall && retCall->isPrimitive(PRIM_RETURN));

    // This appears to be prior to any insertion of call temps, etc, so it seems
    // okay for now to just insert the conversion call around the function call.
    CallExpr* transformRet = new CallExpr("convertToExternalArray",
                                          retCall->get(1)->remove());
    retCall->insertAtTail(transformRet);
  }
}

static void normalizeYields(FnSymbol* fn) {
  INT_ASSERT(fn->isIterator());
  SET_LINENO(fn);

  std::vector<CallExpr*> calls;

  collectMyCallExprs(fn, calls, fn);

  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_YIELD)) {

      CallExpr* yield = call;

      // For each yield statement, adjust it similarly to a return.
      SET_LINENO(yield);

      // Create a new YVV variable
      // MPF: I don't think YVV or RVV need to exist in the long term,
      // but using YVV enables minor adjustment in most of the rest of
      // the compiler.
      VarSymbol* retval = newTemp("yret", fn->retType);
      retval->addFlag(FLAG_YVV);

      yield->insertBefore(new DefExpr(retval));
      insertRetMove(fn, retval, yield, false);
      yield->insertBefore(new CallExpr(PRIM_YIELD, retval));
      yield->remove();
    }
  }
}

static bool isVoidReturn(CallExpr* call) {
  bool retval = false;

  if (call->isPrimitive(PRIM_RETURN)) {
    if (SymExpr* arg = toSymExpr(call->get(1))) {
      retval = arg->symbol() == gVoid;
    }
  }

  return retval;
}

static bool hasGenericArrayReturn(FnSymbol* fn) {
  if (returnsArray(fn)) {
    BlockStmt* typeExpr = fn->retExprType;

    // returnsArray ensured this was a call to "chpl__buildArrayRuntimeType"
    CallExpr* call = toCallExpr(typeExpr->body.tail);
    int nArgs = call->numActuals();
    Expr* domExpr = call->get(1);
    Expr* eltExpr = nArgs == 2 ? call->get(2) : NULL;
    bool noDom = (isSymExpr(domExpr) && toSymExpr(domExpr)->symbol() == gNil);

    if (noDom || eltExpr == NULL) {
      // Either the domain is not provided explicitly as part of the return
      // type, or the element type is not provided, or both

      return true;
    }
  }

  return false;
}

// Validates the declared domain, if it exists.
static void insertDomainCheck(Expr* actualRet, CallExpr* retVar,
                              Expr* domExpr) {
  CallExpr* checkDom = new CallExpr("chpl__checkDomainsMatch",
                                    actualRet->copy(),
                                    domExpr->copy());
  retVar->insertBefore(checkDom);
}

// Validates the declared element type, if it exists
static void insertElementTypeCheck(Expr* declaredRet, Expr* actualRet,
                                   CallExpr* retVar) {
  CallExpr* checkEltType = new CallExpr("chpl__checkEltTypeMatch",
                                        actualRet->copy(),
                                        declaredRet->copy());
  retVar->insertBefore(checkEltType);
}

static void modifyPartiallyGenericArrayReturn(FnSymbol* fn,
                                              VarSymbol* retval,
                                              CallExpr* ret,
                                              Expr* retExpr) {
  BlockStmt* typeExpr = fn->retExprType;

  CallExpr* call = toCallExpr(typeExpr->body.tail);
  int nArgs = call->numActuals();
  Expr* domExpr = call->get(1);
  Expr* retEltExpr = nArgs == 2 ? call->get(2) : NULL;
  bool noDom = (isSymExpr(domExpr) && toSymExpr(domExpr)->symbol() == gNil);

  if (!noDom) {
    // Add checks against the declared domain
    insertDomainCheck(retExpr, ret, domExpr);
  }

  if (retEltExpr != NULL) {
    insertElementTypeCheck(retEltExpr, retExpr, ret);
  }

  // TODO: Do something about coercion

  ret->insertBefore(new CallExpr(PRIM_MOVE, retval, retExpr));
}

static void insertRetMove(FnSymbol* fn, VarSymbol* retval, CallExpr* ret,
                          bool genericArrayRet) {
  Expr* retExpr = ret->get(1)->remove();

  if (fn->returnsRefOrConstRef() == true) {
    CallExpr* addrOf = new CallExpr(PRIM_ADDR_OF, retExpr);

    ret->insertBefore(new CallExpr(PRIM_MOVE, retval, addrOf));

  } else if (genericArrayRet) {
    modifyPartiallyGenericArrayReturn(fn, retval, ret, retExpr);

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
* Do not transform calls that are nested within a DefExpr (if stmt-expr is    *
* NULL). Calls within DefExprs do not have call-temps inserted, which if      *
* transformed would lead to incorrect AST like:                               *
*   (new C (call foo))                                                        *
* The expectation is that the expressions will be normalized later once the   *
* DefExpr's init/type expressions are copied into a BlockStmt.                *
*                                                                             *
************************************** | *************************************/

static void fixPrimNew(CallExpr* primNewToFix);

static bool isCallToConstructor(CallExpr* call) {
  return call->isPrimitive(PRIM_NEW);
}

static void normalizeCallToConstructor(CallExpr* call) {
  if (call->getStmtExpr() != NULL) {
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
        } else if (subCall->partialTag == true && subCall->methodTag == true) {
          // This pattern can happen for an aggregate like:
          //   class C {
          //     type t = A;
          //     var x : t;
          //     proc init(i:int) {
          //       x = new t(i); // relevant 'new'
          //     }
          //   }
          //
          // Transforms "new (call ( call ('t' _mt this) args ) )" into:
          //   new ( call ('t' _mt this) args )
          //
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

  Expr* baseExpr = callInNew->baseExpr->remove();
  callInNew->remove();

  primNewToFix->replace(newNew);

  newNew->insertAtHead(baseExpr);

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

static void fixStringLiteralInit(FnSymbol* fn) {
  // BHARSH 2018-05-10: Using something like 'collectCallExprs' here resulted
  // in nontrivial compilation slowdown. We know we're looking for MOVEs from
  // a NEW, so we can just walk the statements.
  Expr* first = fn->body->body.head;
  while (first != NULL) {
    CallExpr* call = toCallExpr(first);
    if (call != NULL && call->isPrimitive(PRIM_MOVE)) {
      CallExpr* rhs = toCallExpr(call->get(2));
      if (rhs != NULL && rhs->isPrimitive(PRIM_NEW)) {
        if (UnresolvedSymExpr* use = toUnresolvedSymExpr(rhs->get(1))) {
          use->replace(new SymExpr(dtString->symbol));
        }
      }
    }
    first = first->next;
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
      if (isAggregateType(ts->type) || isDecoratedClassType(ts->type)) {
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
  if (call->getStmtExpr() != NULL) {
    if (SymExpr* se = toSymExpr(call->baseExpr)) {
      if (TypeSymbol* ts = expandTypeAlias(se)) {
        AggregateType* at = toAggregateType(ts->type);
        if (isDecoratedClassType(ts->type)) {
          at = toAggregateType(canonicalDecoratedClassType(ts->type));
        }

        if (at) {
          SET_LINENO(call);

          if (at->symbol->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION) == true) {
            // Call chpl__buildDistType for syntactic distributions.
            se->replace(new UnresolvedSymExpr("chpl__buildDistType"));

          } else if (SymExpr* riSpec = callUsedInRiSpec(call)) {
            restoreReduceIntentSpecCall(riSpec, call);

          }
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
// We do it here to avoid transforming it into a type constructor call.
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

static bool shouldInsertCallTemps(CallExpr* call);
static void evaluateAutoDestroy(CallExpr* call, VarSymbol* tmp);
static bool moveMakesTypeAlias(CallExpr* call);
static Expr* getCallTempInsertPoint(Expr* expr);

static void insertCallTemps(CallExpr* call) {
  if (shouldInsertCallTemps(call)) {
    insertCallTempsWithStmt(call, getCallTempInsertPoint(call));
  }
}

static void insertCallTempsWithStmt(CallExpr* call, Expr* stmt) {
  SET_LINENO(call);

  CallExpr*  parentCall = toCallExpr(call->parentExpr);
  VarSymbol* tmp        = newTemp("call_tmp");

  stmt->insertBefore(new DefExpr(tmp));

  if (call->isPrimitive(PRIM_NEW) == true) {
    tmp->addFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);

  } else {
    // Add FLAG_EXPR_TEMP unless this tmp is being used
    // as a sub-expression for a variable initialization.
    // This flag triggers autoCopy/autoDestroy behavior.
    if (parentCall == NULL ||
        (parentCall->isNamedAstr(astr_initCopy)  == false &&
         parentCall->isPrimitive(PRIM_INIT_VAR) == false &&
         parentCall->isPrimitive(PRIM_INIT_VAR_SPLIT_INIT) == false)) {
      tmp->addFlag(FLAG_EXPR_TEMP);
    }
  }

  if (call->isPrimitive(PRIM_TYPEOF)) {
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

  call->replace(new SymExpr(tmp));

  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, call));
}

static bool shouldInsertCallTemps(CallExpr* call) {
  Expr*     parentExpr = call->parentExpr;
  CallExpr* parentCall = toCallExpr(parentExpr);
  Expr*     stmt       = call->getStmtExpr();

  if (parentExpr == NULL                                 ||
      isDefExpr(parentExpr)                              ||
      isContextCallExpr(parentExpr)                      ||
      stmt == NULL                                       ||
      call == stmt                                       ||
      call->partialTag                                   ||
      call->isPrimitive(PRIM_TUPLE_EXPAND)               ||
      (parentCall && parentCall->isPrimitive(PRIM_MOVE)) ||
      (parentCall && parentCall->isPrimitive(PRIM_NEW)) )
    return false;

  // Don't normalize lifetime constraint clauses
  if (isInLifetimeClause(call))
    return false;

  return true;
}

// If we're inserting a call-temp for expr, where should we put it?
static Expr* getCallTempInsertPoint(Expr* expr) {
  Expr* stmt = expr->getStmtExpr();
  if (CallExpr* call = toCallExpr(stmt)) {
    if (call->isPrimitive(PRIM_INIT_VAR)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      Symbol* sym = se->symbol();
      // Put the call temps before the DefExpr for se->symbol()
      // This allows callDestructors to use a simple strategy
      // of destroying anything if the DefExpr for it has been
      // reached. The case to think about is a function returning
      // a record that throws (or a throwing initializer).
      if (DefExpr* def = toDefExpr(stmt->prev))
        if (def->sym == sym)
          return def;
    }
  }
  return stmt;
}

static void evaluateAutoDestroy(CallExpr* call, VarSymbol* tmp) {
  Expr*     parentExpr = call->parentExpr;
  CallExpr* parentCall = toCallExpr(parentExpr);
  FnSymbol* fn         = call->getFunction();

  // Noakes 2015/11/02
  //   The expansion of _build_tuple() creates temps that need to be
  //   autoDestroyed.  This is a short-cut to arrange for that to occur.
  //   A better long term solution would be preferred
  if (call->isNamedAstr(astr_initCopy)     == true &&
      parentCall                          != NULL &&
      parentCall->isNamed("_build_tuple") == true) {
    tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  FnSymbol* initFn = fn->getModule()->initFn;

  // Sometimes an array type is represented with a loop-expression, so we
  // need to hoist the resulting runtime type into global scope so that it
  // isn't destroyed at the end of the loop-expr wrapper function.
  bool isGlobalLoopExpr = fn->hasFlag(FLAG_MAYBE_ARRAY_TYPE) &&
                          fn->defPoint->getFunction() == initFn;

  // This code used to look at any call in the initFn, and could trigger for
  // temporaries in nested scopes (e.g. a param for-loop). We should only look
  // for top-level calls/temps.
  bool isCandidateGlobal = fn == initFn &&
                           fn->body == call->getStmtExpr()->parentExpr;

  // TODO: globalTemps needs updating only for isGlobalLoopExpr
  // once all temps are hoisted in this way.
  if (isGlobalLoopExpr || isCandidateGlobal) {
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
      // Add to a set of temps to be hoisted into module scope, and later
      // auto-destroyed in the module deinit.
      globalTemps.insert(tmp);
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void emitTypeAliasInit(Expr* after, Symbol* var, Expr* init) {

  // Generate a type constructor call
  if (SymExpr* se = toSymExpr(init)) {
    if (isTypeSymbol(se->symbol()) &&
        (isAggregateType(se->typeInfo()) || isDecoratedClassType(se->typeInfo()))) {
      init = new CallExpr(se->symbol());
    }
  }

  CallExpr* move = new CallExpr(PRIM_MOVE, var, init->copy());

  if (var->hasFlag(FLAG_EXTERN)) {
    // Put initialization for extern type aliases in a type block
    // to enable early resolution of these types.
    BlockStmt* block = new BlockStmt(BLOCK_EXTERN_TYPE);
    block->insertAtTail(move);
    after->insertAfter(block);
  } else {
    after->insertAfter(move);
  }
}

static void normalizeTypeAlias(DefExpr* defExpr) {
  SET_LINENO(defExpr);

  Symbol* var  = defExpr->sym;
  Expr*   type = defExpr->exprType;
  Expr*   init = defExpr->init;

  if (type == NULL && init == NULL) {
    // Don't worry about compiler temporaries
    INT_ASSERT(var->hasFlag(FLAG_TEMP));
    return;
  }

  // all user variables are dead at end of block
  var->addFlag(FLAG_DEAD_END_OF_BLOCK);

  std::vector<CallExpr*> initAssigns;
  // if there is no init expression, search for initialization
  // points written using '='
  bool foundSplitInit = false;
  bool requestedSplitInit = isSplitInitExpr(init);

  // For now, disable automatic split init on non-user code
  Expr* prevent = NULL;
  foundSplitInit = findInitPoints(defExpr, initAssigns, prevent, true);
  if (foundSplitInit == false)
    errorIfSplitInitializationRequired(defExpr, prevent);

  INT_ASSERT(type == NULL);
  INT_ASSERT(init != NULL);

  if ((init != NULL && !requestedSplitInit) || foundSplitInit == false) {
    // handle non-split initialization
    emitTypeAliasInit(defExpr, var, init);
  } else {
    // handle split initialization for type aliases
    var->addFlag(FLAG_SPLIT_INITED);

    for_vector(CallExpr, call, initAssigns) {
      SET_LINENO(call);
      // Consider the RHS of the '=' call to be the init expr.
      Expr* rhs = call->get(2)->remove();
      emitTypeAliasInit(call, var, rhs);
      call->remove();
    }
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

  INT_ASSERT(type != NULL || init != NULL);

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
      insert->insertAfter(new CallExpr(PRIM_INIT_VAR, varTmp, init->remove()));

    } else if (init == NULL) {
      insert->insertAfter(new CallExpr(PRIM_DEFAULT_INIT_VAR,
                                       varTmp, type->remove()));

    } else if (var->hasFlag(FLAG_PARAM) == true) {
      CallExpr* cast = createCast(init->remove(), type->remove());

      insert->insertAfter(new CallExpr(PRIM_MOVE, var, cast));

    } else if (init->isNoInitExpr() == true) {
      USR_FATAL("noinit is not currently supported for config vars");

    } else {
      insert->insertAfter(new CallExpr(PRIM_INIT_VAR,
                                       varTmp, init->remove(), type->remove()));


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

/************************************* | **************************************
*                                                                             *
* normalizeVariableDefinition removes DefExpr::exprType and DefExpr::init     *
* from a variable's def expression, normalizing the AST with primitive        *
* moves, calls to chpl__initCopy, _init, and _cast, and assignments.          *
*                                                                             *
************************************** | *************************************/

static void           normVarTypeInference(DefExpr* expr);
static void           normVarTypeWoutInit(DefExpr* expr, bool noinit);
static void           normVarTypeWithInit(DefExpr* expr);
static void           normVarNoinit(DefExpr* defExpr);

static Expr* prepareShadowVarForNormalize(DefExpr* def, VarSymbol* var);
static void  restoreShadowVarForNormalize(DefExpr* def, Expr* svarMark);

static void normalizeVariableDefinition(DefExpr* defExpr) {
  SET_LINENO(defExpr);

  VarSymbol* var  = toVarSymbol(defExpr->sym);
  Expr*      type = defExpr->exprType;
  Expr*      init = defExpr->init;
  Expr*  svarMark = prepareShadowVarForNormalize(defExpr, var);
  std::vector<CallExpr*> initAssigns;

  // if there is no init expression, search for initialization
  // points written using '='
  bool foundSplitInit = false;
  bool requestedSplitInit = isSplitInitExpr(init);
  bool refVar = var->hasFlag(FLAG_REF_VAR);

  // all user variables are dead at end of block
  var->addFlag(FLAG_DEAD_END_OF_BLOCK);

  // For now, disable automatic split init on non-user code
  Expr* prevent = NULL;
  foundSplitInit = findInitPoints(defExpr, initAssigns, prevent, true);
  if (foundSplitInit == false)
    errorIfSplitInitializationRequired(defExpr, prevent);

  if ((init != NULL && !requestedSplitInit) ||
      foundSplitInit == false) {
    // handle non-split initialization

    // handle ref variables
    if (refVar) {
      normRefVar(defExpr);

    } else if (type == NULL && init != NULL) {
      normVarTypeInference(defExpr);

    } else if (type != NULL && init == NULL) {
      normVarTypeWoutInit(defExpr, false);

    } else if (type != NULL && init != NULL) {
      if (init->isNoInitExpr() == true) {
        normVarNoinit(defExpr);

      } else {
        normVarTypeWithInit(defExpr);
      }
    }
  } else {
    // handle split initialization
    var->addFlag(FLAG_SPLIT_INITED);

    // remove the init expression if present
    if (init != NULL)
      init->remove();

    VarSymbol* typeTemp = NULL;

    if (refVar == false) {
      // Emit PRIM_INIT_VAR_SPLIT for non-ref variables
      // It sets the type, if we have a type expression.
      if (type == NULL) {
        defExpr->insertAfter( new CallExpr(PRIM_INIT_VAR_SPLIT_DECL, var));
      } else {
        VarSymbol* tt = newTemp("type_tmp");
        tt->addFlag(FLAG_MAYBE_PARAM);
        tt->addFlag(FLAG_MAYBE_TYPE);

        DefExpr* def = new DefExpr(tt);

        // after the def, put
        //   declare type_tmp
        //   move type_tmp, type-expr
        //   PRIM_INIT_VAR_SPLIT_DECL var type_tmp
        defExpr->insertAfter(new CallExpr(PRIM_INIT_VAR_SPLIT_DECL, var, tt));
        emitTypeAliasInit(defExpr, tt, defExpr->exprType->remove());
        defExpr->insertAfter(def);

        typeTemp = tt;
      }
    }

    for_vector(CallExpr, call, initAssigns) {
      SET_LINENO(call);

      // Consider the RHS of the '=' call to be the init expr.
      Expr* rhs = call->get(2)->remove();

      if (refVar) {
        //       ref x = <value>;
        // const ref y : <type> = <value>;
        emitRefVarInit(call, var, rhs);
        call->remove();

      } else {
        // var x;
        // x = <value>;
        // const y: int;
        // y = <value>

        CallExpr* init2 = NULL;
        if (type == NULL)
          init2 = new CallExpr(PRIM_INIT_VAR_SPLIT_INIT, var, rhs);
        else
          init2 = new CallExpr(PRIM_INIT_VAR_SPLIT_INIT, var, rhs, typeTemp);

        call->replace(init2);
      }
    }
  }

  restoreShadowVarForNormalize(defExpr, svarMark);
}

static void errorIfSplitInitializationRequired(DefExpr* def, Expr* cur) {
  // Don't worry about compiler temporaries or index variables
  // or reduction variables
  if (def->sym->hasFlag(FLAG_TEMP) ||
      def->sym->hasFlag(FLAG_INDEX_VAR) ||
      def->sym->hasFlag(FLAG_UNSAFE)    ||
      isShadowVarSymbol(def->sym))
    return;

  // Don't worry about declarations in unsafe functions
  if (def->getFunction()->hasFlag(FLAG_UNSAFE))
    return;

  bool requestedSplitInit = isSplitInitExpr(def->init);
  // Don't worry about non-split init
  if (def->init != NULL && !requestedSplitInit)
    return;

  bool refVar = def->sym->hasFlag(FLAG_REF_VAR);

  bool canDefaultInit = true;
  Type* nonNilableType = NULL;
  Type* genericDecoratorClass = NULL;
  Type* genericType = NULL;

  // No type or init expr, so can't default init
  if (def->exprType == NULL &&
      (def->init == NULL || isSplitInitExpr(def->init)))
    canDefaultInit = false;
  // Can't default init a ref ever
  if (refVar)
    canDefaultInit = false;
  // Check for a few other cases to give better errors now
  // (this will not catch all patterns)
  if (SymExpr* typeSe = toSymExpr(def->exprType)) {
    if (TypeSymbol* ts = toTypeSymbol(typeSe->symbol())) {

      AggregateType* at = toAggregateType(canonicalClassType(ts->type));
      if (at && at->hasUserDefinedInit) {
        // OK, in the event user supplied an init() function
        // (if not, expect an error later)
      } else if (at && at->isGenericWithDefaults()) {
        // OK, this can still be default-inited
      } else if (ts->hasFlag(FLAG_GENERIC) || (at && at->isGeneric())) {
        // can't default init a generic type
        canDefaultInit = false;
        genericType = ts->type;
      }

      // can't default init a generic management/nilability
      if (isClassLikeOrManaged(ts->type)) {
        ClassTypeDecorator d = classTypeDecorator(ts->type);
        if (isDecoratorUnknownManagement(d) ||
            isDecoratorUnknownNilability(d)) {
          canDefaultInit = false;
          genericDecoratorClass = ts->type;
          genericType = ts->type;
        }
      }

      // can't default init a non-nilable class type
      if (isNonNilableClassType(ts->type)) {
        canDefaultInit = false;
        nonNilableType = ts->type;
      }
    }
  }

  if (canDefaultInit == false) {
    const char* name = def->sym->name;

    // Don't give errors for compiler generated functions at this time.
    FnSymbol* inFn = def->getFunction();
    if (inFn->hasFlag(FLAG_COMPILER_GENERATED))
      return;

    if (def->sym->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL_CONT(def, "type alias '%s' is not initialized", name);
    } else if (refVar) {
      USR_FATAL_CONT(def, "reference '%s' is not initialized", name);
    } else {
      if (def->exprType == NULL)
        USR_FATAL_CONT(def, "variable '%s' is not initialized and has no type",
                       name);
      else
        USR_FATAL_CONT(def, "variable '%s' is not initialized", name);

      if (genericDecoratorClass != NULL) {
        USR_PRINT("undecorated class type %s does not "
                  "support default initialization",
                  toString(genericDecoratorClass));
        USR_PRINT("consider adding a management decorator "
                  "such as 'owned', 'shared', 'borrowed', or 'unmanaged'");
      } else if (nonNilableType != NULL) {
        USR_PRINT("non-nilable class type %s does not "
                  "support default initialization",
                  toString(nonNilableType));

        Type* type = nonNilableType;
        AggregateType* at = toAggregateType(canonicalDecoratedClassType(type));
        ClassTypeDecorator d = classTypeDecorator(type);
        Type* suggestedType = at->getDecoratedClass(addNilableToDecorator(d));
        USR_PRINT("Consider using the type %s instead", toString(suggestedType));
      } else if (genericType != NULL) {
        USR_PRINT("generic type %s does not support default initialization",
                  toString(genericType));
      }
    }

    if (cur) {
      if (def->exprType == NULL || genericType != NULL) {
        USR_PRINT(cur,
                  "'%s' use here prevents split-init from establishing the type",
                  name);
      } else {
        USR_PRINT(cur,
                  "'%s' use here prevents split-init",
                  name);
      }
    }
  }
}



static void emitRefVarInit(Expr* after, Symbol* var, Expr* init) {
  Expr* varLocation = NULL;

  // If this is a const reference to an immediate, we need to insert a temp
  // variable so we can take the address of it, non-const references to an
  // immediate are not allowed.
  if (var->hasFlag(FLAG_CONST)) {
    if (SymExpr* initSym = toSymExpr(init)) {
      if (initSym->symbol()->isImmediate()) {
        VarSymbol* constRefTemp  = newTemp("const_ref_immediate_tmp");

        // Store the temp variable to the immediate next to the
        // variable definition because `after` might be at an inner scope
        Expr* before = var->defPoint;
        before->insertBefore(new DefExpr(constRefTemp));
        before->insertBefore(new CallExpr(PRIM_MOVE, constRefTemp, init));

        varLocation = new SymExpr(constRefTemp);
      }
    }
  }

  if (varLocation == NULL && init != NULL) {
    varLocation = init;
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
      } else if (arg->hasFlag(FLAG_ARG_THIS) &&
                 arg->intent == INTENT_BLANK &&
                 (arg->getFunction()->isInitializer() || arg->getFunction()->isCopyInit())) {
        // InitNormalize.cpp will handle the case of initializing a ref-var
        // from 'this' inside an initializer.
        error = false;
      }
    }

    if (error == true) {
      SymExpr* initSym = toSymExpr(init);
      if (initSym && initSym->symbol() == gNil)
        USR_FATAL_CONT(sym, "Cannot create a non-const reference to nil");
      else if (!isSplitInitExpr(init))
        USR_FATAL_CONT(sym,
                       "Cannot set a non-const reference to a const variable.");
    }
  }

  after->insertAfter(new CallExpr(PRIM_MOVE,
                                  var,
                                  new CallExpr(PRIM_ADDR_OF, varLocation)));
}

static void normRefVar(DefExpr* defExpr) {
  VarSymbol* var         = toVarSymbol(defExpr->sym);
  Expr*      init        = defExpr->init;

  if (init != NULL)
    init->remove();

  emitRefVarInit(defExpr, var, init);
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

  defExpr->insertAfter(new CallExpr(PRIM_INIT_VAR, var, initExpr));
  return;
}

//
// const <name> : <type>;
// param <name> : <type>;
// var   <name> : <type>;
//
// The type is explicit and the initial value is implied by the type
//

static void normVarTypeWoutInit(DefExpr* defExpr, bool noinit) {
  Symbol* var      = defExpr->sym;
  Expr*   typeExpr = defExpr->exprType->remove();

  PrimitiveTag prim = noinit?PRIM_NOINIT_INIT_VAR:PRIM_DEFAULT_INIT_VAR;
  CallExpr* init = new CallExpr(prim, var, typeExpr);

  if (var->hasFlag(FLAG_EXTERN)) {
    // Put initialization for extern vars in a type block since
    // Chapel should take no action to actually initialize
    // (extern code does that) and since
    // resolution needs to resolve extern vars early.

    BlockStmt* block = new BlockStmt(BLOCK_EXTERN_TYPE);
    block->insertAtTail(init);
    defExpr->insertAfter(block);
  } else {
    defExpr->insertAfter(init);
  }
}

static void normVarTypeWithInit(DefExpr* defExpr) {
  Symbol* var      = defExpr->sym;
  Expr*   typeExpr = defExpr->exprType->remove();
  Expr*   initExpr = defExpr->init->remove();

  //
  // e.g. const x : int     = 10;
  //      var   y : int(32) = 20;
  //
  //      var   x : MyCls   = new MyCls(1, 2);
  //
  defExpr->insertAfter(new CallExpr(PRIM_INIT_VAR, var, initExpr, typeExpr));
}

static void normVarNoinit(DefExpr* defExpr) {
  normVarTypeWoutInit(defExpr, true);
}

//
// We want the initialization code for a task-private variable to be
// in that variable's initBlock(). This is not where its DefExpr is,
// however. So, in order to use existing normalization code as-is, we
// move the DefExpr into a temporary BlockStmt. This BlockStmt also
// preserves the DefExpr's position in the ForallStmt::shadowVariables()
// list, which is where it lives. Once normalization completes, we
// move the resulting ASTs into the initBlock(), and the DefExpr back
// to its home list.

static Expr* prepareShadowVarForNormalize(DefExpr* def, VarSymbol* var) {
  ShadowVarSymbol* svar = toShadowVarSymbol(var);
  if (svar == NULL || !svar->isTaskPrivate())
    // Not a task-private variable, nothing to do.
    return NULL;

  BlockStmt* normBlock = new BlockStmt();
  def->replace(normBlock);
  normBlock->insertAtTail(def);

  return normBlock;
}

static void  restoreShadowVarForNormalize(DefExpr* def, Expr* svarMark) {
  if (!svarMark)
    return;

  BlockStmt* normBlock = toBlockStmt(svarMark);
  BlockStmt* initBlock = toShadowVarSymbol(def->sym)->initBlock();
  AList&     initList  = initBlock->body;

  normBlock->insertAfter(def->remove());

  for_alist(stmt, normBlock->body)
    initList.insertAtTail(stmt->remove());

  normBlock->remove();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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
      fn->hasFlag(FLAG_INIT_COPY_FN)     == false) { // Note 3.

    // Note that if the DefExpr is at module scope, the auto-destroy
    // for it will end up in the module deinit function.
    var->addFlag(FLAG_INSERT_AUTO_DESTROY);
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
          // MPF: this seems wrong since the result is a value not
          // a type.
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
        Expr* only = arg->typeExpr->body.only();
        Type* type = only->typeInfo();

        // Works around an issue with generic types:
        //
        // The function 'normalizeCallToTypeConstructor' may create a SymExpr
        // to a type constructor, in which case the return type will likely
        // not be 'dtUnknown' or 'dtAny' and may be generic. If the return
        // type is generic we do not want to remove the type constructor call
        // because resolution will not be able to handle the resulting AST.
        if (CallExpr* call = toCallExpr(only)) {
          if (SymExpr* se = toSymExpr(call->baseExpr)) {
            if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
              type = dtUnknown;
            }
          }
        }

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
* We cannot export a function with an array argument directly, due to the     *
* type being considered generic and due to other languages not understanding  *
* our array structure, as well as our normal array structure assuming it has  *
* control over the memory involved.  Instead, build a wrapper for the         *
* function.  This wrapper will be modified to take in an instance of our      *
* runtime-defined array wrapper type instead of the original array type, in   *
* addition to its other arguments.                                            *
*                                                                             *
************************************** | *************************************/
static bool hasNonVoidReturnStmt(FnSymbol* fn);

static void makeExportWrapper(FnSymbol* fn) {
  bool argsToReplace = false;
  for_formals(formal, fn) {
    if (isArrayFormal(formal)) {
      argsToReplace = true;
    }
  }

  if (argsToReplace || returnsArray(fn)) {
    // Either we have at least one array argument, or we return an array.
    // Need to make a version of this function that can be exported
    FnSymbol* newFn = fn->copy();
    newFn->addFlag(FLAG_COMPILER_GENERATED);
    // Avoid resolution conflicts when the arguments remain unchanged (but we
    // needed to make a wrapper due to the return type)
    newFn->addFlag(FLAG_LAST_RESORT);

    fn->defPoint->insertBefore(new DefExpr(newFn));

    if ((fn->retExprType != NULL &&
         fn->retExprType->body.tail->typeInfo() != dtVoid) ||
        hasNonVoidReturnStmt(fn)) {
      newFn->body->replace(new BlockStmt(new CallExpr(PRIM_RETURN,
                                                      new CallExpr(fn->name))));
    } else {
      newFn->body->replace(new BlockStmt(new CallExpr(fn->name)));
    }
    fn->removeFlag(FLAG_EXPORT);
  }
}

// If we run into issues due to this computation, just require explicitly
// declared return types for exported functions
static bool hasNonVoidReturnStmt(FnSymbol* fn) {
  std::vector<CallExpr*> calls;

  collectMyCallExprs(fn, calls, fn);
  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_RETURN) == true) {
      if (isVoidReturn(call) == false) {
        return true;
      }
    }
  }
  return false;
}

// Determines if we explicitly declare an exported function as returning an
// array.  In that case, we should make a wrapper (if we aren't already), and
// transform that array into a form that a caller from another programming
// language would understand.
static bool returnsArray(FnSymbol* fn) {
  if (fn->retExprType != NULL) {
    if (CallExpr* call = toCallExpr(fn->retExprType->body.tail)) {
      if (call->isNamed("chpl__buildArrayRuntimeType")) {
        return true;
      }
    }
  }
  // SIMPLIFYING ASSUMPTION:
  // If we don't have a declared return type, assume we don't return an array
  return false;
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

static void fixupExportedArrayFormals(FnSymbol* fn);

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
  if (fn->hasFlag(FLAG_EXPORT) && fn->hasFlag(FLAG_COMPILER_GENERATED) &&
      fn->hasFlag(FLAG_MODULE_INIT) == false) {
    fixupExportedArrayFormals(fn);

  } else {
    for_formals(formal, fn) {
      if (isArrayFormal(formal)) {
        fixupArrayFormal(fn, formal);
      }
    }
  }
}

static bool skipFixup(ArgSymbol* formal, Expr* domExpr, Expr* eltExpr) {
  if ((formal->intent & INTENT_FLAG_IN) ||
      formal->intent == INTENT_OUT) {
    if (isDefExpr(domExpr) || isDefExpr(eltExpr)) {
      return false;
    } else if (SymExpr* se = toSymExpr(domExpr)) {
      if (se->symbol() == gNil) {
        return false;
      }
    } else {
      INT_ASSERT(domExpr);
      if (eltExpr == NULL)
        USR_FATAL_CONT(formal->defPoint,
                       "%s array formals specifying a domain currently "
                       "require an array element type",
                       formal->intentDescrString());
      return true;
    }
  }

  return false;
}

static bool isArrayFormal(ArgSymbol* arg) {
  if (BlockStmt* typeExpr = arg->typeExpr) {
    //
    // The body is usually a single callExpr.  However there are rare
    // cases in which normalization generates one or more call_temps
    // i.e. a sequence of defExpr/primMove pairs.
    //
    // In either case the desired callExpr is the tail of the body.
    //

    if (CallExpr* call = toCallExpr(typeExpr->body.tail)) {
      if (call->isNamed("chpl__buildArrayRuntimeType")) {
        return true;
      }
    }
  }
  return false;
}

static CondStmt* makeCondToTransformArr(ArgSymbol* formal, VarSymbol* newArr,
                                        Expr* eltExpr, Expr* oldCall);

static void fixupExportedArrayFormals(FnSymbol* fn) {
  CallExpr* retCall = toCallExpr(fn->body->body.tail);
  INT_ASSERT(retCall);
  CallExpr* callOrigFn = retCall;
  if (retCall->isPrimitive(PRIM_RETURN)) {
    INT_ASSERT(retCall->numActuals() == 1);
    callOrigFn = toCallExpr(retCall->get(1));
  }

  INT_ASSERT(callOrigFn);

  for_formals(formal, fn) {
    if (isArrayFormal(formal)) {
      BlockStmt*            typeExpr = formal->typeExpr;

      // call is chpl__buildArrayRuntimeType, which takes in a domain and
      // optionally a type expression.  The domain information will get
      // validated during resolution, but we need the type expression to
      // correctly create our Chapel array wrapper.
      CallExpr*             call     = toCallExpr(typeExpr->body.tail);
      // Should be handled by the isArrayFormal call
      INT_ASSERT(call->isNamed("chpl__buildArrayRuntimeType"));
      int                   nArgs    = call->numActuals();
      Expr*                 eltExpr  = nArgs == 2 ? call->get(2) : NULL;

      if (eltExpr == NULL) {
        USR_FATAL(formal, "array argument '%s' in exported function '%s'"
                  " must specify its type", formal->name, fn->name);
        continue;
      }
      if (formal->intent == INTENT_OUT) {
        USR_FATAL(formal, "array argument '%s' in exported function '%s'"
                  " uses out intent - out intent not yet supported"
                  " for arrays in export procs",
                  formal->name, fn->name);
        continue;
      }

      // Save the element type we shuffle away, so that it can be referenced at
      // codegen.  We may want to move these operations after type resolution to
      // handle move complicated types
      if (SymExpr* eltSym = toSymExpr(eltExpr)) {
        if (TypeSymbol* eltType = toTypeSymbol(eltSym->symbol())) {
          exportedArrayElementType[formal] = eltType;
        } else {
          // TODO: handle things like type variables (not supported yet)
        }
      } else {
        // TODO: handle call expression stuff.
      }

      SymExpr* dom = toSymExpr(call->get(1));
      if ((dom != NULL && dom->symbol() == gNil) ||
          isDefExpr(call->get(1))) {
        // Either the domain is nil or the domain is a query expression, meaning
        // the argument did not specify its domain.
        // chpl_opaque_array cannot adjust for that today, but
        // chpl_external_array might be able to, so try to make a
        // chpl_external_array with it.  If that doesn't work, the user must be
        // more explicit with their domain.
        if (!fLibraryFortran) {
          formal->typeExpr->replace(
            new BlockStmt(new SymExpr(dtExternalArray->symbol)));
        } else {
          formal->typeExpr->replace(
            new BlockStmt(new SymExpr(dtCFI_cdesc_t->symbol)));
          formal->intent = INTENT_REF;
          formal->originalIntent = INTENT_REF;
        }
      } else {
        // Create a representation of the array argument that is accessible
        // outside of Chapel, depending on the type of the array.  If the array
        // is supported by our C/Python interop, it will be represented as a
        // chpl_external_array.  Otherwise, it will be a chpl_opaque_array
        VarSymbol* typeExprTmp = new VarSymbol("typeExpr");
        typeExprTmp->addFlag(FLAG_MAYBE_TYPE);

        CallExpr* newFormalType = new CallExpr("getExternalArrayType",
                                               typeExprTmp);
        formal->typeExpr->replace(new BlockStmt(new DefExpr(typeExprTmp)));
        formal->typeExpr->insertAtTail(new CallExpr(PRIM_MOVE, typeExprTmp,
                                                    call->copy()));
        formal->typeExpr->insertAtTail(newFormalType);
      }

      VarSymbol* chplArr = new VarSymbol(astr(formal->name, "_arr"));
      retCall->insertBefore(new DefExpr(chplArr));

      // Transform the outside representation into a Chapel array, and send that
      // in the call to the original function.
      CondStmt* cond = makeCondToTransformArr(formal, chplArr, eltExpr, call);

      retCall->insertBefore(cond);

      callOrigFn->insertAtTail(new SymExpr(chplArr));

    } else {
      callOrigFn->insertAtTail(new SymExpr(formal));
    }
  }
}

// Create an if statement based on the formal type to transform
// chpl_external_array, CFI_cdesc_t, or chpl_opaque_array into a normal
// Chapel array
static CondStmt* makeCondToTransformArr(ArgSymbol* formal, VarSymbol* newArr,
                                        Expr* eltExpr, Expr* oldCall) {
  // if (formal.type is dtExternalArray) then
  //   formalname_arr = makeArrayFromExternArray(formal, eltExpr)
  // else if (formal.type is dtCFI_cdesc_t) then
  //   formalname_arr = makeArrayFromFortranArray(formal, eltExpr)
  // else
  //   formalname_arr = makeArrayFromOpaque(formal, oldTypeExpr)
  CallExpr* checkFormalType = new CallExpr(PRIM_IS_SUBTYPE,
                                           dtExternalArray->symbol,
                                           new CallExpr(PRIM_TYPEOF, formal));

  // Handle chpl_external_array
  BlockStmt* ifBody = new BlockStmt();
  CallExpr* makeChplArrayFromExt = new CallExpr("makeArrayFromExternArray",
                                                new SymExpr(formal),
                                                eltExpr->copy());
  ifBody->insertAtTail(new CallExpr(PRIM_MOVE, newArr, makeChplArrayFromExt));

  BlockStmt* elseIfBody = new BlockStmt();
  CallExpr* makeChplArrayFromFort = new CallExpr("makeArrayFromFortranArray",
                                                 new SymExpr(formal),
                                                 eltExpr->copy());

  elseIfBody->insertAtTail(new CallExpr(PRIM_MOVE, newArr, makeChplArrayFromFort));

  // Handle chpl_opaque_array
  BlockStmt* elseBody = new BlockStmt();
  // Array type creator
  VarSymbol* oldTypeExpr = new VarSymbol(astr(formal->name,
                                              "_oldTypeExpr"));
  oldTypeExpr->addFlag(FLAG_MAYBE_TYPE);
  elseBody->insertAtTail(new DefExpr(oldTypeExpr));
  elseBody->insertAtTail(new CallExpr(PRIM_MOVE, oldTypeExpr, oldCall->copy()));
  // Get the _instance field's type for that array
  VarSymbol* instanceType = new VarSymbol(astr(formal->name, "_instype"));
  instanceType->addFlag(FLAG_MAYBE_TYPE);
  CallExpr* makeIT = new CallExpr(PRIM_STATIC_FIELD_TYPE, oldTypeExpr,
                                  new_StringSymbol("_instance"));
  elseBody->insertAtTail(new DefExpr(instanceType));
  elseBody->insertAtTail(new CallExpr(PRIM_MOVE, instanceType, makeIT));
  // Make a proper array using the argument and the _instance field's type
  // that we determined
  CallExpr* makeChplArray = new CallExpr("makeArrayFromOpaque",
                                         new SymExpr(formal), instanceType);
  elseBody->insertAtTail(new CallExpr(PRIM_MOVE, newArr, makeChplArray));
  Stmt* elseStmt;
  if (fLibraryFortran) {
    CallExpr* checkFormalType2 = new CallExpr(PRIM_IS_SUBTYPE,
                                              dtCFI_cdesc_t->symbol,
                                              new CallExpr(PRIM_TYPEOF, formal));
    elseStmt = new CondStmt(checkFormalType2, elseIfBody, elseBody);
  } else {
    elseStmt = elseBody;
  }
  CondStmt* cond = new CondStmt(checkFormalType, ifBody, elseStmt);
  return cond;
}

// Preliminary validation is performed within the caller
static void fixupArrayFormal(FnSymbol* fn, ArgSymbol* formal) {
  BlockStmt*            typeExpr = formal->typeExpr;

  CallExpr*             call     = toCallExpr(typeExpr->body.tail);
  int                   nArgs    = call->numActuals();
  Expr*                 domExpr  = call->get(1);
  Expr*                 eltExpr  = nArgs == 2 ? call->get(2) : NULL;

  std::vector<SymExpr*> symExprs;

  // Our AST and transformations are not set up to handle multiple query
  // expressions inside of an array's domain.  Give the user an error for now.
  if (!isDefExpr(domExpr)) {
    std::vector<DefExpr*> defExprs;
    collectDefExprs(domExpr, defExprs);
    for_vector(DefExpr, def, defExprs) {
      USR_FATAL_CONT(def, "cannot query part of a domain");
    }
  }
  //
  // Only fix array formals with 'in' intent if there was:
  // - a type query, or
  // - a domain query, or
  // - no domain expression
  //
  // This 'fixing' makes it difficult to find runtime type information later
  // when we need it.
  //
  if (skipFixup(formal, domExpr, eltExpr)) return;

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
* Add a second formal for each inout formal so that later parts of            *
* compilation can handle the `in` and `out` parts separately.                 *
*                                                                             *
************************************** | *************************************/
static void fixupInoutFormals(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (formal->intent == INTENT_INOUT) {
      if (fn->hasFlag(FLAG_EXTERN)) {
        formal->originalIntent = INTENT_REF;
        formal->intent = INTENT_REF;
      } else if (formal->variableExpr != NULL) {
        USR_FATAL_CONT(formal, "inout varargs not currently supported");
        formal->originalIntent = INTENT_REF;
        formal->intent = INTENT_REF;
      } else {
        // Add a hidden out formal after the inout one.
        ArgSymbol* outFormal = formal->copy();
        outFormal->name = astr(outFormal->name, "_out");
        outFormal->originalIntent = INTENT_OUT;
        outFormal->intent = INTENT_OUT;
        outFormal->addFlag(FLAG_HIDDEN_FORMAL_INOUT);
        outFormal->defaultExpr = new BlockStmt(new SymExpr(formal));
        DefExpr* def = new DefExpr(outFormal);
        formal->defPoint->insertAfter(def);
      }
    }
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

static void cloneParameterizedPrimitive(FnSymbol* fn, ArgSymbol* formal, CallExpr* typeSpecifier);

static void cloneParameterizedPrimitive(FnSymbol* fn,
                                        ArgSymbol* formal,
                                        Expr*     query,
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
          cloneParameterizedPrimitive(fn, formal, typeSpecifier);

          break;
        }
      }
    }
  }
}

// e.g. x : int(?w) or int(?)
static bool isParameterizedPrimitive(CallExpr* typeSpecifier) {
  bool retval = false;

  if (SymExpr* callFnSymExpr = toSymExpr(typeSpecifier->baseExpr)) {
    if (typeSpecifier->numActuals() == 1) {
      Expr* first = typeSpecifier->get(1);
      SymExpr* query = toSymExpr(first);
      if (isDefExpr(first) || (query && query->symbol() == gUninstantiated)) {
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
  }

  return retval;
}

// return true if a type specifier has the form `bool(?)` and false if
// it's `bool(?w)`
//
static bool typeSpecifierUnnamedQuery(CallExpr* typeSpecifier) {
  if (typeSpecifier->numActuals()      ==    1) {
    if (DefExpr* de = toDefExpr(typeSpecifier->get(1))) {
      return strncmp("chpl__query", de->sym->name, strlen("chpl__query")) == 0;
    } else if (SymExpr* se = toSymExpr(typeSpecifier->get(1))) {
      return se->symbol() == gUninstantiated;
    }
  }
  return false;
}


// 'formal' is certain to be a parameterized primitive e.g int(?w)
static void cloneParameterizedPrimitive(FnSymbol* fn, ArgSymbol* formal, CallExpr* typeSpecifier) {
  Symbol* callFnSym = toSymExpr(typeSpecifier->baseExpr)->symbol();
  Expr*   query     = typeSpecifier->get(1);

  if (callFnSym == dtBools[BOOL_SIZE_DEFAULT]->symbol) {
    // If 'bool(?)', instantiate for 'bool', and all 'bool(w)'
    // If 'bool(?w)', skip 'bool' instantiation since 'w' is unknown
    int start = typeSpecifierUnnamedQuery(typeSpecifier) ? BOOL_SIZE_SYS
                                                         : BOOL_SIZE_8;
    for (int i = start; i < BOOL_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, formal, query, ((i == BOOL_SIZE_SYS) ?
                                             BOOL_SYS_WIDTH :
                                             get_width(dtBools[i])));
    }

  } else if (callFnSym == dtInt [INT_SIZE_DEFAULT]->symbol ||
             callFnSym == dtUInt[INT_SIZE_DEFAULT]->symbol) {
    for (int i = INT_SIZE_8; i < INT_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, formal, query, get_width(dtInt[i]));
    }

  } else if (callFnSym == dtReal[FLOAT_SIZE_DEFAULT]->symbol ||
             callFnSym == dtImag[FLOAT_SIZE_DEFAULT]->symbol) {
    for (int i = FLOAT_SIZE_32; i < FLOAT_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, formal, query, get_width(dtReal[i]));
    }

  } else if (callFnSym == dtComplex[COMPLEX_SIZE_DEFAULT]->symbol) {
    for (int i = COMPLEX_SIZE_64; i < COMPLEX_SIZE_NUM; i++) {
      cloneParameterizedPrimitive(fn, formal, query, get_width(dtComplex[i]));
    }
  }

  fn->defPoint->remove();
}

static void cloneParameterizedPrimitive(FnSymbol* fn,
                                        ArgSymbol* formal,
                                        Expr*     query,
                                        int       width) {
  SymbolMap map;
  FnSymbol* newFn = fn->copy(&map);

  if (DefExpr* def = toDefExpr(query)) {
    Symbol* newSym = map.get(def->sym);
    std::vector<SymExpr*> symExprs;

    newSym->defPoint->replace(new SymExpr(new_IntSymbol(width)));

    collectSymExprsFor(newFn, newSym, symExprs);

    for_vector(SymExpr, se, symExprs) {
        se->setSymbol(new_IntSymbol(width));
    }
  } else {
    ArgSymbol* newFormal = toArgSymbol(map.get(formal));
    CallExpr* typeSpecifier = toCallExpr(newFormal->typeExpr->body.tail);
    typeSpecifier->get(1)->replace(new SymExpr(new_IntSymbol(width)));
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

static void fixDecoratedTypePrimitives(FnSymbol* fn, ArgSymbol* formal);

static void expandQueryForGenericTypeSpecifier(FnSymbol*  fn,
                                               ArgSymbol* formal);

static void replaceQueryUses(ArgSymbol*             formal,
                             DefExpr*               def,
                             CallExpr*              query,
                             std::vector<SymExpr*>& symExprs);

static void addToWhereClause(FnSymbol*  fn,
                             ArgSymbol* formal,
                             Expr*      test);

static void fixupQueryFormals(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (BlockStmt* typeExpr = formal->typeExpr) {
      Expr* tail = typeExpr->body.tail;

      if  (isDefExpr(tail) == true) {
        replaceUsesWithPrimTypeof(fn, formal);

      } else if (isQueryForGenericTypeSpecifier(formal) == true) {
        fixDecoratedTypePrimitives(fn, formal);
        expandQueryForGenericTypeSpecifier(fn, formal);
      } else if (SymExpr* se = toSymExpr(tail)) {
        if (se->symbol() == gUninstantiated) {
          formal->typeExpr->remove();
          formal->type = dtAny;
        }
      }
    }
  }
}

// The type-expr is known to be a simple DefExpr
static void replaceUsesWithPrimTypeof(FnSymbol* fn, ArgSymbol* formal) {
  BlockStmt*            typeExpr = formal->typeExpr;
  DefExpr*              def      = toDefExpr(typeExpr->body.tail);
  std::vector<SymExpr*> symExprs;

  collectSymExprsFor(fn, def->sym, symExprs);

  if (formal->variableExpr) // varargs argument e.g. proc f(x...)
    addToWhereClause(fn, formal, new CallExpr(PRIM_IS_STAR_TUPLE_TYPE, formal));

  for_vector(SymExpr, se, symExprs) {
      if (formal->variableExpr)
        // e.g. proc foo(arg:?t ...)
        // formal is a tuple but the query should be of the tuple elements
        // 1 is the size field, 2 is the index of the first tuple field
        se->replace(new CallExpr(PRIM_QUERY, formal, new_IntSymbol(2)));
      else
        se->replace(new CallExpr(PRIM_TYPEOF, formal));
  }

  formal->typeExpr->remove();

  formal->type = dtAny;
}

static bool isGenericActual(Expr* expr) {
  if (isDefExpr(expr))
    return true;
  if (SymExpr* se = toSymExpr(expr)) {
    if (se->symbol() == gUninstantiated) {
      return true;
    } else if (TypeSymbol* ts = toTypeSymbol(se->symbol())) {
      Type* canonicalType = canonicalDecoratedClassType(ts->type);
      if (AggregateType* at = toAggregateType(canonicalType))
        if (at->isGeneric() && !at->isGenericWithDefaults())
          return true;
      if (ts->hasFlag(FLAG_GENERIC))
        return true;
    }
  }

  return false;
}

// 2 types of generic actuals:
//  1. Generic type symbol as a leaf, where that type does not have
//     defaults for every field)
//       e.g. Owned(MyGenericClass)
//  2. Call to a generic type with a type query expression within
//       e.g. MyGenericClass(?) / MyGenericClass(?t) / MyGenericClass(t=?t)
static bool doesCallContainGenericActual(CallExpr* call) {
  // Is this a partial type expression?
  if (SymExpr* se = toSymExpr(call->baseExpr)) {
    if (TypeSymbol* ts = toTypeSymbol(se->symbol())) {
      if (AggregateType* at = toAggregateType(canonicalDecoratedClassType(ts->type))) {
        if (at->isGeneric() &&
            !at->isGenericWithDefaults() &&
            at->genericFields.size() != (unsigned int)(call->numActuals())) {
          return true;
        }
      }
    }
  }

  for_actuals(actual, call) {
    if (isGenericActual(actual)) {
      return true;

    } else if (NamedExpr* named = toNamedExpr(actual)) {
      if (isGenericActual(named->actual)) {
        return true;
      }

    } else if (CallExpr* subcall = toCallExpr(actual)) {
      if (doesCallContainGenericActual(subcall)) {
        return true;
      }
    }
  }

  return false;
}

static bool isQueryForGenericTypeSpecifier(ArgSymbol* formal) {
  bool retval = false;

  if (CallExpr* call = toCallExpr(formal->typeExpr->body.tail)) {
    retval = doesCallContainGenericActual(call);
  }

  return retval;
}

static void fixDecoratedTypePrimitives(FnSymbol* fn, ArgSymbol* formal) {
  std::vector<CallExpr*> calls;

  collectCallExprs(formal->typeExpr, calls);

  for_vector(CallExpr, call, calls) {
    resolveUnmanagedBorrows(call);
  }
}

static void expandQueryForGenericTypeSpecifier(FnSymbol*  fn,
                                               std::vector<SymExpr*>& symExprs,
                                               ArgSymbol* formal,
                                               CallExpr* call,
                                               Expr* queried);

static TypeSymbol* getTypeForSpecialConstructor(CallExpr* call);

// The type-expr is known to be a CallExpr with a query definition
static void expandQueryForGenericTypeSpecifier(FnSymbol*  fn,
                                               ArgSymbol* formal) {
  BlockStmt*            typeExpr  = formal->typeExpr;
  Expr*                 tail      = typeExpr->body.tail;
  CallExpr*             call      = toCallExpr(tail);

  std::vector<SymExpr*> symExprs;

  collectSymExprs(fn, symExprs);

  Expr* queried = new SymExpr(formal);

  // Queries access the 1st tuple element for varargs functions
  if (formal->variableExpr) {
    // 1 is the size field, 2 is the index of the first tuple field
    queried = new CallExpr(PRIM_QUERY, formal, new_IntSymbol(2));
    // Add check that passed tuple is homogeneous
    addToWhereClause(fn, formal, new CallExpr(PRIM_IS_STAR_TUPLE_TYPE, formal));
  }

  expandQueryForGenericTypeSpecifier(fn, symExprs, formal, call, queried);

  // Remove the queries from the formal argument type
  Expr* usetype = NULL;
  if (TypeSymbol* ts = getTypeForSpecialConstructor(call)) {
    usetype = new SymExpr(ts);
  } else if (call->baseExpr) {
    usetype = call->baseExpr->remove();
  } else {
    usetype = call->remove();
    INT_ASSERT(!doesCallContainGenericActual(call));
  }

  formal->typeExpr->replace(new BlockStmt(usetype));

  formal->addFlag(FLAG_MARKED_GENERIC);
}

static TypeSymbol* getTypeForSpecialConstructor(CallExpr* call) {
  if (call->isNamed("_build_tuple") || call->isNamed("*")) {
    INT_ASSERT(!call->isPrimitive(PRIM_MULT));
    return dtTuple->symbol;
  }

  // TODO: remove the below code
  else if (call->isNamed("_to_unmanaged") ||
             call->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
             call->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED)) {
    return dtUnmanaged->symbol;
  } else if (call->isNamed("_to_borrowed") ||
             call->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
             call->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED)) {
    return dtBorrowed->symbol;
  }
  return NULL;
}


// Constructs a PRIM_QUERY with arguments a, b
// Copies an expr argument but not a symbol argument
static CallExpr* makePrimQuery(BaseAST* a, BaseAST* b=NULL) {
  Expr* aExpr = NULL;
  Expr* bExpr = toExpr(b);
  if (Symbol* sym = toSymbol(a))
    aExpr = new SymExpr(sym);
  else if (Expr* expr = toExpr(a))
    aExpr = expr->copy();

  INT_ASSERT(aExpr);

  if (b) {
    if (Symbol* symB = toSymbol(b))
      bExpr = new SymExpr(symB);
    else if (Expr* expr = toExpr(b))
      bExpr = expr->copy();

    INT_ASSERT(bExpr);
  }

  if (bExpr == NULL)
    return new CallExpr(PRIM_QUERY, aExpr);
  else
    return new CallExpr(PRIM_QUERY, aExpr, bExpr);
}

// call - the call containing something
// query - the just-created PRIM_QUERY to add to the ast somewhere
//         (not yet in the AST)
// actual - the actual argument to the nested type constructor
//
// symExprs are the symExprs referring to e.g. t for ?t we might replace
static void expandQueryForActual(FnSymbol*  fn,
                                 std::vector<SymExpr*>& symExprs,
                                 ArgSymbol* formal,
                                 CallExpr* call,
                                 CallExpr* query,
                                 Expr* actual) {
  CallExpr* subcall = toCallExpr(actual);

  if (isGenericActual(actual)) {
    if (DefExpr* def = toDefExpr(actual)) {
      replaceQueryUses(formal, def, query, symExprs);
    } else if (SymExpr* se = toSymExpr(actual)) {
      if (se->symbol() == gUninstantiated) {
        // No need for further action here
      } else {
        TypeSymbol* ts = toTypeSymbol(se->symbol());
        INT_ASSERT(ts);
        Expr* subtype = new SymExpr(ts);
        addToWhereClause(fn, formal,
                         new CallExpr(PRIM_IS_INSTANTIATION_ALLOW_VALUES,
                                      subtype, query->copy()));
      }
    } else {
      INT_FATAL("case not handled");
    }
  } else if (subcall && (doesCallContainGenericActual(subcall))) {
    Expr* subtype = NULL;
    if (TypeSymbol* ts = getTypeForSpecialConstructor(subcall)) {
      subtype = new SymExpr(ts);
    } else if (subcall->baseExpr) {
      subtype = subcall->baseExpr->copy();
    } else {
      subtype = subcall->copy();
      INT_ASSERT(!doesCallContainGenericActual(subcall));
    }
    // Add check that actual type satisfies
    addToWhereClause(fn, formal,
                     new CallExpr(PRIM_IS_INSTANTIATION_ALLOW_VALUES,
                                  subtype, query->copy()));
    // Recurse to handle any nested DefExprs
    expandQueryForGenericTypeSpecifier(fn, symExprs, formal,
                                       subcall, query);
  } else {
    // Add check that formal type/param matches actual
    addToWhereClause(fn, formal,
                     new CallExpr("==", actual->copy(), query));
  }
}

// call - the type constructor or build tuple call currently being considered
// queried - a PRIM_QUERY formal, ... recording the path to the current call
//           but not in the AST (it should be copied)
static void expandQueryForGenericTypeSpecifier(FnSymbol*  fn,
                                               std::vector<SymExpr*>& symExprs,
                                               ArgSymbol* formal,
                                               CallExpr* call,
                                               Expr* queried) {

  int position = 1;
  bool isTuple = false;

  if (call->isNamed("_build_tuple")) {
    isTuple = true;
    Expr*     actual = new SymExpr(new_IntSymbol(call->numActuals()));
    CallExpr* query  = makePrimQuery(queried, new_CStringSymbol("size"));

    // Add check that actual tuple size == formal tuple size
    addToWhereClause(fn, formal, new CallExpr("==", actual, query));

    call->baseExpr->replace(new SymExpr(dtTuple->symbol));

    position = position + 1; // tuple size is technically 1st param/type

  } else if (call->isNamed("*")) {
    // it happens to be that 1st actual == size so that will be checked below
    addToWhereClause(fn, formal,
                     new CallExpr(PRIM_IS_STAR_TUPLE_TYPE, queried->copy()));
  }

  // Fix type constructor calls to owned e.g.
  // (call dtOwned SomeGenericClass( arg ))
  // This is relevant to test/functions/ferguson/query/owned-generic
  if (call->baseExpr != NULL) {
    if (SymExpr* se = toSymExpr(call->baseExpr)) {
      if (TypeSymbol* ts = toTypeSymbol(se->symbol())) {
        if (isManagedPtrType(ts->type)) {
          Type* manager = getManagedPtrManagerType(ts->type);

          // Where clause checks that it has the right manager
          addToWhereClause(fn, formal,
                           new CallExpr(PRIM_IS_INSTANTIATION_ALLOW_VALUES,
                                        manager->symbol, queried->copy()));

          if (call->numActuals() == 1) {
            if (SymExpr* se2 = toSymExpr(call->get(1))) {
              if (TypeSymbol* ts2 = toTypeSymbol(se2->symbol())) {
                // e.g. owned MyGenericClass
                ClassTypeDecorator d = CLASS_TYPE_GENERIC_NONNIL;
                if (isNilableClassType(ts2->type))
                  d = CLASS_TYPE_GENERIC_NILABLE;
                Type* genericMgmt = getDecoratedClass(ts2->type, d);
                CallExpr* c = new CallExpr(PRIM_IS_INSTANTIATION_ALLOW_VALUES,
                                           genericMgmt->symbol, queried);
                addToWhereClause(fn, formal, c);
                // Nothing else to do here since there is no nested call.
                return;
              }
            } else if (CallExpr* subCall = toCallExpr(call->get(1))) {
              if (SymExpr* subBase = toSymExpr(subCall->baseExpr)) {
                if (TypeSymbol* ts = toTypeSymbol(subBase->symbol())) {
                  if (isClassLike(ts->type)) {
                    // e.g. owned SomeClass( arg )
                    // Continue on using call for anything else
                    call = subCall;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  CallExpr* gatheringNamedArgs = makePrimQuery(queried);

  for_actuals(actual, call) {
    if (NamedExpr* named = toNamedExpr(actual)) {
      Symbol*   name = new_CStringSymbol(named->name);
      CallExpr* query = makePrimQuery(queried, name);

      gatheringNamedArgs->insertAtTail(new SymExpr(name));

      expandQueryForActual(fn, symExprs, formal, call, query, named->actual);
    }

    // not a NamedExpr? handled in next loop.
  }

  // Used to prevent redundant errors
  bool foundUninstantiated = false;
  bool multipleQuestionError = false;
  bool positionalQuestionError = false;

  for_actuals(actual, call) {
    if (isNamedExpr(actual) == false) {
      if (isSymExpr(actual) && toSymExpr(actual)->symbol() == gUninstantiated) {
        if (!isTuple) {
          if (foundUninstantiated && !multipleQuestionError) {
            multipleQuestionError = true;
            USR_FATAL_CONT(call, "formal '%s' may not have a type expression with multiple '?'", formal->name);
            USR_PRINT(call, "'?' cannot be used to positionally indicate uninstantiated fields");
          } else {
            foundUninstantiated = true;
          }
        }
      } else if (foundUninstantiated && !positionalQuestionError) {
        positionalQuestionError = true;
        USR_FATAL_CONT(call, "type expression of formal '%s' cannot use '?' before unnamed arguments to a type specifier", formal->name);
        USR_PRINT(call, "'?' cannot be used to positionally indicate uninstantiated fields");
      }

      CallExpr* query = gatheringNamedArgs->copy();
      query->insertAtTail(new SymExpr(new_IntSymbol(position)));

      expandQueryForActual(fn, symExprs, formal, call, query, actual);

      position = position + 1;
    }
  }
}

static void replaceQueryUses(ArgSymbol*             formal,
                             DefExpr*               def,
                             CallExpr*              query,
                             std::vector<SymExpr*>& symExprs) {
  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == def->sym) {
      CallExpr* myQuery = query->copy();
      se->replace(myQuery);
    }
  }
}

static void addToWhereClause(FnSymbol*  fn,
                             ArgSymbol* formal,
                             Expr*      test) {
  Expr*     where  = NULL;

  if (fn->where == NULL) {
    where = new SymExpr(gTrue);

    fn->where = new BlockStmt(where);

    insert_help(fn->where, NULL, fn);

    fn->addFlag(FLAG_COMPILER_ADDED_WHERE);

  } else {
    where = fn->where->body.tail;
  }

  formal->addFlag(FLAG_NOT_FULLY_GENERIC);

  // Replace where with (where & test)
  CallExpr* combine = new CallExpr("&");
  where->replace(combine);
  combine->insertAtTail(where);
  combine->insertAtTail(test);
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

static void updateInitMethod(FnSymbol* fn) {
  Type* thisType = fn->_this->type;

  if (isAggregateType(thisType) == true) {

    if (fn->name == astrInitEquals) {
      if (isClass(thisType))
        USR_FATAL_CONT(fn, "init= may not be defined on class types");
      if (thisType->symbol->hasFlag(FLAG_EXTERN))
        USR_FATAL_CONT(fn, "init= may not currently be defined on extern types");
    }

    preNormalizeInitMethod(fn);

  } else if (thisType == dtUnknown) {
    INT_FATAL(fn, "'this' argument has unknown type");

  } else {
    USR_FATAL_CONT(fn, "initializers may currently only be defined on class, record, or union types");
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
