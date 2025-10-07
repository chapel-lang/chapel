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

#include "pass-manager-passes.h"

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
#include <vector>

namespace {
  // As a means of abbreviation.
  using Pass = InsertLineNumbers;
}

//
// insertLineNumbers() inserts line numbers and filenames into
// functions and calls so that runtime errors show line number and
// filename information.  The line numbers and filenames always point
// to user code, i.e., code that is not in a standard module or in a
// compiler-generated function.  To this effect, functions that are in
// a standard module or are compiler-generated are passed a line
// number and a filename.
//

std::vector<std::string> Pass::gFilenameLookup;
std::unordered_map<std::string, int> Pass::gFilenameLookupCache;

int Pass::addFilenameTableEntry(const std::string& name) {
  int idx = -1;

  if (auto it1 = gFilenameLookupCache.find(name);
           it1 != gFilenameLookupCache.end()) {
    // Use cached position in the table
    idx = it1->second;
  } else {
    // not found - new add new filename to the cache
    
    auto it2 = std::find(gFilenameLookup.begin(), gFilenameLookup.end(), name);
    if (it2 == gFilenameLookup.end()) {
      // Not in the lookup table either, add it (This case should only be for
      // command line casts, doesn't really matter if something else sneaks in
      // though)
      gFilenameLookup.push_back(name);
      idx = gFilenameLookup.size() - 1;
    } else {
      idx = it2 - gFilenameLookup.begin();
    }

    gFilenameLookupCache.insert(std::pair<std::string, int>(name, idx));
  }

  INT_ASSERT(idx >= 0);
  INT_ASSERT((size_t)idx < gFilenameLookup.size());

  return idx;
}

int Pass::getFilenameTableIndex(const std::string& name) {
  if (auto it = gFilenameLookupCache.find(name);
           it != gFilenameLookupCache.end()) {
    // Use cached position in the table
    return it->second;
  }
  INT_FATAL("Entry not in table!");
  return -1;
}

