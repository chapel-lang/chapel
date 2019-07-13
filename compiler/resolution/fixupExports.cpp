/*
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

#include "fixupExports.h"

#include "astutil.h"
#include "baseAST.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <map>
#include <vector>

static std::map<FnSymbol*, FnSymbol*> wrapperMap;
static std::map<const char*, FnSymbol*> conversionCallMap;

static void attemptFixups(FnSymbol* fn);
static Type* maybeUnwrapRef(Type* t);
static bool needsFixup(Type* t);
static bool validateFormalIntent(FnSymbol* fn, ArgSymbol* as);
static bool validateReturnIntent(FnSymbol* fn);
static FnSymbol* createWrapper(FnSymbol* fn);
static Type* getTypeForFixup(Type* t, bool ret=false);
static const char* getConversionCallName(Type* srctype, Type* dsttype);
static FnSymbol* getConversionCall(Type* srctype, Type* dsttype);
static FnSymbol* resolveConversionCall(Type* srctype, Type* dsttype);
static VarSymbol* fixupFormal(FnSymbol* wrapper, int idx);
static void changeRetType(FnSymbol* wrapper);
static void insertUnwrappedCall(FnSymbol* wrapper, FnSymbol* fn,
                                const std::vector<VarSymbol*>& tmps);

//
// Some type information is lost during this pass (IE, converting `string`
// formals to `c_string`), and this information might be useful when
// generating code in later passes.
//
FnSymbol* getUnwrappedFunction(FnSymbol* wrapper) {
  std::map<FnSymbol*, FnSymbol*>::iterator it;
  if ((it = wrapperMap.find(wrapper)) != wrapperMap.end()) {
    return it->second;
  }
  return NULL;
}

void fixupExportedFunctions(const std::vector<FnSymbol*>& fns) {
  for_vector(FnSymbol, fn, fns) {
    fixupExportedFunction(fn);
  }
}

void fixupExportedFunction(FnSymbol* fn) {
  SET_LINENO(fn);
  attemptFixups(fn);
}

Type* getCharPtrType(void) {
  static Type* result = NULL;

  if (result != NULL) { return result; }

  const char* modname = "ExternalString";
  const char* symname = "chpl__exportTypeCharPtr";
  ModuleSymbol* esm = NULL;
  Symbol* alias = NULL;

  // Get a handle to the ExternalString module.
  forv_Vec(ModuleSymbol, md, allModules) {
    if (md->modTag == MOD_INTERNAL && !strcmp(md->name, modname)) {
      esm = md;
      break;
    }
  }

  INT_ASSERT(esm != NULL);

  // Get a handle to the c_ptr(c_char) type from our global alias.
  for_alist(expr, esm->block->body) {
    DefExpr* de = toDefExpr(expr);
    if (de) {
      Symbol* sym = de->sym;
      if (!strcmp(sym->name, symname)) {
        alias = sym;
        break;
      }
    }
  }

  SymExpr* rctx = new SymExpr(alias);

  // Insert into tree and resolve, just in case.
  chpl_gen_main->insertAtHead(rctx);
  resolveExpr(rctx);
  rctx->remove();

  INT_ASSERT(alias->type != dtUnknown);
  result = alias->type;

  return result;
}

static void attemptFixups(FnSymbol* fn) {
  std::vector<VarSymbol*> tmps;
  FnSymbol* wrapper = NULL;

  if (!fn->hasFlag(FLAG_EXPORT)) { return; }

  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);

    if (needsFixup(as->type) && validateFormalIntent(fn, as)) {
      if (wrapper == NULL) { wrapper = createWrapper(fn); }
      VarSymbol* tmp = fixupFormal(wrapper, i);
      INT_ASSERT(tmp != NULL);
      tmps.push_back(tmp);
    } else {
      // Push back a NULL sentry value for unconverted formals.
      tmps.push_back(NULL);
    }
  }

  if (needsFixup(fn->retType) && validateReturnIntent(fn)) {
    if (wrapper == NULL) { wrapper = createWrapper(fn); }
    changeRetType(wrapper);
  }

  // If a wrapper hasn't been made yet, there's nothing to do.
  if (wrapper == NULL) { return; }

  insertUnwrappedCall(wrapper, fn, tmps);

  wrapperMap[wrapper] = fn;

  return;
}

static Type* maybeUnwrapRef(Type* t) {
  if (t->isRef()) { return t->getValType(); }
  return t;
}

static bool needsFixup(Type* t) {
  Type* actual = maybeUnwrapRef(t);
  if (actual == dtString) {
    return true;
  }

  return false;
}

static bool validateFormalIntent(FnSymbol* fn, ArgSymbol* as) {
  Type* t = maybeUnwrapRef(as->type);

  //
  // TODO: If we ever add more types to these fixup routines, we really ought
  // to put these conditions in tables.
  //
  if (t == dtString) {
    IntentTag tag = as->intent;

    // TODO: After resolution, have abstract intents been normalized?
    if (tag != INTENT_CONST &&
        tag != INTENT_CONST_REF &&
        tag != INTENT_BLANK) {
      SET_LINENO(fn);
      USR_FATAL_CONT(as,  "Formal \'%s\' of type \'%s\' in exported routine "
                          "\'%s\' may only have the %s",
                          as->name, t->name(), fn->userString,
                          intentDescrString(INTENT_CONST_REF));
      return false;
    }
  }

  return true;
}

static bool validateReturnIntent(FnSymbol* fn) {
  Type* t = maybeUnwrapRef(fn->retType);
  RetTag tag = fn->retTag;

  if (t == dtString && tag != RET_VALUE) {
    SET_LINENO(fn);
    USR_FATAL_CONT(fn,  "Exported routine \'%s\' may only return the \'%s\' "
                        "type by %s",
                        fn->userString, t->name(),
                        retTagDescrString(RET_VALUE));
    return false;
  }

  return true;
}

static FnSymbol* createWrapper(FnSymbol* fn) {
  FnSymbol* result = fn->copy();
  fn->defPoint->insertAfter(new DefExpr(result));
  result->body->replace(new BlockStmt());
  fn->removeFlag(FLAG_EXPORT);
  return result;
}

//
// TODO: Do we need explicit cases for reference types here?
//
static Type* getTypeForFixup(Type* t, bool ret) {
  if (t == dtString) {
    Type* result = ret ? getCharPtrType() : dtStringC;
    return result;
  } else {
    INT_FATAL("Unsupported type for formal in: %s", __FUNCTION__);
  }

  // Should never reach here.
  return NULL;
}

//
// TODO: Can insert non-copying versions when we are doing multi-locale
// interop.
//
static const char* getConversionCallName(Type* srctype, Type* dsttype) {
  const char* result = NULL;

  //
  // TODO (from @benharsh): An alternative to this is to have a single
  // function and rely on formal constraints to resolve correctly. For
  // example...
  //
  // proc chpl__exportWrapper(val: string, type ret:c_string): ret
  // proc chpl__exportWrapper(val: string, type ret:c_ptr(c_char)): ret
  // ...
  //
  if (srctype == dtString && dsttype == dtStringC) {
    result = "chpl__exportStringToConstCharPtr";
  } else if (srctype == dtStringC && dsttype == dtString) {
    result = "chpl__exportConstCharPtrToString";
  } else if (srctype == dtString && dsttype == getCharPtrType()) {
    result = "chpl__exportStringToCharPtr";
  } else if (srctype == getCharPtrType() && dsttype == dtString) {
    result = "chpl__exportCharPtrToString";
  } else {
    INT_FATAL("Bad types for conversion call in: %s", __FUNCTION__);
  }

  return result;
}

static FnSymbol* getConversionCall(Type* srctype, Type* dsttype) {
  const char* name = getConversionCallName(srctype, dsttype);
  INT_ASSERT(name != NULL);

  std::map<const char*, FnSymbol*>::iterator it;
  it = conversionCallMap.find(name);
  if (it != conversionCallMap.end()) { return it->second; }

  FnSymbol* result = resolveConversionCall(srctype, dsttype);
  conversionCallMap[name] = result;

  return result;
}

//
// This assumes that the names of the conversion calls are unique and
// _are not_ being overloaded.
//
static FnSymbol* resolveConversionCall(Type* srctype, Type* dsttype) {
  const char* name = getConversionCallName(srctype, dsttype);

  BlockStmt* block = new BlockStmt();
  VarSymbol* tmp = newTemp(srctype);
  CallExpr* call = new CallExpr(name, tmp);
  FnSymbol* result = NULL;
 
  // Modifications to `chpl_gen_main` are only temporary.
  chpl_gen_main->body->insertAtHead(block);

  block->insertAtTail(new DefExpr(tmp));
  block->insertAtTail(call);

  tryResolveCall(call);

  result = call->resolvedFunction();
  INT_ASSERT(result != NULL);

  resolveFunction(result);

  call->remove();
  tmp->defPoint->remove();
  block->remove();

  return result;
}

static VarSymbol* fixupFormal(FnSymbol* wrapper, int idx) {
  ArgSymbol* as = wrapper->getFormal(idx);
  Type* otype = as->type;
  Type* wtype = getTypeForFixup(otype);

  as->type = wtype;

  //
  // Now create a new temporary, and assign it the result of a conversion call
  // on the formal we are performing a fixup on.
  //

  VarSymbol* tmp = newTemp(otype);
  VarSymbol* result = tmp;

  wrapper->body->insertAtTail(new DefExpr(tmp));

  FnSymbol* conv = getConversionCall(wtype, otype);
  INT_ASSERT(conv != NULL);
  
  CallExpr* call = new CallExpr(conv, as);
  CallExpr* move = new CallExpr(PRIM_MOVE, tmp, call);
  wrapper->body->insertAtTail(move);
  
  return result;
}

static void changeRetType(FnSymbol* wrapper) {
  Type* otype = wrapper->retType;
  Type* wtype = getTypeForFixup(otype, true);

  wrapper->retType = wtype;

  return;
}

static void insertUnwrappedCall(FnSymbol* wrapper, FnSymbol* fn,
                                const std::vector<VarSymbol*>& tmps) {
  bool isVoid = (fn->retType == dtVoid);

  VarSymbol* utmp = isVoid ? NULL : newTemp(fn->retType);
  VarSymbol* rtmp = isVoid ? NULL : newTemp(wrapper->retType);

  CallExpr* call = new CallExpr(fn);
  BlockStmt* wbody = wrapper->body;

  if (!isVoid) {
    wbody->insertAtTail(new DefExpr(utmp));
    wbody->insertAtTail(new DefExpr(rtmp));
  }

  //
  // Loop through a list of VarSymbols for formals, supplying the VarSymbol
  // as the actual to the call, or the original ArgSymbol for that formal if
  // the slot is NULL.
  //
  for (size_t i = 0; i < tmps.size(); i++) {
    VarSymbol* tmp = tmps[i];
    if (tmp != NULL) {
      call->insertAtTail(tmp);
    } else {
      int idx = (int) i + 1;
      ArgSymbol* as = wrapper->getFormal(idx);
      call->insertAtTail(as);
    }
  }

  if (!isVoid) {
    CallExpr* move = new CallExpr(PRIM_MOVE, utmp, call);
    wbody->insertAtTail(move);
  } else {
    wbody->insertAtTail(call);
  }

  //
  // If the call is void, emit a void return statement, then leave.
  //
  if (isVoid) {
    CallExpr* ret = new CallExpr(PRIM_RETURN, gVoid);
    wbody->insertAtTail(ret);
    return;
  }

  //
  // Possibly perform a conversion call and store the result in `rtmp`. If no
  // conversion call was needed, then `utmp` and `rtmp` have the same type,
  // so just move the first into the second.
  //
  if (needsFixup(fn->retType)) {
    Type* src = fn->retType;
    Type* dst = wrapper->retType;
   
    FnSymbol* conv = getConversionCall(src, dst);

    CallExpr* call = new CallExpr(conv, utmp);
    CallExpr* move = new CallExpr(PRIM_MOVE, rtmp, call);

    wbody->insertAtTail(move);
  } else {
    CallExpr* move = new CallExpr(PRIM_MOVE, rtmp, utmp);
    wbody->insertAtTail(move);
  }

  // Return the result temporary.
  CallExpr* ret = new CallExpr(PRIM_RETURN, rtmp);
  wbody->insertAtTail(ret);

  return;
}

