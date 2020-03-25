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

#include "insertLineNumbers.h"

#include "astutil.h"
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

int getFilenameLookupPosition(std::string name) {
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

static void moveLinenoInsideArgBundle();

//
// The queue keeps track of the functions to which line number and
// filename arguments have been added so that calls to these functions
// can be updated with new actual arguments.
//
static std::queue<FnSymbol*> queue;

static Map<FnSymbol*,ArgSymbol*> linenoMap; // fn to line number argument
static Map<FnSymbol*,ArgSymbol*> filenameMap; // fn to filename argument

static ArgSymbol* newLine(FnSymbol* fn) {
  ArgSymbol* line = new ArgSymbol(INTENT_CONST_IN, "_ln", dtInt[INT_SIZE_DEFAULT]);
  fn->insertFormalAtTail(line);
  linenoMap.put(fn, line);
  if (Vec<FnSymbol*>* rootFns = virtualRootsMap.get(fn)) {
    forv_Vec(FnSymbol, rootFn, *rootFns)
      if (!linenoMap.get(rootFn))
        newLine(rootFn);
  } else if (Vec<FnSymbol*>* childrenFns = virtualChildrenMap.get(fn)) {
    forv_Vec(FnSymbol, childrenFn, *childrenFns)
      if (!linenoMap.get(childrenFn))
        newLine(childrenFn);
  }
  return line;
}

static ArgSymbol* newFile(FnSymbol* fn) {
  ArgSymbol* file = new ArgSymbol(INTENT_IN, "_fn", dtInt[INT_SIZE_32]);
  fn->insertFormalAtTail(file);
  filenameMap.put(fn, file);
  queue.push(fn);
  if (Vec<FnSymbol*>* rootFns = virtualRootsMap.get(fn)) {
    forv_Vec(FnSymbol, rootFn, *rootFns)
      if (!filenameMap.get(rootFn))
        newFile(rootFn);
  } else if (Vec<FnSymbol*>* childrenFns = virtualChildrenMap.get(fn)) {
    forv_Vec(FnSymbol, childrenFn, *childrenFns)
      if (!filenameMap.get(childrenFn))
        newFile(childrenFn);
  }
  return file;
}

//
// insert a line number and filename actual into a call; add line
// number and filename formal arguments to the function in which this
// call lives if necessary, and add it to the queue.
//
static void
insertLineNumber(CallExpr* call) {
  SET_LINENO(call);

  FnSymbol*     fn   = call->getFunction();
  ModuleSymbol* mod  = fn->getModule();
  ArgSymbol*    fileArg = filenameMap.get(fn);
  ArgSymbol*    lineArg = linenoMap.get(fn);
  Symbol*       file    = NULL;
  Symbol*       line    = NULL;

  if (strcmp(fn->name, "chpl__heapAllocateGlobals") == 0 ||
      strcmp(fn->name, "chpl__initStringLiterals")  == 0 ||
      strcmp(fn->name, "chpl__initModuleGuards")    == 0 ||
      strcmp(fn->name, "chpl_gen_main") == 0) {
    assert( fn->hasFlag(FLAG_EXPORT) &&
           !fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO));
  }

  bool preferASTLine = false;

  // Should we prefer to use the AST's line and file info
  // rather than adding an argument and propagating it at runtime?
  if ((fn->hasFlag(FLAG_EXTERN) || fn->hasFlag(FLAG_EXPORT)) &&
      !fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO))
    preferASTLine = true; // don't add arguments to extern/export fns
                          // but do use arguments already added if the fn
                          // is marked with FLAG_INSERT_LINE_FILE_INFO
  else if (ftableMap.count(fn))
    preferASTLine = true; // don't add arguments to ftable calls
  else if ((mod->modTag == MOD_USER || fn->hasFlag(FLAG_LINE_NUMBER_OK)) &&
           !fn->hasFlag(FLAG_COMPILER_GENERATED) &&
           !fn->hasFlag(FLAG_INLINE))
    preferASTLine = true; // in user's code, it's better to use AST line number
                          // as long as it's not compiler generated/inline
  else if (developer && !fn->hasFlag(FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO))
    preferASTLine = true; // developer mode generally uses AST line numbers
                          // FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO overrides

  if (preferASTLine) {
    // This branch handles the case in which line number
    // should just be whatever is in the AST node
    // Sets file and line based on call's AST node

    if (call->isResolved() &&
        call->resolvedFunction()->hasFlag(FLAG_COMMAND_LINE_SETTING)) {
      // Make up pretend line numbers for errors with command line
      // configuration variables.
      line = new_IntSymbol(0);

      FnSymbol* fn = call->resolvedFunction();

      INT_ASSERT(fn);
      INT_ASSERT(fn->substitutions.n);

      VarSymbol* var = toVarSymbol(fn->substitutions.v[0].value);

      INT_ASSERT(var);
      INT_ASSERT(var->immediate);
      INT_ASSERT(var->immediate->const_kind == CONST_KIND_STRING);

      const char* cmdLineSetting = astr("<command line setting of '",
                                        var->immediate->v_string,
                                        "'>");

      int         filenameIdx    = getFilenameLookupPosition(cmdLineSetting);

      file = new_IntSymbol(filenameIdx, INT_SIZE_32);

    } else {
      // Apply the line number from the call AST node
      line = new_IntSymbol(call->linenum());

      int filenameIdx = getFilenameLookupPosition(call->fname());

      file = new_IntSymbol(filenameIdx, INT_SIZE_32);
    }
  } else {
    // This branch handles the case in which we should be using
    // file/line arguments in the enclosing function.

    // Create them if they don't exist yet.
    if (lineArg == NULL)
      lineArg = newLine(fn);
    if (fileArg == NULL)
      fileArg = newFile(fn);

    line = lineArg;
    file = fileArg;
  }

  if (call->isPrimitive(PRIM_GET_USER_FILE) ||
      call->isPrimitive(PRIM_GET_USER_LINE)) {

    // Replace these primitives with whatever line/file we chose
    // (probably an argument)
    if (call->isPrimitive(PRIM_GET_USER_FILE)) {
      call->replace(new SymExpr(file));
    } else if (call->isPrimitive(PRIM_GET_USER_LINE)) {
      call->replace(new SymExpr(line));
    }

  } else {
    // add line/file as arguments to the call
    call->insertAtTail(line);
    call->insertAtTail(file);
  }
}


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


