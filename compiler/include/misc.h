/* -*-Mode: c++;-*-
*/
#ifndef _misc_H_
#define _misc_H_

#include "driver.h"

class d_loc_t;
class BaseAST;
class IFAAST;
class Var;

void clean_exit(int status);

int show_error(char *str, d_loc_t &loc, ...);
int show_error(char *str, IFAAST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
void fail(char *str, ...);
char *loc_string(d_loc_t &l);
// char *dupstr(char *s, char *e = 0); in map.h because of template's in header hell


// INT_FATAL(ast, format, ...)
//   where ast         == IFAAST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)

#define INT_FATAL \
  if (setupDevelError(__FILE__, __LINE__, true , false , false ) && developer) printProblem

#define INT_WARNING \
  if (setupDevelError(__FILE__, __LINE__, false , false , true ) && developer) printProblem

#define USR_FATAL \
  if (setupDevelError(__FILE__, __LINE__, true , true , false )) printProblem

#define USR_FATAL_CONT \
  if (setupDevelError(__FILE__, __LINE__, true , true , true )) printProblem

#define USR_WARNING \
  if (setupDevelError(__FILE__, __LINE__, false, true , true )) printProblem

int setupDevelError(char* filename, int lineno, bool fatal, bool user, bool cont);
void printProblem(char* fmt, ...);
void printProblem(IFAAST* ast, char* fmt, ...);
void printProblem(BaseAST* ast, char* fmt, ...);
void myassert(char *file, int line, char *str);

void startCatchingSignals(void);
void stopCatchingSignals(void);

#endif
