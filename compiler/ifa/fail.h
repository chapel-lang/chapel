/* -*-Mode: c++;-*-
*/
#ifndef _fail_H_
#define _fail_H_

class IFAAST;
class Var;

int show_error(char *str, IFAAST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
void fail(char *str, ...);
char *get_file_line(char *filename, int lineno);

// char *dupstr(char *s, char *e = 0); in map.h because of template's in header hell

void myassert(char *file, int line, char *str);

#endif
