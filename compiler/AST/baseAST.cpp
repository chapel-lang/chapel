/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "baseAST.h"

#include "astutil.h"
#include "CForLoop.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "driver.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "log.h"
#include "LoopExpr.h"
#include "ModuleSymbol.h"
#include "ParamForLoop.h"
#include "parser.h"
#include "passes.h"
#include "resolution.h"
#include "runpasses.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "type.h"
#include "WhileStmt.h"

#include <ostream>
#include <sstream>
#include <string>

//
// declare global vectors gSymExprs, gCallExprs, gFnSymbols, ...
//
#define decl_gvecs(type) Vec<type*> g##type##s
foreach_ast(decl_gvecs);

static int uid = 1;

#define decl_counters(type)                                             \
  int n##type = g##type##s.n, k##type = n##type*sizeof(type)/1024

#define sum_gvecs(type) g##type##s.n

#define def_vec_hash(SomeType) \
    template<> \
    uintptr_t _vec_hasher(SomeType* obj) { \
      if (obj == NULL) { \
        return 0; \
      } else { \
        return (uintptr_t)((BaseAST*)obj)->id; \
      } \
    }

foreach_ast(def_vec_hash);
def_vec_hash(Symbol);
def_vec_hash(Type);
def_vec_hash(BaseAST);

#undef def_vec_hash

