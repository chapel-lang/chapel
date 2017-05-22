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

#include "scopeResolve.h"

#include "astutil.h"
#include "build.h"
#include "clangUtil.h"
#include "driver.h"
#include "expr.h"
#include "externCResolve.h"
#include "initializerRules.h"
#include "LoopStmt.h"
#include "passes.h"
#include "ResolveScope.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "visibleFunctions.h"

#include <algorithm>
#include <map>
#include <set>

#ifdef HAVE_LLVM
// TODO: Remove uses of old-style collectors from LLVM-specific code.
#include "oldCollectors.h"
#include "llvm/ADT/SmallSet.h"
#endif

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
static std::map<BlockStmt*, Vec<UseStmt*>*>   moduleUsesCache;
static bool                                   enableModuleUsesCache = false;

//
// The aliasFieldSet is a set of names of fields for which arrays may
// be passed in by named argument as aliases, as in new C(A=>GA) (see
// test/arrays/deitz/test_array_alias_field.chpl).
//
static Vec<const char*>                       aliasFieldSet;


// To avoid duplicate user warnings in checkIdInsideWithClause().
// Using pair<> instead of astlocT to avoid defining operator<.
typedef std::pair< std::pair<const char*,int>, const char* >  WFDIWmark;
static std::set< std::pair< std::pair<const char*,int>, const char* > > warnedForDotInsideWith;

static void          addToSymbolTable();

static void          processImportExprs();

static void          addRecordDefaultConstruction();

static void          resolveGotoLabels();

static void          resolveUnresolvedSymExprs();

static void          resolveEnumeratedTypes();

static void          destroyModuleUsesCaches();

static void          renameDefaultTypesToReflectWidths();

static bool          lookupThisScopeAndUses(const char*           name,
                                            BaseAST*              context,
                                            BaseAST*              scope,
                                            std::vector<Symbol*>& symbols);

static ModuleSymbol* definesModuleSymbol(Expr* expr);

