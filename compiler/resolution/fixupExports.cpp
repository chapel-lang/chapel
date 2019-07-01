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
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <map>
#include <vector>

static std::map<FnSymbol*, FnSymbol*> wrapperMap;
static std::map<const char*, FnSymbol*> conversionCallMap;

static void attemptFixups(FnSymbol* fn);
static bool needsFixup(ArgSymbol* as);
static bool needsFixup(Type* t);
static FnSymbol* createWrapper(FnSymbol* fn);
static Type* getCharPtrType(void);
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
// formals to `c_string`, and this information might be useful when
// generating code in later passes.
//
FnSymbol* getUnwrappedFunction(FnSymbol* wrapper) {
  std::map<FnSymbol*, FnSymbol*>& m = wrapperMap;
  if (m.find(wrapper) == m.end()) {
    return NULL;
  }
  return m[wrapper];
}

//
//
//
void fixupExportedFunctions(const std::vector<FnSymbol*>& fns) {
  getCharPtrType();
  for (size_t i = 0; i < fns.size(); i++) {
    FnSymbol* fn = fns[i];
    SET_LINENO(fn);
    attemptFixups(fn);
  }
}

//
//
//
void fixupExportedFunction(FnSymbol* fn) {
  attemptFixups(fn);
}

static void attemptFixups(FnSymbol* fn) {
  std::vector<VarSymbol*> tmps;
  FnSymbol* wrapper = NULL;

  if (not fn->hasFlag(FLAG_EXPORT)) { return; }

  // Remember that formals are one indexed!
  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);

    if (needsFixup(as)) {
      if (wrapper == NULL) { wrapper = createWrapper(fn); }
      VarSymbol* tmp = fixupFormal(wrapper, i);
      INT_ASSERT(tmp != NULL);
      tmps.push_back(tmp);
    } else {
      // Push back a NULL sentry value for unconverted formals.
      tmps.push_back(NULL);
    }
  }

  if (needsFixup(fn->retType)) {
    if (wrapper == NULL) { wrapper = createWrapper(fn); }
    changeRetType(wrapper);
  }

  // If a wrapper hasn't been made yet, there's nothing to do.
  if (wrapper == NULL) { return; }

  insertUnwrappedCall(wrapper, fn, tmps);

  //
  // TODO: We can't currently call normalize/resolve on the wrapper, because
  // it is already itself a copy of a function that has been normalized
  // and resolved.
  //
  if (false) {
    normalize(wrapper);
    resolveFunction(wrapper);
  }

  //
  // Map the wrapper to the original function (the wrapper is the key).
  // This will allow us to fetch the original later and deduce type
  // info from it.
  //
  wrapperMap[wrapper] = fn;

  return;
}

static bool needsFixup(ArgSymbol* as) {
  return needsFixup(as->type);
}

static bool needsFixup(Type* t) {
  if (t == dtString) {
    return true;
  }

  return false;
}

//
//  - Create a wrapper function that has the same signature as the original.
//  - The wrapper function should have an empty body.
//  - Mark the original as no longer being export.
//
static FnSymbol* createWrapper(FnSymbol* fn) {
  FnSymbol* result = fn->copy();
  fn->defPoint->insertAfter(new DefExpr(result));
  // TODO: Is there a better way to empty the block body?
  result->body->replace(new BlockStmt());
  INT_ASSERT(fn->hasFlag(FLAG_EXPORT));
  fn->removeFlag(FLAG_EXPORT);
  return result;
}

static Type* getCharPtrType(void) {
  static Type* result = NULL;
  if (result != NULL) { return result; }

  VarSymbol* tmp = newTemp();
  BlockStmt* block = new BlockStmt();
  Expr* tsym = new UnresolvedSymExpr("chpl__exportTypeCharPtr");
  CallExpr* move = new CallExpr(PRIM_MOVE, tmp, tsym);

  chpl_gen_main->insertAtHead(new DefExpr(block));


  result = resolveTypeAlias(texpr);
  INT_ASSERT(result != NULL);
  return result;
}

static Type* getTypeForFixup(Type* t, bool ret) {
  if (t == dtString) {
    return dtStringC;
  } else {
    INT_FATAL("Unsupported type for formal in: %s", __FUNCTION__);
  }

  return NULL;
}

//
// TODO: These conversion calls should be namespaced? IE, the call should be
// to "ExternalString.convertToConstChar" just to be safe.
// TODO: Can insert non-copying versions when we are doing multi-locale
// interop.
//
static const char* getConversionCallName(Type* srctype, Type* dsttype) {
  const char* result = NULL;

  if (srctype == dtString && dsttype == dtStringC) {
    result = "chpl__exportConvertToConstChar";
  } else if (srctype == dtStringC && dsttype == dtString) {
    result = "chpl__exportConvertToString";
  } else {
    INT_FATAL("Bad types for conversion call in: %s", __FUNCTION__);
  }

  return result;
}