// insert nil checks primitives in front of most member accesses
//
// Exceptions:
//   1) The Chapel specification indicates that it is acceptable to
//      invoke the destructor on NIL, so we avoid doing so when
//      handling a call to a destructor.
//
static void insertNilChecks() {
  forv_Vec(CallExpr, call, gCallExprs) {
    // A member access is one of these primitives or a method call.
    // A method call is expected to access its "this" argument.
    if (call->isPrimitive(PRIM_GET_MEMBER)       ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_SET_MEMBER)       ||
        call->isPrimitive(PRIM_GETCID)           ||
        call->isPrimitive(PRIM_TESTCID)          ||
        isClassMethodCall(call)) {
      Expr*          arg0 = call->get(1);
      AggregateType* ct   = toAggregateType(arg0->typeInfo());

      if (ct && (isClass(ct) || ct->symbol->hasFlag(FLAG_WIDE_CLASS))) {
        FnSymbol* fn = call->resolvedFunction();

        // Avoid inserting a nil-check if this is a call to a destructor
        if (fn == NULL || fn->hasFlag(FLAG_DESTRUCTOR) == false) {
          Expr* stmt = call->getStmtExpr();

          SET_LINENO(stmt);

          stmt->insertBefore(new CallExpr(PRIM_CHECK_NIL, arg0->copy()));
        }
      }
    }
  }
}

