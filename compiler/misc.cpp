/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include <signal.h>
#include "files.h"
#include "geysa.h"

int verbose_level = 0;
int debug_level = 0;
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
  snprintf(nstr, 1023, "%s:%d: %s\n", a->pathname, a->line, str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  return -1;
}

int
show_error(char *str, Var *v, ...) {
  char nstr[1024];
  va_list ap;
  va_start(ap, v);
  if (v->sym->ast)
    snprintf(nstr, 1023, "%s:%d: %s\n", v->sym->ast->pathname, v->sym->ast->line, str);
  else if (v->def && v->def->code && v->def->code->ast)
    snprintf(nstr, 1023, "%s:%d: %s\n", v->def->code->ast->pathname, v->def->code->ast->line, str);
  else
    snprintf(nstr, 1023, "error: %s\n", str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  return -1;
}

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

static char *interrorfile;
static int interrorline;

void setupIntError(char *filename, int lineno) {
  interrorfile = filename;
  interrorline = lineno;
}


void intFatal(AST* ast, char *fmt, ...) {
  va_list args;
  int usrlineno = 0;
  char *usrfilename = NULL;

  if (ast) {
    usrlineno = ast->line;
    usrfilename = ast->pathname;
  }
  
  fprintf(stderr, "%s:%d: internal error: ", 
	  interrorfile, interrorline);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  if (usrfilename || usrlineno) {
    fprintf(stderr, " (");
    if (usrfilename) {
      fprintf(stderr, "%s", usrfilename);
    }
    if (usrlineno) {
      if (usrfilename) {
	fprintf(stderr, ":");
      } else {
	fprintf(stderr, "line ");
      }
      fprintf(stderr, "%d", usrlineno);
    }
    fprintf(stderr, ")");
  }
  fprintf(stderr, "\n\n");

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
