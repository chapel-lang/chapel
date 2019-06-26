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

#include "passes.h"

#include "astutil.h"
#include "baseAST.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <iostream>
#include <map>

static void attemptFixups(FnSymbol* fn);
static bool needsFixup(ArgSymbol* as);
static bool needsFixup(Type* t);
static FnSymbol* createWrapper(FnSymbol* fn);
static Type* getTypeForFixup(Type* t);
static const char* getConversionCallName(Type* from, Type* to);
static void fixupFormal(FnSymbol* wrapper, int idx);
static void changeRetType(FnSymbol* wrapper, std::vector<VarSymbol*> tmps);
static void insertConversionCalls(FnSymbol* wrapper, FnSymbol* fn);
static void insertUnwrappedCall(FnSymbol* wrapper, FnSymbol* fn);
static void updateCallsTo(FnSymbol* fn, FnSymbol* wrapper);

void fixupExportedFunctions(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_EXPORT)) { attemptFixups(fn); }
  }
}

static void attemptFixups(FnSymbol* fn) {
  std::vector<VarSymbol*> tmps;
  FnSymbol* wrapper = NULL;

  if (not fn->hasFlag(FLAG_EXPORT)) { return; }

  for (int i = 1; i < fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);
    if (not needsFixup(as)) {
      // Push back a NULL sentry value for unconverted formals.
      tmps.push_back(NULL);
      continue; 
    } else {
      if (wrapper == NULL) { wrapper = createWrapper(fn); }
      VarSymbol* tmp = fixupFormal(wrapper, i);
      tmps.push_back(tmp);
    }
  }

  if (needsFixup(fn->retType)) {
    if (wrapper == NULL) { wrapper = createWrapper(fn); }
    changeRetType(wrapper);
  }

  // If a copy hasn't been made yet, there's nothing to change.
  if (wrapper == NULL) { return; }
 
  insertUnwrappedCall(wrapper, fn, tmps);

  normalize(wrapper);
  resolveFunction(wrapper);

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
  result->replaceBodyStmtsWithStmt(new BlockStmt());
  INT_ASSERT(fn->hasFlag(FLAG_EXPORT));
  fn->removeFlag(FLAG_EXPORT);
  return result;
}

static Type* getTypeForFixup(Type* t) {
  if (t == dtString) {
    return dtStringC;
  } else {
    INT_FATAL("Unsupported type for formal in: %s", __FUNCTION__);
  }

  return NULL;
}

//
// TODO: These conversion calls should be namespaced? IE, the call should be
// to "ExternalString.convertToConstChar" just in case.
//
static const char* getConversionCallName(Type* srctype, Type* dsttype) {
  const char* result = NULL;

  if (srctype == dtString && dsttype == dtStringC) {
    result = "ExternalString.convert";
  } else if (srctype == dtStringC && dsttype == dtString) {
    result = "chpl__exportConvertConstCharToString";
  } else {
    INT_FATAL("Bad types for conversion call in: %s", __FUNCTION__);
  }

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

  VarSymbol* tmp = fn->newTemp(otype);
  VarSymbol* result = tmp;

  // Get conversion call from wrapper type to original type.
  const char* convSym = getConversionCallName(wtype, otype);
  INT_ASSERT(convSym != NULL);

  CallExpr* conv = new CallExpr(new UnresolvedSymExpr(convSym), as);
  CallExpr* move = new CallExpr(PRIM_MOVE, tmp, conv);
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
                                std::vector<VarSymbol*> tmps) {
  VarSymbol* utmp = wrapper->newTemp(fn->retType);
  VarSymbol* rtmp = wrapper->newTemp(wrapper->retType);
  BlockExpr* body = wrapper->body;

  CallExpr* unwrappedCall = new CallExpr(fn);

  for (int i = 0; i < tmps.size(); i++) {
    VarSymbol* tmp = tmps[i];
    if (tmp != NULL) {
      unwrappedCall.insertAtTail(tmp);
    } else {
      int idx = i + 1;
      ArgSymbol* as = wrapper->getFormal(idx);
      unwrappedCall.insertAtTail(as);
    }
  }

  // Move the result of the unwrapped call into unwrapped temp.
  body->insertAtTail(new CallExpr(PRIM_MOVE, utmp, unwrappedCall));

  if (needsFixup(fn->retType)) {
    // Get conversion call from original type to wrapper type.
    Type* src = fn->retType;
    Type* dst = wrapper->retType;
    const char* convSym = getConversionCallName(src, dst);

    //
    // Issue a conversion call on unwrapped temp, then move the resulting
    // value into the result temp.
    //
    CallExpr* conv = new CallExpr(new UnresolvedSymExpr(convSym), utmp);
    CallExpr* move = new CallExpr(PRIM_MOVE, rtmp, conv);
    body->insertAtTail(move);
  } else {
    // If no conversion call was needed, both temps have the same type.
    body->insertAtTail(new CallExpr(PRIM_MOVE, rtmp, utmp));
  }

  // Return the result temporary.
  CallExpr* ret = new CallExpr(PRIM_RETURN, rtmp);
  body->insertAtTail(ret);

  return;
}