static FnSymbol* getConversionCall(Type* srctype, Type* dsttype) {
  const char* name = getConversionCallName(srctype, dsttype);
  INT_ASSERT(name != NULL);

  // Check to see if the conversion call is already resolved...
  std::map<const char*, FnSymbol*>& m = conversionCallMap;
  std::map<const char*, FnSymbol*>::iterator it;
  it = m.find(name);
  if (it != m.end()) { return it->second; }

  // Resolve it manually and add it to the map.
  FnSymbol* result = resolveConversionCall(srctype, dsttype);
  m[name] = result;

  return result;
}

//
// TODO: This assumes that the names of the conversion calls are unique and
// _are not_ being overloaded.
//
static FnSymbol* resolveConversionCall(Type* srctype, Type* dsttype) {
  const char* name = getConversionCallName(srctype, dsttype);
  INT_ASSERT(name != NULL);

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

  //
  // These are not called by user code paths, so we must resolve them now or
  // we will get AST errors during late stage checking.
  //
  normalize(result);
  resolveFunction(result);

  call->remove();
  tmp->defPoint->remove();
  block->remove();

  return result;
}

//
//  - Change the type of the formal at "idx" based on conversion rules.
//  - Get the conversion routine to call based on formal's to/from types.
//  - Insert "tmp = conversion(formal)" into the AST.
//  > Return the temporary containing result of conversion call.
//
static VarSymbol* fixupFormal(FnSymbol* wrapper, int idx) {
  ArgSymbol* as = wrapper->getFormal(idx);

  Type* otype = as->type;
  INT_ASSERT(needsFixup(otype));

  Type* wtype = getTypeForFixup(otype);
  INT_ASSERT(wtype != NULL);

  // Change the type of the formal.
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
  INT_ASSERT(needsFixup(otype));

  Type* wtype = getTypeForFixup(otype);
  INT_ASSERT(wtype != NULL);

  // Change the return type.
  wrapper->retType = wtype;

  return;
}

//
//  - Create a temporary to contain result of unwrapped call.
//  - Create new CallExpr for unwrapped call, pointing to unwrapped routine.
//  - Loop through temps for each formal:
//    > If NULL, fetch the ArgSymbol at that index, and call "insertAtTail"
//      on the CallExpr.
//    > Else, call "insertAtTail" with the tmp.
//  - Create a new CallExpr that is a PRIM_MOVE of the unwrapped call, with
//    the unwrapped temporary as the target.
//  - Issue a conversion call on the result temporary, if necessary.
//  - 
//
//
static void insertUnwrappedCall(FnSymbol* wrapper, FnSymbol* fn,
                                const std::vector<VarSymbol*>& tmps) {
  bool isVoid = (fn->retType == dtVoid);
  VarSymbol* utmp = (not isVoid) ? newTemp(fn->retType) : NULL;
  VarSymbol* rtmp = (not isVoid) ? newTemp(wrapper->retType) : NULL;
  CallExpr* call = new CallExpr(fn);
  BlockStmt* wbody = wrapper->body;

  if (not isVoid) {
    wbody->insertAtTail(new DefExpr(utmp));
    wbody->insertAtTail(new DefExpr(rtmp));
  }

  //
  // Loop through a list of VarSymbols for formals, supplying the VarSymbol
  // as the actual to the call, or the original ArgSymbol for that formal if
  // the slot is NULL (no conversion call, so no temporary).
  //
  for (int i = 0; i < tmps.size(); i++) {
    VarSymbol* tmp = tmps[i];
    if (tmp != NULL) {
      call->insertAtTail(tmp);
    } else {
      int idx = i + 1;
      ArgSymbol* as = wrapper->getFormal(idx);
      call->insertAtTail(as);
    }
  }

  if (not isVoid) {
    CallExpr* move = new CallExpr(PRIM_MOVE, utmp, call);
    wbody->insertAtTail(move);
  } else {
    wbody->insertAtTail(call);
  }

  // If the call is void, then we're done.
  if (isVoid) { return; }

  //
  // Possibly perform a conversion call and store the result in `rtmp`. If no
  // conversion call was needed, then `utmp` and `rtmp` have the same type,
  // so just move the first into the second.
  //
  if (needsFixup(fn->retType)) {
    Type* src = fn->retType;
    Type* dst = wrapper->retType;
   
    FnSymbol* conv = getConversionCall(src, dst);
    INT_ASSERT(conv != NULL);

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

