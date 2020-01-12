/*
 * Copyright 2004-2020 Cray Inc.
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
#include "library.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

#include <map>
#include <utility>
#include <vector>

static std::map<FnSymbol*, FnSymbol*> wrapperMap;
static std::map<ArgSymbol*, ArgSymbol*> wrapperArgMap;
static std::map<FnSymbol*, Type*> wrapperRetTypeMap;

Type* exportTypeChplByteBuffer = NULL;

std::set<FnSymbol*> exportedStrRets;

static void resolveExportWrapperTypeAliases(void);
static void attemptFixups(FnSymbol* fn);
static Type* maybeUnwrapRef(Type* t);
static Type* resolveTypeAlias(const char* mod, const char* alias);
static bool needsFixup(Type* t);
static bool validateFormalIntent(FnSymbol* fn, ArgSymbol* as);
static bool validateReturnIntent(FnSymbol* fn);
static FnSymbol* createWrapper(FnSymbol* fn);
static Type* getTypeForFixup(Type* t, bool ret=false);
static bool isTypeThatMightRequireCopy(Type* t);
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

ArgSymbol* getUnwrappedArg(ArgSymbol* arg) {
  std::map<ArgSymbol*, ArgSymbol*>::iterator it;
  if ((it = wrapperArgMap.find(arg)) != wrapperArgMap.end()) {
    return it->second;
  }
  return NULL;
}

Type* getUnwrappedRetType(FnSymbol* fn) {
  std::map<FnSymbol*, Type*>::iterator it;
  if ((it = wrapperRetTypeMap.find(fn)) != wrapperRetTypeMap.end()) {
    return it->second;
  }
  return NULL;
}

static void resolveExportWrapperTypeAliases(void) {
  static bool resolved = false;
  if (resolved) { return; }
  resolved = true;

  const char* mod = "ExportWrappers";

  exportTypeChplByteBuffer =
    resolveTypeAlias(mod, "chpl__exportTypeChplByteBuffer");

  return;
}

void fixupExportedFunctions(const std::vector<FnSymbol*>& fns) {
  const bool isLibraryCompile = fLibraryCompile || fMultiLocaleInterop;
  if (!isLibraryCompile) { return; }

  //
  // We have to resolve type aliases for export wrapper types even if we
  // don't need to perform any fixups. This is because some of these
  // wrapper types might be used at code generation.
  //
  resolveExportWrapperTypeAliases();

  for_vector(FnSymbol, fn, fns) {
    fixupExportedFunction(fn);
  }

  return;
}

void fixupExportedFunction(FnSymbol* fn) {
  SET_LINENO(fn);
  attemptFixups(fn);
}

//
// Note that this is only valid to call at the same point in resolution where
// the "fixupExports" routine is being called. At any time after, all
// type aliases have already been resolved.
//
Type* resolveTypeAlias(const char* mod, const char* alias) {
  ModuleSymbol* msym = NULL;
  Symbol* asym = NULL;
  Type* result = NULL;

  forv_Vec(ModuleSymbol, md, allModules) {
    if (md->modTag == MOD_INTERNAL && !strcmp(md->name, mod)) {
      msym = md;
      break;
    }
  }

  if (msym == NULL) {
    INT_FATAL("Failed to find module %s in %s", mod, __FUNCTION__);
  }

  for_alist(expr, msym->block->body) {
    DefExpr* de = toDefExpr(expr);
    if (de) {
      Symbol* sym = de->sym;
      if (!strcmp(sym->name, alias)) {
        asym = sym;
        break;
      }
    }
  }

  if (asym == NULL) {
    INT_FATAL("Failed to find type alias %s in module %s in %s",
              alias, mod, __FUNCTION__);
  }

  SET_LINENO(asym);

  SymExpr* rctx = new SymExpr(asym);

  // Insert into tree and resolve, just in case.
  chpl_gen_main->insertAtHead(rctx);
  resolveExpr(rctx);
  rctx->remove();

  INT_ASSERT(asym->type != dtUnknown);
  result = asym->type;

  return result;
}

static void attemptFixups(FnSymbol* fn) {
  std::vector<VarSymbol*> tmps;
  FnSymbol* wrapper = NULL;

  if (!fn->hasFlag(FLAG_EXPORT)) { return; }

  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);

    //
    // TODO: Originally, only types that required fixups had their formal
    // intents validated. However I think it might be a better idea to
    // pull the formal/return type validation out into a separate set of
    // routines that could occur in a pass before this, for the sake of
    // clarity.
    // 
    if (validateFormalIntent(fn, as) && needsFixup(as->type)) {
      if (wrapper == NULL) { wrapper = createWrapper(fn); }
      VarSymbol* tmp = fixupFormal(wrapper, i);
      INT_ASSERT(tmp != NULL);
      tmps.push_back(tmp);

      //
      // Some types (IE, string, bytes) map to the same interop type, i.e,
      // `chpl_byte_buffer`, so we need the original ArgSymbol to
      // disambiguate them in later passes.
      //
      wrapperArgMap[wrapper->getFormal(i)] = as;
    } else {
      // Push back a NULL sentry value for unconverted formals.
      tmps.push_back(NULL);
    }
  }

  if (needsFixup(fn->retType) && validateReturnIntent(fn)) {
    if (wrapper == NULL) { wrapper = createWrapper(fn); }
    if (fMultiLocaleInterop) {
      exportedStrRets.insert(wrapper);
    }
    changeRetType(wrapper);

    // Some return types can change after lowering, so map them as well.
    wrapperRetTypeMap[wrapper] = fn->retType;
  }

  // If a wrapper hasn't been made yet, there's nothing to do.
  if (wrapper == NULL) { return; }

  insertUnwrappedCall(wrapper, fn, tmps);

  wrapperMap[wrapper] = fn;

  return;
}

static Type* maybeUnwrapRef(Type* t) {
  return t->getValType();
}

static bool needsFixup(Type* t) {
  Type* actual = maybeUnwrapRef(t);
  return actual == dtString || actual == dtBytes;
}

static bool validateFormalIntent(FnSymbol* fn, ArgSymbol* as) {
  Type* t = maybeUnwrapRef(as->type);

  //
  // TODO: If we ever add more types to these fixup routines, we really ought
  // to put these conditions in tables.
  //
  if (t == dtBytes || t == dtString || t == dtStringC
                   || t == dtExternalArray) {
    IntentTag tag = as->intent;

    bool multiloc = fMultiLocaleInterop || strcmp(CHPL_COMM, "none");

    if ((multiloc || fLibraryPython) && isUserRoutine(fn)) {
      // TODO: After resolution, have abstract intents been normalized?
      if (tag != INTENT_IN &&
          tag != INTENT_CONST_IN) {
        std::string libdesc;
        if (multiloc) {
          if (fLibraryPython) {
            libdesc = "multilocale python";
          } else {
            libdesc = "multilocale";
          }
        } else {
          libdesc = "python";
        }
        const char* typeName = (t == dtExternalArray) ? "array" : t->name();
        SET_LINENO(fn);
        if (tag == INTENT_BLANK) {
          USR_FATAL_CONT(as,  "Formal \'%s\' of type \'%s\' in exported "
                         "routine \'%s\' may not be passed by const ref in "
                         "%s libraries",
                         as->name, typeName, fn->name, libdesc.c_str());

        } else {
          USR_FATAL_CONT(as,  "Formal \'%s\' of type \'%s\' in exported "
                         "routine \'%s\' may not have the %s in "
                         "%s libraries",
                         as->name, typeName, fn->name,
                         intentDescrString(tag), libdesc.c_str());
        }
        return false;
      }
    } else if (t == dtString || t == dtBytes) {
      // TODO: After resolution, have abstract intents been normalized?
      if (tag != INTENT_CONST &&
          tag != INTENT_CONST_REF &&
          tag != INTENT_BLANK) {
        SET_LINENO(fn);
        USR_FATAL_CONT(as,  "Formal \'%s\' of type \'%s\' in exported routine "
                       "\'%s\' may not have the %s",
                       as->name, t->name(), fn->name,
                       intentDescrString(tag));
        return false;
      }
    }
  }

  return true;
}

static bool validateReturnIntent(FnSymbol* fn) {
  Type* t = maybeUnwrapRef(fn->retType);
  RetTag tag = fn->retTag;

  // Both string and bytes must be returned by value.
  if ((t == dtString || t == dtBytes) && tag != RET_VALUE) {
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
  result->addFlag(FLAG_COMPILER_GENERATED);
  result->addFlag(FLAG_EXPORT);
  fn->removeFlag(FLAG_EXPORT);
  return result;
}

//
// TODO: Do we need explicit cases for reference types here?
//
static Type* getTypeForFixup(Type* t, bool ret) {
  if (t == dtBytes || t == dtString) {
    return exportTypeChplByteBuffer;
  } else {
    INT_FATAL("Type %s is unsupported in %s", t->name(), __FUNCTION__);
  }

  // Should never reach here.
  return NULL;
}

static bool isTypeThatMightRequireCopy(Type* t) {
  Type* actual = t->getValType();
  return (isRecord(actual) || actual == dtStringC);
}

static VarSymbol* fixupFormal(FnSymbol* wrapper, int idx) {
  ArgSymbol* as = wrapper->getFormal(idx);
  Type* origt = maybeUnwrapRef(as->type);
  Type* wrapt = getTypeForFixup(origt);
  bool doCopyConversionCall = false;

  // Normalize intents for types that may need it.
  if (isTypeThatMightRequireCopy(origt)) {
    const IntentTag oldIntent = as->intent;
    IntentTag newIntent = oldIntent;

    // TODO: How would we support the INOUT or OUT intents here?
    // TODO: Can we eliminate copies for multilocale libraries?
    if (oldIntent == INTENT_IN || oldIntent == INTENT_CONST_IN) {
      doCopyConversionCall = true;
    }

    //
    // Interop wrapper records need to have the IN intent to prevent them
    // from being exposed to the end user by ref (the records may
    // contain additional fields that will assist the end user in passing
    // the type into Chapel "by reference". We attach the "export wrapper"
    // pragma to their symbols (in module code) to communicate this.
    //
    if (wrapt->symbol->hasFlag(FLAG_EXPORT_WRAPPER)) {
      newIntent = INTENT_IN;
    }

    as->intent=newIntent;
  } 

  // Adjust the wrapper type.
  as->type = wrapt;

  // Create a temporary to store the converted unwrapped formal.
  VarSymbol* result = newTemp(origt);
  wrapper->body->insertAtTail(new DefExpr(result));

  // Make a call to the appropriate conversion routine.
  CallExpr* call = new CallExpr("chpl__exportArg",
                                new_BoolSymbol(doCopyConversionCall),
                                as,
                                origt->symbol);

  // Unwrap the wrapped formal using the conversion call, store in temp.
  CallExpr* move = new CallExpr(PRIM_MOVE, result, call);
  wrapper->body->insertAtTail(move);

  // Since conversion calls are overloaded.
  resolveCallAndCallee(call);

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

    // Make a call to the appropriate conversion routine.
    CallExpr* call = new CallExpr("chpl__exportRet", utmp,
                                  wrapper->retType->symbol);

    // Move the result of the conversion call into the result temp.
    CallExpr* move = new CallExpr(PRIM_MOVE, rtmp, call);
    wbody->insertAtTail(move);

    // Resolve conversion call now, since it is overloaded.
    resolveCallAndCallee(call);
  } else {
    CallExpr* move = new CallExpr(PRIM_MOVE, rtmp, utmp);
    wbody->insertAtTail(move);
  }

  // Return the result temporary.
  CallExpr* ret = new CallExpr(PRIM_RETURN, rtmp);
  wbody->insertAtTail(ret);

  return;
}

