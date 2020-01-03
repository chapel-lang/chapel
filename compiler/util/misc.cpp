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

static const char* help_url = "https://chapel-lang.org/bugs.html";

// Support for internal errors, adopted from ZPL compiler

static bool        exit_immediately = true;
static bool        exit_eventually  = false;
static bool        exit_end_of_pass = false;

static const char* err_subdir       = NULL;
static const char* err_filename     = NULL;

static int         err_lineno       =    0;
static int         err_fatal        =    0;
static int         err_user         =    0;
static int         err_print        =    0;
static int         err_ignore       =    0;

static FnSymbol*   err_fn           = NULL;
static int         err_fn_id        = 0;
static bool        err_fn_header_printed = false;

static bool forceWidePtrs();

// must be non-static to avoid dead-code elim. when compiling -O3
void gdbShouldBreakHere() {

}

void setupError(const char* subdir, const char* filename, int lineno, int tag) {
  err_subdir        = subdir;
  err_filename      = filename;
  err_lineno        = lineno;
  err_fatal         = tag == 1 || tag == 2 || tag == 3;
  err_user          = tag != 1;
  err_print         = tag == 5;
  err_ignore        = ignore_warnings && tag == 4;

  exit_immediately  = tag == 1 || tag == 2;
  exit_eventually  |= tag == 3;
}

bool forceWidePtrsForLocal() {
  return fLocal && forceWidePtrs();
}

bool requireWideReferences() {
  return !fLocal || forceWidePtrs();
}

//
// If the --no-local flag is used, or the locale model is not 'flat'
// (i.e., has sub-locales that an on-clause might target), we should
// require on-clauses to be "outlined" (i.e., we should not assume the
// on-clause is a no-op and execute the associated statement locally.
//
bool requireOutlinedOn() {
  return requireWideReferences();
}

const char* cleanFilename(const char* name) {
  static int  chplHomeLen = strlen(CHPL_HOME);
  const char* retval      = NULL;

  if (strncmp(name, CHPL_HOME, chplHomeLen) == 0) {
    retval = astr("$CHPL_HOME", name + chplHomeLen);
  } else {
    retval = name;
  }

  return retval;
}

const char* cleanFilename(const BaseAST* ast) {
  const char* retval = NULL;

  if (const char* astFname = ast->fname()) {
    retval = cleanFilename(astFname);

  } else if (yyfilename != NULL) {
    retval = cleanFilename(yyfilename);

  } else {
    retval = astr("<unknown>");
  }

  return retval;
}


static void cleanup_for_exit() {
  deleteTmpDir();
  stopCatchingSignals();
}


//
// Chances are that all non-flat locale models will require wide
// pointers.  Ultimately, we'd like to have such decisions be made by
// param fields/methods within the locale models themselves, but that
// would require a fairly large refactoring, so for now, we
// special-case 'flat' with the expectation that most other locale
// models will not be flat.
//
static bool forceWidePtrs() {
  return (strcmp(CHPL_LOCALE_MODEL, "flat") != 0);
}

static void print_user_internal_error() {
  char error[20];

  const char* filename_start = strrchr(err_filename, '/');
  const char* filename_end = NULL;
  const char* directory_start = err_subdir;
  char        version[128]   = { '\0' };

  // Fill error with _
  for (int i = 0; i < (int)sizeof(error) - 1; i++) {
    error[i] = ' ';
  }
  error[sizeof(error)-1] = '\0';

  if (filename_start) {
    filename_start++;
  } else {
    filename_start = err_filename;
  }

  filename_end = strrchr(filename_start, '.');
  if (filename_end - filename_start >= 6)
    filename_end = filename_end - 3;
  else
    filename_end = NULL;

  int idx = 0;
  // first 3 characters are from directory
  if (directory_start && strlen(directory_start) >= 3) {
    strncpy(&error[idx], directory_start, 3);
    idx += 3;
    error[idx++] = '-';
  }

  // next 3 characters are start of filename
  strncpy(&error[idx], filename_start, 3);
  idx += 3;

  // next 3 characters are end of the filename
  if (filename_end) {
    error[idx++] = '-';
    strncpy(&error[idx], filename_end, 3);
    idx += 3;
  }

  error[idx++] = '-';
  // next 4 characters are the line number
  sprintf(&error[idx], "%04d", err_lineno);

  // now make the error string upper case
  for (int i = 0; i < (int)sizeof(error) && error[i]; i++) {
    if (error[i] >= 'a' && error[i] <= 'z') {
      error[i] += 'A' - 'a';
    }
  }

  fprintf(stderr, "%s ", error);

  get_version(version);

  fprintf(stderr, "chpl version %s", version);
}


