/*
 * Copyright 2004-2015 Cray Inc.
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
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

//
// insertLineNumbers() inserts line numbers and filenames into
// functions and calls so that runtime errors show line number and
// filename information.  The line numbers and filenames always point
// to user code, i.e., code that is not in a standard module or in a
// compiler-generated function.  To this effect, functions that are in
// a standard module or are compiler-generated are passed a line
// number and a filename.
//

static void moveLinenoInsideArgBundle();

//
// The queue keeps track of the functions to which line number and
// filename arguments have been added so that calls to these functions
// can be updated with new actual arguments.
//
static Vec<FnSymbol*> queue;

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
  ArgSymbol* file = new ArgSymbol(INTENT_CONST_REF, "_fn", dtStringC);
  fn->insertFormalAtTail(file);
  filenameMap.put(fn, file);
  queue.add(fn);
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
  FnSymbol* fn = call->getFunction();
  ModuleSymbol* mod = fn->getModule();
  ArgSymbol* file = filenameMap.get(fn);
  ArgSymbol* line = linenoMap.get(fn);
  SET_LINENO(call);

  if (call->isPrimitive(PRIM_GET_USER_FILE) || 
      call->isPrimitive(PRIM_GET_USER_LINE)) {
    
    // add both arguments or none
    if (!file) { 
      line = newLine(fn);
      file = newFile(fn);
    }
    
    // 
    if (call->isPrimitive(PRIM_GET_USER_FILE)) {
      call->replace(new SymExpr(file));
    } else if (call->isPrimitive(PRIM_GET_USER_LINE)) {
      call->replace(new SymExpr(line));
    }
  } else if (fn->hasFlag(FLAG_EXTERN) ||
             !strcmp(fn->name, "chpl__heapAllocateGlobals") ||
             !strcmp(fn->name, "chpl__initModuleGuards") ||
             !strcmp(fn->name, "chpl_gen_main") ||
             (mod->modTag == MOD_USER && 
              !fn->hasFlag(FLAG_COMPILER_GENERATED) && !fn->hasFlag(FLAG_INLINE)) ||
             (developer && strcmp(fn->name, "halt"))) {
    // hilde sez: This special casing is suspect.  Can't we key off a flag?

    // call is in user code; insert AST line number and filename
    // or developer flag is on and the call is not the halt() call
    if (call->isResolved() &&
        call->isResolved()->hasFlag(FLAG_COMMAND_LINE_SETTING)) {
      call->insertAtTail(new_IntSymbol(0));
      FnSymbol* fn = call->isResolved();
      INT_ASSERT(fn);
      INT_ASSERT(fn->substitutions.n);
      VarSymbol* var = toVarSymbol(fn->substitutions.v[0].value);
      INT_ASSERT(var);
      INT_ASSERT(var->immediate);
      INT_ASSERT(var->immediate->const_kind == CONST_KIND_STRING);
      call->insertAtTail(new_StringSymbol(astr("<command line setting of '",
                                               var->immediate->v_string,
                                               "'>")));
    } else {
      if (fCLineNumbers) {
        if (!gCLine) {
          gCLine = new VarSymbol("__LINE__", dtInt[INT_SIZE_DEFAULT]);
          rootModule->block->insertAtTail(new DefExpr(gCLine));
          gCFile = new VarSymbol("__FILE__", dtStringC);
          rootModule->block->insertAtTail(new DefExpr(gCFile));
        }
        call->insertAtTail(gCLine);
        call->insertAtTail(gCFile);
      } else {
        call->insertAtTail(new_IntSymbol(call->linenum()));
        call->insertAtTail(new_StringSymbol(call->fname()));
      }
    }
  } else if (file) {
    // call is in non-user code, but the function already has line
    // number and filename arguments
    call->insertAtTail(line);
    call->insertAtTail(file);
  } else {
    // call is in non-user code, and the function requires new line
    // number and filename arguments
    line = newLine(fn);
    file = newFile(fn);

    call->insertAtTail(line);
    call->insertAtTail(file);
  }
}


static bool isClassMethodCall(CallExpr* call) {
  FnSymbol* fn = call->isResolved();
  if (fn && fn->isMethod() && fn->_this) {
    if (AggregateType* ct = toAggregateType(fn->_this->typeInfo())) {
      if (fn->numFormals() > 0 &&
          fn->getFormal(1)->typeInfo() == fn->_this->typeInfo()) {
        if (isClass(ct) || ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          // TODO: When strings are records, remove this protective if.
          // isClassMethodCall is only used in insertNilChecks() and widened
          // strings gain FLAG_WIDE_CLASS, but it doesn't make sense to check
          // strings against nil.
          if (strcmp(ct->symbol->name, "__wide_chpl_string")) {
            return true;
          }
        }
      }
    }
  }
  return false;
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
        FnSymbol* fn = call->isResolved();

        // Avoid inserting a nil-check if this is a call to a destrutor
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
    if (fn->hasFlag(FLAG_INSERT_LINE_FILE_INFO)) {
      if (!filenameMap.get(fn)) {  // avoid duplicates
        SET_LINENO(fn);
        newLine(fn);
        newFile(fn);
      }
    }
  }

  // loop over all functions in the queue and all calls to these
  // functions, and pass the calls an actual line number and filename
  forv_Vec(FnSymbol, fn, queue) {
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
    // If we added arguments to a the the following wrapper functions,
    //  the number of formals should be now be (precisely two) greater
    //  than the expected number.  Both block types below expect an
    //  argument bundle, and the on-block expects an additional argument
    //  that is the locale on which it should be executed.
    if ((fn->numFormals() > 2 && fn->hasFlag(FLAG_ON_BLOCK)) ||
        (fn->numFormals() > 1 && !fn->hasFlag(FLAG_ON_BLOCK) &&
         (fn->hasFlag(FLAG_BEGIN_BLOCK) ||
          fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK)))) {

      // This task (wrapper) function is not actually called with lineno, fname
      // arguments, so remove them.
      SET_LINENO(fn);
      DefExpr* fileArg = toDefExpr(fn->formals.tail);
      fileArg->remove();
      DefExpr* lineArg = toDefExpr(fn->formals.tail);
      lineArg->remove();

      // In the body of the wrapper, create local lineno and fname variables 
      // initialized from the corresponding fields in the argument bundle.
      DefExpr* bundleArg = toDefExpr(fn->formals.tail);
      AggregateType* bundleType = toAggregateType(bundleArg->sym->typeInfo());
      VarSymbol* lineField = newTemp("_ln", lineArg->sym->typeInfo());
      bundleType->fields.insertAtTail(new DefExpr(lineField));
      VarSymbol* fileField = newTemp("_fn", fileArg->sym->typeInfo());
      bundleType->fields.insertAtTail(new DefExpr(fileField));
      VarSymbol* fileLocal = newTemp("_fn", fileArg->sym->typeInfo());
      VarSymbol* lineLocal = newTemp("_ln", lineArg->sym->typeInfo());
      fn->insertAtHead("'move'(%S, '.v'(%S, %S))", fileLocal, bundleArg->sym, fileField);
      fn->insertAtHead(new DefExpr(fileLocal));
      fn->insertAtHead("'move'(%S, '.v'(%S, %S))", lineLocal, bundleArg->sym, lineField);
      fn->insertAtHead(new DefExpr(lineLocal));

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
        call->insertBefore(new CallExpr(PRIM_SET_MEMBER, bundleActual->copy(), lineField, lineActual));
        call->insertBefore(new CallExpr(PRIM_SET_MEMBER, bundleActual->copy(), fileField, fileActual));
      }
    }
  }
}
