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

void cleanup_for_exit(void) {
  deleteTmpDir();
  stopCatchingSignals();
}

// must be non-static to avoid dead-code elim. when compiling -O3
void gdbShouldBreakHere(void) {
}

char *
dupstr(char *s, char *e) {
  int l = e ? e-s : strlen(s);
  char *ss = (char*)malloc(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}

void
clean_exit(int status) {
  if (status != 0) {
    gdbShouldBreakHere();
  }
  cleanup_for_exit();
  exit(status);
}


// Support for internal errors, adopted from ZPL compiler

static bool exit_immediately = true;
static bool exit_eventually = false;

static char* err_filename;
static int err_lineno;
static int err_fatal;
static int err_user;
static int err_print;
static FnSymbol* err_fn = NULL;


static char* cleanFilename(BaseAST* ast) {
  if (strstr(ast->filename, "/modules/standard"))
    return stringcat("$CHPL_HOME", strstr(ast->filename, "/modules/standard"));
  return ast->filename;
}


static void
print_user_internal_error() {
  static char error[8];

  char* filename_start = strrchr(err_filename, '/');
  if (filename_start)
    filename_start++;
  else
    filename_start = err_filename;
  strncpy(error, filename_start, 3);
  sprintf(error, "%s%04d", error, err_lineno);
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


bool
setupError(char *filename, int lineno, bool fatal, bool user, bool cont,
           bool print) {
  err_filename = filename;
  err_lineno = lineno;
  err_fatal = fatal;
  err_user = user;
  err_print = print;
  exit_immediately = !cont;
  if (err_fatal)
    exit_eventually = true;
  return true;
}


static void
printDevelErrorHeader(BaseAST* ast) {
  if (!err_print) {
    if (Expr* expr = dynamic_cast<Expr*>(ast)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(expr->parentSymbol)) {
        if (fn != err_fn) {
          err_fn = fn;
          while ((fn = dynamic_cast<FnSymbol*>(err_fn->defPoint->parentSymbol))) {
            err_fn = fn;
          }
          if (err_fn->getModule()->initFn != err_fn && !err_fn->isCompilerTemp &&
              err_fn->filename && err_fn->lineno) {
            fprintf(stderr, "%s:%d: In %s '%s':\n",
                    err_fn->filename, err_fn->lineno, 
                    (err_fn->fnClass == FN_ITERATOR ? "iterator" : "function"), 
                    err_fn->name);
          }
        }
      }
    }
  }


  if (ast && ast->filename && ast->lineno)
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



void printProblem(char *fmt, ...) {
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


void printProblem(BaseAST* ast, char *fmt, ...) {
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


void check_fatal_errors_encountered() {
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
