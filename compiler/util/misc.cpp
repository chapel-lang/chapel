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

#include "misc.h"

#include "baseAST.h"
#include "chpl.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "parser.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "version.h"

#include <csignal>
#include <fcntl.h>
#include <sys/stat.h>

static void cleanup_for_exit(void) {
  deleteTmpDir();
  stopCatchingSignals();
}

// must be non-static to avoid dead-code elim. when compiling -O3
void gdbShouldBreakHere(void) {
}

// Support for internal errors, adopted from ZPL compiler

static bool exit_immediately = true;
static bool exit_eventually = false;
static bool exit_end_of_pass = false;

static const char* err_filename;
static int err_lineno;
static int err_fatal;
static int err_user;
static int err_print;
static int err_ignore;
static FnSymbol* err_fn = NULL;

bool forceWidePtrs() {
  return !strcmp(CHPL_LOCALE_MODEL, "numa");
}

bool forceWidePtrsForLocal() {
  return fLocal && forceWidePtrs();
}

bool requireWideReferences() {
  return !fLocal || forceWidePtrs();
}

bool requireOutlinedOn() {
  return !fLocal || forceWidePtrs();
}

const char* cleanFilename(const char* name) {
  static int chplHomeLen = strlen(CHPL_HOME);

  if (!strncmp(name, CHPL_HOME, chplHomeLen)) {
    return astr("$CHPL_HOME", name + chplHomeLen);
  } else {
    return name;
  }
}


const char* cleanFilename(BaseAST* ast) {
  const char* astFname = ast->fname();
  if (astFname)
    return cleanFilename(astFname);
  else if (yyfilename)
    return cleanFilename(yyfilename);
  else
    return astr("<unknown>");
}


static void
print_user_internal_error() {
  static char error[8];

  const char* filename_start = strrchr(err_filename, '/');
  if (filename_start)
    filename_start++;
  else
    filename_start = err_filename;
  strncpy(error, filename_start, 3);
  sprintf(error+3, "%04d", err_lineno);
  for (int i = 0; i < 7; i++) {
    if (error[i] >= 'a' && error[i] <= 'z') {
      error[i] += 'A' - 'a';
    }
  }

  fprintf(stderr, "%s ", error);
  char version[128];
  get_version(version);
  fprintf(stderr, "chpl Version %s\n", version);
  if (err_fatal)
    clean_exit(1);
}


// find a caller (direct or not) that is not in a task function,
// for line number reporting
static FnSymbol*
findNonTaskCaller(FnSymbol* fn) {
  if (!fn || !fn->inTree()) return fn;
  while (true) {
    if (!isTaskFun(fn)) return fn;

    // who calls this?
    FnSymbol* caller = NULL;
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->inTree()) {
        if (FnSymbol* cfn = call->isResolved()) {
          if (cfn == fn) {
            caller = toFnSymbol(call->parentSymbol);
            break;
          }
        }
      }
    }
    if (!caller) return fn; // or should it return the original value of 'fn'?
    fn = caller;
  }
}

void
setupError(const char *filename, int lineno, int tag) {
  err_filename = filename;
  err_lineno = lineno;
  err_fatal = tag == 1 || tag == 2 || tag == 3;
  err_user = tag != 1;
  err_print = tag == 5;
  err_ignore = ignore_warnings && tag == 4;
  exit_immediately = tag == 1 || tag == 2;
  exit_eventually |= tag == 3;
}


static void
printDevelErrorHeader(BaseAST* ast) {
  if (!err_print) {
    if (Expr* expr = toExpr(ast)) {
      Symbol* parent = expr->parentSymbol;
      if (isArgSymbol(parent))
        parent = parent->defPoint->parentSymbol;
      FnSymbol* fn = toFnSymbol(parent);
      fn = findNonTaskCaller(fn);
      if (fn && fn != err_fn) {
        err_fn = fn;
        while ((fn = toFnSymbol(err_fn->defPoint->parentSymbol))) {
          if (fn == fn->getModule()->initFn)
            break;
          err_fn = fn;
        }
        // If the function is compiler-generated, or inlined, or doesn't match
        // the error function and line number, nothing is printed.
        if (err_fn->getModule()->initFn != err_fn &&
            !err_fn->hasFlag(FLAG_COMPILER_GENERATED) &&
            !err_fn->hasFlag(FLAG_INLINE) &&
            err_fn->linenum()) {
          fprintf(stderr, "%s:%d: In ",
                  cleanFilename(err_fn), err_fn->linenum());
          if (!strncmp(err_fn->name, "_construct_", 11)) {
            fprintf(stderr, "constructor '%s':\n", err_fn->name+11);
          } else {
            fprintf(stderr, "%s '%s':\n",
                    (err_fn->hasFlag(FLAG_ITERATOR_FN) ? "iterator" : "function"),
                    err_fn->name);
          }
        }
      }
    }
  }


  if (ast && ast->linenum())
    fprintf(stderr, "%s:%d: ", cleanFilename(ast), ast->linenum());

  if (err_print) {
    fprintf(stderr, "note: ");
  } else if (err_fatal) {
    if (err_user) {
      fprintf(stderr, "error: ");
    } else {
      fprintf(stderr, "internal error: ");
    }
  } else {
    fprintf(stderr, "warning: ");
  }

  if (!err_user && !developer) {
    print_user_internal_error();
  }

}