//
// Throughout printStatistics(), "n" indicates the number of nodes;
// "k" indicates how many KiB memory they occupy: k = n * sizeof(node) / 1024.
//
void printStatistics(const char* pass) {
  static int last_nasts = -1;
  static int maxK = -1, maxN = -1;

  if (strcmp(pass, "codegen") == 0) {
    if (strstr(fPrintStatistics, "m")) {
      fprintf(stderr, "Maximum # of ASTS: %d\n", maxN);
      fprintf(stderr, "Maximum Size (KB): %d\n", maxK);
    }
  }

  int nasts = foreach_ast_sep(sum_gvecs, +);

  if (last_nasts == nasts) {
    fprintf(stderr, "%23s%s\n", "", pass);
    return;
  }

  foreach_ast(decl_counters);

  int nStmt = nBlockStmt + nCondStmt + nDeferStmt + nGotoStmt + nUseStmt +
    nImportStmt + nExternBlockStmt + nForallStmt + nTryStmt + nForwardingStmt +
    nCatchStmt + nImplementsStmt;
  int kStmt = kBlockStmt + kCondStmt + kDeferStmt + kGotoStmt + kUseStmt +
    kImportStmt + kExternBlockStmt + kForallStmt + kTryStmt + kForwardingStmt +
    kCatchStmt + kImplementsStmt;
  int nExpr = nUnresolvedSymExpr + nSymExpr + nDefExpr + nCallExpr +
    nContextCallExpr + nLoopExpr + nNamedExpr + nIfcConstraint + nIfExpr +
    nTemporaryConversionThunk;
  int kExpr = kUnresolvedSymExpr + kSymExpr + kDefExpr + kCallExpr +
    kContextCallExpr + kLoopExpr + kNamedExpr + kIfcConstraint + kIfExpr +
    kTemporaryConversionThunk;
  int nSymbol = nModuleSymbol+nVarSymbol+nArgSymbol+nShadowVarSymbol+nTypeSymbol+nFnSymbol+nInterfaceSymbol+nEnumSymbol+nLabelSymbol+nTemporaryConversionSymbol;
  int kSymbol = kModuleSymbol+kVarSymbol+kArgSymbol+kShadowVarSymbol+kTypeSymbol+kFnSymbol+kInterfaceSymbol+kEnumSymbol+kLabelSymbol+kTemporaryConversionSymbol;
  int nType = nPrimitiveType+nConstrainedType+nEnumType+nAggregateType+nFunctionType+nTemporaryConversionType+nDecoratedClassType;
  int kType = kPrimitiveType+kConstrainedType+kEnumType+kAggregateType+kFunctionType+kTemporaryConversionType+kDecoratedClassType;

  fprintf(stderr, "%7d asts (%6dK) %s\n", nStmt+nExpr+nSymbol+nType, kStmt+kExpr+kSymbol+kType, pass);

  if (nStmt+nExpr+nSymbol+nType > maxN)
    maxN = nStmt+nExpr+nSymbol+nType;

  if (kStmt+kExpr+kSymbol+kType > maxK)
    maxK = kStmt+kExpr+kSymbol+kType;

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Stmt %9d  Cond %9d  Block %9d  Goto  %9d\n",
            nStmt, nCondStmt, nBlockStmt, nGotoStmt);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Stmt %9dK Cond %9dK Block %9dK Goto  %9dK\n",
            kStmt, kCondStmt, kBlockStmt, kGotoStmt);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Stmt %6dK Cond %6dK Block %6dK Goto  %6dK\n",
            kStmt, kCondStmt, kBlockStmt, kGotoStmt);

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %9d  Unre %9d  Sym   %9d  Def   %9d  Call  %9d  Forall %9d  Named %9d  If   %9d  Thunk %9d\n",
            nExpr, nUnresolvedSymExpr, nSymExpr, nDefExpr, nCallExpr, nLoopExpr, nNamedExpr, nIfExpr, nTemporaryConversionThunk);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %9dK Unre %9dK Sym   %9dK Def   %9dK Call  %9dK Forall %9dk Named %9dK If   %9d  Thunk %9dK\n",
            kExpr, kUnresolvedSymExpr, kSymExpr, kDefExpr, kCallExpr, kLoopExpr, kNamedExpr, kIfExpr, kTemporaryConversionThunk);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %6dK Unre %6dK Sym   %6dK Def   %6dK Call  %6dK Forall %6dk Named %6dK If   %6d  Thunk %9dK\n",
            kExpr, kUnresolvedSymExpr, kSymExpr, kDefExpr, kCallExpr, kLoopExpr, kNamedExpr, kIfExpr, kTemporaryConversionThunk);

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Sym  %9d  Mod  %9d  Var   %9d  Arg   %9d  Shd   %9d  Type   %9d  Fn    %9d  Enum %9d  Label %9d\n",
            nSymbol, nModuleSymbol, nVarSymbol, nArgSymbol, nShadowVarSymbol, nTypeSymbol, nFnSymbol, nEnumSymbol, nLabelSymbol);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Sym  %9dK Mod  %9dK Var   %9dK Arg   %9dK Shd   %9dK Type   %9dK Fn    %9dK Enum %9dK Label %9dK\n",
            kSymbol, kModuleSymbol, kVarSymbol, kArgSymbol, kShadowVarSymbol, kTypeSymbol, kFnSymbol, kEnumSymbol, kLabelSymbol);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Sym  %6dK Mod  %6dK Var   %6dK Arg   %6dK Shd   %6dK Type   %6dK Fn    %6dK Enum %6dK Label %6dK\n",
            kSymbol, kModuleSymbol, kVarSymbol, kArgSymbol, kShadowVarSymbol, kTypeSymbol, kFnSymbol, kEnumSymbol, kLabelSymbol);

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %9d  Prim %9d  Enum  %9d  Class %9d \n",
            nType, nPrimitiveType, nEnumType, nAggregateType);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %9dK Prim %9dK Enum  %9dK Class %9dK\n",
            kType, kPrimitiveType, kEnumType, kAggregateType);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %6dK Prim %6dK Enum  %6dK Class %6dK\n",
            kType, kPrimitiveType, kEnumType, kAggregateType);
  last_nasts = nasts;
}

/* Certain AST elements, such as PRIM_END_OF_STATEMENT, should just
   be adjusted when variables are removed. */
static void remove_weak_links(VarSymbol* var) {
  if (var != NULL) {
    for_SymbolSymExprs(se, var) {
      if (isAlive(se))
        if (CallExpr* call = toCallExpr(se->parentExpr))
          if (call->isPrimitive(PRIM_END_OF_STATEMENT))
            se->remove();
    }
  }
}

// for debugging purposes only
void trace_remove(BaseAST* ast, char flag) {
  // crash if deletedIdHandle is not initialized but deletedIdFilename is
  if (deletedIdON() == true) {
    fprintf(deletedIdHandle, "%d %c %p %d\n",
            currentPassNo, flag, ast, ast->id);
  }
  if (ast->id == breakOnRemoveID) {
    if (deletedIdON() == true) fflush(deletedIdHandle);
    debuggerBreakHere();
  }
  // There should never be an attempt to delete a global type.
  if (flag != 'z' && // At least, not before compiler shutdown.
      isPrimitiveType(ast) &&
      toPrimitiveType(ast)->symbol->hasFlag(FLAG_GLOBAL_TYPE_SYMBOL))
    INT_FATAL(ast, "Unexpected attempt to eviscerate a global type symbol.");
}

