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
#include "dparse.h"

extern D_ParserTables parser_tables_make_prims;

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
  *buf = (char*)malloc(*len + 2);
  (*buf)[*len] = 0;             /* terminator */
  (*buf)[*len + 1] = 0;         /* sentinal */
  read(fd, *buf, *len);
  close(fd);
  return *len;
}

int
main(int argc, char *argv[]) {
  int i, len = 0;
  char *buf = NULL, *fn;
  D_Parser *p;
  D_ParseNode *pn = NULL;

  p = new_D_Parser(&parser_tables_make_prims, 4);
  p->save_parse_tree = 1;
  for (i = 1; i < argc; i++) {
    p->loc.pathname = argv[i];
    p->loc.line = 1;
    p->loc.col = p->loc.previous_col = 0;
    if (buf_read(argv[i], &buf, &len) > 0)
      pn = dparse(p, buf, len);
    else 
      printf("unable to read file '%s'", argv[i]);
    if (pn) {
      free_D_ParseNode(p, pn);
      pn = 0;
    } else {
      fn = d_dup_pathname_str(p->loc.pathname);
      fprintf(stderr, "fatal error, '%s' line %d\n", fn, p->loc.line);
      free(fn);
    }
    if (buf)
      free(buf);
  }
  free_D_Parser(p);
  exit(0);
}