void insertLineNumbers() {
  compute_call_sites();

  // Temporary vector that stores some constant filenames that are used
  // directly by the runtime. The index for these matter, and are provided to
  // the runtime as defines in chpl-linefile-support.h
  std::vector<std::string> constantFilenames;

  // Put a null string into the iterator at the first position, some runtime
  // calls will pass in NULL for their filename, we can then use this null
  // string to deal with that case.
  constantFilenames.push_back("");

  // Add "<internal>" to the filename table if it didn't make it in there, some
  // runtime functions use this name directly, and it doesn't always end up in
  // the table otherwise
  constantFilenames.push_back("<internal>");

  gFilenameLookup.insert(gFilenameLookup.begin(),
                         constantFilenames.begin(),
                         constantFilenames.end());

  if (!fNoNilChecks) {
    insertNilChecks();
  }

  // loop over all primitives that require a line number and filename
  // and pass them an actual line number and filename
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->primitive && call->primitive->passLineno) {
      insertLineNumber(call);
    }
  }

  // loop over all marked functions ("insert line file info"), add
  // line number and filename arguments to these functions, and add
  // them to the queue
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO) &&
        !fn->hasFlag(FLAG_LINE_NUMBER_OK)) {
      if (!filenameMap.get(fn)) {  // avoid duplicates
        SET_LINENO(fn);
        newLine(fn);
        newFile(fn);
      }
    }
  }

  // loop over all functions in the queue and all calls to these
  // functions, and pass the calls an actual line number and filename
  // Note: 'queue' may be appended to during this loop
  while (queue.empty() == false) {
    FnSymbol* fn = queue.front();
    queue.pop();

    forv_Vec(CallExpr, call, *fn->calledBy) {
      insertLineNumber(call);
    }
  }

  moveLinenoInsideArgBundle();
}

static void moveLinenoInsideArgBundle()
{
  // pass line number and filename arguments to functions that are
  // forked via the argument class
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    ArgSymbol* lineArgSym;
    ArgSymbol* filenameArgSym;

    if (fn->hasFlag(FLAG_ON_BLOCK) ||
        fn->hasFlag(FLAG_BEGIN_BLOCK) ||
        fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK)) {

      lineArgSym = linenoMap.get(fn);
      filenameArgSym = filenameMap.get(fn);

      // Either both should be set, or neither.
      INT_ASSERT( (lineArgSym == NULL && filenameArgSym == NULL) ||
                  (lineArgSym != NULL && filenameArgSym != NULL) );

      if (lineArgSym != NULL && filenameArgSym != NULL) {
        // This task (wrapper) function is not actually called with lineno, fname
        // arguments, so remove them.
        SET_LINENO(fn);
        DefExpr* fileArg = filenameArgSym->defPoint;
        fileArg->remove();
        DefExpr* lineArg = lineArgSym->defPoint;
        lineArg->remove();

        // In the body of the wrapper, create a local lineno variable initialized
        // from the new corresponding field in the argument bundle.
        DefExpr* bundleArg = toDefExpr(fn->formals.tail);
        AggregateType* bundleType = toAggregateType(bundleArg->sym->typeInfo());

        VarSymbol* lineField = newTemp("_ln", lineArg->sym->typeInfo());
        bundleType->fields.insertAtTail(new DefExpr(lineField));

        VarSymbol* lineLocal = newTemp("_ln", lineArg->sym->typeInfo());

        fn->insertAtHead("'move'(%S, '.v'(%S, %S))", lineLocal, bundleArg->sym, lineField);
        fn->insertAtHead(new DefExpr(lineLocal));

        // Same thing, just for the filename index now.

        VarSymbol* fileField = newTemp("_fn", fileArg->sym->typeInfo());
        bundleType->fields.insertAtTail(new DefExpr(fileField));

        VarSymbol* fileLocal = newTemp("_fn", fileArg->sym->typeInfo());

        fn->insertAtHead("'move'(%S, '.v'(%S, %S))", fileLocal, bundleArg->sym, fileField);
        fn->insertAtHead(new DefExpr(fileLocal));

        // Replace references to the (removed) arguments with
        // references to these local variables.
        SymbolMap update;
        update.put(fileArg->sym, fileLocal);
        update.put(lineArg->sym, lineLocal);
        update_symbols(fn->body, &update);

        // In each direct caller of this wrapper function (are there any?),
        // Remove actual arguments containing the lineno and filename.
        // Put these in the argument bundle instead.
        forv_Vec(CallExpr, call, *fn->calledBy) {
          SET_LINENO(call);
          Expr* fileActual = call->argList.tail->remove();
          Expr* lineActual = call->argList.tail->remove();
          Expr* bundleActual = call->argList.tail;
          call->insertBefore(new CallExpr(PRIM_SET_MEMBER, bundleActual->copy(),
                                          lineField, lineActual));
          call->insertBefore(new CallExpr(PRIM_SET_MEMBER, bundleActual->copy(),
                                          fileField, fileActual));
        }
      }
    }
  }
}
