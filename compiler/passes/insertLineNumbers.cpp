/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#include "insertLineNumbers.h"

#include "CallExpr.h"
#include "FnSymbol.h"
#include "astutil.h"
#include "baseAST.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "virtualDispatch.h"

#include <algorithm>
#include <queue>
#include <unordered_set>

//
// insertLineNumbers() inserts line numbers and filenames into
// functions and calls so that runtime errors show line number and
// filename information.  The line numbers and filenames always point
// to user code, i.e., code that is not in a standard module or in a
// compiler-generated function.  To this effect, functions that are in
// a standard module or are compiler-generated are passed a line
// number and a filename.
//

std::vector<std::string>   gFilenameLookup;

// Caches lookups into our filename vector
std::map<std::string, int> gFilenameLookupCache;

int getFilenameLookupPosition(const std::string& name) {
  std::map<std::string, int>::iterator it = gFilenameLookupCache.find(name);
  int idx;
  if (it != gFilenameLookupCache.end()) {
    // Use cached position in the table
    idx = it->second;
  } else {
    // not found - new add new filename to the cache
    std::vector<std::string>::iterator vIt =
        std::find(gFilenameLookup.begin(), gFilenameLookup.end(), name);
    if (vIt == gFilenameLookup.end()) {
      // Not in the lookup table either, add it (This case should only be for
      // command line casts, doesn't really matter if something else sneaks in
      // though)
      gFilenameLookup.push_back(name);
      idx = gFilenameLookup.size() - 1;
    } else {
      idx = vIt - gFilenameLookup.begin();
    }
    gFilenameLookupCache.insert(std::pair<std::string, int>(name, idx));
  }
  INT_ASSERT(idx >= 0);
  INT_ASSERT((size_t)idx < gFilenameLookup.size());
  return idx;
}

// TODO refactor into a nicer home
static bool isClassMethodCall(CallExpr* call) {
  FnSymbol* fn     = call->resolvedFunction();
  bool      retval = false;

  if (fn && fn->isMethod() && fn->_this) {
    if (AggregateType* ct = toAggregateType(fn->_this->typeInfo())) {
      if (fn->numFormals()             >  0 &&
          fn->getFormal(1)->typeInfo() == fn->_this->typeInfo()) {
        if (isClass(ct) || ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          retval = true;
        }
      }
    }
  }
  return retval;
}

static bool isWrapper(FnSymbol* fn) {
  return (fn->hasFlag(FLAG_ON_BLOCK) ||
          fn->hasFlag(FLAG_BEGIN_BLOCK) ||
          fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK));
}

// TODO GLOBALS the use of virtualRootsMap and virtualChildrenMap
static Vec<FnSymbol*>* rootsOrChildren(FnSymbol* fn) {
  // TODO refactor question: why are these branches exclusive
  if (Vec<FnSymbol*>* rootFns = virtualRootsMap.get(fn)) {
    return rootFns;
  } else if (Vec<FnSymbol*>* childrenFns = virtualChildrenMap.get(fn)) {
    return childrenFns;
  }
  return nullptr;
}

// ---------- InsertLineNumbers ----------

// NOTE, shouldPreferASTLine, makeASTLine, and insertLineNumber are static
// functions so that we might unit test or otherwise execercise them on their
// own them one day in the future

// Should we prefer to use the AST's line and file info
// rather than adding an argument and propagating it at runtime?
// `fn` is const in this function but ftableMap stores by non-const
// `mod` can be passed in if its already been looked up
// TODO GLOBALS use of developer and ftableMap
bool InsertLineNumbers::shouldPreferASTLine(/*const*/ FnSymbol* fn,
                                            ModuleSymbol* mod) {
  if (!mod) {
    mod = fn->getModule();
  }
  // don't add arguments to extern/export fns
  // but do use arguments already added if the fn
  // is marked with FLAG_INSERT_LINE_FILE_INFO
  if ((fn->hasFlag(FLAG_EXTERN) || fn->hasFlag(FLAG_EXPORT)) &&
      !fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO))
    return true;

  // TODO can we mark the FnSymbol directly instead of checking this global map
  // don't add arguments to ftable calls
  if (ftableMap.count(fn))
    return true;

  // in user's code, it's better to use AST line number
  // as long as it's not compiler generated/inline
  if ((mod->modTag == MOD_USER || fn->hasFlag(FLAG_LINE_NUMBER_OK)) &&
      !fn->hasFlag(FLAG_COMPILER_GENERATED) && !fn->hasFlag(FLAG_INLINE))
    return true;

  // developer mode generally uses AST line numbers
  // FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO overrides
  else if (developer && !fn->hasFlag(FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO))
    return true;

  return false;
}