#define clean_gvec(type)                        \
  int i##type = 0;                              \
  forv_Vec(type, ast, g##type##s) {             \
    if (isAlive(ast) || isRootModuleWithType(ast, type)) { \
      g##type##s.v[i##type++] = ast;            \
    } else {                                    \
      if (E_##type == E_VarSymbol)              \
        remove_weak_links(toVarSymbol(ast));    \
      trace_remove(ast, 'x');                   \
      delete ast; ast = 0;                      \
    }                                           \
  }                                             \
  g##type##s.n = i##type


static void clean_modvec(Vec<ModuleSymbol*>& modvec) {
  int aliveMods = 0;

  forv_Vec(ModuleSymbol, mod, modvec) {
    if (isAlive(mod) || isRootModuleWithType(mod, ModuleSymbol)) {
      modvec.v[aliveMods++] = mod;
    }
  }

  modvec.n = aliveMods;
}

void cleanAst() {
  std::vector<Type*> keysToRm;

  for (auto it = serializeMap.begin() ; it != serializeMap.end() ; it++) {
    if (!isAlive(it->first)) {
      keysToRm.push_back(it->first);
    }
  }

  for_vector (Type, key, keysToRm) {
    serializeMap.erase(key);
  }

  std::set<FunctionType*> functionTypesToRemove;

  //
  // clear back pointers to dead ast instances
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (auto ft = toFunctionType(ts->type)) {
      if (!ft->inTree()) functionTypesToRemove.insert(ft);
    }

    for (int i = 0; i < ts->type->methods.n; i++) {
      FnSymbol* method = ts->type->methods.v[i];

      if (method && !isAliveQuick(method)) {
        ts->type->methods.v[i] = NULL;
      }

      if (AggregateType* ct = toAggregateType(ts->type)) {

        if (ct->hasDestructor()                  == true &&
            isAliveQuick(ct->getDestructor())    == false) {
          ct->setDestructor(NULL);
        }
      }
    }

    if (AggregateType* at = toAggregateType(ts->type)) {
      for (int i = 0; i < at->dispatchChildren.n; i++) {
        if (AggregateType* type = at->dispatchChildren.v[i]) {
          if (isAlive(type) == false) {
            at->dispatchChildren.v[i] = NULL;
          }
        }
      }

      //
      // If an internal aggregate type is being deleted, set its global
      // handle to NULL. See #15619.
      //
      if (!isAlive(at)) {
        if (at == dtBytes) dtBytes = NULL;
        if (at == dtString) dtString = NULL;
        if (at == dtLocale) dtLocale = NULL;
        if (at == dtOwned) dtOwned = NULL;
        if (at == dtShared) dtShared = NULL;
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    auto ft = toFunctionType(fn->type);
    if (ft && !ft->inTree()) {
      // Set the type to 'nullptr', it can be recomputed later.
      fn->type = nullptr;

      // We should be removing the type.
      INT_ASSERT(functionTypesToRemove.find(ft) !=
                 functionTypesToRemove.end());
    }
  }

  removedIterResumeLabels.clear();

  copiedIterResumeGotos.clear();

  // clean the other module vectors, without deleting the ast instances
  // (they will be deleted with the clean_gvec call for ModuleSymbols.)
  clean_modvec(allModules);
  clean_modvec(userModules);

  //
  // clean global vectors and delete dead ast instances
  //
  foreach_ast(clean_gvec);

  // Finally, clean up any 'FunctionType' since they do not have a 'gvec'.
  for (auto ast : functionTypesToRemove) {
    trace_remove(ast, 'x');
    delete ast;
  }
}


void destroyAst() {
  #define destroy_gvec(type)                    \
    forv_Vec(type, ast, g##type##s) {           \
      trace_remove(ast, 'z');                   \
      delete ast;                               \
    }
  foreach_ast(destroy_gvec);
}


void
verify() {
  verifyRemovedIterResumeGotos();
  verifyCopiedIterResumeGotos();

  #define verify_gvec(type)                       \
    forv_Vec(type, ast, g##type##s) {             \
     if (isAlive(ast)) {                          \
      ast->verify();                              \
     }                                            \
    }
  foreach_ast(verify_gvec);

  // rootModule does not pass isAlive(), yet is "alive" - needs to be  verified
  rootModule->verify();
}


int breakOnID = -1;
int breakOnRemoveID = -1;

int lastNodeIDUsed() {
  return uid - 1;
}


// This is here so that we can break on the creation of a particular
// BaseAST instance in gdb.
static void checkid(int id) {
  if (id == breakOnID) {
    debuggerBreakHere();
  }
}


BaseAST::BaseAST(AstTag type) :
  astTag(type),
  id(uid++),
  astloc(yystartlineno, yyfilename)
{
  checkid(id);
  if (!astloc.isEmpty()) {
    // OK, set from yyfilename
  } else {
    if (!currentAstLoc.isEmpty()) {
      astloc = currentAstLoc;
    } else {
      // neither yy* nor currentAstLoc are set
      if (developer || fVerify) {
        INT_FATAL("no line number available");
      } else {
        astloc = astlocT(0, astr("[file unknown]"));
      }
    }
  }
}

const std::string BaseAST::tabText = "   ";

int BaseAST::linenum() const {
  return astloc.lineno();
}

const char* BaseAST::fname() const {
  return astloc.filename();
}

const char* BaseAST::stringLoc(void) const {
  return astloc.stringLoc();
}


ModuleSymbol* BaseAST::getModule() {
  ModuleSymbol* retval = NULL;

  if (ModuleSymbol* x = toModuleSymbol(this)) {
    retval = x;

  } else if (Type* x = toType(this)) {
    if (x->symbol != NULL)
      retval = x->symbol->getModule();

  } else if (Symbol* x = toSymbol(this)) {
    if (x->defPoint != NULL)
      retval = x->defPoint->getModule();

  } else if (Expr* x = toExpr(this)) {
    if (x->parentSymbol != NULL)
      retval = x->parentSymbol->getModule();

  } else {
    INT_FATAL(this, "Unexpected case in BaseAST::getModule()");
  }

  return retval;
}

bool BaseAST::wasResolvedEarly() {
  if (auto sym = toSymbol(this)) {
    if (sym->hasFlag(FLAG_RESOLVED_EARLY)) {
      INT_ASSERT(!isModuleSymbol(sym));
      return true;
    }
  }

  if (auto t = toType(this)) {
    if (t->symbol->hasFlag(FLAG_RESOLVED_EARLY)) return true;
  }

  // Check to see if the AST is in a dyno-generated function symbol.
  auto fn = this->getFunction();
  if (fn && fn->hasFlag(FLAG_RESOLVED_EARLY)) return true;

  return false;
}

bool BaseAST::isRef() {
  return this->qualType().isRef();
}

bool BaseAST::isWideRef() {
  return this->qualType().isWideRef();
}

bool BaseAST::isRefOrWideRef() {
  return this->qualType().isRefOrWideRef();
}

FnSymbol* BaseAST::getFunction() {
  BaseAST* cur = this;
  while (cur != NULL) {
    // base cases
    if (ModuleSymbol* x = toModuleSymbol(cur))
      return x->initFn;
    else if (FnSymbol* x = toFnSymbol(cur))
      return x;
    // inductive cases
    else if (Type* x = toType(cur))
      cur = x->symbol;
    else if (Symbol* x = toSymbol(cur))
      cur = x->defPoint;
    else if (Expr* x = toExpr(cur))
      cur = x->parentSymbol;
    else
      INT_FATAL(this, "Unexpected case in BaseAST::getFunction()");
  }
  return NULL;
}


Type* BaseAST::getValType() {
  Type* type = typeInfo();
  INT_ASSERT(type);
  if (type->symbol->hasFlag(FLAG_REF))
    return type->getField("_val")->type;
  else if (type->symbol->hasFlag(FLAG_WIDE_REF))
    return type->getField("addr")->getValType();
  else
    return type;
}

Type* BaseAST::getRefType() {
  Type* type = typeInfo();
  INT_ASSERT(type);
  if (type->symbol->hasFlag(FLAG_REF))
    return type;
  else if (type->symbol->hasFlag(FLAG_WIDE_REF))
    return type->getField("addr")->type;
  else
    return type->refType;
}

Type* BaseAST::getWideRefType() {
  Type* type = typeInfo();
  INT_ASSERT(type);
  if (type->symbol->hasFlag(FLAG_REF))
    return wideRefMap.get(type);
  else if (type->symbol->hasFlag(FLAG_WIDE_REF))
    return type;
  else
    return wideRefMap.get(type->getRefType());
}

const char* BaseAST::astTagAsString() const {
  switch (astTag) {
    case E_TemporaryConversionThunk: return "TemporaryConversionThunk";
    case E_TemporaryConversionType:  return "TemporaryConversionType";
    case E_PrimitiveType:      return "PrimitiveType";
    case E_ConstrainedType:    return "ConstrainedType";
    case E_EnumType:           return "EnumType";
    case E_AggregateType:      return "AggregateType";
    case E_FunctionType:       return "FunctionType";
    case E_DecoratedClassType: return "DecoratedClassType";
    case E_ModuleSymbol:       return "ModuleSymbol";
    case E_VarSymbol:          return "VarSymbol";
    case E_ArgSymbol:          return "ArgSymbol";
    case E_ShadowVarSymbol:    return "ShadowVarSymbol";
    case E_TypeSymbol:         return "TypeSymbol";
    case E_FnSymbol:           return "FnSymbol";
    case E_InterfaceSymbol:    return "InterfaceSymbol";
    case E_EnumSymbol:         return "EnumSymbol";
    case E_LabelSymbol:        return "LabelSymbol";
    case E_TemporaryConversionSymbol: return "TemporaryConversionSymbol";
    case E_SymExpr:            return "SymExpr";
    case E_UnresolvedSymExpr:  return "UnresolvedSymExpr";
    case E_DefExpr:            return "DefExpr";
    case E_CallExpr:           return "CallExpr";
    case E_ContextCallExpr:    return "ContextCallExpr";
    case E_LoopExpr:           return "LoopExpr";
    case E_NamedExpr:          return "NamedExpr";
    case E_IfcConstraint:      return "IfcConstraint";
    case E_IfExpr:             return "IfExpr";
    case E_UseStmt:            return "UseStmt";
    case E_ImportStmt:         return "ImportStmt";
    case E_CondStmt:           return "CondStmt";
    case E_GotoStmt:           return "GotoStmt";
    case E_DeferStmt:          return "DeferStmt";
    case E_ForallStmt:         return "ForallStmt";
    case E_TryStmt:            return "TryStmt";
    case E_ForwardingStmt:     return "ForwardingStmt";
    case E_CatchStmt:          return "CatchStmt";
    case E_ImplementsStmt:     return "ImplementsStmt";
    case E_ExternBlockStmt:    return "ExternBlockStmt";
    case E_BlockStmt: {
        // see AST_CHILDREN_CALL
        const BlockStmt* stmt = toConstBlockStmt(this);
        if (false) return "";
        else if (stmt->isCForLoop())     return "CForLoop";
        else if (stmt->isForLoop())      return "ForLoop";
        else if (stmt->isParamForLoop()) return "ParamForLoop";
        else if (stmt->isWhileDoStmt())  return "WhileDoStmt";
        else if (stmt->isDoWhileStmt())  return "DoWhileStmt";
        else                             return "BlockStmt";
      }
  }
  return "BaseAST??";
}


void registerModule(ModuleSymbol* mod) {
  switch (mod->modTag) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
  case MOD_INTERNAL:
    if (strcmp(mod->name, "_root"))
      allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}

static Symbol* lookupTransitively(SymbolMap* map, Symbol* sym) {
  Symbol* x = map->get(sym);
  if (!x) return x;

  // If the symbol is re-mapped again (e.g., x was y and y was z),
  // we need to keep looking until we find the final symbol.
  while (Symbol* y = map->get(x)) {
    // Detect naive cycles. Note that this will not find multi-step cycles,
    // but they shouldn't come up. If they do, might need to switch this
    // to a tortoise-and-hare algorithm (Floyd's?)
    if (y == x) break;
    x = y;
  }

  return x;
}

#define SUB_SYMBOL(x)                                   \
  do {                                                  \
    if (x)                                              \
      if (Symbol* y = lookupTransitively(map, x))       \
        x = y;                                          \
  } while (0)

#define SUB_TYPE(x)                                       \
  do {                                                    \
    if (x)                                                \
      if (Symbol* y = lookupTransitively(map, x->symbol)) \
        x = y->type;                                      \
  } while (0)

void update_symbols(BaseAST* ast, SymbolMap* map) {
  if (SymExpr* sym_expr = toSymExpr(ast)) {
    if (sym_expr->symbol()) {
      if (Symbol* y = lookupTransitively(map, sym_expr->symbol())) {
        bool skip = false;

        // Do not replace symbols for type constructor calls
        //
        // BENHARSH TODO 2019-06-20: I think we need to do this because in
        // some cases the SymbolMap contains a mapping from the generic 'T' to
        // an instantiation of 'T'. Is that mapping necessary?
        if (isTypeSymbol(sym_expr->parentSymbol)) {
          CallExpr* call = toCallExpr(sym_expr->parentExpr);
          if (call != NULL && call->baseExpr == sym_expr) {
            if (y->getValType()->symbol->hasFlag(FLAG_TUPLE) == false &&
                y->getValType() != dtUnknown &&
                sym_expr->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
              skip = true;
            }
          }
        }

        if (!skip) {
          sym_expr->setSymbol(y);
        }
      }
    }


  } else if (DefExpr* defExpr = toDefExpr(ast)) {
    SUB_TYPE(defExpr->sym->type);

  } else if (LoopStmt* ls = toLoopStmt(ast)) {
    LabelSymbol* breakLabel    = ls->breakLabelGet();
    LabelSymbol* continueLabel = ls->continueLabelGet();

    if (breakLabel != 0) {
      if (LabelSymbol* y = toLabelSymbol(map->get(breakLabel))) {
        ls->breakLabelSet(y);
      }
    }

    if (continueLabel != 0) {
      if (LabelSymbol* y = toLabelSymbol(map->get(continueLabel))) {
        ls->continueLabelSet(y);
      }
    }

  } else if (ForallStmt* forall = toForallStmt(ast)) {
    if (forall->fContinueLabel) {
      if (LabelSymbol* y = toLabelSymbol(lookupTransitively(map, forall->fContinueLabel)))
          forall->fContinueLabel = y;
    } else if (forall->fErrorHandlerLabel) {
      if (LabelSymbol* y = toLabelSymbol(lookupTransitively(map, forall->fErrorHandlerLabel)))
          forall->fErrorHandlerLabel = y;
    }
  } else if (VarSymbol* ps = toVarSymbol(ast)) {
    SUB_TYPE(ps->type);

  } else if (FnSymbol* ps = toFnSymbol(ast)) {
    SUB_TYPE(ps->type);
    SUB_TYPE(ps->retType);
    SUB_SYMBOL(ps->_this);

  } else if (ArgSymbol* ps = toArgSymbol(ast)) {
    SUB_TYPE(ps->type);

  } else if (ShadowVarSymbol* ss = toShadowVarSymbol(ast)) {
    SUB_TYPE(ss->type);
  }

  AST_CHILDREN_CALL(ast, update_symbols, map);
}

GenRet baseASTCodegen(BaseAST* ast)
{
  GenRet ret = ast->codegen();
  if (!ret.chplType)
    ret.chplType = ast->typeInfo();
  ret.isUnsigned = ! is_signed(ret.chplType);
  return ret;
}

GenRet baseASTCodegenInt(int x)
{
  return baseASTCodegen(new_IntSymbol(x, INT_SIZE_64));
}

GenRet baseASTCodegenString(const char* str)
{
  return baseASTCodegen(new_CStringSymbol(str));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isLoopStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isLoopStmt()) ? true : false;
}

bool isWhileStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isWhileStmt()) ? true : false;
}

bool isWhileDoStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isWhileDoStmt()) ? true : false;
}

bool isDoWhileStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isDoWhileStmt()) ? true : false;
}

bool isParamForLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isParamForLoop()) ? true : false;
}

bool isForLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isForLoop()) ? true : false;
}

bool isCoforallLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isCoforallLoop()) ? true : false;
}

bool isCForLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isCForLoop()) ? true : false;
}
