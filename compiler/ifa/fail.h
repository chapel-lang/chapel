/* -*-Mode: c++;-*-
*/
#ifndef _fail_H_
#define _fail_H_

class IFAAST;
class Var;

extern int ifa_verbose;
extern int ifa_debug;

int show_error(char *str, IFAAST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
void fail(char *str, ...);
char *get_file_line(char *filename, int lineno);
void myassert(char *file, int line, char *str);

#endif