// This creates the line and file symbols for a call when we want it to come
// from the AST itself
// TODO GLOBALS getFilenamelookupposition
LineAndFile InsertLineNumbers::makeASTLine(CallExpr* call) {
  SET_LINENO(call);

  if (call->isResolved() &&
      call->resolvedFunction()->hasFlag(FLAG_COMMAND_LINE_SETTING)) {
    // Make up pretend line numbers for errors with command line
    // configuration variables.
    Symbol* line = new_IntSymbol(0);

    FnSymbol* fn = call->resolvedFunction();

    INT_ASSERT(fn);
    INT_ASSERT(fn->substitutionsPostResolve.size() > 0);

    VarSymbol* var = toVarSymbol(fn->substitutionsPostResolve[0].value);

    INT_ASSERT(var);
    INT_ASSERT(var->immediate);
    INT_ASSERT(var->immediate->const_kind == CONST_KIND_STRING);

    const char* cmdLineSetting = astr("<command line setting of '",
                                      var->immediate->v_string.c_str(), "'>");

    int filenameIdx = getFilenameLookupPosition(cmdLineSetting);

    Symbol* file = new_IntSymbol(filenameIdx, INT_SIZE_32);

    return {line, file};

  } else {
    // Apply the line number from the call AST node
    Symbol* line = new_IntSymbol(call->linenum());

    int filenameIdx = getFilenameLookupPosition(call->fname());

    Symbol* file = new_IntSymbol(filenameIdx, INT_SIZE_32);
    return {line, file};
  }
}

//
// insert a line number and filename actual into a call
// This can either be:
//   * PRIM_GET_USER_FILE     -> replace
//   * PRIM_GET_USER_LINE     -> replace
//   * a call to a wrapper fn -> assign to arg bundle
//   * a regular call         -> append to call args
//
void InsertLineNumbers::insertLineNumber(CallExpr* call, LineAndFile lineAndFile) {
  SET_LINENO(call);
  // Replace these primitives with whatever line/file we chose
  // (probably an argument)
  if (call->isPrimitive(PRIM_GET_USER_FILE)) {
    call->replace(new SymExpr(lineAndFile.file));
  } else if (call->isPrimitive(PRIM_GET_USER_LINE)) {
    call->replace(new SymExpr(lineAndFile.line));
  } else {
    FnSymbol* fn = call->resolvedFunction();
    if (fn && isWrapper(fn)) {
      SET_LINENO(call);
      // Refactor note: I pulled this case out of the moveLinenoInsideArgBundle
      // pass b/c it seems better to handle in one go instead of fixing things
      // up later. But, we do pull out the lineField and fileField each time
      // here, which is an implicit coupling with the field names (and also a
      // string lookup). Perhaps these should go in a map
      //
      // add line/file as assignments into the arg bundle
      DefExpr* bundleArg = toDefExpr(fn->formals.tail);
      Expr* bundleActual = call->argList.tail;
      AggregateType* bundleType = toAggregateType(bundleArg->sym->typeInfo());
      Symbol* lineField = bundleType->getField("_ln");
      Symbol* fileField = bundleType->getField("_fn");
      call->insertBefore(new CallExpr(PRIM_SET_MEMBER, bundleActual->copy(),
                                      lineField, lineAndFile.line));
      call->insertBefore(new CallExpr(PRIM_SET_MEMBER, bundleActual->copy(),
                                      fileField, lineAndFile.file));

    } else {
      // add line/file as arguments to the call
      call->insertAtTail(lineAndFile.line);
      call->insertAtTail(lineAndFile.file);
    }
  }
}

