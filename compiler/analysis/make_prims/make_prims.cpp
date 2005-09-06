#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include "vec.h"

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

FILE *fp = 0;
FILE *hfp = 0;

char *
dupstr(char *s, char *e) {
  int l = e-s;
  char *ss = (char*)MALLOC(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}

char *
catstr(char *s, char *ss) {
  int l = strlen(s) + strlen(ss) + 1;
  char *x = (char*)MALLOC(l+1);
  strcpy(x, s);
  strcat(x, ss);
  return x;
}

struct Line {
  char *name;
  char *string;
  char *nargs;
  char *pos;
  char *nres;
  char *argtypes;
  char *rettypes;
  char *options;
  int index;
};
#define forv_Line(_x, _y) forv_Vec(Line, _x, _y)

#define EOF_TOK ((char *)(intptr_t)-1)

typedef char *charp;

char *
get(charp &p, int optnum = 0) {
  while (*p && isspace(*p)) p++;
  char *s = p;
  if (optnum && (*p != '-' && !isdigit(*p))) return NULL;
  if (*s == '{') {
    while (*p && *p != '}') p++;
    if (*p) p++;
  } else if (*s == '"') {
    p++;
    while (*p && *p != '"') p++;
    if (*p) p++;
  } else 
    while (*p && !isspace(*p)) p++;
  if (!*p) return EOF_TOK;
  if (*s == ';') return NULL;
  return dupstr(s, p);
}

void
get_lines(char *b, Vec<Line *> &lines) {
  int index = 0;
  while (1) {
    Line *l = new Line;
    l->index = index++;
    do {
      while (*b && isspace(*b)) b++;
      if (*b != '/') 
        break;
      while (*b && *b != '\n') b++;
    } while (*b);
    if ((l->name = get(b)) == EOF_TOK) return;
    if ((l->string = get(b)) == EOF_TOK) return;
    if ((l->nargs = get(b)) == EOF_TOK) return;
    if ((l->pos = get(b)) == EOF_TOK) return;
    if ((l->nres = get(b, 1)) == EOF_TOK) return;
    if ((l->argtypes = get(b)) == EOF_TOK) return;
    if ((l->rettypes = get(b)) == EOF_TOK) return;
    if ((l->options = get(b)) == EOF_TOK) return;
    lines.add(l);
  }
}

void
declare_data(Vec<Line *> &lines) {
  forv_Line(l, lines) {
    fprintf(hfp, "extern Prim *%s;\n", l->name);
    fprintf(hfp, "#define P_%s %d\n", l->name, l->index);
  }
}

void
define_data(Vec<Line *> &lines) {
  forv_Line(l, lines)
    fprintf(fp, "Prim *%s = 0;\n", l->name);
}

void
build_data(Vec<Line *> &lines) {
  forv_Line(l, lines) {
    int nargs = 0;
    char *rets = l->nres;
    if (!rets) rets = "1";
    fprintf(fp, "  static PrimType %s_arg_types[] = %s;\n", l->name, l->argtypes);
    fprintf(fp, "  static PrimType %s_ret_types[] = %s;\n", l->name, l->rettypes);
    fprintf(fp, "  %s = new Prim(%d, %s, \"%s\", %s, %s, %s, %s_arg_types, %s_ret_types, %s);\n", 
            l->name, l->index, l->string, l->name, l->nargs, l->pos, rets, l->name,
            l->name, l->options ? "PRIM_NON_FUNCTIONAL" : "0");
    fprintf(fp, "  n = if1->strings.put(%s);\n", l->string);
    nargs = atoi(l->nargs);
    nargs -= 2;
    if (nargs < 0)
      nargs = 0;
    fprintf(fp, "  p->prims.add(%s);\n", l->name);
    fprintf(fp, "  p->prim_map[%d][%s].put(n, %s);\n", nargs, l->pos, l->name);
  }
}

int
main(int argc, char *argv[]) {
  int i, len = 0;
  char *buf = NULL;
  Vec<Line *> lines;

  if (argc < 1 || buf_read(argv[1], &buf, &len) < 0) {
    printf("unable to read file '%s'", argv[i]);
    exit(-1);
  }
  get_lines(buf, lines);

  hfp = fopen("prim_data.h", "w");
  fprintf(hfp, "#ifndef _prim_data_H\n");
  fprintf(hfp, "#define _prim_data_H\n\n");
  fprintf(hfp, "void prim_init(Primitives *p, IF1 *if1);\n\n");
  declare_data(lines);
  fprintf(hfp, "#endif\n");
  fclose(hfp);

  fp = fopen("prim_data.cpp", "w");
  fprintf(fp, "#include \"prim_data_incs.h\"\n\n");
  define_data(lines);
  fprintf(fp, "\nvoid prim_init(Primitives *p, IF1 *if1) {\n");
  fprintf(fp, "  char *n;\n");
  build_data(lines);
  fprintf(fp, "}\n");
  fclose(fp);
}
