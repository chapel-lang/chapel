#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "driver.h"
#include "../main/version.h"
#include "files.h"
#include "chpl.h"
#include "ast.h"
#include "if1.h"
#include "var.h"
#include "pnode.h"
#include "baseAST.h"

void cleanup_for_exit(void) {
  deleteTmpDir();
  stopCatchingSignals();
}

static void gdbShouldBreakHere() {
}

char *
dupstr(char *s, char *e) {
  int l = e ? e-s : strlen(s);
  char *ss = (char*)MALLOC(l+1);
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

#ifdef TEST_LIB
bool ignore_errors = 0;
bool developer = false;
void get_version(char *) {}
#endif


// Support for internal errors, adopted from ZPL compiler

static bool exit_immediately = true;
static bool exit_eventually = false;


static char* internalErrorCode(char* filename, int lineno) {
  static char error[8];

  char* filename_start = strrchr(filename, '/');
  if (filename_start) {
    filename_start++;
  }
  else {
    filename_start = filename;
  }
  strncpy(error, filename_start, 3);
  sprintf(error, "%s%04d", error, lineno);
  for (int i = 0; i < 7; i++) {
    if (error[i] >= 'a' && error[i] <= 'z') {
      error[i] += 'A' - 'a';
    }
  }
  return error;
}


int setupDevelError(char *filename, int lineno, bool fatal, bool user, bool cont) {
  if (developer) {
    fprintf(stderr, "[%s:%d] ", filename, lineno);
  }

  if (!user) {
    if (fatal) {
      fprintf(stderr, "Internal error: ");
    }
    else {
      fprintf(stderr, "Internal warning: ");
    }
  }
  else {
    if (fatal) {
      fprintf(stderr, "Error: ");
    }
    else {
      fprintf(stderr, "Warning: ");
    }
  }

  if (!user && !developer) {
    char version[128];
    fprintf(stderr, "%s ", internalErrorCode(filename, lineno));
    get_version(version);
    fprintf(stderr, "chpl Version %s\n", version);
    if (fatal) {
      clean_exit(1);
    }
  }

  if (fatal)
    exit_eventually = true;
  exit_immediately = !cont;
  return 1;
}


int setupDevelPrint(char *filename, int lineno) {
  if (developer) {
    fprintf(stderr, "[%s:%d] ", filename, lineno);
  }
  return 1;
}


static void printUsrLocation(char* filename, int lineno) {
  if (filename || lineno) {
    fprintf(stderr, " (");
    if (filename) {
      fprintf(stderr, "%s", filename);
    }
    if (lineno) {
      if (filename) {
        fprintf(stderr, ":");
      } else {
        fprintf(stderr, "line ");
      }
      fprintf(stderr, "%d", lineno);
    }
    fprintf(stderr, ")");
  }
  fprintf(stderr, "\n");
}


void printProblem(char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printUsrLocation(NULL, 0);

  if (exit_immediately && !ignore_errors) {
    clean_exit(1);
  }
}


void printProblem(IFAAST* ast, char *fmt, ...) {
  va_list args;
  int usrlineno = 0;
  char *usrfilename = NULL;

  if (ast) {
    usrlineno = ast->line();
    usrfilename = ast->pathname();
  }
  
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printUsrLocation(usrfilename, usrlineno);

  if (exit_immediately && !ignore_errors) {
    clean_exit(1);
  }
}


void printProblem(BaseAST* ast, char *fmt, ...) {
  va_list args;
  int usrlineno = 0;
  char *usrfilename = NULL;

  if (ast) {
    usrfilename = ast->filename;
    usrlineno = ast->lineno;
  }
  
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printUsrLocation(usrfilename, usrlineno);

  if (developer && ast) {
    if (ast->traversalInfo) {
      fprintf(stderr, "  Constructed in traversal %s\n", ast->traversalInfo);
    }
    if (ast->copyInfo) {
      forv_Vec(char, str, *(ast->copyInfo)) {
        fprintf(stderr, "  Copied in traversal %s\n", str);
      }
    }
  }

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