// TODO refactor -- pulled this out as it can be checked per fn instead of per call
void InsertLineNumbers::precondition(FnSymbol* fn) {
  if (strcmp(fn->name, "chpl__heapAllocateGlobals") == 0 ||
      strcmp(fn->name, "chpl__initStringLiterals") == 0 ||
      strcmp(fn->name, "chpl__initModuleGuards") == 0 ||
      strcmp(fn->name, "chpl_gen_main") == 0) {
    assert(fn->hasFlag(FLAG_EXPORT) &&
           !fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO));
  }
}

//
// Get the line/file symbols for a function, creating them if necessary
// The fn is either
//   * a wrapper    -> add fields to arg bundle
//   * a regular fn -> add formals to fn
//
// Since adding additional args (in any form) imposes work on all of fn's
// callers, we enqueue each callsite to get procesed later. We don't fix them
// immediately so that we aren't modifying someone else's FnSymbol
//
// If we have virtual roots or children, we also need them to to take line/file
// args (if they aren't already). Here we do mark them with
// FLAG_INSERT_LINE_FILE_INFO so that later processing can treat them
// appropriately
//
LineAndFile InsertLineNumbers::getLineAndFileForFn(FnSymbol *fn) {
  auto it = lineAndFilenameMap.find(fn);
  if (it != lineAndFilenameMap.end()) {
    return it->second;
  }

  SET_LINENO(fn);

  LineAndFile result;

  if (isWrapper(fn)) {
    // In the body of the wrapper, create a local lineno variable initialized
    // from the new corresponding field in the argument bundle.
    DefExpr* bundleArg = toDefExpr(fn->formals.tail);
    AggregateType* bundleType = toAggregateType(bundleArg->sym->typeInfo());

    VarSymbol* lineField = newTemp("_ln", dtInt[INT_SIZE_DEFAULT]);
    bundleType->fields.insertAtTail(new DefExpr(lineField));

    VarSymbol* lineLocal = newTemp("_ln", dtInt[INT_SIZE_DEFAULT]);

    fn->insertAtHead("'move'(%S, '.v'(%S, %S))", lineLocal, bundleArg->sym,
                     lineField);
    fn->insertAtHead(new DefExpr(lineLocal));

    // Same thing, just for the filename index now.

    VarSymbol* fileField = newTemp("_fn", dtInt[INT_SIZE_32]);
    bundleType->fields.insertAtTail(new DefExpr(fileField));

    VarSymbol* fileLocal = newTemp("_fn", dtInt[INT_SIZE_32]);

    fn->insertAtHead("'move'(%S, '.v'(%S, %S))", fileLocal, bundleArg->sym,
                     fileField);
    fn->insertAtHead(new DefExpr(fileLocal));

    result = {lineLocal, fileLocal};

  } else {
    ArgSymbol* line =
        new ArgSymbol(INTENT_CONST_IN, "_ln", dtInt[INT_SIZE_DEFAULT]);
    fn->insertFormalAtTail(line);

    ArgSymbol* file = new ArgSymbol(INTENT_IN, "_fn", dtInt[INT_SIZE_32]);
    fn->insertFormalAtTail(file);

    result = {line, file};
  }

  lineAndFilenameMap.emplace_hint(it, fn, result);

  for (CallExpr* call : *fn->calledBy) {
    enqueue(call->getFunction(), call);
  }

  if (auto* others = rootsOrChildren(fn)) {
    for (FnSymbol* fn : *others) {
      fn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
      enqueue(fn);
    }
  }

  return result;
}

