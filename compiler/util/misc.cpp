#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "driver.h"
#include "../main/version.h"
#include "files.h"
#include "chpl.h"
#include "baseAST.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "misc.h"
#include "yy.h"

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

static const char* err_filename;
static int err_lineno;
static int err_fatal;
static int err_user;
static int err_print;
static int err_ignore;
static FnSymbol* err_fn = NULL;


static const char* cleanFilename(BaseAST* ast) {
  ModuleSymbol* mod = ast->getModule();
  if (mod) {
    if (strstr(ast->getModule()->filename, "/modules/standard")) {
      return astr("$CHPL_HOME", strstr(ast->getModule()->filename, "/modules/standard"));
    } else {
      return ast->getModule()->filename;
    }
  } else if (strstr(yyfilename, "/modules/standard")) {
    return astr("$CHPL_HOME", strstr(yyfilename, "/modules/standard"));
  } else {
    return yyfilename;
  }
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

  fprintf(stderr, "internal failure %s ", error);
  char version[128];
  get_version(version);
  fprintf(stderr, "chpl Version %s\n", version);
  if (err_fatal)
    clean_exit(1);
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
      if (fn && fn != err_fn) {
        err_fn = fn;
        while ((fn = toFnSymbol(err_fn->defPoint->parentSymbol))) {
          if (fn == fn->getModule()->initFn)
            break;
          err_fn = fn;
        }
        if (err_fn->getModule()->initFn != err_fn &&
            !err_fn->isCompilerTemp &&
            !err_fn->hasPragma(PRAG_INLINE) &&
            err_fn->lineno) {
          fprintf(stderr, "%s:%d: In ",
                  cleanFilename(err_fn), err_fn->lineno);
          if (!strncmp(err_fn->name, "_construct_", 11)) {
            fprintf(stderr, "constructor '%s':\n", err_fn->name+11);
          } else {
            fprintf(stderr, "%s '%s':\n",
                    (err_fn->hasPragma(PRAG_ITERATOR_FN) ? "iterator" : "function"),
                    err_fn->name);
          }
        }
      }
    }
  }


  if (ast && ast->lineno)
    fprintf(stderr, "%s:%d: ", cleanFilename(ast), ast->lineno);

  fprintf(stderr, err_print ? "note: " : err_fatal ? "error: " : "warning: ");

  if (!err_user && !developer) {
    print_user_internal_error();
  }

}

static void printDevelErrorFooter(void) {
  if (developer)
    fprintf(stderr, " [%s:%d]", err_filename, err_lineno);
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

  if (exit_immediately && !ignore_errors) {
    clean_exit(1);
  }
}


void handleError(BaseAST* ast, const char *fmt, ...) {
  if (err_ignore)
    return;

  printDevelErrorHeader(ast);

  if (!err_user && !developer)
    return;

  va_list args;
  
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printDevelErrorFooter();

  fprintf(stderr, "\n");

  if (exit_immediately && !ignore_errors) {
    clean_exit(1);
  }
}


void exitIfFatalErrorsEncountered() {
  if (exit_eventually && !ignore_errors) {
    clean_exit(1);
  }
}


static void handleInterrupt(int sig) {
  INT_FATAL("received interrupt");
}


static void handleSegFault(int sig) {
  INT_FATAL("seg fault");
}


void startCatchingSignals(void) {
  if (!developer) {
    signal(SIGINT, handleInterrupt);
    signal(SIGSEGV, handleSegFault);
  }
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
