/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "misc.h"

#include "astlocs.h"
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

static bool        handle_erroneous_fns = true;

astlocT            last_error_loc(0, NULL);

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

static void vprint_error(const char* format, va_list vl) {
  vfprintf(stderr, format, vl);

  // This function could hide errors & save them for later re-issue.
  // See the commit history for this comment for a start in that direction.
}

static void print_error(const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  vprint_error(format, vl);
  va_end(vl);
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

  print_error("%s ", error);

  get_version(version);

  print_error("chpl version %s", version);
}

static bool isTaskFunWrapper(FnSymbol* fn) {
  return fn->hasFlag(FLAG_ON_BLOCK) ||
         fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK) ||
         fn->hasFlag(FLAG_BEGIN_BLOCK) ||
         fn->hasFlag(FLAG_LOCAL_ON);
}

static bool isTaskFnOrWrapper(FnSymbol* fn) {
  return isTaskFunWrapper(fn) || isTaskFun(fn);
}

// If it's not a task function, return it;
// otherwise, find a function calling it that is not a task function.
static FnSymbol* findNonTaskFn(FnSymbol* fn) {

  FnSymbol* lastFn = fn;
  while (true) {
    // If we ran out of functions in tree, use the last one
    if (fn == NULL || fn->inTree() == false)
      return lastFn;

    // If it's not a task function, we are done
    if (isTaskFnOrWrapper(fn) == false)
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

static void gatherFunctionsCalledTransitively(FnSymbol* fn,
                                              std::set<FnSymbol*>& fns) {
  if (fns.count(fn) == 0) {
    fns.insert(fn);

    // Gather calls from the function body & declaration
    std::vector<CallExpr*> calls;
    collectVirtualAndFnCalls(fn, calls);

    // gather calls from the called functions (transitively)
    for_vector(CallExpr, call, calls) {
      FnSymbol* calledFn = call->resolvedOrVirtualFunction();
      gatherFunctionsCalledTransitively(calledFn, fns);
    }
  }
}

static CallExpr* findACallSite(FnSymbol* fn, std::set<FnSymbol*> ignoreFns) {
  for_SymbolSymExprs(se, fn) {
    CallExpr* call = toCallExpr(se->parentExpr);
    if (se == call->baseExpr) {
      FnSymbol* inFn = call->getFunction();
      if (inFn && ignoreFns.count(inFn) == 0)
        return call;
    }
  }

  return NULL;
}

static bool isModuleInitFunction(FnSymbol* fn) {
  ModuleSymbol* mod = fn->getModule();
  if (mod && mod->initFn == fn)
    return true;
  if (fn->hasFlag(FLAG_MODULE_INIT))
    return true;

  return false;
}


static const char* fnKindAndName(FnSymbol* fn) {
  if (fn == NULL)
    return "";

  ModuleSymbol* mod = fn->getModule();
  if (mod && mod->initFn == fn)
    return astr("module ", "'", mod->name, "'");

  if (strcmp(fn->name, "init") == 0)
    return astr("initializer");

  if (fn->hasFlag(FLAG_IMPLEMENTS_WRAPPER))
    return astr("'implements ", interfaceNameForWrapperFn(fn), "' statement");

  if (fn->isIterator())
    return astr("iterator ", "'", fn->name, "'");
  else if (fn->isMethod())
    return astr("method ", "'", fn->name, "'");

  return astr("function ", "'", fn->name, "'");
}

// should the function name itself be hidden?
static bool isHiddenFunction(FnSymbol* fn) {
  return (strncmp(fn->name, "chpl_", 5) == 0);
}

// should the body of the function be hidden?
static bool isInternalFunction(FnSymbol* fn) {
  ModuleSymbol* module = fn->getModule();
  return (fn->hasFlag(FLAG_COMPILER_GENERATED) ||
          strncmp(fn->name, "chpl_", 5) == 0 ||
          module->modTag == MOD_INTERNAL);
}

static Expr* findBestCallSite(FnSymbol* errFn,
                              std::set<FnSymbol*>& currentFns) {
  // Gather functions called, transitively, so that we can rule out
  // recursive calls when showing a stack trace.
  gatherFunctionsCalledTransitively(errFn, currentFns);

  // Find the first call to the function within the instantiation point,
  // so that we can have a better error message line number.
  BlockStmt* instantiationPoint = errFn->instantiationPoint();
  Expr* bestPoint = instantiationPoint;

  if (instantiationPoint != NULL) {
    std::vector<CallExpr*> calls;
    collectFnCalls(instantiationPoint, calls);
    for_vector(CallExpr, call, calls) {
      if (FnSymbol* fn = call->resolvedOrVirtualFunction()) {
        if (fn == errFn) {
          bestPoint = call;
          break;
        }
      }
    }
  } else {
    // Find a call to the function
    bestPoint = findACallSite(errFn, currentFns);
  }

  return bestPoint;
}

// Note - this function is recursive.
static void printCallstack(FnSymbol* errFn, FnSymbol* prevFn,
                           std::set<FnSymbol*>& currentFns,
                           bool& printedUnderline,
                           bool& lastHidden) {

  // Stop now if it's a module init function
  if (isModuleInitFunction(errFn))
    return;

  Expr* bestPoint = findBestCallSite(errFn, currentFns);

  if (bestPoint != NULL) {
    FnSymbol* inFn = findNonTaskFn(bestPoint->getFunction());
    if (inFn == NULL)
      return;

    // Don't print "called from chpl_gen_main"
    bool calledFromGenMain = inFn->hasFlag(FLAG_GEN_MAIN_FUNC);
    if (calledFromGenMain == false) {
      bool hideErrFn = false;
      bool hideInFn = false;
      if (developer == false && fPrintCallStackOnError == false) {
        // don't print 'from function' if the call site is internal
        hideInFn = isInternalFunction(inFn);
        // don't print out callstack entry at all if the function is chpl_
        // or it is internal and not the "In function" call
        hideErrFn = isHiddenFunction(errFn) ||
                    (prevFn != NULL && isInternalFunction(errFn));
      }

      // Continue printing stack frames until not generic;
      // or, with --print-callstack-on-error, module/main is reached
      bool recurse = (inFn->instantiatedFrom != NULL ||
                      inFn->hasFlag(FLAG_INSTANTIATED_GENERIC) ||
                      hideErrFn || hideInFn ||
                      fPrintCallStackOnError);

      if (hideErrFn == false) {
        std::string nameAndArgs = errFn->nameAndArgsToString(", ", true,
                                                             printedUnderline);

        if (nameAndArgs.empty()) {
          print_error("  %s:%d: %s called",
                      cleanFilename(bestPoint),
                      bestPoint->linenum(),
                      fnKindAndName(errFn));
        } else {
          print_error("  %s:%d: called as %s",
                      cleanFilename(bestPoint),
                      bestPoint->linenum(),
                      nameAndArgs.c_str());
        }

        if (recurse && hideInFn == false) {
          // finish the current line
          print_error(" from %s\n", fnKindAndName(inFn));
          // continue to print call sites
        } else {
          // finish the current line
          print_error("\n");
        }
        lastHidden = false;
      } else {
        if (lastHidden == false) {
          print_error("  within internal functions "
                      "(use --print-callstack-on-error to see)\n");
        }
        lastHidden = true;
      }

      if (recurse) {
        printCallstack(inFn, errFn, currentFns,
                       printedUnderline, lastHidden);
      }
    }
  }
}

// Print instantiation information for err_fn.
// Should be called at USR_STOP or just before the next
// error changing err_fn is printed.
static void printCallstackForLastError() {
  if (err_fn_header_printed && err_fn) {
    // Clear out err_fn to avoid infinite loop if an error
    // is encountered when printing the call stack.
    // Clearing it is also helpful in case err_fn is deleted
    // in a future pass and then an error is reported.
    FnSymbol* fn = err_fn;
    err_fn = NULL;

    bool printStack = false;
    if (fAutoPrintCallStackOnError)
      printStack = (fn->instantiatedFrom != NULL ||
                    fn->hasFlag(FLAG_INSTANTIATED_GENERIC));
    else
      printStack = fPrintCallStackOnError;

    if (printStack) {
      std::set<FnSymbol*> currentFns;
      bool printedUnderline = false;
      bool lastHidden = false;
      printCallstack(fn, NULL, currentFns,
                     printedUnderline, lastHidden);
      if (printedUnderline)
        USR_PRINT("generic instantiations are underlined in the above callstack");
    }
  }
}

static bool printErrorHeader(BaseAST* ast, astlocT astloc) {

  if (Expr* expr = toExpr(ast)) {
    Expr* use = findLocationIgnoringInternalInlining(expr);
    if (use != NULL)
      ast = use;
  }

  if (!err_print) {
    if (Expr* expr = toExpr(ast)) {
      FnSymbol* fn = NULL;
      if (expr && expr->parentSymbol != NULL)
        fn = findNonTaskFn(expr->getFunction());

      // Don't consider functions that aren't in the tree
      if (fn != NULL)
        if (fn->defPoint == NULL || !fn->inTree())
          fn = NULL;

      if (fn && fn->id != err_fn_id) {
        printCallstackForLastError();
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
        if (!err_fn->hasFlag(FLAG_COMPILER_GENERATED) &&
            err_fn->linenum()) {
          bool suppress = false;

          // Suppress internal function names
          if (!developer && strncmp(err_fn->name, "chpl_", 5) == 0) {
            suppress = true;
          }

          if (suppress == false) {
            print_error("%s:%d: In %s:\n",
                        cleanFilename(err_fn), err_fn->linenum(),
                        fnKindAndName(err_fn));

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
  } else if ( astloc.filename != NULL) {
    have_ast_line = true;
    filename = cleanFilename(astloc.filename);
    linenum = astloc.lineno;
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
    if (err_fatal && err_user) {
      // save the error location for printsSameLocationAsLastError
      last_error_loc.filename = filename;
      last_error_loc.lineno = linenum;
    }
    print_error("%s:%d: ", filename, linenum);
  }

  if (err_print) {
    print_error("note: ");
  } else if (err_fatal) {
    print_error("%s", boldErrorFormat());
    if (err_user) {
      print_error("error: ");
    } else {
      print_error("internal error: ");
    }
    print_error("%s", clearErrorFormat());
  } else {
    print_error("%s", boldErrorFormat());
    print_error("warning: ");
    print_error("%s", clearErrorFormat());
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
    print_error(" [%s/%s:%d]", err_subdir, err_filename, err_lineno);

  //
  // For users and developers, if the source line was a guess (i.e., an
  // AST was not passed to the INT_FATAL() macro and we relied on the
  // global SET_LINENO() information instead), indicate that.
  //
  if (guess) {
    print_error("\nNote: This source location is a guess.");
  }

  //
  // Apologize for our internal errors to the end-user
  //
  if (!developer && !err_user) {
    print_error("\n\n"
      "Internal errors indicate a bug in the Chapel compiler (\"It's us, not you\"),\n"
      "and we're sorry for the hassle.  We would appreciate your reporting this bug -- \n"
      "please see %s for instructions.  In the meantime,\n"
      "the filename + line number above may be useful in working around the issue.\n\n",
      help_url);

    //
    // and exit if it's fatal (isn't it always?)
    //
    if (err_fatal && !(err_user && ignore_user_errors)) {
      printCallstackForLastError();
      clean_exit(1);
    }
  }
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

static bool isErrorInOrCallingErroneousFunction(BaseAST* ast) {
  if (ast == NULL)
    return false;

  if (CallExpr* call = toCallExpr(ast)) {
    FnSymbol* fn = call->resolvedFunction();
    if (fn && fn->hasFlag(FLAG_ERRONEOUS_COPY))
      return true;
  }

  FnSymbol* fn = ast->getFunction();
  return fn && fn->hasFlag(FLAG_ERRONEOUS_COPY);
}

static void reportErroneousFunctionCall(BaseAST* ast) {
  INT_ASSERT(ast);

  FnSymbol* fn = NULL;
  if (CallExpr* call = toCallExpr(ast)) {
    FnSymbol* calledFn = call->resolvedFunction();
    if (calledFn && calledFn->hasFlag(FLAG_ERRONEOUS_COPY))
      fn = calledFn;
  }

  if (fn == NULL)
    fn = ast->getFunction();

  INT_ASSERT(fn && fn->hasFlag(FLAG_ERRONEOUS_COPY));

  // find a call site that is not in an erroneous function call
  BaseAST* cur = ast;
  Expr* next;
  const char* err = getErroneousCopyError(fn);

  std::set<FnSymbol*> currentFns;

  while (true) {
    fn = cur->getFunction();
    if (fn == NULL)
      break;
    if (!fn->hasFlag(FLAG_ERRONEOUS_COPY))
      break;

    err = getErroneousCopyError(fn);
    next = findBestCallSite(fn, currentFns);
    if (next == NULL)
      break;

    cur = next;
  }

  INT_ASSERT(err);
  INT_ASSERT(cur);

  bool save_handle_erroneous_fns = handle_erroneous_fns;
  handle_erroneous_fns = false;
  handleError(cur, "%s", err);
  handle_erroneous_fns = save_handle_erroneous_fns;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void vhandleError(const BaseAST* ast,
                         astlocT        astloc,
                         const char*    fmt,
                         va_list        args);

void handleError(const char *fmt, ...) {
  astlocT astloc(0, NULL);

  va_list args;

  va_start(args, fmt);

  vhandleError(NULL, astloc, fmt, args);

  va_end(args);
}

void handleError(const BaseAST* ast, const char *fmt, ...) {
  astlocT astloc(0, NULL);

  va_list args;

  va_start(args, fmt);

  vhandleError(ast, astloc, fmt, args);

  va_end(args);
}

void handleError(astlocT astloc, const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);

  vhandleError(NULL, astloc, fmt, args);

  va_end(args);
}


static void vhandleError(const BaseAST* ast,
                         astlocT        astloc,
                         const char*    fmt,
                         va_list        args) {
  if (err_ignore) {
    return;
  }

  if (handle_erroneous_fns && err_fatal &&
      isErrorInOrCallingErroneousFunction(const_cast<BaseAST*>(ast))) {
    bool save_exit_immediately = exit_immediately;

    // don't exit yet
    if (exit_immediately) {
      exit_immediately = false;
      exit_eventually = true;
    }

    // Report the saved error at the call site
    reportErroneousFunctionCall(const_cast<BaseAST*>(ast));

    exit_immediately = save_exit_immediately;

    if (fPrintAdditionalErrors == false) {
      print_error("note: An additional error is hidden. "
                  "Use --print-additional-errors to see it.\n");
      exitIfFatalErrorsEncountered();
      return;
    }
    print_error("note: Additional error follows\n");
    // now the rest of this function will report the additional error
  }

  bool guess = false;

  guess = printErrorHeader(const_cast<BaseAST*>(ast), astloc);

  //
  // Only print out the arguments if this is a user error or we're
  // in developer mode.
  //
  if (err_user || developer) {
    vprint_error(fmt, args);
  }

  if (fPrintIDonError && ast) {
    print_error(" [%d]", ast->id);
  }

  printErrorFooter(guess);

  print_error("\n");

  if (!err_user && !developer) {
    return;
  }

  if (exit_immediately) {
    if (ignore_errors_for_pass) {
      exit_end_of_pass = true;
    } else if (!ignore_errors && !(ignore_user_errors && err_user)) {
      printCallstackForLastError();
      clean_exit(1);
    }
  }
}


void exitIfFatalErrorsEncountered() {
  printCallstackForLastError();

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

bool printsSameLocationAsLastError(const BaseAST* ast) {
  astlocT loc(0, NULL);

  if ( ast && ast->linenum() ) {
    loc.filename = cleanFilename(ast);
    loc.lineno = ast->linenum();
  }

  return loc == last_error_loc;
}

void clearLastErrorLocation() {
  last_error_loc.filename = NULL;
  last_error_loc.lineno = 0;
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

static const char* gErrorFormatBold = "";
static const char* gErrorFormatUnderline = "";
static const char* gErrorFormatClearAll = "";
static bool gErrorFormatsSetup = false;

static void setupErrorFormatEscapes() {
  if (gErrorFormatsSetup == false) {
    gErrorFormatsSetup = true;

    // The normal configuration is
    //   fUseColorTerminal=false
    //   fDetectColorTerminal=true
    // Other settings of these variables are for testing the formatting itself.
    bool isColorTerm = fUseColorTerminal;
    if (fDetectColorTerminal) {
      // Check the TERM variable. This approach and these
      // terminals are taken from googletest; see
      // https://github.com/google/googletest/blob/master/googletest/src/gtest.cc#L3216
      const char* term = getenv("TERM");
      const char* colorTerms[] = {"xterm",
                                  "xterm-color",
                                  "xterm-256color",
                                  "screen",
                                  "screen-256color",
                                  "tmux",
                                  "tmux-256color",
                                  "rxvt-unicode",
                                  "rxvt-unicode-256color",
                                  "linux",
                                  "cygwin",
                                  NULL};
      if (term == NULL) term = "";
      for (int i = 0; colorTerms[i] != NULL; i++) {
        if (0 == strcmp(term, colorTerms[i]))
          isColorTerm = true;
      }

      // Check if errors will be output to a tty. If not,
      // the format codes will just store "" and have no effect.
      if (isatty(fileno(stderr)) == 0) {
        isColorTerm = false;
      }
    }

    if (isColorTerm) {
      gErrorFormatBold = "\x1B[1m";
      gErrorFormatUnderline = "\x1B[4m";
      gErrorFormatClearAll = "\x1B[0m";
    } else {
      gErrorFormatBold = "";
      gErrorFormatUnderline = "";
      gErrorFormatClearAll = "";
    }
  }
}

// These are specifically for output to stderr (from USR_PRINT etc)
const char* boldErrorFormat() {
  setupErrorFormatEscapes();
  return gErrorFormatBold;
}
const char* underlineErrorFormat() {
  setupErrorFormatEscapes();
  return gErrorFormatUnderline;
}
const char* clearErrorFormat() {
  setupErrorFormatEscapes();
  return gErrorFormatClearAll;
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