//
// InsertLineNumbers works on a function and has to take care of
//   * modifying our own function (if necessary) to take line/file formals
//   * modifying calls in our body that need line/file actuals
//
void InsertLineNumbers::process(FnSymbol *fn) {
  // Refactor note: this precondition is a general check that could be hoisted
  precondition(fn);

  // if we must have line/file info formals
  if (fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO) &&
      !fn->hasFlag(FLAG_LINE_NUMBER_OK)) {
    // Explicitly ignore the return result since we don't need it right now. Our
    // call sites will be enqueued
    std::ignore = getLineAndFileForFn(fn);
  }

  // We only need to process each call in our body once; calls which may later
  // require line numbers will be enqueued by their function.
  // We may need to process the fn itself more than once, so that if we discover
  // a function needs a line/file formal after processing the first time, we can
  // do so
  if (alreadyProcessed(fn))
    return;

  // TODO iterator (or reuse calls)
  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);

  for (CallExpr* call : calls) {
    if (call->primitive && call->primitive->passLineno) {
      process(fn, call);
    }
  }
}

//
// inserts the line number for `call` which is in the body `fn`
// The line/file either come from the AST itself, or from the fn formals (or arg
// pack), which is determined by `shouldPreferASTLine`
//
void InsertLineNumbers::process(FnSymbol *fn, CallExpr* call) {
  if (shouldPreferASTLine(fn)) {
    insertLineNumber(call, makeASTLine(call));

  } else {
    insertLineNumber(call, getLineAndFileForFn(fn));
  }
}

// TODO move this into a more sensible home
// ---------- InsertNilChecksPass ----------
//
// insert nil checks primitives in front of most member accesses
//
// Exceptions:
//   1) The Chapel specification indicates that it is acceptable to
//      invoke the destructor on NIL, so we avoid doing so when
//      handling a call to a destructor.
//
bool InsertNilChecksPass::shouldProcess(CallExpr *call) {
  if (!(call->isPrimitive(PRIM_GET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_SET_MEMBER) ||
        call->isPrimitive(PRIM_GETCID) ||
        call->isPrimitive(PRIM_TESTCID) ||
        isClassMethodCall(call))) {
    return false;
  }

  Expr* arg0 = call->get(1);
  AggregateType* ct = toAggregateType(arg0->typeInfo());

  if (ct && (isClass(ct) || ct->symbol->hasFlag(FLAG_WIDE_CLASS))) {
    // Avoid inserting a nil-check if this is a call to a destructor
    if (FnSymbol* fn = call->resolvedFunction()) {
      return !fn->hasFlag(FLAG_DESTRUCTOR);
    }

    return true;
  }

  return false;
}

void InsertNilChecksPass::process(CallExpr* call) {
  Expr* arg0 = call->get(1);
  Expr* stmt = call->getStmtExpr();

  SET_LINENO(stmt);

  stmt->insertBefore(new CallExpr(PRIM_CHECK_NIL, arg0->copy()));
}

// --------------------

#include "global-ast-vecs.h"

void insertLineNumbers() {
  PassManager pm;

  // It's pretty apparent in this refactoring that we don't actually need to
  // recompute all call sites, but only the subset we'll be working on, which
  // is probably better done on demand
  pm.runPass<FnSymbol*>(ComputeCallSitesPass(), gFnSymbols);

  {
    std::vector<std::string> constantFilenames;

    // Put a null string into the iterator at the first position, some runtime
    // calls will pass in NULL for their filename, we can then use this null
    // string to deal with that case.
    constantFilenames.push_back("");

    // Add "<internal>" to the filename table if it didn't make it in there,
    // some runtime functions use this name directly, and it doesn't always end
    // up in the table otherwise
    constantFilenames.push_back("<internal>");

    gFilenameLookup.insert(gFilenameLookup.begin(), constantFilenames.begin(),
                           constantFilenames.end());
  }

  // refactoring  TODO: why is InsertNilChecks in this pass?
  if (!fNoNilChecks) {
    pm.runPass<CallExpr*>(InsertNilChecksPass(), gCallExprs);
  }

  // TODO: this is a WIP state on integrating with the PassManager
  // right now, we pass the same instance of InsertLineNumbers so it can
  // maintain state and it handles the queuing
  //
  pm.runPass<FnSymbol*>(InsertLineNumbers(), gFnSymbols);
}
