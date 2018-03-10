/*
 * Copyright 2004-2018 Cray Inc.
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

static const char* err_filename     = NULL;

static int         err_lineno       =    0;
static int         err_fatal        =    0;
static int         err_user         =    0;
static int         err_print        =    0;
static int         err_ignore       =    0;

static FnSymbol*   err_fn           = NULL;

static bool forceWidePtrs();

// must be non-static to avoid dead-code elim. when compiling -O3
void gdbShouldBreakHere() {

}

void setupError(const char* filename, int lineno, int tag) {
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
  static char error[8];

  const char* filename_start = strrchr(err_filename, '/');
  char        version[128]   = { '\0' };

  if (filename_start) {
    filename_start++;
  } else {
    filename_start = err_filename;
  }

  strncpy(error, filename_start, 3);

  sprintf(error + 3, "%04d", err_lineno);

  for (int i = 0; i < 7; i++) {
    if (error[i] >= 'a' && error[i] <= 'z') {
      error[i] += 'A' - 'a';
    }
  }

  fprintf(stderr, "%s ", error);

  get_version(version);

  fprintf(stderr, "chpl version %s", version);
}


// find a caller (direct or not) that is not in a task function,
// for line number reporting
static FnSymbol* findNonTaskCaller(FnSymbol* fn) {
  FnSymbol* retval = NULL;

  if (fn == NULL || fn->inTree() == false) {
    retval = fn;

  } else {
    while (retval == NULL) {
      if (isTaskFun(fn) == false) {
        retval = fn;
      } else {
        FnSymbol* caller = NULL;

        forv_Vec(CallExpr, call, gCallExprs) {
          if (call->inTree() == true) {
            if (FnSymbol* cfn = call->resolvedFunction()) {
              if (cfn == fn) {
                caller = toFnSymbol(call->parentSymbol);
                break;
              }
            }
          }
        }

        if (caller == NULL) {
          retval = fn;
        } else {
          fn     = caller;
        }
      }
    }
  }

  return retval;
}

static bool printErrorHeader(const BaseAST* ast) {
  if (!err_print) {
    if (const Expr* expr = toConstExpr(ast)) {
      Symbol* parent = expr->parentSymbol;

      if (isArgSymbol(parent))
        parent = parent->defPoint->parentSymbol;

      FnSymbol* fn = toFnSymbol(parent);

      fn = findNonTaskCaller(fn);

      if (fn && fn != err_fn) {
        err_fn = fn;

        while ((fn = toFnSymbol(err_fn->defPoint->parentSymbol))) {
          if (fn == fn->getModule()->initFn) {
            break;
          }

          err_fn = fn;
        }

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

          if (suppress == false) {
            fprintf(stderr,
                    "%s:%d: In ",
                    cleanFilename(err_fn),
                    err_fn->linenum());

            if (strncmp(err_fn->name, "_construct_", 11) == 0) {
              fprintf(stderr, "initializer '%s':\n", err_fn->name+11);

            } else if (strcmp(err_fn->name, "init") == 0) {
              fprintf(stderr, "initializer:\n");

            } else {
              fprintf(stderr,
                      "%s '%s':\n",
                      (err_fn->isIterator() ? "iterator" : "function"),
                      err_fn->name);
            }
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
    fprintf(stderr, " [%s:%d]", err_filename, err_lineno);

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
    if (err_fatal) {
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
    } else if (!ignore_errors) {
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
    guess = printErrorHeader(ast);
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