// find an AST location that is:
//   not in an inlined function or a task function in an inlined function
//     in non-user modules
//     (assuming preserveInlinedLineNumbers==false)
// to use for line number reporting.
static Expr* findLocationIgnoringInternalInlining(Expr* cur) {

  while (true) {
    if (cur == NULL || cur->parentSymbol == NULL)
      return cur;

    FnSymbol* curFn = cur->getFunction();
    // If we didn't find a function, or it's not in tree, give up
    if (curFn == NULL || curFn->inTree() == false)
      return cur;

    // If it's already in user code, use that, because
    // the line number is probably better
    if (curFn->getModule()->modTag == MOD_USER)
      return cur;

    bool inlined = curFn->hasFlag(FLAG_INLINED_FN);

    if (inlined == false || preserveInlinedLineNumbers)
      return cur;

    // Look for a call to that function
    for_SymbolSymExprs(se, curFn) {
      CallExpr* call = toCallExpr(se->parentExpr);
      if (se == call->baseExpr) {
        // Switch to considering that call point
        cur = call;
        break;
      }
    }
  }

  return cur; // never reached
}

bool printsUserLocation(const BaseAST* astIn) {
  BaseAST* ast = const_cast<BaseAST*>(astIn);

  if (Expr* expr = toExpr(ast)) {
    Expr* foundExpr = findLocationIgnoringInternalInlining(expr);
    if (foundExpr != NULL)
      ast = foundExpr;
  }

  return (ast && ast->getModule()->modTag == MOD_USER);
}

// find a caller (direct or not) that is not in a task function,
// for line number reporting
static FnSymbol* findNonTaskCaller(FnSymbol* fn) {

  FnSymbol* lastFn = fn;
  while (true) {
    // If we ran out of functions in tree, use the last one
    if (fn == NULL || fn->inTree() == false)
      return lastFn;

    // If it's not a task function, we are done
    if (isTaskFun(fn) == false)
      return fn;

    // Otherwise, find the call site, and continue the search.
    for_SymbolSymExprs(se, fn) {
      CallExpr* call = toCallExpr(se->parentExpr);
      if (se == call->baseExpr) {
        lastFn = fn;
        fn = call->getFunction();
        break;
      }
    }
  }

  return lastFn; // never reached
}

// Print instantiation information for err_fn.
// Should be called at USR_STOP or just before the next
// error changing err_fn is printed.
static void printInstantiationNoteForLastError() {
  if (err_fn_header_printed && err_fn && err_fn->instantiatedFrom) {

    // Find the first call to the function within the instantiation point,
    // so that we can have a better error message line number.
    BlockStmt* instantiationPoint = err_fn->instantiationPoint();
    Expr* bestPoint = instantiationPoint;

    if (instantiationPoint != NULL) {
      std::vector<CallExpr*> calls;
      collectFnCalls(instantiationPoint, calls);
      for_vector(CallExpr, call, calls) {
        if (FnSymbol* fn = call->resolvedOrVirtualFunction()) {
          if (fn == err_fn) {
            bestPoint = call;
            break;
          }
        }
      }

      const char* subsDesc = err_fn->substitutionsToString(", ");

      const char* intro = "";
      if (strcmp(err_fn->name, "init") == 0)
        intro = "Initializer";
      else if (err_fn->isIterator())
        intro = astr("Iterator ", "'", err_fn->name, "'");
      else
        intro = astr("Function ", "'", err_fn->name, "'");

      if (subsDesc == NULL || subsDesc[0] == '\0') {
        fprintf(stderr,
                "%s:%d: %s instantiated here\n",
                cleanFilename(bestPoint),
                bestPoint->linenum(),
                intro);
      } else {
        fprintf(stderr,
                "%s:%d: %s instantiated as: %s(%s)\n",
                cleanFilename(bestPoint),
                bestPoint->linenum(),
                intro,
                err_fn->name,
                subsDesc);
      }
    }
  }

  // Clear this variable in case e.g. err_fn is deleted in a future pass
  err_fn = NULL;
}

