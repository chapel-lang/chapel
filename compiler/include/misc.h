/* -*-Mode: c++;-*-
*/
#ifndef _misc_H_
#define _misc_H_

#include "driver.h"

class d_loc_t;
class AST;
class Var;
class Loc;

void clean_exit(int status);

int show_error(char *str, d_loc_t &loc, ...);
int show_error(char *str, AST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
void fail(char *str, ...);
char *loc_string(d_loc_t &l);
// char *dupstr(char *s, char *e = 0); in map.h because of template's in header hell


// INT_FATAL(ast, format, ...)
//   where ast         == AST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)

#define INT_FATAL \
  if (setupDevelError(__FILE__, __LINE__, true , false) && developer) printProblem

#define INT_WARNING \
  if (setupDevelError(__FILE__, __LINE__, false , false) && developer) printProblem

#define USR_FATAL \
  if (setupDevelError(__FILE__, __LINE__, true , true )) printProblem

#define USR_WARNING \
  if (setupDevelError(__FILE__, __LINE__, false, true )) printProblem

int setupDevelError(char* filename, int lineno, bool fatal, bool user);
void printProblem(char* fmt, ...);
void printProblem(AST* ast, char* fmt, ...);
void printProblem(Loc* loc, char* fmt, ...);
void myassert(char *file, int line, char *str);

void startCatchingSignals(void);
void stopCatchingSignals(void);

#endif