void scopeResolve() {
  //
  // add all program asts to the symbol table
  //
  addToSymbolTable();

  processImportExprs();

  enableModuleUsesCache = true;

  //
  // compute class hierarchy
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    ct->addClassToHierarchy();
  }

  //
  // determine fields (by name) that may be passed in arrays to alias
  //
  forv_Vec(NamedExpr, ne, gNamedExprs) {
    if (strncmp(ne->name, "chpl__aliasField_", 17) == 0) {
      CallExpr* pne  = toCallExpr(ne->parentExpr);
      CallExpr* ppne = (pne) ? toCallExpr(pne->parentExpr) : NULL;

      if (!ppne || !ppne->isPrimitive(PRIM_NEW)) {
        USR_FATAL(ne,
                  "alias-named-argument passing can only be used "
                  "in constructor calls");
      }

      aliasFieldSet.set_add(astr(&ne->name[17]));
    }
  }

  //
  // add implicit fields for implementing alias-named-argument passing
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    for_fields(field, ct) {
      if (strcmp(field->name, "outer") == 0) {
        USR_FATAL_CONT(field,
                       "Cannot have a field named 'outer'. "
                       "'outer' is used to refer to an outer class "
                       "from within a nested class.");
      }

      if (aliasFieldSet.set_in(field->name)) {
        SET_LINENO(field);

        const char* aliasName  = astr("chpl__aliasField_", field->name);
        Symbol*     aliasField = new VarSymbol(aliasName);
        DefExpr*    def        = new DefExpr(aliasField);

        aliasField->addFlag(FLAG_CONST);
        aliasField->addFlag(FLAG_IMPLICIT_ALIAS_FIELD);

        def->init     = new UnresolvedSymExpr("false");
        def->exprType = new UnresolvedSymExpr("bool");

        ct->fields.insertAtTail(def);
      }
    }
  }

  addRecordDefaultConstruction();

  //
  // resolve type of this for methods
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->_this != NULL && fn->_this->type == dtUnknown) {
      Expr* stmt = toArgSymbol(fn->_this)->typeExpr->body.only();

      if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(stmt)) {
        SET_LINENO(fn->_this);

        if (TypeSymbol* ts = toTypeSymbol(lookup(sym->unresolved, sym))) {
          sym->replace(new SymExpr(ts));

          fn->_this->type = ts->type;
          fn->_this->type->methods.add(fn);

          AggregateType::setCreationStyle(ts, fn);

        } else {
          USR_FATAL(fn, "cannot resolve base type for method '%s'", fn->name);
        }

      } else if (SymExpr* sym = toSymExpr(stmt)) {
        fn->_this->type = sym->symbol()->type;
        fn->_this->type->methods.add(fn);

        AggregateType::setCreationStyle(sym->symbol()->type->symbol, fn);
      }

    } else if (fn->_this) {
      AggregateType::setCreationStyle(fn->_this->type->symbol, fn);
    }
  }

  //
  // build constructors (type and value versions)
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    ct->buildConstructors();
  }

  resolveGotoLabels();

  resolveUnresolvedSymExprs();

  resolveEnumeratedTypes();

  ResolveScope::destroyAstMap();

  destroyModuleUsesCaches();

  warnedForDotInsideWith.clear();

  renameDefaultTypesToReflectWidths();

  cleanupExternC();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void addRecordDefaultConstruction() {
  forv_Vec(DefExpr, def, gDefExprs) {
    // We're only interested in declarations that do not have initializers.
    if (def->init != NULL) {

    } else if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (AggregateType* at = toAggregateType(var->type)) {
        if (at->isRecord() == false) {

        // No initializer for extern records.
        } else if (at->symbol->hasFlag(FLAG_EXTERN) == true) {

        } else {
          SET_LINENO(def);

          CallExpr* ctor_call = new CallExpr(new SymExpr(at->symbol));

          def->init = new CallExpr(PRIM_NEW, ctor_call);

          insert_help(def->init, def, def->parentSymbol);
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* addToSymbolTable adds the asts in a vector to the global symbolTable such   *
* that symbol definitions are added to entries in the table and new           *
* enclosing asts become entries                                               *
*                                                                             *
************************************** | *************************************/

static void addToSymbolTable(DefExpr* def);

static void addToSymbolTable() {
  forv_Vec(DefExpr, def, gDefExprs) {
    addToSymbolTable(def);
  }
}

static void addToSymbolTable(DefExpr* def) {
  Symbol* newSym = def->sym;

  if (newSym->hasFlag(FLAG_TEMP) == false &&
      isLabelSymbol(newSym)      == false) {
    ResolveScope* entry = ResolveScope::findOrCreateScopeFor(def);

    entry->extend(newSym);
  }
}


// Exported entry point for AggregateType
void addToSymbolTable(FnSymbol* fn) {
  std::vector<DefExpr*> defs;

  collectDefExprs(fn, defs);

  for_vector(DefExpr, def, defs) {
    addToSymbolTable(def);
  }
}


/************************************* | **************************************
*                                                                             *
* Transform module uses into calls to initialize functions; store the         *
* relevant scoping information in BlockStmt::modUses                          *
*                                                                             *
************************************** | *************************************/

static void processImportExprs(ModuleSymbol* topLevelModule);

static void processImportExprs() {
  for_alist(expr, theProgram->block->body) {
    if (UseStmt* useStmt = toUseStmt(expr))  {
      useStmt->scopeResolve();

    } else if (ModuleSymbol* mod = definesModuleSymbol(expr)) {
      processImportExprs(mod);
    }
  }
}

static void processImportExprs(ModuleSymbol* topLevelModule) {
  std::vector<BaseAST*> asts;

  // Collect *all* asts within this top-level module in text order
  collect_asts(topLevelModule, asts);

  for_vector(BaseAST, item, asts) {
    if (UseStmt* useStmt = toUseStmt(item)) {
      if (useStmt->isValid() == true) {
        useStmt->scopeResolve();
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveGotoLabels() {
  forv_Vec(GotoStmt, gs, gGotoStmts) {
    SET_LINENO(gs);

    if (SymExpr* label = toSymExpr(gs->label)) {
      if (label->symbol() == gNil) {
        LoopStmt* loop = LoopStmt::findEnclosingLoop(gs);

        if (!loop)
          USR_FATAL(gs, "break or continue is not in a loop");

        if (gs->gotoTag == GOTO_BREAK) {
          Symbol* breakLabel = loop->breakLabelGet();

          INT_ASSERT(breakLabel);
          gs->label->replace(new SymExpr(breakLabel));

        } else if (gs->gotoTag == GOTO_CONTINUE) {
          Symbol* continueLabel = loop->continueLabelGet();
          INT_ASSERT(continueLabel);

          gs->label->replace(new SymExpr(continueLabel));

        } else
          INT_FATAL(gs, "unexpected goto type");
      }

    } else if (UnresolvedSymExpr* label = toUnresolvedSymExpr(gs->label)) {
      const char* name = label->unresolved;
      LoopStmt*   loop = LoopStmt::findEnclosingLoop(gs);

      while (loop && (!loop->userLabel || strcmp(loop->userLabel, name))) {
        loop = LoopStmt::findEnclosingLoop(loop->parentExpr);
      }

      if (!loop) {
        USR_FATAL(gs, "bad label on break or continue");
      }

      if (gs->gotoTag == GOTO_BREAK)
        label->replace(new SymExpr(loop->breakLabelGet()));

      else if (gs->gotoTag == GOTO_CONTINUE)
        label->replace(new SymExpr(loop->continueLabelGet()));

      else
        INT_FATAL(gs, "unexpected goto type");
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveUnresolvedSymExpr(UnresolvedSymExpr* usymExpr);

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

#ifdef HAVE_LLVM
static bool tryCResolve(ModuleSymbol* module, const char* name);
#endif

static void resolveUnresolvedSymExprs() {
  //
  // Translate M.x where M is a ModuleSymbol into just x where x is
  // the symbol in module M; for functions, insert a "module=" token
  // that is used to determine visible functions.
  //

  int maxResolved = 0;
  int i           = 0;

  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    resolveUnresolvedSymExpr(unresolvedSymExpr);

    maxResolved++;
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    resolveModuleCall(call);
  }

  // Note that the extern C resolution might add new UnresolvedSymExprs, and it
  // might do that within resolveModuleCall, so we try resolving unresolved
  // symbols a second time as the extern C block support might have added some.
  // Alternatives include:
  //  - have the extern C wrapper-builder directly call resolveUnresolved
  //      (but that complicates the way scopeResolve works now)
  //  - import all C symbols at an earlier point
  //      (but that might add lots of unused garbage to the Chapel AST
  //       for e.g. #include <stdio.h>; and it might cause the C-to-Chapel
  //       translator to need to handle more platform/compiler-specific stuff,
  //       and it might lead to extra naming conflicts).
  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    // Only try resolving symbols that are new after last attempt.
    if (i >= maxResolved) {
      resolveUnresolvedSymExpr(unresolvedSymExpr);
    }

    i++;
  }
}

static void resolveUnresolvedSymExpr(UnresolvedSymExpr* usymExpr) {
  SET_LINENO(usymExpr);

  const char* name = usymExpr->unresolved;

  if (strcmp(name, ".") == 0 || usymExpr->parentSymbol == NULL) {

  } else if (Symbol* sym = lookup(name, usymExpr)) {
    FnSymbol* fn = toFnSymbol(sym);

    if (fn == NULL) {
      SymExpr* symExpr = new SymExpr(sym);

      usymExpr->replace(symExpr);

      updateMethod(usymExpr, sym, symExpr);

    // sjd: stopgap to avoid shadowing variables or functions by methods
    } else if (fn->hasFlag(FLAG_METHOD) == true) {
      updateMethod(usymExpr);

    // handle function call without parentheses
    } else if (fn->hasFlag(FLAG_NO_PARENS) == true) {
      checkIdInsideWithClause(usymExpr, usymExpr);
      usymExpr->replace(new CallExpr(fn));

    } else if (Expr* parent = usymExpr->parentExpr) {
      CallExpr* call = toCallExpr(parent);

      if (call == NULL || call->baseExpr != usymExpr) {
        CallExpr* primFn = NULL;

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

  } else {
    updateMethod(usymExpr);

#ifdef HAVE_LLVM
    if (externC == true && tryCResolve(usymExpr->getModule(), name) == true) {
      // Try resolution again since the symbol should exist now
      resolveUnresolvedSymExpr(usymExpr);
    }
#endif
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
      isAggr = isAggregateType(cts->type);
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
    for (int i = 0; i < nestDepth; i++) {
      dot = new CallExpr(".", dot, new_CStringSymbol("outer"));
    }
  }

  if (isTypeSymbol(sym) == true) {
    dot = new CallExpr(".", dot, sym);
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
  if (strcmp(name, type->symbol->name) == 0) {
    return false;
  }

  forv_Vec(Symbol, method, type->methods) {
    if (method != NULL && strcmp(name, method->name) == 0) {
      return true;
    }
  }

  forv_Vec(Type, pt, type->dispatchParents) {
    if (isMethodName(name, pt) == true) {
      return true;
    }
  }

  if (AggregateType* ct = toAggregateType(type)) {
    Type* outerType = ct->symbol->defPoint->parentSymbol->type;

    if (AggregateType* outer = toAggregateType(outerType)) {
      if (isMethodName(name, outer) == true) {
        return true;
      }
    }
  }

  return false;
}


//
// isMethodNameLocal returns true iff 'name' names a method of 'type'
// excluding methods of an outer type
//
static bool isMethodNameLocal(const char* name, Type* type) {
  if (strcmp(name, type->symbol->name) == 0) {
    return false;
  }

  forv_Vec(Symbol, method, type->methods) {
    if (method != NULL && strcmp(name, method->name) == 0) {
      return true;
    }
  }

  forv_Vec(Type, pt, type->dispatchParents) {
    if (isMethodName(name, pt) == true) {
      return true;
    }
  }

  return false;
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
  // A 'with' clause for a forall loop.
  if (BlockStmt* parent = toBlockStmt(exprInAst->parentExpr)) {
    if (ForallIntents* fi = parent->forallIntents) {
      for_vector(Expr, fiVar, fi->fiVars) {
        if (exprInAst == fiVar) {
          errorDotInsideWithClause(origUSE, "forall loop");
          return;
        }
      }
    }
  }

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

static void resolveModuleCall(CallExpr* call) {
  if (call->isNamed(".") == true) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        SET_LINENO(call);

        ModuleSymbol* enclosingModule = call->getModule();
        Symbol*       sym             = NULL;
        const char*   mbrName         = get_string(call->get(2));

        enclosingModule->moduleUseAdd(mod);

        if (ResolveScope* scope = ResolveScope::getScopeFor(mod->block)) {
          sym = scope->lookup(mbrName);
        }

        if (sym != NULL) {
          if (sym->isVisible(call) == false) {
            // The symbol is not visible at this scope because it is
            // private to mod!  Error out
            USR_FATAL(call,
                      "Cannot access '%s', '%s' is private to '%s'",
                      mbrName,
                      mbrName,
                      mod->name);

          } else if (FnSymbol* fn = toFnSymbol(sym)) {
            if (fn->_this == NULL && fn->hasFlag(FLAG_NO_PARENS)) {
              call->replace(new CallExpr(fn));

            } else {
              UnresolvedSymExpr* se     = new UnresolvedSymExpr(mbrName);

              call->replace(se);

              CallExpr*          parent = toCallExpr(se->parentExpr);

              INT_ASSERT(parent);

              parent->insertAtHead(mod);
              parent->insertAtHead(gModuleToken);
            }

          } else {
            call->replace(new SymExpr(sym));
          }

#ifdef HAVE_LLVM
        } else if (externC                                 == true &&
                   tryCResolve(call->getModule(), mbrName) == true) {
          // Try to resolve again now that the symbol should
          // be in the table
          resolveModuleCall(call);
#endif

        } else {
          USR_FATAL_CONT(call,
                         "Symbol '%s' undeclared in module '%s'",
                         mbrName,
                         mod->name);
        }
      }
    }
  }
}

#ifdef HAVE_LLVM
static bool tryCResolve(ModuleSymbol*                     module,
                        const char*                       name,
                        llvm::SmallSet<ModuleSymbol*, 24> visited);

static bool tryCResolve(ModuleSymbol* module, const char* name) {
  llvm::SmallSet<ModuleSymbol*, 24> visited;

  return tryCResolve(module, name, visited);
}

static bool tryCResolve(ModuleSymbol*                     module,
                        const char*                       name,
                        llvm::SmallSet<ModuleSymbol*, 24> visited) {

  if (module == NULL) {
    return false;

  } else if (llvm_small_set_insert(visited, module)) {
    // visited.insert(module)) {
    // we added it to the set, so continue.

  } else {
    // It was already in the set.
    return false;
  }

  // Is it resolveable in this module?
  if (module->extern_info != NULL) {
    // Try resolving it
    Vec<Expr*> c_exprs;

    // Try to create an extern declaration for name,
    //  if it exists in the module's extern blocks.
    // The resulting Chapel extern declarations are put into
    //  c_exprs and will need to be resolved.
    convertDeclToChpl(module, name, c_exprs);

    if (c_exprs.count()) {
      forv_Vec(Expr*, c_expr, c_exprs) {
        std::vector<DefExpr*> v;

        collectDefExprs(c_expr, v);

        for_vector(DefExpr, def, v) {
          addToSymbolTable(def);
        }

        if (DefExpr* de = toDefExpr(c_expr)) {
          if (TypeSymbol* ts = toTypeSymbol(de->sym)) {
            if (AggregateType* ct = toAggregateType(ts->type)) {
              SET_LINENO(ct->symbol);
              // If this is a class DefExpr,
              //  make sure its initializer gets created.
              ct->buildConstructors();
            }
          }
        }
      }

      //any new UnresolvedSymExprs will be called in another for loop
      // in scopeResolve.
      return true;
    }
  }

  // Otherwise, try the modules used by this module.
  forv_Vec(ModuleSymbol, usedMod, module->modUseList) {
    if (tryCResolve(usedMod, name, visited) == true) {
      return true;
    }
  }

  return false;
}

#endif


/************************************* | **************************************
*                                                                             *
* resolves EnumTypeName.fieldName to the symbol named fieldName in the        *
* enumerated type named EnumTypeName                                          *
*                                                                             *
************************************** | *************************************/

static void resolveEnumeratedTypes() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isNamed(".")) {
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
* delete the module uses cache                                                *
*                                                                             *
************************************** | *************************************/

static void destroyModuleUsesCaches() {
  std::map<BlockStmt*, Vec<UseStmt*>*>::iterator use;

  for (use = moduleUsesCache.begin(); use != moduleUsesCache.end(); use++) {
    delete use->second;
  }

  moduleUsesCache.clear();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

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

                   std::vector<Symbol*>& symbols);

// Given a name and a calling context, determine the symbol referred to
// by that name in the context of that call
Symbol* lookup(const char* name, BaseAST* context) {
  std::vector<Symbol*> symbols;
  Symbol*              retval = NULL;

  lookup(name, context, symbols);

  if (symbols.size() == 0) {
    retval = NULL;

  } else if (symbols.size() == 1) {
    retval = symbols[0];

  } else {
    // Multiple symbols found for this name.
    // If they're all functions
    //   then      assume function resolution will be applied
    //   otherwise fail

    for_vector(Symbol, sym, symbols) {
      if (isFnSymbol(sym) == false) {
        USR_FATAL_CONT(sym, "Symbol %s multiply defined", name);
      }
    }

    USR_STOP();

    retval = NULL;
  }

  return retval;
}

void lookup(const char*           name,
            BaseAST*              context,
            std::vector<Symbol*>& symbols) {
  Vec<BaseAST*> visited;

  lookup(name, context, context, visited, symbols);
}

static void lookup(const char*           name,
                   BaseAST*              context,

                   BaseAST*              scope,
                   Vec<BaseAST*>&        visited,

                   std::vector<Symbol*>& symbols) {

  if (!visited.set_in(scope)) {
    visited.set_add(scope);

    if (lookupThisScopeAndUses(name, context, scope, symbols) == true) {
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
      if (getScope(scope) != NULL) {
        lookup(name, context, getScope(scope), visited, symbols);
      }

    } else {
      // Otherwise, look in the next scope up.
      FnSymbol* fn = toFnSymbol(scope);

      if (fn != NULL && fn->_this) {
        // If currently in a method, the next scope up is anything visible
        // within the aggregate type
        if (AggregateType* ct = toAggregateType(fn->_this->type)) {
          lookup(name, context, ct->symbol, visited, symbols);
        }
      }

      // Check if found something in last lookup call
      if (symbols.size() == 0) {
        // If we didn't find something in the aggregate type that matched,
        // or we weren't in an aggregate type method, so look at next scope up.
        lookup(name, context, getScope(scope), visited, symbols);
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

static void      buildBreadthFirstModuleList(Vec<UseStmt*>* modules);

static void      buildBreadthFirstModuleList(
                      Vec<UseStmt*>*                             modules,
                      Vec<UseStmt*>*                             current,
                      std::map<Symbol*, std::vector<UseStmt*> >* alreadySeen);

static bool      skipUse(std::map<Symbol*, std::vector<UseStmt*> >* seen,
                         UseStmt*                                   current);

static bool lookupThisScopeAndUses(const char*           name,
                                   BaseAST*              context,
                                   BaseAST*              scope,
                                   std::vector<Symbol*>& symbols) {
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
        Vec<UseStmt*>* moduleUses = NULL;

        if (moduleUsesCache.count(block) == 0) {
          moduleUses = new Vec<UseStmt*>();

          for_actuals(expr, block->useList) {
            UseStmt* use = toUseStmt(expr);
            INT_ASSERT(use);

            moduleUses->add(use);
          }

          INT_ASSERT(moduleUses->n);

          buildBreadthFirstModuleList(moduleUses);

          if (enableModuleUsesCache)
            moduleUsesCache[block] = moduleUses;
        } else {
          moduleUses = moduleUsesCache[block];
        }

        forv_Vec(UseStmt, use, *moduleUses) {
          if (use != NULL) {
            if (use->skipSymbolSearch(name) == false) {
              const char* nameToUse = use->isARename(name) ? use->getRename(name) : name;
              BaseAST* scopeToUse = use->getSearchScope();

              if (Symbol* sym = inSymbolTable(nameToUse, scopeToUse)) {
                if (sym->hasFlag(FLAG_PRIVATE) == true) {
                  if (sym->isVisible(context) == true &&
                      isRepeat(sym, symbols)  == false) {
                    symbols.push_back(sym);
                  }

                } else if (isRepeat(sym, symbols) == false) {
                  symbols.push_back(sym);
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
    if (Symbol* sym = scope->lookup(name)) {
      if (sym->hasFlag(FLAG_METHOD) == false) {
        retval = sym;

      } else if (FnSymbol* fn = toFnSymbol(sym)) {
        if (methodMatched(ast, fn) == true) {
          retval = sym;
        }
      }
    }
  }

  return retval;
}

static Symbol* inType(const char* name, BaseAST* scope) {
  Symbol* retval = NULL;

  if (TypeSymbol* ts = toTypeSymbol(scope)) {
    if (AggregateType* ct = toAggregateType(ts->type)) {
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
  if (strcmp(name, type->symbol->name) == 0) {
    return NULL;
  }

  // Looks for name in methods defined directly on this type
  forv_Vec(FnSymbol, method, type->methods) {
    if (method != NULL && strcmp(name, method->name) == 0) {
      return method;
    }
  }

  // Looks for name in methods defined on parent types
  forv_Vec(Type, pt, type->dispatchParents) {
    if (FnSymbol* sym = getMethod(name, pt)) {
      return sym;
    }
  }

  // Looks for name in types wrapping this type definition
  if (AggregateType* ct = toAggregateType(type)) {
    Type *outerType = ct->symbol->defPoint->parentSymbol->type;

    if (AggregateType* outer = toAggregateType(outerType)) {
      if (FnSymbol* sym = getMethod(name, outer)) {
        return sym;
      }
    }
  }

  return NULL;
}

static void buildBreadthFirstModuleList(Vec<UseStmt*>* modules) {
  std::map<Symbol*, std::vector<UseStmt* > > seen;

  return buildBreadthFirstModuleList(modules, modules, &seen);
}

// If the uses of a particular module are considered its level 1 uses, then
// this function will only add level 2 and lower uses to the modules vector
// argument.
static void buildBreadthFirstModuleList(
                 Vec<UseStmt*>*                             modules,
                 Vec<UseStmt*>*                             current,
                 std::map<Symbol*, std::vector<UseStmt*> >* alreadySeen) {
 // use NULL as a sentinel to identify modules of equal depth
  modules->add(NULL);

  Vec<UseStmt*> next;

  forv_Vec(UseStmt, source, *current) {
    if (!source) {
      break;
    } else {
      SymExpr* se = toSymExpr(source->src);
      INT_ASSERT(se);
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (mod->block->useList != NULL) {
          for_actuals(expr, mod->block->useList) {
            UseStmt* use = toUseStmt(expr);
            INT_ASSERT(use);

            SymExpr* useSE = toSymExpr(use->src);
            INT_ASSERT(useSE);

            UseStmt* useToAdd = NULL;
            if (!useSE->symbol()->hasFlag(FLAG_PRIVATE)) {
              // Uses of private modules are not transitive -
              // the symbols in the private modules are only visible to
              // itself and its immediate parent.  Therefore, if the symbol
              // is private, we will not traverse it further and will merely
              // add it to the alreadySeen map.
              useToAdd = use->applyOuterUse(source);

              if (useToAdd                       != NULL &&
                  skipUse(alreadySeen, useToAdd) == false) {
                next.add(useToAdd);
                modules->add(useToAdd);
              }

              // if applyOuterUse returned NULL, the number of symbols that
              // could be provided from this use was 0, so it didn't need to be
              // added to the alreadySeen map.
              if (useToAdd != NULL) {
                (*alreadySeen)[useSE->symbol()].push_back(useToAdd);
              }

            } else {
              (*alreadySeen)[useSE->symbol()].push_back(use);
            }
          }
        }
      }
    }
  }

  if (next.n) {
    buildBreadthFirstModuleList(modules, &next, alreadySeen);
  }
}

// Returns true if we should skip looking at this use, because the symbols it
// provides have already been covered by a previous use.
static bool skipUse(std::map<Symbol*, std::vector<UseStmt*> >* seen,
                    UseStmt*                                   current) {
  SymExpr* useSE = toSymExpr(current->src);

  INT_ASSERT(useSE);

  std::vector<UseStmt*> vec = (*seen)[useSE->symbol()];

  if (vec.size() > 0) {
    // We've already seen at least one use of this module, but it might
    // not be thorough enough to justify skipping the newest 'use'.
    for_vector(UseStmt, use, vec) {
      if (current->providesNewSymbols(use) == false) {
        // We found a prior use that covered all the symbols available
        // from current.  We can skip looking at current
        return true;
      }
    }
  }

  // We didn't have a prior use, or all the prior uses we missing at
  // least one of the symbols current provides.  Don't skip current.
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
    BlockStmt* block = toBlockStmt(expr->parentExpr);

    // SCOPELESS and TYPE blocks do not define scopes
    if (block && block->blockTag == BLOCK_NORMAL) {
      return block;

    } else if (expr->parentExpr) {
      return getScope(expr->parentExpr);

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