static bool printErrorHeader(BaseAST* ast) {

  if (Expr* expr = toExpr(ast)) {
    Expr* use = findLocationIgnoringInternalInlining(expr);
    if (use != NULL)
      ast = use;
  }

  if (!err_print) {
    if (Expr* expr = toExpr(ast)) {
      FnSymbol* fn = NULL;
      if (expr && expr->parentSymbol != NULL)
        fn = findNonTaskCaller(expr->getFunction());

      // Don't consider functions that aren't in the tree
      if (fn != NULL)
        if (fn->defPoint == NULL || !fn->inTree())
          fn = NULL;

      if (fn && fn->id != err_fn_id) {
        printInstantiationNoteForLastError();
        err_fn_header_printed = false;
        err_fn = fn;

        while ((fn = toFnSymbol(err_fn->defPoint->parentSymbol))) {
          if (fn == fn->getModule()->initFn) {
            break;
          }

          err_fn = fn;
        }
        err_fn_id = err_fn->id;

        // If the function is compiler-generated, or inlined, or doesn't match
        // the error function and line number, nothing is printed.
        if (err_fn->getModule()->initFn != err_fn     &&
            !err_fn->hasFlag(FLAG_COMPILER_GENERATED) &&
            err_fn->linenum()) {
          bool suppress = false;

          // Initializer might be inlined
          if (err_fn->hasFlag(FLAG_INLINE) == true) {
            suppress = (strcmp(err_fn->name, "init") != 0) ? true : false;
          }

          // Suppress internal function names
          if (!developer && strncmp(err_fn->name, "chpl_", 5) == 0) {
            suppress = true;
          }

          if (suppress == false) {
            fprintf(stderr,
                    "%s:%d: In ",
                    cleanFilename(err_fn),
                    err_fn->linenum());

            if (strcmp(err_fn->name, "init") == 0) {
              fprintf(stderr, "initializer:\n");

            } else {
              fprintf(stderr,
                      "%s '%s':\n",
                      (err_fn->isIterator() ? "iterator" : "function"),
                      err_fn->name);
            }
            // We printed the header, so can print instantiation notes.
            err_fn_header_printed = true;
          }
        }
      }
    }
  }

  bool have_ast_line = false;
  const char* filename;
  int linenum;

  if ( ast && ast->linenum() ) {
    have_ast_line = true;
    filename = cleanFilename(ast);
    linenum = ast->linenum();
  } else {
    have_ast_line = false;
    if ( !err_print && currentAstLoc.filename && currentAstLoc.lineno > 0 ) {
      // Use our best guess for the line number for user errors,
      // but don't do that for err_print (USR_PRINT) notes that don't
      // come with line numbers.
      filename = cleanFilename(currentAstLoc.filename);
      linenum = currentAstLoc.lineno;
    } else {
      filename = NULL;
      linenum = -1;
    }
  }

  bool guess = filename && !have_ast_line;

  if (filename) {
    fprintf(stderr, "%s:%d: ", filename, linenum);
  }

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

  if (!err_user) {
    if (!developer) {
      print_user_internal_error();
    }
  }
  return guess;
}


