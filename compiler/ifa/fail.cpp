#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "defs.h"
#include "ast.h"
#include "if1.h"
#include "var.h"
#include "pnode.h"
#include "fail.h"

int ifa_verbose = 0;
int ifa_debug = 0;

int *assert_NULL_var = 0;

int
show_error(char *str, IFAAST *a, ...) {
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
#else
bool ignore_errors = 0;
bool developer = false;
void get_version(char *) {}
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
  (*buf)[*len] = 0;             /* terminator */
  (*buf)[*len + 1] = 0;         /* sentinal */
  read(fd, *buf, *len);
  close(fd);
  return *len;
}

char *
get_file_line(char *filename, int lineno) {
  static char *last_filename = 0;
  static char *last_buf = 0;
  static Vec<char *> last_lines;

  if (!last_filename || strcmp(filename, last_filename)) {
    int len = 0;
    char *new_buf = 0;
    if (buf_read(filename, &new_buf, &len) < 0)
      return 0;
    last_filename = _dupstr(filename);
    last_buf = new_buf;
    char *b = new_buf;
    last_lines.clear();
    last_lines.add(b);
    b = strchr(b, '\n');
    while (b) {
      *b = 0;
      b++;
      last_lines.add(b);
      b = strchr(b, '\n');
    }
  }
  lineno--; // 0 based
  if (lineno < 0 || lineno > last_lines.n)
    return NULL;
  return last_lines.v[lineno];
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
  exit(1);
}

void 
myassert(char *file, int line, char *str) {
  printf("assert %s:%d: %s\n", file, line, str);
  *(int*)0 = 1;
}

