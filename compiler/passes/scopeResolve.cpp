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

#include "scopeResolve.h"

#include "astutil.h"
#include "baseAST.h"
#include "build.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "clangUtil.h"
#include "driver.h"
#include "externCResolve.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "initializerRules.h"
#include "LoopExpr.h"
#include "LoopStmt.h"
#include "passes.h"
#include "ResolveScope.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "TryStmt.h"
#include "view.h"
#include "visibleFunctions.h"
#include "wellknown.h"

#include <algorithm>
#include <map>
#include <set>
#include <stack>

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// The moduleUsesCache is a cache from blocks with use-statements to
// the modules that they use arranged in breadth-first order and
// separated by NULL modules to indicate that the modules are not at
// the same depth.
//
// Note that this caching is not enabled until after use expression
// have been resolved.
//
static std::map<BlockStmt*, Vec<VisibilityStmt*>*> moduleUsesCache;
static bool enableModuleUsesCache = false;

// To avoid duplicate user warnings in checkIdInsideWithClause().
// Using pair<> instead of astlocT to avoid defining operator<.
typedef std::pair< std::pair<const char*,int>, const char* >  WFDIWmark;
static std::set< std::pair< std::pair<const char*,int>, const char* > > warnedForDotInsideWith;

// To avoid duplicate user warnings between resolveUnresolvedSymExpr and
// resolveModuleCall
static std::vector<BaseAST*> failedUSymExprs;

static void          scopeResolve(ModuleSymbol*       module,
                                  const ResolveScope* root);

static void          scopeResolveExpr(Expr* expr, ResolveScope* scope);

static astlocT*      resolveUnresolvedSymExpr(UnresolvedSymExpr* usymExpr,
                                              bool returnRename = false);

static void          resolveUnresolvedSymExpr(UnresolvedSymExpr* usymExpr,
                                              Symbol* sym);

static bool lookupThisScopeAndUses(const char*           name,
                                   BaseAST*              context,
                                   BaseAST*              scope,
                                   std::vector<Symbol*>& symbols,
                                   std::map<Symbol*, astlocT*>& renameLocs,
                                   bool storeRenames);

static ModuleSymbol* definesModuleSymbol(Expr* expr);

static void computeClassHierarchy() {
  //
  // compute class hierarchy
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    ct->addClassToHierarchy();
  }
}

static void handleReceiverFormals() {
  //
  // resolve type of this for methods
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {

    if (fn->_this == NULL) continue; // not a method

    if (fn->_this->type == dtUnknown) {
      Expr* stmt = toArgSymbol(fn->_this)->typeExpr->body.only();

      if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(stmt)) {
        SET_LINENO(fn->_this);

        if (TypeSymbol* ts = toTypeSymbol(lookup(sym->unresolved, sym))) {
          sym->replace(new SymExpr(ts));

          fn->_this->type = ts->type;
          fn->_this->type->methods.add(fn);

          AggregateType::setCreationStyle(ts, fn);
        }

      } else if (SymExpr* sym = toSymExpr(stmt)) {
        fn->_this->type = sym->symbol()->type;
        fn->_this->type->methods.add(fn);

        AggregateType::setCreationStyle(sym->symbol()->type->symbol, fn);
      }

    } else {
      AggregateType::setCreationStyle(fn->_this->type->symbol, fn);
    }
  }
}

static void markGenerics() {
  // Figure out which types are generic, in a transitive closure manner
    bool changed;
    do {
      changed = false;
      forv_Vec(AggregateType, at, gAggregateTypes) {
        // don't try to mark generic again
        if (!at->isGeneric()) {

          bool anyGeneric = false;
          bool anyNonDefaultedGeneric = false;
          for_fields(field, at) {
            bool hasDefault = false;
            if (at->fieldIsGeneric(field, hasDefault)) {
              anyGeneric = true;
              if (hasDefault == false)
                anyNonDefaultedGeneric = true;
            }
          }

          if (anyGeneric) {
            at->markAsGeneric();
            if (anyNonDefaultedGeneric == false)
              at->markAsGenericWithDefaults();
            changed = true;
          }
        }
      }
    } while (changed);
}

static void processGenericFields() {
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    // Build the type constructor now that we know which types are generic
    if (isClass(ct) && ct->symbol->hasFlag(FLAG_EXTERN)) {
      USR_FATAL_CONT(ct, "Extern classes are not supported.");
    }
    ct->processGenericFields();
  }
}

/************************************* | **************************************
*                                                                             *
* addToSymbolTable adds the asts in a vector to the global symbolTable such   *
* that symbol definitions are added to entries in the table and new           *
* enclosing asts become entries                                               *
*                                                                             *
************************************** | *************************************/

// 2017/05/23: Noakes
//
// This is a specialized walk for the simplified case of chpl__Program.
// This provides an anchor to start the transition to a more general
// version of a conventional top-down traversal.

// It also serves as a template for the more general version.
// Eventually it should be possible to use the general implementation
// to handle chpl__Program with little or no special casing.