const std::vector<std::string>& Pass::getFilenameTable() {
  return gFilenameLookup;
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

// Considering the containing function (e.g., `call->getFunction()`), decide
// whether or not AST line numbers should be used for `call`. This function
// assumes that line/file info has to be appended to call - the only question
// is whether to propagate existing line/file info or construct a new one
// representing the callsite.
//
// TODO GLOBALS use of developer and ftableMap
bool Pass::shouldPreferASTLine(CallExpr* call) {
  auto fn = call->getFunction();
  auto mod = call->getModule();

  INT_ASSERT(fn && mod);

  // don't add arguments to extern/export fns
  // but do use arguments already added if the fn
  // is marked with FLAG_INSERT_LINE_FILE_INFO
  if ((fn->hasFlag(FLAG_EXTERN) || fn->hasFlag(FLAG_EXPORT)) &&
      !fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO))
    return true;

  // TODO can we mark the FnSymbol directly instead of checking this global map?
  //      Or maybe you could know it is an ftable call based on how it is called
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
Pass::LineAndFile Pass::makeASTLine(CallExpr* call) {
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

    int filenameIdx = addFilenameTableEntry(cmdLineSetting);

    Symbol* file = new_IntSymbol(filenameIdx, INT_SIZE_32);

    return {line, file};

  } else {
    // Apply the line number from the call AST node
    Symbol* line = new_IntSymbol(call->linenum());

    int filenameIdx = addFilenameTableEntry(call->fname());

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
void Pass::insertLineNumber(CallExpr* call, LineAndFile lineAndFile) {
  SET_LINENO(call);

  auto fn = call->resolvedFunction();

  // Replace these primitives with whatever line/file we chose
  // (probably an argument)
  if (call->isPrimitive(PRIM_GET_USER_FILE)) {
    call->replace(new SymExpr(lineAndFile.file));

  } else if (call->isPrimitive(PRIM_GET_USER_LINE)) {
    call->replace(new SymExpr(lineAndFile.line));

  } else {
    if (fn && isWrapper(fn)) {
      SET_LINENO(call);
      // TODO
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
      call->insertAtTail(lineAndFile.line);
      call->insertAtTail(lineAndFile.file);
    }
  }
}

void Pass::assertInvariants(FnSymbol* fn) {
  if (strcmp(fn->name, "chpl__heapAllocateGlobals") == 0 ||
      strcmp(fn->name, "chpl__initStringLiterals") == 0 ||
      strcmp(fn->name, "chpl__initModuleGuards") == 0 ||
      strcmp(fn->name, "chpl_gen_main") == 0) {
    INT_ASSERT(fn->hasFlag(FLAG_EXPORT));
    INT_ASSERT(!fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO));
  }
}

static VarSymbol* getOrCreateField(AggregateType* aggType,
                                   const char* fieldName, Type* fieldType) {
  VarSymbol* field = toVarSymbol(aggType->getField(fieldName, /*fatal*/ false));
  if (field) {
    // if needed, we can turn this assertion into a check
    INT_ASSERT(field->typeInfo() == fieldType);
    return field;
  }
  else {
    // Engin: temp field doesn't make much sense, but I am keeping the logic
    field = newTemp(fieldName, fieldType);
    aggType->fields.insertAtTail(new DefExpr(field));
    return field;
  }
}

//
// Get the line/file symbols for a function, creating them if necessary
// The fn is either
//   * a wrapper    -> add fields to arg bundle
//   * a regular fn -> add formals to fn
//
// Since adding additional args (in any form) imposes work on all of fn's
// callers, we enqueue each callsite to get processed later. We don't fix them
// immediately so that we aren't modifying someone else's FnSymbol
//
// If we have virtual roots or children, we also need them to to take line/file
// args (if they aren't already). Here we do mark them with
// FLAG_INSERT_LINE_FILE_INFO so that later processing can treat them
// appropriately
//
Pass::LineAndFile Pass::getLineAndFileForFn(FnSymbol *fn) {
  auto it = lineAndFilenameMap.find(fn);
  if (it != lineAndFilenameMap.end()) {
    return it->second;
  }

  SET_LINENO(fn);

  LineAndFile result;

  auto formals = FunctionType::constructLineFileInfoFormals();
  auto& lineFormal = formals[0];
  auto& fileFormal = formals[1];

  if (isWrapper(fn)) {
    // In the body of the wrapper, create a local lineno variable initialized
    // from the new corresponding field in the argument bundle.
    DefExpr* bundleArg = toDefExpr(fn->formals.tail);
    AggregateType* bundleType = toAggregateType(bundleArg->sym->typeInfo());

    // lineField could have been created already. Today we see this only with
    // --gpu-specialization, which can clone `coforall_fn`s while using the same
    // arg bundle for both clones. We want to create lineField only once.
    VarSymbol* lineField = getOrCreateField(bundleType, lineFormal.name(),
                                            lineFormal.type());
    VarSymbol* lineLocal = newTemp(lineFormal.name(), lineFormal.type());

    fn->insertAtHead("'move'(%S, '.v'(%S, %S))", lineLocal, bundleArg->sym,
                     lineField);
    fn->insertAtHead(new DefExpr(lineLocal));

    // Same thing, just for the filename index now.

    // See the comment above lineField.
    VarSymbol* fileField = getOrCreateField(bundleType, fileFormal.name(),
                                            fileFormal.type());
    VarSymbol* fileLocal = newTemp(fileFormal.name(), fileFormal.type());

    fn->insertAtHead("'move'(%S, '.v'(%S, %S))", fileLocal, bundleArg->sym,
                     fileField);
    fn->insertAtHead(new DefExpr(fileLocal));

    result = {lineLocal, fileLocal};

  } else {
    auto line = new ArgSymbol(lineFormal.intent(), lineFormal.name(),
                              lineFormal.type());
    auto file = new ArgSymbol(fileFormal.intent(), fileFormal.name(),
                              fileFormal.type());
    fn->insertFormalAtTail(line);
    fn->insertFormalAtTail(file);

    result = {line, file};

    // Recompute the function's type as it will be propagated to values.
    if (fn->isUsedAsValue()) fn->computeAndSetType();
  }

  lineAndFilenameMap.emplace_hint(it, fn, result);

  // NOTE: this requires that we have run 'ComputeCallSitesPass'
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

bool Pass::mustAddLineInfoFormalsToFn(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO) &&
      !fn->hasFlag(FLAG_LINE_NUMBER_OK)) {
    return true;
  }

  if (fn->isUsedAsValue()) {
    if (fn->hasFlag(FLAG_EXPORT) || fn->hasFlag(FLAG_EXTERN)) {
      INT_FATAL(fn, "Case not handled yet!");
    }

    // We need to uniformly pass line/file info to functions used as values
    // (even if that information is never used), because at this point the
    // ABI for function types depends on it: a previous pass already adjusted
    // all procedure types to be passed line and file information.
    return true;
  }

  return false;
}

bool Pass::mustAddLineInfoActualsToCall(CallExpr* call) {
  if (call->primitive && call->primitive->passLineno) return true;
  if (call->isIndirectCall()) return true;
  return false;
}

//
// InsertLineNumbers works on a function and has to take care of
//   * modifying our own function (if necessary) to take line/file formals
//   * modifying calls in our body that need line/file actuals
//
void Pass::process(FnSymbol* fn) {
  assertInvariants(fn);

  if (mustAddLineInfoFormalsToFn(fn)) {
    // Explicitly ignore the return result since we don't need it right now.
    // This will enqueue call sites to work on.
    std::ignore = getLineAndFileForFn(fn);
  }

  // This check is after the above getLineAndFileForFn because we only
  // need to process each call in our body once; calls which may later
  // require line numbers will be enqueued by their function.  We may
  // need to process the fn itself more than once, so that if we
  // discover a function needs a line/file formal after processing the
  // first time, we can do so
  if (alreadyProcessed(fn))
    return;

  // TODO iterator (or reuse calls)
  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);

  for (CallExpr* call : calls) {
    if (mustAddLineInfoActualsToCall(call)) {
      process(fn, call);
    }
  }
}

//
// This function operates on a 'call' that is within the body of 'fn'.
// In the most common case it will append line/file info to the call,
// however in some other cases it will do a separate AST manipulation
// such as adjust the types of local variables. This function may
// enqueue more work.
//
void Pass::process(FnSymbol *fn, CallExpr* call) {
  if (auto pair = fixedCalls.insert(call); !pair.second) return;

  if (shouldPreferASTLine(call)) {
    insertLineNumber(call, makeASTLine(call));
  } else {
    insertLineNumber(call, getLineAndFileForFn(fn));
  }
}
