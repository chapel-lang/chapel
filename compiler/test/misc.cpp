#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "files.h"
#include "geysa.h"
#include "../vparser/parse_structs.h"
#include "dparse.h"
#include "dparse_tables.h"
#include "ast.h"
#include "if1.h"
#include "loc.h"
#include "var.h"
#include "pnode.h"

int verbose_level = 0;
int *assert_NULL_var = 0;

static void cleanup(void) {
  deleteTmpDir();
  stopCatchingSignals();
}

void
clean_exit(int status) {
  cleanup();
  exit(status);
}

char *
loc_string(d_loc_t &l) {
  char nstr[1024];
  snprintf(nstr, 1023, "%s:%d", l.pathname, l.line);
  return dupstr(nstr);
}

int
show_error(char *str, d_loc_t &loc, ...) {
  char nstr[1024];
  va_list ap;
  va_start(ap, loc);
  snprintf(nstr, 1023, "%s:%d: %s\n", loc.pathname, loc.line, str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  return -1;
}

int
show_error(char *str, AST *a, ...) {
  char nstr[1024];
  va_list ap;
  va_start(ap, a);
  snprintf(nstr, 1023, "%s:%d: %s\n", a->pathname(), a->line(), str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  return -1;
}

#ifndef TEST_LIB
int
show_error(char *str, Var *v, ...) {
  char nstr[1024];
  va_list ap;
  va_start(ap, v);
  if (v->sym->ast)
    snprintf(nstr, 1023, "%s:%d: %s\n", v->sym->pathname(), v->sym->line(), str);
  else if (v->def && v->def->code && v->def->code->ast)
    snprintf(nstr, 1023, "%s:%d: %s\n", v->def->code->pathname(), v->def->code->line(), str);
  else
    snprintf(nstr, 1023, "error: %s\n", str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  return -1;
}
#endif

int
buf_read(char *pathname, char **buf, int *len) {
  struct stat sb;
  int fd;

  *buf = 0;
  *len = 0;
  fd = open(pathname, O_RDONLY);
  if (fd <= 0) 
    return -1;
  memset(&sb, 0, sizeof(sb));
  fstat(fd, &sb);
  *len = sb.st_size;
  *buf = (char*)MALLOC(*len + 2);
  (*buf)[*len] = 0;		/* terminator */
  (*buf)[*len + 1] = 0;		/* sentinal */
  read(fd, *buf, *len);
  close(fd);
  return *len;
}

void
fail(char *str, ...) {
  char nstr[256];
  va_list ap;

  fflush(stdout);
  fflush(stderr);

  va_start(ap, str);
  snprintf(nstr, 255, "fail: %s\n", str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  clean_exit(1);
}

char *
dupstr(char *s, char *e) {
  int l = e ? e-s : strlen(s);
  char *ss = (char*)MALLOC(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}

void myassert(char *file, int line, char *str) {
  printf("assert %s:%d: %s\n", file, line, str);
  *(int*)0 = 1;
}


// Support for internal errors, adopted from ZPL compiler

static bool isError = true;

void setupIntError(char *filename, int lineno, bool error) {
  if (error) {
    fprintf(stderr, "%s:%d: internal error: ", filename, lineno);
  } else {
    fprintf(stderr, "%s:%d: internal warning: ", filename, lineno);
  }
  isError = error;
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
  fprintf(stderr, "\n\n");
}


void intProblem(char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printUsrLocation(NULL, 0);

  if (isError) {
    clean_exit(1);
  }
}


void intProblem(AST* ast, char *fmt, ...) {
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

  if (isError) {
    clean_exit(1);
  }
}


void intProblem(Loc* loc, char *fmt, ...) {
  va_list args;
  int usrlineno = 0;
  char *usrfilename = NULL;

  if (loc) {
    usrfilename = loc->filename;
    usrlineno = loc->lineno;
  }
  
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  printUsrLocation(usrfilename, usrlineno);

  if (isError) {
    clean_exit(1);
  }
}


void USR_FATAL(Loc* loc, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  loc->printLoc(stderr);
  fprintf(stderr, ": ");
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  clean_exit(1);
}


static void handleInterrupt(int sig) {
  fail("received interrupt");
}

static void handleSegFault(int sig) {
  fail("seg fault");
}


void startCatchingSignals(void) {
  signal(SIGINT, handleInterrupt);
  signal(SIGSEGV, handleSegFault);
}


void stopCatchingSignals(void) {
  signal(SIGINT, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
}