static void addToSymbolTable() {
  rootScope = ResolveScope::getRootModule();

  // Extend the rootScope with every top-level definition
  for_alist(stmt, theProgram->block->body) {
    if (DefExpr* def = toDefExpr(stmt)) {
      rootScope->extend(def->sym, /* isTopLevel= */ true);
    }
  }

  // This would be the place to handle use statements but
  // skipping for now as chpl__Program does not have any.

  // Now recurse on every top-level module
  for_alist(stmt, theProgram->block->body) {
    if (ModuleSymbol* mod = definesModuleSymbol(stmt)) {
      scopeResolve(mod, rootScope);
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Exported entry point for AggregateType                                      *
*                                                                             *
************************************** | *************************************/

void addToSymbolTable(FnSymbol* fn) {
  std::vector<DefExpr*> defs;

  collectDefExprs(fn, defs);

  for_vector(DefExpr, def, defs) {
    addToSymbolTable(def);
  }
}

void addToSymbolTable(DefExpr* def) {
  Symbol* newSym = def->sym;

  if (newSym->hasFlag(FLAG_TEMP) == false &&
      isLabelSymbol(newSym)      == false) {
    ResolveScope* entry = ResolveScope::findOrCreateScopeFor(def);

    entry->extend(newSym);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void scopeResolve(FnSymbol*           fn,
                         const ResolveScope* parent);

static void scopeResolve(BlockStmt*          block,
                         const ResolveScope* parent);

static void scopeResolve(TypeSymbol*         typeSym,
                         const ResolveScope* parent);

static void scopeResolve(const AList&        alist,
                         ResolveScope*       scope);

static void scopeResolve(ModuleSymbol*       module,
                         const ResolveScope* parent) {
  ResolveScope* scope = new ResolveScope(module, parent);

  scopeResolve(module->block->body, scope);
}

static void scopeResolve(BlockStmt*          block,
                         const ResolveScope* parent) {
  ResolveScope* scope = new ResolveScope(block, parent);

  scopeResolve(block->body,         scope);
}

static void scopeResolve(ForallStmt*         forall,
                         const ResolveScope* parent)
{
  INT_ASSERT(forall->fRecIterIRdef == NULL); //nothing to resolve in fRecIter*

  BlockStmt*    loopBody     = forall->loopBody();
  ResolveScope* stmtScope = new ResolveScope(forall, parent);
  ResolveScope* bodyScope = new ResolveScope(loopBody, stmtScope);

  // 'stmtScope' contains loop induction variables + shadow variables
  //             incl. task-private variables.
  // 'bodyScope' is for the loop body.

  // cf. scopeResolve(FnSymbol*,parent)
  for_alist(ivdef, forall->inductionVariables()) {
    Symbol* sym = toDefExpr(ivdef)->sym;
    // "chpl__tuple_blank" indicates the underscore placeholder for
    // the induction variable. Do not add it. Because if there are two
    // (legally) ex. "forall (_,_) in ...", we get an error.
    if (strcmp(sym->name, "chpl__tuple_blank"))
      stmtScope->extend(sym);
  }

  for_alist(itexpr, forall->iteratedExpressions()) {
    scopeResolveExpr(itexpr, stmtScope);
  }

  for_shadow_vars_and_defs(svar, sdef, temp, forall) {
    stmtScope->extend(svar);
    if (sdef->init != NULL)
      scopeResolveExpr(sdef->init, stmtScope);
  }

  scopeResolve(loopBody->body, bodyScope);
}

static void scopeResolve(FnSymbol*           fn,
                         const ResolveScope* parent) {
  ResolveScope* scope = new ResolveScope(fn, parent);

  for_alist(formal, fn->formals) {
    if (DefExpr* def = toDefExpr(formal)) {
      Symbol* sym = def->sym;

      // Remarkably, there is such a thing as TEMP formals!
      if (sym->hasFlag(FLAG_TEMP) == false) {
        if (ArgSymbol* formal = toArgSymbol(sym)) {
          scope->extend(formal);

          if (formal->typeExpr != NULL) {
            std::vector<BaseAST*> asts;

            // The typeExpr may define query variables.  The DefExpr for
            // these are unconventional so fall back on the collect()
            // functions.

            // Collect *all* asts within this top-level module in text order
            collect_asts(formal->typeExpr, asts);

            for_vector(BaseAST, item, asts) {
              if (DefExpr* subDef = toDefExpr(item)) {
                scope->extend(subDef->sym);
              }
            }
          }

          if (formal->variableExpr != NULL) {
            std::vector<BaseAST*> asts;

            // Use the same scheme as for typeExpr
            collect_asts(formal->variableExpr, asts);

            for_vector(BaseAST, item, asts) {
              if (DefExpr* subDef = toDefExpr(item)) {
                scope->extend(subDef->sym);
              }
            }
          }

        } else {
          INT_ASSERT(false);
        }
      }

    } else {
      INT_ASSERT(false);
    }
  }

  if (fn->where != NULL) {
    scopeResolve(fn->where, scope);
  }

  if (fn->retExprType != NULL) {
    scopeResolve(fn->retExprType, scope);
  }

  scopeResolve(fn->body, scope);
}

static void scopeResolve(TypeSymbol*         typeSym,
                         const ResolveScope* parent) {
  Type* type = typeSym->type;

  if (AggregateType* at = toAggregateType(type)) {
    ResolveScope* scope = new ResolveScope(typeSym, parent);

    scopeResolve(at->fields,    scope);

  } else if (EnumType* et = toEnumType(type)) {
    ResolveScope* scope = new ResolveScope(typeSym, parent);

    scopeResolve(et->constants, scope);
  }
}

static void scopeResolve(IfExpr* ife, ResolveScope* scope) {
  scopeResolve(ife->getThenStmt(), scope);
  scopeResolve(ife->getElseStmt(), scope);
}

static void scopeResolve(LoopExpr* fe, ResolveScope* parent) {
  scopeResolveExpr(fe->iteratorExpr, parent);

  ResolveScope* scope = new ResolveScope(fe, parent);
  for_alist(ind, fe->defIndices) {
    DefExpr* def = toDefExpr(ind);
    scope->extend(def->sym);
  }

  if (fe->indices) scopeResolveExpr(fe->indices, scope);
  if (fe->cond) scopeResolveExpr(fe->cond, scope);

  scopeResolveExpr(fe->loopBody, scope);
}

static void scopeResolve(CallExpr* call, ResolveScope* scope) {
  for_actuals(actual, call) {
    scopeResolveExpr(actual, scope);
  }
}

static void scopeResolveExpr(Expr* expr, ResolveScope* scope) {
  if (CallExpr* call = toCallExpr(expr)) {
    scopeResolve(call, scope);
  } else if (IfExpr* ife = toIfExpr(expr)) {
    scopeResolve(ife, scope);
  } else if (LoopExpr* fe = toLoopExpr(expr)) {
    scopeResolve(fe, scope);
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    scopeResolve(block, scope);
  } else if (NamedExpr* named = toNamedExpr(expr)) {
    scopeResolveExpr(named->actual, scope);
  }
}

static void scopeResolve(const AList& alist, ResolveScope* scope) {
  // Add the local definitions to the scope
  for_alist(stmt, alist) {
    if (DefExpr* def = toDefExpr(stmt))   {
      Symbol* sym = def->sym;

      if (sym->hasFlag(FLAG_TEMP) == false &&
          isLabelSymbol(sym)      == false) {
        scope->extend(sym);
      }
    }
  }

  // Should process use statements here

  // Process the remaining statements
  for_alist(stmt, alist) {
    if (DefExpr* def = toDefExpr(stmt))   {
      Symbol* sym = def->sym;

      if (sym->hasFlag(FLAG_TEMP) == false &&
          isLabelSymbol(sym)      == false) {
        if (ModuleSymbol* modSym  = toModuleSymbol(sym)) {
          scopeResolve(modSym,  scope);

        } else if (FnSymbol* fnSym = toFnSymbol(sym))     {
          scopeResolve(fnSym,   scope);

        } else if (TypeSymbol* typeSym = toTypeSymbol(sym))   {
          scopeResolve(typeSym, scope);
        }
      }

      if (def->init != NULL) {
        scopeResolveExpr(def->init, scope);
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      if (block->blockTag == BLOCK_NORMAL) {
        scopeResolve(block,       scope);

      } else {
        scopeResolve(block->body, scope);
      }

    } else if (CondStmt* cond = toCondStmt(stmt))  {
      scopeResolveExpr(cond->condExpr, scope);

      scopeResolve(cond->thenStmt, scope);

      if (cond->elseStmt != NULL) {
        scopeResolve(cond->elseStmt, scope);
      }

    } else if (TryStmt* tryStmt = toTryStmt(stmt)) {
      scopeResolve(tryStmt->_body, scope);

      for_alist(item, tryStmt->_catches) {
        if (CatchStmt* catchStmt = toCatchStmt(item)) {
          scopeResolve(catchStmt->_body, scope);

        } else {
          INT_ASSERT(false);
        }
      }

    } else if (ForallStmt* forallStmt = toForallStmt(stmt)) {
      scopeResolve(forallStmt, scope);

    } else if (DeferStmt* deferStmt = toDeferStmt(stmt)) {
      scopeResolve(deferStmt->body(), scope);

    } else if (isUseStmt(stmt)           == true ||
               isImportStmt(stmt)        == true ||
               isUnresolvedSymExpr(stmt) == true ||
               isSymExpr(stmt)           == true ||
               isGotoStmt(stmt)          == true) {

    // May occur in --llvm runs
    } else if (isExternBlockStmt(stmt)   == true) {

    } else {
      scopeResolveExpr(stmt, scope);
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Transform module uses into calls to initialize functions; store the         *
* relevant scoping information in BlockStmt::modUses                          *
*                                                                             *
************************************** | *************************************/

static void processImportExprs() {
  for_alist(expr, theProgram->block->body) {
    if (ModuleSymbol* topLevelModule = definesModuleSymbol(expr)) {
      std::vector<BaseAST*> asts;

      // Collect *all* asts within this top-level module in text order
      collect_asts(topLevelModule, asts);

      std::stack<ResolveScope*> scopes;
      for_vector(BaseAST, item, asts) {
        Expr* exprItem = toExpr(item);
        if (exprItem != NULL && exprItem->parentExpr != NULL) {
          BaseAST*      astScope = getScope(item);
          ResolveScope* scope    = ResolveScope::getScopeFor(astScope);

          // Resolve any uses or imports we find
          if (UseStmt* useStmt = toUseStmt(item)) {
            useStmt->scopeResolve(scope);
          } else if (ImportStmt* importStmt = toImportStmt(item)) {
            importStmt->scopeResolve(scope);
          }

          // As we finish with this statement, check to see if we've exited one
          // or more scopes and update its/their statuses to reflect that we
          // have finished resolving all the use or import statements within
          // them.
          if (scope != NULL) {
            if (scopes.empty()) {
              scopes.push(scope);
            }
            if (scope != scopes.top()) {
              ResolveScope* last = scopes.top();
              BlockStmt* lastAst = last->asBlockStmt();
              BlockStmt* curBlock = scope->asBlockStmt();
              if (curBlock != NULL) {
                if (lastAst->contains(curBlock)) {
                  scopes.push(scope);
                } else {
                  while (!lastAst->contains(curBlock) && !scopes.empty()) {
                    scopes.pop();
                    last->progress = IUP_COMPLETED;
                  }
                }
              }
            }
          }
        }
      }

      // Once we've finished traversing, close any remaining unclosed scopes.
      while (!scopes.empty()) {
        ResolveScope* last = scopes.top();
        scopes.pop();
        last->progress = IUP_COMPLETED;
      }
    }
  }
}

/************************************* | *************************************/

static void handleLoopStmtGoto(LoopStmt* loop, GotoStmt* gs) {
  if (gs->gotoTag == GOTO_BREAK) {
    gs->label->replace(new SymExpr(loop->breakLabelGet()));

  } else if (gs->gotoTag == GOTO_CONTINUE) {
    gs->label->replace(new SymExpr(loop->continueLabelGet()));

  } else {
    INT_FATAL(gs, "unexpected goto type");
  }
}

/*
Note that break and continue statements that are placed incorrectly
inside a forall loop are flagged by checkControlFlow() during parsing.
This includes 'continue' to a named loop outside of the forall,
which the code here would not catch.
*/
static void handleForallGoto(ForallStmt* forall, GotoStmt* gs) {
  if (gs->gotoTag == GOTO_BREAK) {
    INT_ASSERT(false);

  } else if (gs->gotoTag == GOTO_CONTINUE) {
    INT_ASSERT(isSymExpr(gs->label) == true);
    gs->label->replace(new SymExpr(forall->continueLabel()));

  } else {
    INT_FATAL(gs, "unexpected goto type");
  }
}

static void resolveGotoLabels() {
  forv_Vec(GotoStmt, gs, gGotoStmts) {
    SET_LINENO(gs);

    Stmt* loop = NULL;

    if (isSymExpr(gs->label) == true) {
      loop = LoopStmt::findEnclosingLoopOrForall(gs);

      if (loop == NULL) {
        USR_FATAL_CONT(gs, "break or continue is not in a loop");
      }

    } else if (UnresolvedSymExpr* label = toUnresolvedSymExpr(gs->label)) {
      loop = LoopStmt::findEnclosingLoop(gs, label->unresolved);

      if (loop == NULL) {
        USR_FATAL_CONT(gs, "bad label '%s' on break or continue",
                       label->unresolved);
      }
    }

    if (loop == NULL) {
      // Handled above as needed. Nothing to do here.

    } else if (LoopStmt* loopS = toLoopStmt(loop)) {
      handleLoopStmtGoto(loopS, gs);

    } else if (ForallStmt* forall = toForallStmt(loop)) {
      handleForallGoto(forall, gs);

    } else {
      INT_ASSERT(false); // should not have any other loops here
    }
  }
}


/************************************* | *************************************/

static void updateMethod(UnresolvedSymExpr* usymExpr);

static void updateMethod(UnresolvedSymExpr* usymExpr,
                         Symbol*            sym);

static void updateMethod(UnresolvedSymExpr* usymExpr,
                         Symbol*            sym,
                         SymExpr*           symExpr);

static bool isFunctionNameWithExplicitScope(Expr* expr);

static void insertFieldAccess(FnSymbol*          method,
                              UnresolvedSymExpr* usymExpr,
                              Symbol*            sym,
                              Expr*              expr);

static int  computeNestedDepth(const char* name,
                               Type*       type);

static void resolveModuleCall(CallExpr* call);

static bool isMethodName(const char* name, Type* type);
static bool isMethodNameLocal(const char* name, Type* type);

static void checkIdInsideWithClause(Expr*              exprInAst,
                                    UnresolvedSymExpr* origUSE);

static void checkModuleSymExpr(SymExpr* se);

static void resolveUnresolvedSymExprs() {
  //
  // Translate M.x where M is a ModuleSymbol into just x where x is
  // the symbol in module M; for functions, insert a "module=" token
  // that is used to determine visible functions.
  //

  forv_Vec(CallExpr, call, gCallExprs) {
    resolveModuleCall(call);
  }

  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    resolveUnresolvedSymExpr(unresolvedSymExpr);
  }

}

void resolveUnresolvedSymExprs(BaseAST* inAst) {
   std::vector<BaseAST*> asts;
   collect_asts(inAst, asts);

   for_vector(BaseAST, ast, asts) {
     if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(ast)) {
       resolveUnresolvedSymExpr(urse);
     }
   }
}

static bool callSpecifiesClassKind(CallExpr* call) {
  return (call->isNamed("_to_borrowed") ||
          call->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
          call->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED) ||
          call->isNamed("_to_unmanaged") ||
          call->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
          call->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED) ||
          call->isNamed("_owned") ||
          call->isNamed("_shared") ||
          call->isNamed("Owned") ||
          call->isNamed("Shared") ||
          call->isNamed("chpl__distributed"));
}

static astlocT* resolveUnresolvedSymExpr(UnresolvedSymExpr* usymExpr,
                                         bool returnRename) {
  SET_LINENO(usymExpr);

  const char* name = usymExpr->unresolved;
  int nSymbols = 0;

  if (name == astrSdot || !usymExpr->inTree())
    return NULL;

  // Avoid duplicate work by not trying to resolve UnresolvedSymExprs that we've
  // already encountered an error when trying to resolve.
  for_vector(BaseAST, node, failedUSymExprs) {
    // Should only be expensive in case where we are already erroring out,
    // and we're already exiting compilation early in that case.
    if (node == usymExpr)
      return NULL;
  }

  astlocT* renameLoc = NULL;
  Symbol* sym = lookupAndCount(name, usymExpr, nSymbols, returnRename,
                               &renameLoc);
  if (sym != NULL) {
    resolveUnresolvedSymExpr(usymExpr, sym);
  } else {
    updateMethod(usymExpr);

#ifdef HAVE_LLVM
    if (nSymbols == 0 && gExternBlockStmts.size() > 0) {
      Symbol* got = tryCResolve(usymExpr, name);
      if (got != NULL)
        resolveUnresolvedSymExpr(usymExpr, got);
    }
#endif
  }
  return renameLoc;
}

// usymExpr is the UnresolveSymExpr and sym is what we
// have resolved it to with scope resolution.
static void resolveUnresolvedSymExpr(UnresolvedSymExpr* usymExpr,
                                     Symbol* sym) {
  FnSymbol* fn = toFnSymbol(sym);

  if (fn == NULL) {
    SymExpr* symExpr = NULL;

    // Adjust class type symbols for generic management / generic nilability
    if (isTypeSymbol(sym) && isClassLikeOrManaged(sym->type)) {
      if (isDecoratedClassType(sym->type)) {
        // Don't adjust already-decorated class types
      } else if (isManagedPtrType(sym->type)) {
        // e.g. 'owned' becomes 'owned with any nilability'
        AggregateType* at = toAggregateType(sym->type);
        INT_ASSERT(at);
        Type* t = at->getDecoratedClass(CLASS_TYPE_MANAGED);
        INT_ASSERT(t);
        sym = t->symbol;
      } else if (isClass(sym->type)) {
        // Make 'MyClass' mean generic-management.
        // Switch to the CLASS_TYPE_GENERIC_NONNIL decorated class type.
        ClassTypeDecorator d = CLASS_TYPE_GENERIC_NONNIL;
        Type* t = getDecoratedClass(sym->type, d);
        sym = t->symbol;
      }
    }

    symExpr = new SymExpr(sym);
    usymExpr->replace(symExpr);

    updateMethod(usymExpr, sym, symExpr);

    // Check for invalid uses of module symbols
    if (isModuleSymbol(sym)) {
      checkModuleSymExpr(symExpr);
    }

  // sjd: stopgap to avoid shadowing variables or functions by methods
  } else if (fn->isMethod() == true) {
    updateMethod(usymExpr);

  // handle function call without parentheses
  } else if (fn->hasFlag(FLAG_NO_PARENS) == true) {
    checkIdInsideWithClause(usymExpr, usymExpr);
    usymExpr->replace(new CallExpr(fn));

  } else if (Expr* parent = usymExpr->parentExpr) {
    CallExpr* call = toCallExpr(parent);

    if (call == NULL || call->baseExpr != usymExpr) {
      CallExpr* primFn = NULL;

      // Avoid duplicate wrapping with PRIM_CAPTURE_FN_*
      if (call != NULL && (call->isPrimitive(PRIM_CAPTURE_FN_FOR_C) ||
                           call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL)))
        return;

      // Wrap the FN in the appropriate way
      if (call != NULL && call->isNamed("c_ptrTo") == true) {
        primFn = new CallExpr(PRIM_CAPTURE_FN_FOR_C);
      } else {
        primFn = new CallExpr(PRIM_CAPTURE_FN_FOR_CHPL);
      }

      usymExpr->replace(primFn);

      primFn->insertAtTail(usymExpr);

    } else {
      updateMethod(usymExpr, sym);
    }

  } else {
    updateMethod(usymExpr, sym);
  }
}

// Apply 'this' and 'outer' in methods where necessary
static void updateMethod(UnresolvedSymExpr* usymExpr) {
  updateMethod(usymExpr, NULL);
}

static void updateMethod(UnresolvedSymExpr* usymExpr,
                         Symbol*            sym) {
  updateMethod(usymExpr, sym, NULL);
}

static void updateMethod(UnresolvedSymExpr* usymExpr,
                         Symbol*            sym,
                         SymExpr*           symExpr) {
  Expr*   expr   = (symExpr != NULL) ? (Expr*) symExpr : (Expr*) usymExpr;
  Symbol* parent = expr->parentSymbol;
  bool    isAggr = false;

  if (sym != NULL) {
    if (TypeSymbol* cts = toTypeSymbol(sym->defPoint->parentSymbol)) {
      isAggr = isAggregateType(canonicalClassType(cts->type));
    }
  }

  while (isModuleSymbol(parent) == false) {
    if (FnSymbol* method = toFnSymbol(parent)) {
      // stopgap bug fix: do not let methods shadow symbols
      // that are more specific than methods
      if (sym != NULL && sym->defPoint->getFunction() == method) {
        break;

      } else if (method->_this != NULL) {
        if (symExpr == NULL || symExpr->symbol() != method->_this) {
          const char* name = usymExpr->unresolved;
          Type*       type = method->_this->type;

          if (isAggr == true || isMethodName(name, type) == true) {
            if (isFunctionNameWithExplicitScope(expr) == false) {
              insertFieldAccess(method, usymExpr, sym, expr);
            }
          }

          break;
        }
      }
    }

    parent = parent->defPoint->parentSymbol;
  }
}

//
// Is <expr> one of
//        ModName.<expr>( ... );                 or
//        aggrType.<expr>( ... );                ?
//
// These will be have been converted to one of
//        <expr>(_module=, <mod>,  ...)          or
//        <expr>(_mt,      <this>, ...)
//

static bool isFunctionNameWithExplicitScope(Expr* expr) {
  bool retval = false;

  if (CallExpr* call = toCallExpr(expr->parentExpr)) {
    if (expr == call->baseExpr && call->numActuals() >= 2) {
      if (SymExpr* arg1 = toSymExpr(call->get(1))) {
        if (arg1->symbol() == gModuleToken ||
            arg1->symbol() == gMethodToken) {
          retval = true;
        }
      }
    }
  }

  return retval;
}

// Apply implicit this pointers and outer this pointers
static void insertFieldAccess(FnSymbol*          method,
                              UnresolvedSymExpr* usymExpr,
                              Symbol*            sym,
                              Expr*              expr) {
  const char* name      = usymExpr->unresolved;
  int         nestDepth = computeNestedDepth(name, method->_this->type);
  Expr*       dot       = new SymExpr(method->_this);

  checkIdInsideWithClause(expr, usymExpr);

  if (nestDepth > 0) {
    USR_FATAL_CONT("Illegal use of identifier '%s' from enclosing type", name);
  }

  if (isTypeSymbol(sym) == true) {
    AggregateType* at = toAggregateType(canonicalClassType(sym->type));
    if (at != NULL && at->hasInitializers()) {
      dot = new SymExpr(sym);
    } else {
      dot = new CallExpr(".", dot, sym);
    }
  } else {
    dot = new CallExpr(".", dot, new_CStringSymbol(name));
  }

  expr->replace(dot);
}

static int computeNestedDepth(const char* name, Type* type) {
  int retval = 0;

  if (isMethodName(name, type) == true) {
    AggregateType* ct = toAggregateType(type);

    // count how many classes out from current depth that
    // this method is first defined in
    while (ct != NULL && isMethodNameLocal(name, ct) == false) {
      retval = retval + 1;
      ct     = toAggregateType(ct->symbol->defPoint->parentSymbol->type);
    }

  } else {
    // count how many classes out from current depth that
    // this symbol is first defined in
    AggregateType* ct = toAggregateType(type);

    while (ct != NULL && ct->getField(name, false) == NULL) {
      retval = retval + 1;
      ct     = toAggregateType(ct->symbol->defPoint->parentSymbol->type);
    }
  }

  return retval;
}

//
// isMethodName returns true iff 'name' names a method of 'type'
//
static bool isMethodName(const char* name, Type* type) {
  bool retval = false;

  if (strcmp(name, type->symbol->name) == 0) {
    retval = false;

  } else {
    forv_Vec(Symbol, method, type->methods) {
      if (method != NULL && strcmp(name, method->name) == 0) {
        retval = true;
        break;
      }
    }

    if (retval == false) {
      if (AggregateType* at = toAggregateType(type)) {
        Type* outerType = at->symbol->defPoint->parentSymbol->type;

        forv_Vec(AggregateType, pt, at->dispatchParents) {
          if (isMethodName(name, pt) == true) {
            retval = true;
            break;
          }
        }

        if (retval == false) {
          if (AggregateType* outer = toAggregateType(outerType)) {
            if (isMethodName(name, outer) == true) {
              retval = true;
            }
          }
        }
      }
    }
  }

  return retval;
}


//
// isMethodNameLocal returns true iff 'name' names a method of 'type'
// excluding methods of an outer type
//
static bool isMethodNameLocal(const char* name, Type* type) {
  bool retval = false;

  if (strcmp(name, type->symbol->name) == 0) {
    retval = false;

  } else {
    forv_Vec(Symbol, method, type->methods) {
      if (method != NULL && strcmp(name, method->name) == 0) {
        retval = true;
        break;
      }
    }

    if (retval == false) {
      if (AggregateType* at = toAggregateType(type)) {
        forv_Vec(AggregateType, pt, at->dispatchParents) {
          if (isMethodName(name, pt) == true) {
            retval = true;
            break;
          }
        }
      }
    }
  }

  return retval;
}


static void errorDotInsideWithClause(UnresolvedSymExpr* origUSE,
                                     const char*        construct) {
  // As of this writing, a with-clause can be duplicated in the AST.
  // This code avoids multiple error messages for the same symbol.

  std::pair<const char*, int> markLoc(origUSE->astloc.filename,
                                      origUSE->astloc.lineno);

  WFDIWmark                   mark(markLoc, origUSE->unresolved);

  if (warnedForDotInsideWith.count(mark) == 0) {
    USR_FATAL_CONT(origUSE,
                   "cannot reference a field or function '%s' "
                   "in a with-clause of this %s",
                   origUSE->unresolved,
                   construct);

    warnedForDotInsideWith.insert(mark);
  }
}

//
// 'expr' ended up being a field reference (or perhaps a method call).
// If we are inside a 'with' clause, report an error.
//
static void checkIdInsideWithClause(Expr*              exprInAst,
                                    UnresolvedSymExpr* origUSE) {
  // A 'with' clause for a task construct.
  if (Expr* parent1 = exprInAst->parentExpr) {
    if (BlockStmt* parent2 = toBlockStmt(parent1->parentExpr)) {
      if (parent1 == parent2->byrefVars) {
        CallExpr* blockInfo = parent2->blockInfoGet();

        // Ensure that an issue, indeed, occurred a task construct.
        INT_ASSERT(blockInfo->isPrimitive(PRIM_BLOCK_COBEGIN)  ||
                   blockInfo->isPrimitive(PRIM_BLOCK_COFORALL) ||
                   blockInfo->isPrimitive(PRIM_BLOCK_BEGIN));

        errorDotInsideWithClause(origUSE, blockInfo->primitive->name);
      }
    }
  }
}

// se refers to a ModuleSymbol. Check it is a valid mention of a module.
static void checkModuleSymExpr(SymExpr* se) {
  bool validModuleMention = false;

  ModuleSymbol* mod = toModuleSymbol(se->symbol());
  INT_ASSERT(mod != NULL); // caller should ensure this

  CallExpr* call = toCallExpr(se->parentExpr);
  if (call != NULL) {
    // check for (Call gModuleToken, ModuleName, Args)
    if (SymExpr* prevSe = toSymExpr(se->prev))
      if (prevSe->symbol() == gModuleToken)
        validModuleMention = true;
    // check for (Call (Call . ModuleName FnName) Args)
    if (call->isNamedAstr(astrSdot))
      validModuleMention = true;
    // check for PRIM_REFERENCED_MODULES_LIST
    if (call->isPrimitive(PRIM_REFERENCED_MODULES_LIST))
      validModuleMention = true;
  }
  if (Expr* stmt = se->getStmtExpr())
    if (isUseStmt(stmt) || isImportStmt(stmt))
      validModuleMention = true;

  if (validModuleMention == false) {
    bool callOfModule = false;
    if (call != NULL)
      if (SymExpr* baseSe = toSymExpr(call->baseExpr))
        if (baseSe->symbol() == mod)
          callOfModule = true;

    const char* reason = NULL;
    if (callOfModule)
      reason = "cannot be called like procedures";
    else
      reason = "cannot be mentioned like variables";

    USR_FATAL_CONT(se, "modules (like '%s' here) %s", mod->name, reason);
  }
}

static bool hasOuterVariable(ShadowVarSymbol* svar) {
  switch (svar->intent) {
    case TFI_DEFAULT:
    case TFI_CONST:
    case TFI_IN:
    case TFI_CONST_IN:
    case TFI_REF:
    case TFI_CONST_REF:
    case TFI_REDUCE:        return true;

    case TFI_TASK_PRIVATE:  return false;

    case TFI_IN_PARENT:         // these have not been created yet
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:  INT_ASSERT(false); return false;
  }
  INT_FATAL(svar, "garbage intent");
  return false;  //dummy
}

static bool isField(Symbol* sym) {
  // Copied from insertWideReferences.cpp.
  // Alas Symbol::isField is private.
  return isTypeSymbol(sym->defPoint->parentSymbol);
}

static void setupOuterVar(ForallStmt* fs, ShadowVarSymbol* svar) {
  // We pull in the relevant pieces of resolveUnresolvedSymExpr().
  // This is hopefully clearer than generating an UnresolvedSymExpr
  // and calling resolveUnresolvedSymExpr() on it.

  SET_LINENO(svar);

  if (svar->outerVarSE != NULL) {
    // This happens for reduce expressions. Nothing to do.
    INT_ASSERT(svar->intent == TFI_REDUCE);
    return;
  }

  if (Symbol* ovar = lookup(svar->name, fs->parentExpr)) {
    if (isFnSymbol(ovar) || isField(ovar)) {
      // Create a stand-in to use pre-existing code.
      UnresolvedSymExpr* standIn = new UnresolvedSymExpr(svar->name);
      errorDotInsideWithClause(standIn, "forall loop");
    }
    else {
      // The desired case.
      svar->outerVarSE = new SymExpr(ovar);
      insert_help(svar->outerVarSE, NULL, svar);
      checkTypeParamTaskIntent(svar->outerVarSE);
    }
  } else {
    USR_FATAL_CONT(svar,
                   "could not find the outer variable for '%s'", svar->name);
  }
}

// Issue an error if 'tpv' is one of fs's induction variables.
static void checkRefsToIdxVars(ForallStmt* fs, DefExpr* def,
                               ShadowVarSymbol* tpv)
{
  std::vector<SymExpr*> symExprs;
  if (def->init)     collectSymExprs(def->init, symExprs);
  if (def->exprType) collectSymExprs(def->exprType, symExprs);

  // Ensure we do not need to check IB/DB.
  INT_ASSERT(tpv->initBlock()->body.empty());
  INT_ASSERT(tpv->deinitBlock()->body.empty());

  for_vector(SymExpr, se, symExprs)
    if (se->symbol()->defPoint->list == &fs->inductionVariables())
      USR_FATAL_CONT(se, "the initialization or type expression"
                     " of the task-private variable '%s'"
                     " references the forall loop induction variable '%s'",
                     tpv->name, se->symbol()->name);
}

static void setupShadowVars() {
  forv_Vec(ForallStmt, fs, gForallStmts)
    for_shadow_vars_and_defs(svar, def, temp, fs) {
      if (hasOuterVariable(svar))
        setupOuterVar(fs, svar);
      if (svar->isTaskPrivate())
        checkRefsToIdxVars(fs, def, svar);
    }

  // Instead of the two nested loops above, we could march through
  // gShadowVarSymbols and invoke setupOuterVar(svar->parentExpr, svar).
  // The nested loops group together all shadow variables of a given
  // ForallStmt, so hopefully have better cache behavior.

  USR_STOP();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static CallExpr* resolveModuleGetNewExpr(CallExpr* call, Symbol* sym);

// Track modules that we name explicitly in the scope where we do so
static void storeReferencedMod(ModuleSymbol* mod, BaseAST* cur) {
  if (cur == NULL) {
    return;
  }
  BaseAST* scope = getScope(cur);
  if (BlockStmt* block = toBlockStmt(scope)) {
    block->modRefsAdd(mod);
  } else {
    storeReferencedMod(mod, scope);
  }
}

static void resolveModuleCall(CallExpr* call) {
  if (call->isNamedAstr(astrSdot) == true) {
    astlocT* renameLoc = NULL;
    UnresolvedSymExpr* uSE = toUnresolvedSymExpr(call->get(1));
    if (uSE != NULL) {
      renameLoc = resolveUnresolvedSymExpr(uSE, true);
    }

    // Now that we've potentially resolved the unresolved sym expr, check if
    // it's been replaced with a sym expr (or if it was one to start), and
    // operate within it if so.
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        SET_LINENO(call);

        ModuleSymbol* currModule = call->getModule();
        ResolveScope* scope      = ResolveScope::getScopeFor(mod->block);
        const char*   mbrName    = get_string(call->get(2));

        currModule->moduleUseAdd(mod);

        // Track modules that we name explicitly in the scope where we do so
        storeReferencedMod(mod, call);

        // First, try regular scope resolution
        Symbol* sym = scope->lookupNameLocally(mbrName);

        // Then, try public import statements in the module
        if (!sym) {
          sym = scope->lookupPublicVisStmts(mbrName);
        }

        // Then, try public import statements that enables unqualified access
        if (!sym) {
          sym = scope->lookupPublicUnqualAccessSyms(mbrName, call);
        }

        // Adjust class types to undecorated
        if (sym && isClass(sym->type)) {
          // Switch to the CLASS_TYPE_GENERIC_NONNIL decorated class type.
          ClassTypeDecorator d = CLASS_TYPE_GENERIC_NONNIL;
          Type* t = getDecoratedClass(sym->type, d);
          sym = t->symbol;
        }

        // Failing that, try looking in an extern block.
#ifdef HAVE_LLVM
        if (sym == NULL && gExternBlockStmts.size() > 0) {
          sym = tryCResolveLocally(mod, mbrName);
        }
#endif

        if (sym != NULL) {
          if (sym->isVisible(call) == true) {
            if (FnSymbol* fn = toFnSymbol(sym)) {
              if (fn->_this == NULL && fn->hasFlag(FLAG_NO_PARENS) == true) {
                call->replace(new CallExpr(fn));

              } else {
                CallExpr* parent = toCallExpr(call->parentExpr);

                call->replace(new UnresolvedSymExpr(mbrName));

                parent->insertAtHead(mod);
                parent->insertAtHead(gModuleToken);
              }

            } else if (CallExpr* c = resolveModuleGetNewExpr(call, sym)) {
              call->replace(new SymExpr(sym));

              c->insertAtHead(mod);
              c->insertAtHead(gModuleToken);

            } else {
              call->replace(new SymExpr(sym));
            }

          } else {
            if (!uSE || uSE->unresolved == mod->name) {
              USR_FATAL(call,
                        "Cannot access '%s', '%s' is private to '%s'",
                        mbrName,
                        mbrName,
                        mod->name);

            } else {
              USR_FATAL_CONT(call,
                             "Cannot access '%s', '%s' is private to '%s'",
                             mbrName,
                             mbrName,
                             uSE->unresolved);
              USR_PRINT("module '%s' was renamed from '%s' at %s:%d",
                        uSE->unresolved, mod->name, renameLoc->filename,
                        renameLoc->lineno);

            }
          }

        } else {
          // we didn't get a symbol, but it may be due to multiply defined
          // symbols. To check that see if this call wasn't added to
          // failedUSymExprs
          if (std::count(failedUSymExprs.begin(),
                         failedUSymExprs.end(),
                         call) == 0) {
            if (!uSE || uSE->unresolved == mod->name) {
              USR_FATAL_CONT(call,
                             "Symbol '%s' undeclared in module '%s'",
                             mbrName,
                             mod->name);

            } else {
              USR_FATAL_CONT(call,
                             "Symbol '%s' undeclared in module '%s'",
                             mbrName,
                             uSE->unresolved);
              USR_PRINT("module '%s' was renamed from '%s' at %s:%d",
                        uSE->unresolved, mod->name, renameLoc->filename,
                        renameLoc->lineno);
            }
          }
        }
      }
    }
  }
}

// Returns the CallExpr that should get a module= argument
// for new SomeModule.SomeType.
static CallExpr* resolveModuleGetNewExpr(CallExpr* call, Symbol* sym) {
  if (TypeSymbol* ts = toTypeSymbol(sym)) {
    if (isAggregateType(canonicalClassType(ts->type))) {
      if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
        if (CallExpr* grandParentCall = toCallExpr(parentCall->parentExpr)) {
          if (grandParentCall->isPrimitive(PRIM_NEW)) {
            return parentCall;
          } else if(callSpecifiesClassKind(grandParentCall)) {
            if (CallExpr* outerCall = toCallExpr(grandParentCall->parentExpr)) {
              if (outerCall->isPrimitive(PRIM_NEW))
                return parentCall;
            }
          }
        }
      }
    }
  }

  return NULL;
}

/************************************* | **************************************
*                                                                             *
* resolves EnumTypeName.fieldName to the symbol named fieldName in the        *
* enumerated type named EnumTypeName                                          *
*                                                                             *
************************************** | *************************************/

static void resolveEnumeratedTypes() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isNamedAstr(astrSdot)) {
      SET_LINENO(call);

      if (SymExpr* first = toSymExpr(call->get(1))) {
        if (EnumType* type = toEnumType(first->symbol()->type)) {
          if (SymExpr* second = toSymExpr(call->get(2))) {
            const char* name;

            INT_ASSERT(get_string(second, &name));

            for_enums(constant, type) {
              if (!strcmp(constant->sym->name, name)) {
                call->replace(new SymExpr(constant->sym));
              }
            }
            // Unresolved enum symbols are now either resolved or throw an error
            // during the function resolution pass. Permits paren-less methods.
          }
        }
      }
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// Convert each "proc type C.myProc() ..." to, roughly:
// "proc type any.myProc() where isSubtype(this.type, C) ..."
//
static void adjustTypeMethodsOnClasses() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->thisTag != INTENT_TYPE) continue; // handle only type methods

    ArgSymbol* thisArg = toArgSymbol(fn->_this);
    Type*      thisType = thisArg->type;
    if (! isClass(thisType)) continue; // handle only undecorated classes

    if (BlockStmt* typeBlock = thisArg->typeExpr) {
      // Remove the type block, ensuring that its information is preserved.
      SymExpr* typeSE = toSymExpr(typeBlock->body.only());
      INT_ASSERT(thisType->symbol == typeSE->symbol());
      typeBlock->remove();
    }

    // Update the type of 'this'.
    thisArg->type = getDecoratedClass(thisType, CLASS_TYPE_GENERIC);
  }
}