static void printErrorFooter(bool guess) {
  //
  // For developers, indicate the compiler source location where an
  // internal error was generated.
  //
  if (developer && !err_user)
    fprintf(stderr, " [%s/%s:%d]", err_subdir, err_filename, err_lineno);

  //
  // For users and developers, if the source line was a guess (i.e., an
  // AST was not passed to the INT_FATAL() macro and we relied on the
  // global SET_LINENO() information instead), indicate that.
  //
  if (guess) {
    fprintf(stderr, "\nNote: This source location is a guess.");
  }

  //
  // Apologize for our internal errors to the end-user
  //
  if (!developer && !err_user) {
    fprintf(stderr, "\n\n"
            "Internal errors indicate a bug in the Chapel compiler (\"It's us, not you\"),\n"
            "and we're sorry for the hassle.  We would appreciate your reporting this bug -- \n"
            "please see %s for instructions.  In the meantime,\n"
            "the filename + line number above may be useful in working around the issue.\n\n",
            help_url);

    //
    // and exit if it's fatal (isn't it always?)
    //
    if (err_fatal && !(err_user && ignore_user_errors)) {
      printInstantiationNoteForLastError();
      clean_exit(1);
    }
  }
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

  if (!developer) {
    fprintf(out, "while processing the following Chapel call chain:\n");
  }

  for (int i = callStack.n-1; i >= 0; i--) {
    CallExpr*     call   = callStack.v[i];
    FnSymbol*     fn     = call->getFunction();
    ModuleSymbol* module = call->getModule();

    fprintf(out,
            "  %s:%d: %s%s%s\n",
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
void printCallStack() {
  printCallStack(true, true, stdout);
}

// another one
void printCallStackCalls() {
  printf("\n" "callStack %d elms\n\n", callStack.n);

  for (int i = 0; i < callStack.n; i++) {
    CallExpr* call = callStack.v[i];
    FnSymbol* cfn  = call->resolvedFunction();

    printf("%d  %d %s  <-  %d %s\n",
           i,
           cfn  ? cfn->id  : 0, cfn  ? cfn->name         : "<no callee>",
           call ? call->id : 0, call ? call->stringLoc() : "<no call>");
  }

  printf("\n");
}


void handleError(const char* fmt, ...) {
  fflush(stdout);
  fflush(stderr);

  if (err_ignore) {
    return;
  }

  bool guess = printErrorHeader(NULL);

  //
  // Only print out the arguments if this is a user error or we're
  // in developer mode.
  //
  if (err_user || developer) {
    va_list args;

    va_start(args, fmt);

    vfprintf(stderr, fmt, args);

    va_end(args);
  }

  printErrorFooter(guess);
  fprintf(stderr, "\n");

  printCallStackOnError();

  if (!err_user && !developer) {
    return;
  }

  if (exit_immediately) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors && !(ignore_user_errors && err_user)) {
      printInstantiationNoteForLastError();
      clean_exit(1);
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void vhandleError(FILE*          file,
                         const BaseAST* ast,
                         const char*    fmt,
                         va_list        args);

void handleError(const BaseAST* ast, const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);

  vhandleError(stderr, ast, fmt, args);

  va_end(args);
}

void handleError(FILE* file, const BaseAST* ast, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  vhandleError(file, ast, fmt, args);

  va_end(args);
}

static void vhandleError(FILE*          file,
                         const BaseAST* ast,
                         const char*    fmt,
                         va_list        args) {
  if (err_ignore) {
    return;
  }

  bool guess = false;

  if (file == stderr) {
    guess = printErrorHeader(const_cast<BaseAST*>(ast));
  }

  if (err_user || developer) {
    vfprintf(file, fmt, args);
  }

  if (fPrintIDonError && ast) {
    fprintf(file, " [%d]", ast->id);
  }

  if (file == stderr) {
    printErrorFooter(guess);
  }

  fprintf(file, "\n");

  if (file == stderr) {
    printCallStackOnError();
  }

  if (!err_user && !developer) {
    return;
  }

  if (exit_immediately) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors && !(ignore_user_errors && err_user)) {
      printInstantiationNoteForLastError();
      clean_exit(1);
    }
  }
}


void exitIfFatalErrorsEncountered() {
  printInstantiationNoteForLastError();

  if (exit_eventually) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors && !(ignore_user_errors && err_user)) {
      clean_exit(1);
    }
  }
}


void considerExitingEndOfPass() {
  if (exit_end_of_pass) {
    if (!ignore_errors && !(ignore_user_errors && err_user)) {
      clean_exit(1);
    }
  }
}

bool fatalErrorsEncountered() {
  return exit_eventually || exit_end_of_pass;
}

void clearFatalErrors() {
  exit_eventually = false;
  exit_end_of_pass = false;
}

static void handleInterrupt(int sig) {
  stopCatchingSignals();
  fprintf(stderr, "error: received interrupt\n");
  fflush(stdout);
  fflush(stderr);

  clean_exit(1);
}


static void handleSegFault(int sig) {
  stopCatchingSignals();
  INT_FATAL("seg fault");
}


void startCatchingSignals() {
  signal(SIGINT,  handleInterrupt);
  signal(SIGTERM, handleInterrupt);
  signal(SIGHUP,  handleInterrupt);
  signal(SIGSEGV, handleSegFault);
}


void stopCatchingSignals() {
  signal(SIGINT,  SIG_DFL);
  signal(SIGTERM, SIG_DFL);
  signal(SIGHUP,  SIG_DFL);
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