static void printDevelErrorFooter(void) {
  if (developer)
    fprintf(stderr, " [%s:%d]", err_filename, err_lineno);
}


//
// Print the module name, line number, and function signature of each function
// on the call stack. This can be called from a debugger to to see what the
// call chain looks like e.g. after a resolution error.
//
void printCallStack(bool force, bool shortModule, FILE* out) {
  if (!force) {
    if (!fPrintCallStackOnError || err_print || callStack.n <= 1)
      return;
  }
  if (!developer)
    fprintf(out, "while processing the following Chapel call chain:\n");
  for (int i = callStack.n-1; i >= 0; i--) {
    CallExpr* call = callStack.v[i];
    FnSymbol* fn = call->getFunction();
    ModuleSymbol* module = call->getModule();
    fprintf(out, "  %s:%d: %s%s%s\n",
            (shortModule ? module->name : cleanFilename(fn->fname())),
            call->linenum(), toString(fn),
            (module->modTag == MOD_INTERNAL ? " [internal module]" : ""),
            (fn->hasFlag(FLAG_COMPILER_GENERATED) ? " [compiler-generated]" : ""));
  }
}

static void printCallStackOnError() {
  printCallStack(false, false, stderr);
}

//
// debugging convenience
//
void printCallStack();
void printCallStack() {
  printCallStack(true, true, stdout);
}

// another one
void printCallStackCalls();
void printCallStackCalls() {
  printf("\n" "callStack %d elms\n\n", callStack.n);
  for (int i = 0; i < callStack.n; i++) {
    CallExpr* call = callStack.v[i];
    FnSymbol* cfn = call->isResolved();
    printf("%d  %d %s  <-  %d %s\n", i,
           cfn ? cfn->id : 0, cfn ? cfn->name: "<no callee>",
           call ? call->id : 0, call ? call->stringLoc() : "<no call>");
  }
  printf("\n");
}


void handleError(const char *fmt, ...) {
  fflush(stdout);
  fflush(stderr);
  if (err_ignore)
    return;

  printDevelErrorHeader(NULL);

  if (!err_user && !developer)
    return;

  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printDevelErrorFooter();

  fprintf(stderr, "\n");

  printCallStackOnError();

  if (exit_immediately) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors) {
      clean_exit(1);
    }
  }
}


static void vhandleError(FILE* file, BaseAST* ast, const char *fmt, va_list args);

void handleError(BaseAST* ast, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vhandleError(stderr, ast, fmt, args);
  va_end(args);
}

void handleError(FILE* file, BaseAST* ast, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vhandleError(file, ast, fmt, args);
  va_end(args);
}

static void vhandleError(FILE* file, BaseAST* ast, const char *fmt, va_list args) {
  if (err_ignore)
    return;

  if (file == stderr)
    printDevelErrorHeader(ast);

  if (!err_user && !developer)
    return;

  vfprintf(file, fmt, args);

  if (fPrintIDonError && ast)
    fprintf(file, " [%d]", ast->id);

  if (file == stderr)
    printDevelErrorFooter();

  fprintf(file, "\n");

  if (file == stderr)
    printCallStackOnError();

  if (exit_immediately) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors) {
      clean_exit(1);
    }
  }
}


void exitIfFatalErrorsEncountered() {
  if (exit_eventually) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors) {
      clean_exit(1);
    }
  }
}


void considerExitingEndOfPass() {
  if (exit_end_of_pass) {
    if (!ignore_errors) {
      clean_exit(1);
    }
  }
}


static void handleInterrupt(int sig) {
  USR_FATAL("received interrupt");
}


static void handleSegFault(int sig) {
  INT_FATAL("seg fault");
}


void startCatchingSignals(void) {
  signal(SIGINT, handleInterrupt);
  signal(SIGTERM, handleInterrupt);
  signal(SIGHUP, handleInterrupt);
  signal(SIGSEGV, handleSegFault);
}


void stopCatchingSignals(void) {
  signal(SIGINT, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
}


//
// Put this last to minimize the amount of code affected by this #undef
//
#ifdef exit
#undef exit
#endif
void clean_exit(int status) {
  if (status != 0) {
    gdbShouldBreakHere();
  }
  cleanup_for_exit();
  deleteStrings();
  exit(status);
}