void destroyModuleUsesCaches() {
  std::map<BlockStmt*, Vec<VisibilityStmt*>*>::iterator use;

  for (use = moduleUsesCache.begin(); use != moduleUsesCache.end(); use++) {
    delete use->second;
  }

  moduleUsesCache.clear();
}


static void renameDefaultType(Type* type, const char* newname);

static void renameDefaultTypesToReflectWidths(void) {
  renameDefaultType(dtInt[INT_SIZE_DEFAULT],         "int(64)");
  renameDefaultType(dtUInt[INT_SIZE_DEFAULT],        "uint(64)");
  renameDefaultType(dtReal[FLOAT_SIZE_DEFAULT],      "real(64)");
  renameDefaultType(dtImag[FLOAT_SIZE_DEFAULT],      "imag(64)");
  renameDefaultType(dtComplex[COMPLEX_SIZE_DEFAULT], "complex(128)");
}

static void renameDefaultType(Type* type, const char* newname) {
  if (strchr(type->symbol->name, '(') != NULL) {
    INT_FATAL("Renaming a default type that already seems to have a width");
  }

  type->symbol->name = astr(newname);
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void lookup(const char*           name,
                   BaseAST*              context,

                   BaseAST*              scope,
                   Vec<BaseAST*>&        visited,

                   std::vector<Symbol*>& symbols,
                   std::map<Symbol*, astlocT*>& renameLocs,
                   bool storeRenames);

// Show what symbols from 'symbols' conflict with the given 'sym'.
static void printConflictingSymbols(std::vector<Symbol*>& symbols, Symbol* sym,
                                    const char* nameUsed,
                                    bool storeRenames,
                                    std::map<Symbol*, astlocT*> renameLocs)
{
  Symbol* sampleFunction = NULL;
  for_vector(Symbol, another, symbols) if (another != sym)
  {
    if (isFnSymbol(another))
      sampleFunction = another;
    else {
      astlocT* renameLoc = renameLocs[another];
      if (storeRenames && renameLoc != NULL) {
        USR_PRINT(another,
                  "symbol '%s', defined here, was renamed to '%s' at %s:%d",
                  another->name, nameUsed, renameLoc->filename,
                  renameLoc->lineno);
      } else {
        USR_PRINT(another, "also defined here");
      }
    }
  }

  if (sampleFunction)
    USR_PRINT(sampleFunction,
              "also defined as a function here (and possibly elsewhere)");
}

void checkConflictingSymbols(std::vector<Symbol *>& symbols,
                             const char* name,
                             BaseAST* context,
                             bool storeRenames,
                             std::map<Symbol*, astlocT*>& renameLocs) {

  // If they're all functions
  //   then      assume function resolution will be applied
  //   otherwise fail
  for_vector(Symbol, sym, symbols) {
    if (!isFnSymbol(sym)) {
      if (std::count(failedUSymExprs.begin(),
                     failedUSymExprs.end(),
                     context) == 0) {
        failedUSymExprs.push_back(context);
        astlocT* symRenameLoc = renameLocs[sym];
        USR_FATAL_CONT(sym, "symbol %s is multiply defined", name);
        if (storeRenames && symRenameLoc != NULL) {
          USR_PRINT("'%s' was renamed to '%s' at %s:%d", sym->name,
                    name, symRenameLoc->filename, symRenameLoc->lineno);
        }
        printConflictingSymbols(symbols, sym, name, storeRenames, renameLocs);
        break;
      }
    }
  }
}

// Given a name and a calling context, determine the symbol referred to
// by that name in the context of that call
Symbol* lookupAndCount(const char*           name,
                       BaseAST*              context,
                       int&                  nSymbolsFound,
                       bool storeRenames,
                       astlocT** renameLoc) {

  std::vector<Symbol*> symbols;
  std::map<Symbol*, astlocT*> renameLocs;
  Symbol*              retval = NULL;

  lookup(name, context, symbols, renameLocs, storeRenames);

  nSymbolsFound = symbols.size();

  if (symbols.size() == 0) {
    retval = NULL;

  } else if (symbols.size() == 1) {
    retval = symbols[0];
    if (storeRenames) {
      if (astlocT* retvalRenameLoc = renameLocs[retval]) {
        *renameLoc = retvalRenameLoc;
      }
    }

  } else {
    // Multiple symbols found for this name.
    if (renameLocs.size() > 0) {
      // this can be the case when we resolved an urse through a public import
      // that renames the symbol
      checkConflictingSymbols(symbols, name, context, true, renameLocs);
    }
    else {
      checkConflictingSymbols(symbols, name, context, storeRenames, renameLocs);
    }
    retval = NULL;
  }

  return retval;
}

Symbol* lookup(const char* name, BaseAST* context) {
  int nSymbolsFound = 0;
  return lookupAndCount(name, context, nSymbolsFound);
}

void lookup(const char*           name,
            BaseAST*              context,
            std::vector<Symbol*>& symbols,
            std::map<Symbol*, astlocT*>& renameLocs,
            bool storeRenames) {
  Vec<BaseAST*> visited;

  lookup(name, context, context, visited, symbols, renameLocs, storeRenames);
}

static void lookup(const char*           name,
                   BaseAST*              context,

                   BaseAST*              scope,
                   Vec<BaseAST*>&        visited,

                   std::vector<Symbol*>& symbols,
                   std::map<Symbol*, astlocT*>& renameLocs,
                   bool storeRenames) {

  if (!visited.set_in(scope)) {
    visited.set_add(scope);

    if (lookupThisScopeAndUses(name, context, scope, symbols, renameLocs,
                               storeRenames) == true) {
      // We've found an instance here.
      // Lydia note: in the access call case, we'd want to look in our
      // surrounding scopes for the symbols on the left and right part
      // of the call (if any) to verify we were finding anything in particular.
      //
      // A symbol could be visible in the innermost scope because it was
      // defined in an outer scope (for instance, if M1 defines foo,
      // M2 doesn't shadow it and we're looking for M1.M2.foo),
      // so that is something to keep in mind as well.

      return;
    }

    if (scope->getModule()->block == scope) {
      BaseAST* outerScope = getScope(scope);
      if (outerScope != NULL) {
        // if this is a top-level module, keep searching upwards to get
        // the built-in stuff defined in the root/program modules
        if (outerScope->getModule() == rootModule ||
            outerScope->getModule() == theProgram) {
          lookup(name, context, outerScope, visited, symbols, renameLocs,
                 storeRenames);
        } else {
          // if it's a nested module, don't look into the parent
          // module (a 'use' or 'import' is required to do that), but
          // do see if ChapelStandard or theProgram resolve things for
          // us that are not yet resolved.
          lookup(name, context, standardModule->block, visited, symbols,
                 renameLocs, storeRenames);
          if (symbols.size() == 0) {
            
            lookup(name, context, theProgram->block, visited, symbols,
                   renameLocs, storeRenames);
          }
        }
        // As a last ditch effort, see if this module's name happens to match.
        // This handles the case when we refer to the name of the module in
        // which we are declared (e.g., `module M { ...M.xyz... }`
        if (symbols.size() == 0) {
          ModuleSymbol* thisMod = scope->getModule();
          if (strcmp(name, thisMod->name) == 0) {
            symbols.push_back(thisMod);
          }
        }
      }

    } else {
      // Otherwise, look in the next scope up.
      FnSymbol* fn = toFnSymbol(scope);

      if (fn != NULL && fn->_this) {
        // If currently in a method, the next scope up is anything visible
        // within the aggregate type
        if (AggregateType* ct =
            toAggregateType(canonicalClassType(fn->_this->type))) {
          lookup(name, context, ct->symbol, visited, symbols, renameLocs,
                 storeRenames);
        }
      }

      // Check if found something in last lookup call
      if (symbols.size() == 0) {
        // If we didn't find something in the aggregate type that matched,
        // or we weren't in an aggregate type method, so look at next scope up.
        lookup(name, context, getScope(scope), visited, symbols, renameLocs,
               storeRenames);
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool      isRepeat(Symbol* toAdd, const std::vector<Symbol*>& symbols);

static Symbol*   inSymbolTable(const char* name, BaseAST* scope);

static Symbol*   inType(const char* name, BaseAST* scope);

static bool      methodMatched(BaseAST* scope, FnSymbol* method);

static FnSymbol* getMethod(const char* name, Type* type);

static void      buildBreadthFirstModuleList(Vec<VisibilityStmt*>* modules);

static void      buildBreadthFirstModuleList(
            Vec<VisibilityStmt*>*                             modules,
            Vec<VisibilityStmt*>*                             current,
            std::map<Symbol*, std::vector<VisibilityStmt*> >* alreadySeen);

static bool      skipUse(std::map<Symbol*, std::vector<VisibilityStmt*> >* seen,
                         UseStmt* current);
static bool      skipUse(std::map<Symbol*, std::vector<VisibilityStmt*> >* seen,
                         ImportStmt* current);

static bool lookupThisScopeAndUses(const char*           name,
                                   BaseAST*              context,
                                   BaseAST*              scope,
                                   std::vector<Symbol*>& symbols,
                                   std::map<Symbol*, astlocT*>& renameLocs,
                                   bool storeRenames) {
  if (Symbol* sym = inSymbolTable(name, scope)) {
    if (sym->hasFlag(FLAG_PRIVATE) == true) {
      if (sym->isVisible(context) == true) {
        symbols.push_back(sym);
      }

    } else {
      symbols.push_back(sym);
    }
  }

  if (Symbol* sym = inType(name, scope)) {
    if (isRepeat(sym, symbols) == true) {
      // If we're looking at the exact same Symbol, there's no need to add it
      // and we can just return.
      return true;
    }

    // When methods and fields can be private, need to check against the
    // rejected private symbols here.  But that's in the future.
    symbols.push_back(sym);
  }

  if (symbols.size() == 0) {
    // Nothing found so far, look into the uses.
    if (BlockStmt* block = toBlockStmt(scope)) {
      if (block->useList != NULL) {
        Vec<VisibilityStmt*>* moduleUses = NULL;

        if (moduleUsesCache.count(block) == 0) {
          moduleUses = new Vec<VisibilityStmt*>();

          for_actuals(expr, block->useList) {
            // Ensure we only have use or import statements in this list
            if (UseStmt* use = toUseStmt(expr)) {
              moduleUses->add(use);
            } else if (ImportStmt* import = toImportStmt(expr)) {
              moduleUses->add(import);
            } else {
              INT_FATAL("Bad contents for useList, expected use or import");
            }
          }

          INT_ASSERT(moduleUses->n);

          buildBreadthFirstModuleList(moduleUses);

          if (enableModuleUsesCache)
            moduleUsesCache[block] = moduleUses;
        } else {
          moduleUses = moduleUsesCache[block];
        }

        forv_Vec(Stmt, stmt, *moduleUses) {
          if (UseStmt* use = toUseStmt(stmt)) {
            if (use->skipSymbolSearch(name) == false) {
              const char* nameToUse = use->isARenamedSym(name) ?
                use->getRenamedSym(name) : name;
              BaseAST* scopeToUse = use->getSearchScope();

              Symbol* sym = inSymbolTable(nameToUse, scopeToUse);
              if (!sym && use->canReexport) {
                if (ResolveScope* rs = ResolveScope::getScopeFor(scopeToUse)) {
                  sym = rs->lookupPublicUnqualAccessSyms(nameToUse, context,
                                                         renameLocs);
                  // propagate this information to the UseStmt
                  if (!rs->canReexport) {
                    use->canReexport = false;
                  }
                }
              }
              if (sym) {
                if (sym->hasFlag(FLAG_PRIVATE) == true) {
                  if (sym->isVisible(context) == true &&
                      isRepeat(sym, symbols)  == false) {
                    symbols.push_back(sym);
                    if (storeRenames && use->isARenamedSym(name)) {
                      renameLocs[sym] = &use->astloc;
                    }
                  }

                } else if (isRepeat(sym, symbols) == false) {
                  symbols.push_back(sym);
                  if (storeRenames && use->isARenamedSym(name)) {
                    renameLocs[sym] = &use->astloc;
                  }
                }
              }
            }
          } else if (ImportStmt* import = toImportStmt(stmt)) {
            // Only traverse import statements that define a symbol with this
            // name for unqualified access.  We're only looking for explicitly
            // named symbols
            if (import->skipSymbolSearch(name) == false) {
              const char* nameToUse = import->isARenamedSym(name) ?
                import->getRenamedSym(name) : name;
              BaseAST* scopeToUse = import->getSearchScope();
              if (Symbol* sym = inSymbolTable(nameToUse, scopeToUse)) {
                if (sym->hasFlag(FLAG_PRIVATE) == true) {
                  if (sym->isVisible(context) == true &&
                      isRepeat(sym, symbols)  == false) {
                    symbols.push_back(sym);
                    if (storeRenames && import->isARenamedSym(name)) {
                      renameLocs[sym] = &import->astloc;
                    }
                  }
                } else if (isRepeat(sym, symbols) == false) {
                  symbols.push_back(sym);
                  if (storeRenames && import->isARenamedSym(name)) {
                    renameLocs[sym] = &import->astloc;
                  }
                }
              }
            }
          } else {
            // break on each new depth if a symbol has been found
            if (symbols.size() > 0) {
              break;
            }
          }
        }

        if (symbols.size() > 0) {
          // We found a symbol in the module use.  This could conflict with
          // the function symbol's arguments if we are at the top level scope
          // within a function.  Note that we'd check the next scope up if
          // size() == 0, so we only need to do this check here because the
          // module case would hide it otherwise
          if (FnSymbol* fn = toFnSymbol(getScope(block))) {
            // The next scope up from the block statement is a function
            // symbol. That means that we need to check the arguments
            if (Symbol* sym = inSymbolTable(name, fn)) {
              // We found it in the arguments.  This should cause a conflict,
              // because it is probably an error that the user had the same
              // name as a module level variable.
              USR_WARN(sym,
                       "Module level symbol is hiding function argument '%s'",
                       name);
            }
          }
        } else {
          // we haven't found a match yet, so as a last resort, let's
          // check the names of the modules in the 'use'/'import' statements
          // themselves...  This effectively places the module names at
          // a scope just a bit further out than the one holding the
          // symbols that they define.
          forv_Vec(VisibilityStmt, stmt, *moduleUses) {
            if (stmt != NULL) {
              if (!isImportStmt(stmt) ||
                  toImportStmt(stmt)->providesQualifiedAccess()) {
                if (Symbol* modSym = stmt->checkIfModuleNameMatches(name)) {
                  if (isRepeat(modSym, symbols) == false) {
                    symbols.push_back(modSym);
                    if (storeRenames && stmt->isARename()) {
                      renameLocs[modSym] = &stmt->astloc;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return symbols.size() != 0;
}

// Returns true if the symbol is present in the vector, false otherwise
static bool isRepeat(Symbol* toAdd, const std::vector<Symbol*>& symbols) {
  for (std::vector<Symbol* >::const_iterator it = symbols.begin();
       it != symbols.end();
       ++it) {
    if (*it == toAdd) {
      return true;
    }
  }

  return false;
}

// Is this name defined in this scope?
static Symbol* inSymbolTable(const char* name, BaseAST* ast) {
  Symbol* retval = NULL;

  if (ResolveScope* scope = ResolveScope::getScopeFor(ast)) {
    if (Symbol* sym = scope->lookupNameLocally(name)) {
      if (FnSymbol* fn = toFnSymbol(sym)) {
        if (fn->isMethod() == false || methodMatched(ast, fn) == true) {
          retval = sym;
        }

      } else {
        retval = sym;
      }
    }
  }

  return retval;
}

static Symbol* inType(const char* name, BaseAST* scope) {
  Symbol* retval = NULL;

  if (TypeSymbol* ts = toTypeSymbol(scope)) {
    if (AggregateType* ct = toAggregateType(canonicalClassType(ts->type))) {
      if (Symbol* sym = ct->getField(name, false)) {
        retval = sym;

      } else if (Symbol* fn = getMethod(name, ct)) {
        if (methodMatched(scope, toFnSymbol(fn)) == true) {
          retval = fn;
        }
      }
    }
  }

  return retval;
}

// For a scope and a given method, determine if the method is visible in this
// scope
// Lydia note (2015/06/26)
// Semantic issue not handled by this function: when a parenthesis-less method
// is defined by an outside module and the use of that module is at the same
// scope as another symbol of the same name as the method, which symbol should
// take precedent?  When the use is not present, both the prior version of
// scopeResolve and the current version don't resolve the symbol, leaving the
// decision to function resolution, which thinks it should have gotten the
// method that wasn't available
static bool methodMatched(BaseAST* scope, FnSymbol* method) {
  bool retval = true;

  if (method->_this->type->symbol == scope) {
    retval = true;

  } else {
    BaseAST* curScope = getScope(scope);

    // Traverse up the scopes either until we find this method or until there
    // are no more scopes to traverse
    while (curScope) {
      if (TypeSymbol* ts = toTypeSymbol(scope)) {
        // Are we in a type symbol?
        if (Symbol* sym = getMethod(method->name, ts->type)) {
          // Does that type symbol have a method with the same name?
          if (sym == method) {
            // Is it us?
            return true;
          } else {
            // We are not in scope
            return false;
          }
        }
      }
      curScope = getScope(curScope);
    }

    retval = false;
  }

  return retval;
}

// Determines and obtains a method by the given name on the given type
//
// This function uses the same methodology as isMethodName but returns the
// symbol found instead of just a boolean
static FnSymbol* getMethod(const char* name, Type* type) {
  FnSymbol* retval = NULL;

  if (strcmp(name, type->symbol->name) == 0) {
    retval = NULL;

  } else {
    // Looks for name in methods defined directly on this type
    forv_Vec(FnSymbol, method, type->methods) {
      if (method != NULL && strcmp(name, method->name) == 0) {
        retval = method;
      }
    }

    if (retval == NULL) {
      if (AggregateType* at = toAggregateType(type)) {
        Type* outerType = at->symbol->defPoint->parentSymbol->type;

        // Looks for name in methods defined on parent types
        forv_Vec(AggregateType, pt, at->dispatchParents) {
          if (FnSymbol* sym = getMethod(name, pt)) {
            retval = sym;
            break;
          }
        }

        // Looks for name in types wrapping this type definition
        if (retval == NULL) {
          if (AggregateType* outer = toAggregateType(outerType)) {
            if (FnSymbol* sym = getMethod(name, outer)) {
              retval = sym;
            }
          }
        }
      }
    }
  }

  return retval;
}

static void buildBreadthFirstModuleList(Vec<VisibilityStmt*>* modules) {
  std::map<Symbol*, std::vector<VisibilityStmt* > > seen;

  return buildBreadthFirstModuleList(modules, modules, &seen);
}

// If the uses of a particular module are considered its level 1 uses, then
// this function will only add level 2 and lower uses to the modules vector
// argument.
static void buildBreadthFirstModuleList(
               Vec<VisibilityStmt*>*                             modules,
               Vec<VisibilityStmt*>*                             current,
               std::map<Symbol*, std::vector<VisibilityStmt*> >* alreadySeen) {
 // use NULL as a sentinel to identify modules of equal depth
  modules->add(NULL);

  Vec<VisibilityStmt*> next;

  forv_Vec(VisibilityStmt, source, *current) {
    if (!source) {
      break;
    } else {
      if (UseStmt* srcUse = toUseStmt(source)) {
        SymExpr* se = toSymExpr(srcUse->src);
        INT_ASSERT(se);
        if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
          if (mod->block->useList != NULL) {
            for_actuals(expr, mod->block->useList) {
              if (UseStmt* use = toUseStmt(expr)) {
                SymExpr* useSE = toSymExpr(use->src);
                INT_ASSERT(useSE);

                UseStmt* useToAdd = NULL;
                if (!use->isPrivate &&
                    !useSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                  // Uses of private modules are not transitive - the symbols
                  // in the private modules are only visible to itself and its
                  // immediate parent.  Therefore, if the symbol is private,
                  // we will not traverse it further and will merely add it to
                  // the alreadySeen map.
                  useToAdd = use->applyOuterUse(srcUse);

                  if (useToAdd                       != NULL &&
                      skipUse(alreadySeen, useToAdd) == false) {
                    next.add(useToAdd);
                    modules->add(useToAdd);
                  }

                  // if applyOuterUse returned NULL, the number of symbols
                  // that could be provided from this use was 0, so it didn't
                  // need to be added to the alreadySeen map.
                  if (useToAdd != NULL) {
                    (*alreadySeen)[useSE->symbol()].push_back(useToAdd);
                  }

                } else if (!use->isPrivate &&
                           useSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                  // Private uses should be skipped, but should not prevent us
                  // from traversing the module in a later use of it, if that
                  // later use is not private.
                  (*alreadySeen)[useSE->symbol()].push_back(use);
                }
              } else if (ImportStmt* import = toImportStmt(expr)) {
                SymExpr* importSE = toSymExpr(import->src);
                INT_ASSERT(importSE);

                if (!import->isPrivate &&
                    !importSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                  ImportStmt* importToAdd = import->applyOuterUse(srcUse);
                  // Imports of private modules are not transitive - the
                  // symbols in the private modules are only visible to itself
                  // and its immediate parent.  Therefore, if the symbol is
                  // private, we will not traverse it further and will merely
                  // add it to the alreadySeen map.
                  if (importToAdd != NULL &&
                      skipUse(alreadySeen, importToAdd) == false) {
                    next.add(importToAdd);
                    modules->add(importToAdd);
                  }

                  if (importToAdd != NULL) {
                    (*alreadySeen)[importSE->symbol()].push_back(importToAdd);
                  }
                } else if (!import->isPrivate &&
                           importSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                  // If we're skipping because the import was public, but the
                  // module was private, then we shouldn't look at the module
                  // again and should add it to the alreadySeen map.  Otherwise
                  // there might be a later import or use that is public, so
                  // we should allow it to be found
                  (*alreadySeen)[importSE->symbol()].push_back(import);
                }
              } else {
                INT_ASSERT("Bad use list, expected UseStmt or ImportStmt");
              }
            }
          }
        }
      } else if (ImportStmt* srcImport = toImportStmt(source)) {
        // Don't traverse the use statements of a module we imported for
        // qualified access, their contents aren't brought into scope.
        SymExpr* se = toSymExpr(srcImport->src);
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->symbol());
        INT_ASSERT(mod);
        if (mod->block->useList != NULL) {
          for_actuals(expr, mod->block->useList) {
            if (UseStmt* use = toUseStmt(expr)) {
              SymExpr* useSE = toSymExpr(use->src);
              INT_ASSERT(useSE);

              ImportStmt* importToAdd = NULL;
              if (!use->isPrivate &&
                  !useSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                // Uses of private modules are not transitive - the symbols
                // in the private modules are only visible to itself and its
                // immediate parent.  Therefore, if the symbol is private,
                // we will not traverse it further and will merely add it to
                // the alreadySeen map.
                importToAdd = use->applyOuterImport(srcImport);

                if (importToAdd                       != NULL &&
                    skipUse(alreadySeen, importToAdd) == false) {
                  next.add(importToAdd);
                  modules->add(importToAdd);
                }

                // if applyOuterUse returned NULL, the number of symbols
                // that could be provided from this use was 0, so it didn't
                // need to be added to the alreadySeen map.
                if (importToAdd != NULL) {
                  (*alreadySeen)[useSE->symbol()].push_back(importToAdd);
                }

              } else if (!use->isPrivate &&
                         useSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                // Private uses should be skipped, but should not prevent us
                // from traversing the module in a later use of it, if that
                // later use is not private.
                (*alreadySeen)[useSE->symbol()].push_back(use);
              }
            } else if (ImportStmt* import = toImportStmt(expr)) {
              SymExpr* importSE = toSymExpr(import->src);
              INT_ASSERT(importSE);

              if (!import->isPrivate &&
                  !importSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                ImportStmt* importToAdd = import->applyOuterImport(srcImport);
                // Imports of private modules are not transitive - the
                // symbols in the private modules are only visible to itself
                // and its immediate parent.  Therefore, if the symbol is
                // private, we will not traverse it further and will merely
                // add it to the alreadySeen map.
                if (importToAdd != NULL &&
                    skipUse(alreadySeen, importToAdd) == false) {
                  next.add(importToAdd);
                  modules->add(importToAdd);
                }

                if (importToAdd != NULL) {
                  (*alreadySeen)[importSE->symbol()].push_back(importToAdd);
                }
              } else if (!import->isPrivate &&
                         importSE->symbol()->hasFlag(FLAG_PRIVATE)) {
                // If we're skipping because the import was public, but the
                // module was private, then we shouldn't look at the module
                // again and should add it to the alreadySeen map.  Otherwise
                // there might be a later import or use that is public, so
                // we should allow it to be found
                (*alreadySeen)[importSE->symbol()].push_back(import);
              }
            } else {
              INT_ASSERT("Bad use list, expected UseStmt or ImportStmt");
            }
          }
        }

      } else {
        INT_ASSERT("Bad use list, expected UseStmt or ImportStmt");
      }
    }
  }

  if (next.n) {
    buildBreadthFirstModuleList(modules, &next, alreadySeen);
  }
}

// Returns true if we should skip looking at this use, because the symbols it
// provides have already been covered by a previous use.
static bool skipUse(std::map<Symbol*, std::vector<VisibilityStmt*> >* seen,
                    UseStmt* current) {
  SymExpr* useSE = toSymExpr(current->src);

  INT_ASSERT(useSE);

  std::vector<VisibilityStmt*> vec = (*seen)[useSE->symbol()];

  if (vec.size() > 0) {
    // We've already seen at least one use of this module, but it might
    // not be thorough enough to justify skipping the newest 'use'.
    for_vector(VisibilityStmt, stmt, vec) {
      if (UseStmt* use = toUseStmt(stmt)) {
        if (current->providesNewSymbols(use) == false) {
          // We found a prior use that covered all the symbols available
          // from current.  We can skip looking at current
          return true;
        }
      } else if (ImportStmt* import = toImportStmt(stmt)) {
        if (current->providesNewSymbols(import) == false) {
          // The current use statement is equivalent to a prior import statement
          // so no need to include it
          return true;
        }
      }
    }
  }

  // We didn't have a prior use, or all the prior uses we missing at
  // least one of the symbols current provides.  Don't skip current.
  return false;
}

// Returns true if we should skip looking at this import, because the symbols it
// provides have already been covered by a previous use.
static bool skipUse(std::map<Symbol*, std::vector<VisibilityStmt*> >* seen,
                    ImportStmt* current) {
  SymExpr* useSE = toSymExpr(current->src);

  INT_ASSERT(useSE);

  std::vector<VisibilityStmt*> vec = (*seen)[useSE->symbol()];

  if (vec.size() > 0) {
    // We've already seen at least one use or import of this module, but it
    // might not be thorough enough to justify skipping the newest 'import'
    for_vector(VisibilityStmt, stmt, vec) {
      if (UseStmt* use = toUseStmt(stmt)) {
        if (current->providesNewSymbols(use) == false) {
          // We found a prior use that covered all the symbols available
          // from current.  We can skip looking at current
          return true;
        }
      } else if (ImportStmt* import = toImportStmt(stmt)) {
        if (current->providesNewSymbols(import) == false) {
          // The current import statement is equivalent to a prior import
          // statement so no need to include it
          return true;
        }
      }
    }
  }

  // We didn't have a prior use or import that covered the symbols this
  // provides.  Don't skip current.
  return false;
}

/************************************* | **************************************
*                                                                             *
* Returns true if this module is capable of being used or traversed as part   *
* of an access in the provided scope, false if the module is private and the  *
* scope is not in its direct parent.                                          *
*                                                                             *
************************************** | *************************************/

bool Symbol::isVisible(BaseAST* scope) const {
  bool retval = true;

  if (hasFlag(FLAG_PRIVATE) == true) {
    BaseAST* parentScope = getScope(defPoint);
    BaseAST* searchScope = scope;

    INT_ASSERT(parentScope != NULL);

    // We need to walk up scopes until we either find our parent scope (in
    // which case, we're visible if it "use"s us) or we run out of scope to
    // check against (in which case we are most certainly *not* visible)
    while (searchScope != NULL) {
      if (searchScope == parentScope) {
        return true;
      }

      searchScope = getScope(searchScope);
    }

    // We got to the top of the scope without finding the parent.
    return false;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* getScope returns the BaseAST that corresponds to the scope where 'ast'      *
* exists; 'ast' must be an Expr or a Symbol.  Note that if you pass this a    *
* BaseAST that defines a scope, the BaseAST that defines the scope where it   *
* exists will be returned.                                                    *
*                                                                             *
* Thus if a BlockStmt nested in another BlockStmt is passed to getScope,      *
* the outer BlockStmt will be returned.                                       *
*                                                                             *
************************************** | *************************************/

BaseAST* getScope(BaseAST* ast) {
  if (Expr* expr = toExpr(ast)) {
    Expr*     parent = expr->parentExpr;
    BlockStmt* block = toBlockStmt(parent);

    // SCOPELESS and TYPE blocks do not define scopes
    if (block && block->blockTag == BLOCK_NORMAL) {
      return block;

    } else if (ForallStmt* forall = toForallStmt(parent)) {
      if (expr->list == &(forall->iteratedExpressions()))
        // Iterable expressions can rely only on symbols that are outside the
        // forall. I.e. outside bodyScope in scopeResolve(ForallStmt*,...).
        return getScope(forall);
      else
        return forall;

    } else if (LoopExpr* fe = toLoopExpr(parent)) {
      if (fe->iteratorExpr == expr ||
          fe->iteratorExpr->contains(expr)) {
        return getScope(fe);
      } else {
        return fe;
      }

    } else if (parent) {
      return getScope(parent);

    } else if (FnSymbol* fn = toFnSymbol(expr->parentSymbol)) {
      return fn;

    } else if (TypeSymbol* ts = toTypeSymbol(expr->parentSymbol)) {
      if (isEnumType(ts->type) || isAggregateType(ts->type)) {
        return ts;
      }
    }

    if (expr->parentSymbol == rootModule)
      return NULL;

    else
      return getScope(expr->parentSymbol->defPoint);

  } else if (Symbol* sym = toSymbol(ast)) {
    if (sym == rootModule)
      return NULL;
    else
      return getScope(sym->defPoint);
  }

  INT_FATAL(ast, "getScope expects an Expr or a Symbol");

  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static ModuleSymbol* definesModuleSymbol(Expr* expr) {
  ModuleSymbol* retval = NULL;

  if (DefExpr* def = toDefExpr(expr)) {
    retval = toModuleSymbol(def->sym);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


// Find 'unmanaged SomeClass' and 'borrowed SomeClass' and replace these
// with the compiler's simpler representation (canonical type or unmanaged type)
void resolveUnmanagedBorrows(CallExpr* call) {
  if (isClassDecoratorPrimitive(call)) {

    // Give up now if the actual is missing.
    if (call->numActuals() < 1)
      return;

    // Make sure to handle nested calls appropriately
    if (CallExpr* sub = toCallExpr(call->get(1))) {
      if (isClassDecoratorPrimitive(sub)) {
        resolveUnmanagedBorrows(sub);
      }
    }

    SymExpr* typeSymbolSe = NULL;
    if (SymExpr* se = toSymExpr(call->get(1))) {
      typeSymbolSe = se;
    } else if (CallExpr* sub = toCallExpr(call->get(1))) {
      if (sub->baseExpr != NULL) {
        if (SymExpr* se = toSymExpr(sub->baseExpr)) {
          typeSymbolSe = se;
        }
      }
    }

    if (typeSymbolSe != NULL) {
      if (TypeSymbol* ts = toTypeSymbol(typeSymbolSe->symbol())) {
        AggregateType* at = toAggregateType(canonicalDecoratedClassType(ts->type));

        ClassTypeDecorator decorator = CLASS_TYPE_BORROWED;
        if (isClassLike(ts->type)) {
          decorator = classTypeDecorator(ts->type);
        } else if (isManagedPtrType(ts->type) &&
                   (call->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
                    call->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED))) {
          decorator = CLASS_TYPE_MANAGED;
        } else {
          const char* type = NULL;
          if (call->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
              call->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED))
            type = "unmanaged";
          else if (call->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
                   call->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED))
            type = "borrowed";
          else if (call->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
                   call->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED))
            type = "?";
          else if (call->isPrimitive(PRIM_TO_NON_NILABLE_CLASS))
            type = "nonnil";

          USR_FATAL_CONT(call, "%s can only apply to class types "
                               "(%s is not a class type)",
                               type, ts->name);
          at = NULL;
        }

        // Compute the decorated class type
        if (call->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
            call->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED)) {
          int tmp = decorator & CLASS_TYPE_NILABILITY_MASK;
          tmp |= CLASS_TYPE_UNMANAGED;
          decorator = (ClassTypeDecorator) tmp;
        } else if (call->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
                   call->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED)) {
          int tmp = decorator & CLASS_TYPE_NILABILITY_MASK;
          tmp |= CLASS_TYPE_BORROWED;
          decorator = (ClassTypeDecorator) tmp;
        } else if (call->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
                   call->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED)) {
          decorator = addNilableToDecorator(decorator);
        } else if (call->isPrimitive(PRIM_TO_NON_NILABLE_CLASS)) {
          decorator = addNonNilToDecorator(decorator);
        }

        Type* dt = NULL;
        if (at) {
          dt = at->getDecoratedClass(decorator);
        } else {
          // e.g. for borrowed?
          switch (decorator) {
            case CLASS_TYPE_BORROWED:
              dt = dtBorrowed;
              break;
            case CLASS_TYPE_BORROWED_NONNIL:
              dt = dtBorrowedNonNilable;
              break;
            case CLASS_TYPE_BORROWED_NILABLE:
              dt = dtBorrowedNilable;
              break;
            case CLASS_TYPE_UNMANAGED:
              dt = dtUnmanaged;
              break;
            case CLASS_TYPE_UNMANAGED_NILABLE:
              dt = dtUnmanagedNilable;
              break;
            case CLASS_TYPE_UNMANAGED_NONNIL:
              dt = dtUnmanagedNonNilable;
              break;
            case CLASS_TYPE_MANAGED:
            case CLASS_TYPE_MANAGED_NONNIL:
            case CLASS_TYPE_MANAGED_NILABLE:
              INT_FATAL("case not handled");
              break;
            case CLASS_TYPE_GENERIC:
              dt = dtAnyManagementAnyNilable;
              break;
            case CLASS_TYPE_GENERIC_NONNIL:
              dt = dtAnyManagementNonNilable;
              break;
            case CLASS_TYPE_GENERIC_NILABLE:
              dt = dtAnyManagementNilable;
              break;
            // no default intentionally
          }
          INT_ASSERT(dt);
        }

        if (dt) {
          // Change the symexpr to point to the fixed type
          typeSymbolSe->setSymbol(dt->symbol);
          // Remove the PRIM_TO_UNMANAGED_CLASS etc
          Expr* sub = call->get(1)->remove();
          call->replace(sub);
        }
      }
    }
  }

  // Fix e.g. call _owned class? or call _owned anymanaged Error
  if (call->numActuals() == 1) {
    SymExpr* se1 = toSymExpr(call->baseExpr);
    SymExpr* se2 = toSymExpr(call->get(1));
    if (se1 != NULL && se2 != NULL) {
      TypeSymbol* ts1 = toTypeSymbol(se1->symbol());
      TypeSymbol* ts2 = toTypeSymbol(se2->symbol());
      if (ts1 != NULL && ts2 != NULL && isManagedPtrType(ts1->type)) {
        AggregateType* mgmt = getManagedPtrManagerType(ts1->type);
        Type* t2 = ts2->type;
        Type* useType = NULL;
        if (t2 == dtAnyManagementAnyNilable)
          useType = mgmt; // e.g. just _owned
        else if (t2 == dtAnyManagementNonNilable)
          useType = mgmt->getDecoratedClass(CLASS_TYPE_MANAGED_NONNIL);
        else if (t2 == dtAnyManagementNilable)
          useType = mgmt->getDecoratedClass(CLASS_TYPE_MANAGED_NILABLE);

        if (useType != NULL) {
          SET_LINENO(call);
          call->replace(new SymExpr(useType->symbol));
        } else if (isClassLike(t2)) {
          Type* canonical = canonicalClassType(t2);
          if (isNilableClassType(t2))
            useType = getDecoratedClass(canonical, CLASS_TYPE_BORROWED_NILABLE);
          else
            useType = canonical;

          se2->setSymbol(useType->symbol);
        }
      }
    }
  }
}

static void resolveUnmanagedBorrows() {
  forv_Vec(CallExpr, call, gCallExprs) {
    resolveUnmanagedBorrows(call);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void markUsedModule(std::set<ModuleSymbol*>& set, ModuleSymbol* mod) {
  // Do nothing if it's already in the set.
  if (set.count(mod) != 0)
    return;

  // Add it to the set
  set.insert(mod);

  // Mark each used module as well
  for_vector(ModuleSymbol, usedMod, mod->modUseList) {
    markUsedModule(set, usedMod);
  }

  // Additionally, mark any parent modules
  while (mod != NULL && mod->defPoint != NULL) {
    mod = mod->defPoint->getModule();
    if (mod != NULL)
      markUsedModule(set, mod);
  }
}

// Figure out if there are any modules that are not used at all.
// If so, completely remove these modules from the tree.
static void removeUnusedModules() {
  std::set<ModuleSymbol*> usedModules;

  markUsedModule(usedModules, stringLiteralModule);

  markUsedModule(usedModules, ModuleSymbol::mainModule());

  if (printModuleInitModule)
    markUsedModule(usedModules, printModuleInitModule);

  // Now remove any module not in the set
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (usedModules.count(mod) == 0) {
      INT_ASSERT(mod->defPoint); // we should not be removing e.g. _root
      mod->defPoint->remove();

      // Check that any mentions of the dead module are in
      // dead modules
      if (fVerify) {
        for_SymbolSymExprs(se, mod) {
          if (ModuleSymbol* inMod = se->getModule()) {
            if (usedModules.count(inMod) != 0) {
              INT_FATAL(se, "Invalid reference to unused module");
            }
          }
        }
      }
    }
  }
}

static void detectUserDefinedBorrowMethods() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isMethod() && fn->name == astrBorrow) {
      Type *thisType = fn->_this->type;
      if (isClassLike(thisType) &&
          !thisType->symbol->hasFlag(FLAG_MANAGED_POINTER)) {
        USR_FATAL("Classes cannot define a method named \"borrow\"");
      }
    }
  }
}

void scopeResolve() {
  addToSymbolTable();

  processImportExprs();

  enableModuleUsesCache = true;

  computeClassHierarchy();

  handleReceiverFormals();

  resolveGotoLabels();

  resolveUnresolvedSymExprs();

  resolveEnumeratedTypes();

  adjustTypeMethodsOnClasses();

  setupShadowVars();

  markGenerics();

  processGenericFields();

  ResolveScope::destroyAstMap();

  destroyModuleUsesCaches();

  warnedForDotInsideWith.clear();

  renameDefaultTypesToReflectWidths();

  cleanupExternC();

  resolveUnmanagedBorrows();

  detectUserDefinedBorrowMethods();

  removeUnusedModules();
}
